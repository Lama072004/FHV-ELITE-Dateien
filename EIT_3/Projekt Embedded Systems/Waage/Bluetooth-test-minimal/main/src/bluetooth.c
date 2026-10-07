/*
 * ============================================================================
 * BLUETOOTH BLE IMPLEMENTIERUNG (für ESP32-S3)
 * ============================================================================
 * 
 * Hinweis: ESP32-S3 unterstützt NUR BLE, nicht Classic Bluetooth!
 * Daher verwenden wir GATT Notifications statt SPP.
 * 
 * Basiert auf ESP-IDF BLE GATT Server Example
 * ============================================================================
 */

#include "Bluetooth.h"
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_common_api.h"

#define GATTS_TAG "BLE_WAAGE"
#define DEVICE_NAME "ESP32_Waage"

// GATT Service und Characteristic UUIDs
#define GATTS_SERVICE_UUID   0x00FF
#define GATTS_CHAR_UUID      0xFF01
#define GATTS_NUM_HANDLE     4

static uint8_t char_value[4] = {0};
static uint16_t conn_id = 0xFFFF;
static uint16_t gatts_if_store = 0xFF;
static uint16_t char_handle = 0;
static bool connected = false;

// GAP Advertising Data
static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = false,
    .min_interval = 0x0006,
    .max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 0,
    .p_service_uuid = NULL,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

// Advertising Parameters
static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

// GATT Attribute Database - nur benötigte Variable
static const uint8_t char_prop_read_notify = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY;

// Attribute Table
enum {
    IDX_SVC,
    IDX_CHAR_DECL,
    IDX_CHAR_VAL,
    IDX_CHAR_CFG,
    HRS_IDX_NB,
};

// GAP Event Handler
static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        esp_ble_gap_start_advertising(&adv_params);
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        if (param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS) {
            ESP_LOGI(GATTS_TAG, "Advertising gestartet");
        }
        break;
    default:
        break;
    }
}

// GATTS Event Handler
static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    switch (event) {
    case ESP_GATTS_REG_EVT:
        ESP_LOGI(GATTS_TAG, "GATT Server registriert");
        esp_ble_gap_set_device_name(DEVICE_NAME);
        esp_ble_gap_config_adv_data(&adv_data);
        gatts_if_store = gatts_if;
        
        // Service erstellen
        esp_ble_gatts_create_service(gatts_if, &(esp_gatt_srvc_id_t){
            .is_primary = true,
            .id.inst_id = 0x00,
            .id.uuid.len = ESP_UUID_LEN_16,
            .id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID,
        }, GATTS_NUM_HANDLE);
        break;
        
    case ESP_GATTS_CREATE_EVT:
        ESP_LOGI(GATTS_TAG, "Service erstellt, handle %d", param->create.service_handle);
        esp_ble_gatts_start_service(param->create.service_handle);
        
        // Characteristic hinzufügen
        esp_ble_gatts_add_char(param->create.service_handle,
            &(esp_bt_uuid_t){.len = ESP_UUID_LEN_16, .uuid.uuid16 = GATTS_CHAR_UUID},
            ESP_GATT_PERM_READ,
            char_prop_read_notify,
            &(esp_attr_value_t){.attr_max_len = 4, .attr_len = 4, .attr_value = char_value},
            NULL);
        break;
        
    case ESP_GATTS_ADD_CHAR_EVT:
        ESP_LOGI(GATTS_TAG, "Characteristic hinzugefügt, handle %d", param->add_char.attr_handle);
        char_handle = param->add_char.attr_handle;
        break;
        
    case ESP_GATTS_CONNECT_EVT:
        ESP_LOGI(GATTS_TAG, "Client verbunden");
        conn_id = param->connect.conn_id;
        connected = true;
        break;
        
    case ESP_GATTS_DISCONNECT_EVT:
        ESP_LOGI(GATTS_TAG, "Client getrennt");
        connected = false;
        esp_ble_gap_start_advertising(&adv_params);
        break;
        
    default:
        break;
    }
}

// Bluetooth initialisieren
void bluetooth_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "Controller init failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "Controller enable failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(GATTS_TAG, "Bluedroid init failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(GATTS_TAG, "Bluedroid enable failed: %s", esp_err_to_name(ret));
        return;
    }

    esp_ble_gatts_register_callback(gatts_event_handler);
    esp_ble_gap_register_callback(gap_event_handler);
    esp_ble_gatts_app_register(0);
    esp_ble_gatt_set_local_mtu(500);

    ESP_LOGI(GATTS_TAG, "BLE Waage initialisiert");
}

// Daten senden (via BLE Notification)
void bluetooth_send_int(int32_t value)
{
    if (connected && char_handle != 0) {
        memcpy(char_value, &value, sizeof(int32_t));
        esp_ble_gatts_send_indicate(gatts_if_store, conn_id, char_handle, 
                                     sizeof(int32_t), char_value, false);
    }
}
