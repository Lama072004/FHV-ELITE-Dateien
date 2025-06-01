static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = false,
    .min_interval = 0x20,
    .max_interval = 0x40,
    .appearance = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 0,
    .p_service_uuid = NULL,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static uint8_t pulse_value[2] = {0x06, 60};
static esp_attr_value_t pulse_attr = {
    .attr_max_len = 2,
    .attr_len     = 2,
    .attr_value   = pulse_value,
};

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch(event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        esp_ble_gap_adv_start(&adv_params);
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(TAG, "Advertising start failed");
        } else {
            ESP_LOGI(TAG, "Advertising started");
        }
        break;
    default:
        break;
    }
}

static void gatts_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    switch (event) {
    case ESP_GATTS_REG_EVT:
        ESP_LOGI(TAG, "GATT server register, status %d, app_id %d", param->reg.status, param->reg.app_id);
        gl_profile_tab[PROFILE_APP_IDX].service_id.is_primary = true;
        gl_profile_tab[PROFILE_APP_IDX].service_id.id.inst_id = SVC_INST_ID;
        gl_profile_tab[PROFILE_APP_IDX].service_id.id.uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_APP_IDX].service_id.id.uuid.uuid.uuid16 = UUID_HEART_RATE_SERVICE;
        esp_ble_gap_set_device_name(DEVICE_NAME);
        esp_ble_gap_config_adv_data(&adv_data);
        esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_APP_IDX].service_id, 4);
        break;
    case ESP_GATTS_CREATE_EVT:
        gl_profile_tab[PROFILE_APP_IDX].service_handle = param->create.service_handle;
        gl_profile_tab[PROFILE_APP_IDX].char_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_APP_IDX].char_uuid.uuid.uuid16 = UUID_HEART_RATE_MEASUREMENT;
        esp_ble_gatts_start_service(gl_profile_tab[PROFILE_APP_IDX].service_handle);
        gl_profile_tab[PROFILE_APP_IDX].property = ESP_GATT_CHAR_PROP_BIT_NOTIFY;
        esp_ble_gatts_add_char(gl_profile_tab[PROFILE_APP_IDX].service_handle, &gl_profile_tab[PROFILE_APP_IDX].char_uuid,
                               ESP_GATT_PERM_READ, gl_profile_tab[PROFILE_APP_IDX].property, &pulse_attr, NULL);
        break;
    case ESP_GATTS_ADD_CHAR_EVT:
        gl_profile_tab[PROFILE_APP_IDX].char_handle = param->add_char.attr_handle;
        break;
    case ESP_GATTS_CONNECT_EVT:
        gl_profile_tab[PROFILE_APP_IDX].conn_id = param->connect.conn_id;
        ESP_LOGI(TAG, "Device connected");
        break;
    case ESP_GATTS_DISCONNECT_EVT:
        ESP_LOGI(TAG, "Device disconnected");
        esp_ble_gap_start_advertising(&adv_params);
        break;
    default:
        break;
    }
}

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    if (event == ESP_GATTS_REG_EVT) {
        if (param->reg.status == ESP_GATT_OK) {
            gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
            gl_profile_tab[param->reg.app_id].gatts_cb = gatts_profile_event_handler;
        } else {
            ESP_LOGI(TAG, "Reg app failed, app_id %04x, status %d", param->reg.app_id, param->reg.status);
            return;
        }
    }
    for (int idx = 0; idx < PROFILE_NUM; idx++) {
        if (gatts_if == ESP_GATT_IF_NONE || gatts_if == gl_profile_tab[idx].gatts_if) {
            if (gl_profile_tab[idx].gatts_cb) {
                gl_profile_tab[idx].gatts_cb(event, gatts_if, param);
            }
        }
    }
}

void ble_init(void)
{
    esp_err_t ret;
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_ERROR_CHECK(esp_ble_gap_register_callback(gap_event_handler));
    ESP_ERROR_CHECK(esp_ble_gatts_register_callback(gatts_event_handler));
    ESP_ERROR_CHECK(esp_ble_gatts_app_register(ESP_APP_ID));
    esp_ble_gap_set_device_name(DEVICE_NAME);
    esp_ble_gap_config_adv_data(&adv_data);
}

void ble_start_notify_task(void) {
    // You can implement a FreeRTOS task to update pulse_value and call esp_ble_gatts_set_attr_value()
}

void ble_notify_values(uint8_t pulse, float spo2) {
    // Update value and send notification
    pulse_value[1] = pulse;
    esp_ble_gatts_set_attr_value(gl_profile_tab[PROFILE_APP_IDX].char_handle, 2, pulse_value);
    esp_ble_gatts_send_indicate(gl_profile_tab[PROFILE_APP_IDX].gatts_if, gl_profile_tab[PROFILE_APP_IDX].conn_id,
                                gl_profile_tab[PROFILE_APP_IDX].char_handle, 2, pulse_value, false);
}