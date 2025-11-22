#include "bluetooth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

// --- BLE-Konstanten ---
#define GATTS_TAG "GATTS_MINI"
#define TEST_DEVICE_NAME            "ESP32_BLE_WAAGE"
#define GATTS_SERVICE_UUID          0x00FF
#define GATTS_CHAR_UUID             0xFF01
#define GATTS_NUM_HANDLE            4      // Service, Char Decl, Char Value, CCCD

#define PREPARE_BUF_MAX_SIZE        1024   // (nicht genutzt im Minimalpfad)

static uint8_t adv_uuid128[16] = {
    0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x00,0xff,0x00,0x00
};

// --- Advertising-Daten ---
static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp    = false,
    .include_name    = true,
    .include_txpower = false,
    .min_interval    = 0x0006,
    .max_interval    = 0x0010,
    .appearance      = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data   = NULL,
    .service_uuid_len = sizeof(adv_uuid128),
    .p_service_uuid   = adv_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_data_t scan_rsp_data = {
    .set_scan_rsp    = true,
    .include_name    = true,
    .include_txpower = true,
    .appearance      = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data   = NULL,
    .service_uuid_len = sizeof(adv_uuid128),
    .p_service_uuid   = adv_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min       = 0x20,  // 40ms
    .adv_int_max       = 0x40,  // 80ms
    .adv_type          = ADV_TYPE_IND,
    .own_addr_type     = BLE_ADDR_TYPE_PUBLIC,
    .channel_map       = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static uint8_t adv_config_done = 0;
enum { adv_config_flag = (1 << 0), scan_rsp_config_flag = (1 << 1) };

// --- GATT-State ---
static esp_gatt_if_t s_gatts_if = ESP_GATT_IF_NONE;
static uint16_t      s_conn_id   = 0xFFFF;

static uint16_t      s_service_handle = 0;
static uint16_t      s_char_handle    = 0;
static uint16_t      s_cccd_handle    = 0;

static bool          s_notify_enabled = false;

// RX-Queue
static QueueHandle_t s_rx_queue = NULL;

// --- Forward Declarations ---
static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

// ===================== IMPLEMENTIERUNG =====================

void bluetooth_init(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));

    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_ERROR_CHECK(esp_ble_gatts_register_callback(gatts_event_handler));
    ESP_ERROR_CHECK(esp_ble_gap_register_callback(gap_event_handler));

    const uint16_t APP_ID = 0x01;
    ESP_ERROR_CHECK(esp_ble_gatts_app_register(APP_ID));

    (void)esp_ble_gatt_set_local_mtu(247);

    s_rx_queue = xQueueCreate(10, sizeof(int32_t));
    configASSERT(s_rx_queue != NULL);
}

bool bluetooth_receive_int(int32_t *out_value, uint32_t timeout_ms)
{
    if (!out_value || !s_rx_queue) return false;
    return xQueueReceive(s_rx_queue, out_value, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

esp_err_t bluetooth_send_int(int32_t value)
{
    if (!s_notify_enabled || s_conn_id == 0xFFFF || s_char_handle == 0 || s_gatts_if == ESP_GATT_IF_NONE)
        return ESP_FAIL;

    uint8_t p[4] = {
        (uint8_t)(value & 0xFF),
        (uint8_t)((value >> 8) & 0xFF),
        (uint8_t)((value >> 16) & 0xFF),
        (uint8_t)((value >> 24) & 0xFF),
    };

    return esp_ble_gatts_send_indicate(s_gatts_if, s_conn_id, s_char_handle, sizeof(p), p, false);
}

// ===================== GAP Handler =====================
static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        adv_config_done &= (~adv_config_flag);
        if (adv_config_done == 0) esp_ble_gap_start_advertising(&adv_params);
        break;
    case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
        adv_config_done &= (~scan_rsp_config_flag);
        if (adv_config_done == 0) esp_ble_gap_start_advertising(&adv_params);
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
            ESP_LOGE(GATTS_TAG, "Advertising start failed");
        else
            ESP_LOGI(GATTS_TAG, "Advertising started");
        break;
    case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
        ESP_LOGI(GATTS_TAG, "conn params: status=%d, conn_int=%d, latency=%d, timeout=%d",
                 param->update_conn_params.status,
                 param->update_conn_params.conn_int,
                 param->update_conn_params.latency,
                 param->update_conn_params.timeout);
        break;
    default:
        break;
    }
}

// ===================== GATT Handler =====================
static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                esp_ble_gatts_cb_param_t *param)
{
    switch (event) {
    case ESP_GATTS_REG_EVT: {
        s_gatts_if = gatts_if;
        ESP_LOGI(GATTS_TAG, "REG_EVT status=%d app_id=%u", param->reg.status, param->reg.app_id);

        ESP_ERROR_CHECK(esp_ble_gap_set_device_name(TEST_DEVICE_NAME));
        adv_config_done |= adv_config_flag;
        ESP_ERROR_CHECK(esp_ble_gap_config_adv_data(&adv_data));
        adv_config_done |= scan_rsp_config_flag;
        ESP_ERROR_CHECK(esp_ble_gap_config_adv_data(&scan_rsp_data));

        // Service anlegen
        esp_gatt_srvc_id_t svc_id = {
            .is_primary = true,
            .id = { .inst_id = 0x00, .uuid = { .len = ESP_UUID_LEN_16, .uuid = { .uuid16 = GATTS_SERVICE_UUID } } }
        };
        ESP_ERROR_CHECK(esp_ble_gatts_create_service(gatts_if, &svc_id, GATTS_NUM_HANDLE));
        break;
    }

    case ESP_GATTS_CREATE_EVT: {
        s_service_handle = param->create.service_handle;
        ESP_LOGI(GATTS_TAG, "CREATE service_handle=%u", s_service_handle);

        ESP_ERROR_CHECK(esp_ble_gatts_start_service(s_service_handle));

        // Characteristic (READ | WRITE | NOTIFY)
        esp_bt_uuid_t cu = { .len = ESP_UUID_LEN_16, .uuid = { .uuid16 = GATTS_CHAR_UUID } };
        esp_gatt_char_prop_t prop = ESP_GATT_CHAR_PROP_BIT_READ |
                                    ESP_GATT_CHAR_PROP_BIT_WRITE |
                                    ESP_GATT_CHAR_PROP_BIT_NOTIFY;

        // Startwert (4 Byte)
        static uint8_t init_val[4] = {0,0,0,0};
        esp_attr_value_t attr = { .attr_max_len = 4, .attr_len = 4, .attr_value = init_val };

        ESP_ERROR_CHECK(esp_ble_gatts_add_char(s_service_handle, &cu,
                                               ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                               prop, &attr, NULL));
        break;
    }

    case ESP_GATTS_ADD_CHAR_EVT: {
        s_char_handle = param->add_char.attr_handle;
        ESP_LOGI(GATTS_TAG, "ADD_CHAR handle=%u", s_char_handle);

        // CCCD hinzufügen
        esp_bt_uuid_t cccd = { .len = ESP_UUID_LEN_16, .uuid = { .uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG } };
        ESP_ERROR_CHECK(esp_ble_gatts_add_char_descr(s_service_handle, &cccd,
                                                     ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                     NULL, NULL));
        break;
    }

    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        s_cccd_handle = param->add_char_descr.attr_handle;
        ESP_LOGI(GATTS_TAG, "ADD_CCCD handle=%u", s_cccd_handle);
        break;

    case ESP_GATTS_CONNECT_EVT: {
        s_conn_id = param->connect.conn_id;
        ESP_LOGI(GATTS_TAG, "CONNECT conn_id=%u  %02X:%02X:%02X:%02X:%02X:%02X",
                 s_conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1],
                 param->connect.remote_bda[2], param->connect.remote_bda[3],
                 param->connect.remote_bda[4], param->connect.remote_bda[5]);

        // sinnvolle Verbindungsparameter
        esp_ble_conn_update_params_t cp = {0};
        memcpy(cp.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        cp.latency = 0; cp.min_int = 0x10; cp.max_int = 0x30; cp.timeout = 400;
        esp_ble_gap_update_conn_params(&cp);
        break;
    }

    case ESP_GATTS_DISCONNECT_EVT:
        ESP_LOGI(GATTS_TAG, "DISCONNECT, restart advertising");
        s_conn_id = 0xFFFF;
        s_notify_enabled = false;
        esp_ble_gap_start_advertising(&adv_params);
        break;

    case ESP_GATTS_READ_EVT: {
        esp_gatt_rsp_t rsp = {0};
        rsp.attr_value.handle = param->read.handle;
        rsp.attr_value.len    = 4;
        rsp.attr_value.value[0]=0xDE; rsp.attr_value.value[1]=0xAD;
        rsp.attr_value.value[2]=0xBE; rsp.attr_value.value[3]=0xEF;
        esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
        break;
    }

    case ESP_GATTS_WRITE_EVT: {
        if (param->write.handle == s_cccd_handle && param->write.len == 2) {
            uint16_t c = (param->write.value[1] << 8) | param->write.value[0];
            s_notify_enabled = (c == 0x0001);
            ESP_LOGI(GATTS_TAG, "CCCD=0x%04X -> notify=%d", c, s_notify_enabled);
        }
        else if (param->write.handle == s_char_handle && param->write.len == 4 && param->write.offset == 0) {
            int32_t v =  ((int32_t)param->write.value[0]) |
                        ((int32_t)param->write.value[1] << 8) |
                        ((int32_t)param->write.value[2] << 16) |
                        ((int32_t)param->write.value[3] << 24);
            (void)xQueueSend(s_rx_queue, &v, 0);
            ESP_LOGI(GATTS_TAG, "RX int=%ld", (long)v);
        }
        if (param->write.need_rsp) {
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
        }
        break;
    }

    default:
        break;
    }
}
