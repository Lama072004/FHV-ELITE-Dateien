#include "DisplayInit.h"
#include "Globals.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "lvgl.h"
#include "esp_lvgl_port.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_vendor.h"
#include <stdio.h>

static const char *TAG = "TetrisDisplay";
lv_disp_t *g_disp = NULL;
static lv_obj_t *label_score = NULL;
static lv_obj_t *label_highscore = NULL;
static lv_obj_t *label_title = NULL;

// Global I2C Bus Handle (for v6.0 API)
static i2c_master_bus_handle_t i2c_bus_handle = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////
// I2C INITIALIZATION (ESP-IDF v6.0 API)
//////////////////////////////////////////////////////////////////////////////////////////////////
static void scan_i2c_devices(void) {
    if (i2c_bus_handle == NULL) {
        ESP_LOGW(TAG, "I2C bus not initialized - skipping device scan");
        return;
    }
    
    ESP_LOGI(TAG, "Scanning I2C bus for devices...");
    uint8_t found_devices = 0;
    
    for (uint8_t addr = 0x08; addr < 0x78; addr++) {
        // v6.0 API: Use i2c_master_probe() instead of cmd_link
        esp_err_t ret = i2c_master_probe(i2c_bus_handle, addr, 50 / portTICK_PERIOD_MS);
        
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "✓ Found I2C device at address: 0x%02X", addr);
            found_devices++;
        }
    }
    
    if (found_devices == 0) {
        ESP_LOGW(TAG, "✗ No I2C devices found! Check wiring: SDA=GPIO%d, SCL=GPIO%d, VCC=3.3V, GND", 
                 I2C_SDA_PIN, I2C_SCL_PIN);
    }
}

static esp_err_t init_i2c(void) {
    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "I2C Display Debug Information");
    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Expected OLED Address: 0x3C (or 0x3D)");
    ESP_LOGI(TAG, "SDA Pin: GPIO%d", I2C_SDA_PIN);
    ESP_LOGI(TAG, "SCL Pin: GPIO%d", I2C_SCL_PIN);
    ESP_LOGI(TAG, "Note: External 4.7kΩ pull-up resistors");
    ESP_LOGI(TAG, "      are REQUIRED between SDA/SCL and 3.3V");
    ESP_LOGI(TAG, "====================================");
    
    // v6.0 API: Create I2C master bus configuration
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = I2C_SCL_PIN,
        .sda_io_num = I2C_SDA_PIN,
        .glitch_ignore_cnt = 7,
        .flags = {
            .enable_internal_pullup = true
        }
    };
    
    // Create I2C master bus
    esp_err_t ret = i2c_new_master_bus(&i2c_bus_config, &i2c_bus_handle);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "I2C master bus creation failed: %s", esp_err_to_name(ret));
        i2c_bus_handle = NULL;
        return ret;
    }
    
    ESP_LOGI(TAG, "✓ I2C initialized: SDA=GPIO%d, SCL=GPIO%d, Freq=50kHz", I2C_SDA_PIN, I2C_SCL_PIN);
    
    // Scan for devices
    vTaskDelay(100 / portTICK_PERIOD_MS);
    scan_i2c_devices();
    
    return ESP_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// DISPLAY INITIALIZATION
//////////////////////////////////////////////////////////////////////////////////////////////////
void display_init(void) {
    // Initialize I2C
    esp_err_t ret = init_i2c();
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "I2C initialization failed - display will not work");
        g_disp = NULL;
        return;
    }

    // Initialize LCD panel IO
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = OLED_I2C_ADDRESS,
        .control_phase_bytes = 1,
        .lcd_cmd_bits = OLED_LCD_CMD_BITS,
        .lcd_param_bits = OLED_LCD_CMD_BITS,
        .dc_bit_offset = 6,
    };
    
    // v6.0 API: Use i2c_bus_handle instead of I2C_HOST
    ret = esp_lcd_new_panel_io_i2c(i2c_bus_handle, &io_config, &io_handle);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Failed to create LCD panel IO: %s - display disabled", esp_err_to_name(ret));
        g_disp = NULL;
        return;
    }

    // Initialize LCD panel
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = OLED_PIN_NUM_RST,
    };

    #if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,3,0))
    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = OLED_LCD_V_RES,
    };
    panel_config.vendor_config = &ssd1306_config;
    #endif

    ret = esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Failed to create SSD1306 panel: %s - display disabled", esp_err_to_name(ret));
        g_disp = NULL;
        return;
    }

    ret = esp_lcd_panel_reset(panel_handle);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Panel reset failed: %s", esp_err_to_name(ret));
    }

    ret = esp_lcd_panel_init(panel_handle);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Panel init failed: %s - display disabled", esp_err_to_name(ret));
        g_disp = NULL;
        return;
    }

    ret = esp_lcd_panel_disp_on_off(panel_handle, true);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Panel disp_on_off failed: %s", esp_err_to_name(ret));
    }

    // Rotate display 180 degrees using panel API
    ret = esp_lcd_panel_mirror(panel_handle, true, true);  // mirror_x=true, mirror_y=true for 180° rotation
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Panel mirror failed: %s", esp_err_to_name(ret));
    }

    // Initialize LVGL
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_port_init(&lvgl_cfg);

    // Initialize LVGL display
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = OLED_LCD_H_RES * OLED_LCD_V_RES,
        .double_buffer = true,
        .hres = OLED_LCD_H_RES,
        .vres = OLED_LCD_V_RES,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,    // Panel rotation already handled by esp_lcd_panel_mirror()
            .mirror_y = false,    // Panel rotation already handled by esp_lcd_panel_mirror()
        }
    };
    g_disp = lvgl_port_add_disp(&disp_cfg);

    // Create UI elements (only if display initialized successfully)
    if (g_disp != NULL && lvgl_port_lock(0)) {
        lv_obj_t *scr = lv_disp_get_scr_act(g_disp);
        
        // Title
        lv_obj_t *label_title = lv_label_create(scr);
        lv_label_set_text(label_title, "TETRIS");
        lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);

        // Current Score
        label_score = lv_label_create(scr);
        lv_label_set_text(label_score, "Score: 0");
        lv_obj_align(label_score, LV_ALIGN_TOP_MID, 0, 20);

        // Highscore
        label_highscore = lv_label_create(scr);
        lv_label_set_text(label_highscore, "High: 0");
        lv_obj_align(label_highscore, LV_ALIGN_TOP_MID, 0, 40);

        lvgl_port_unlock();
        
        ESP_LOGI(TAG, "Display initialized successfully");
    } else {
        ESP_LOGW(TAG, "Display initialization incomplete - continuing without display");
        g_disp = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// UPDATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////
void display_update_score(uint32_t current_score, uint32_t highscore) {
    // KONSISTENTE NULL-CHECK: Display vor Zugriff prüfen (FEHLER BEHOBEN)
    // Vorher: War inconsistent, teilweise ohne Check
    if (!g_disp) return;  // Display not available, skip
    
    if (!lvgl_port_lock(0)) {
        printf("[Display] ERROR: LVGL lock timeout\n");
        return;  // Timeout - vermeide Deadlock
    }
    
    // If labels were removed (e.g. after Game Over), recreate the HUD
    if (label_score == NULL || label_highscore == NULL) {
        lv_obj_t *scr = lv_disp_get_scr_act(g_disp);
        // recreate title and score/highscore labels only if missing
        if (label_title == NULL) {
            label_title = lv_label_create(scr);
            lv_label_set_text(label_title, "TETRIS");
            lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);
        }

        if (label_score == NULL) {
            label_score = lv_label_create(scr);
            lv_label_set_text(label_score, "Score: 0");
            lv_obj_align(label_score, LV_ALIGN_TOP_MID, 0, 20);
        }

        if (label_highscore == NULL) {
            label_highscore = lv_label_create(scr);
            lv_label_set_text(label_highscore, "High: 0");
            lv_obj_align(label_highscore, LV_ALIGN_TOP_MID, 0, 40);
        }
    }
    
    // Update score and highscore labels
    char buf[32];

    // Update current score
    snprintf(buf, sizeof(buf), "Score: %lu", current_score);
    if (label_score != NULL) {
        lv_label_set_text(label_score, buf);
    }

    // Update highscore
    snprintf(buf, sizeof(buf), "High: %lu", highscore);
    if (label_highscore != NULL) {
        lv_label_set_text(label_highscore, buf);
    }

    lvgl_port_unlock();
}

// Create a fresh screen and build the HUD (title / score / highscore)
void display_reset_and_show_hud(uint32_t highscore_val) {
    // KONSISTENTE NULL-CHECK
    if (!g_disp) return;

    if (!lvgl_port_lock(0)) {
        printf("[Display] ERROR: LVGL lock timeout\n");
        return;  // Timeout
    }

    // Create a new screen and load it (removes old objects cleanly)
    lv_obj_t *new_scr = lv_obj_create(NULL);
    lv_disp_load_scr(new_scr);

    // Free pointers logically by creating new ones on new screen
    label_title = lv_label_create(new_scr);
    lv_label_set_text(label_title, "TETRIS");
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 0);

    label_score = lv_label_create(new_scr);
    lv_label_set_text(label_score, "Score: 0");
    lv_obj_align(label_score, LV_ALIGN_TOP_MID, 0, 20);

    label_highscore = lv_label_create(new_scr);
    char buf[32];
    snprintf(buf, sizeof(buf), "High: %lu", highscore_val);
    lv_label_set_text(label_highscore, buf);
    lv_obj_align(label_highscore, LV_ALIGN_TOP_MID, 0, 40);

    lvgl_port_unlock();
}

void display_show_game_over(uint32_t final_score, uint32_t highscore) {
    // KONSISTENTE NULL-CHECK
    if (!g_disp) return;  // Display not available, skip
    
    if (!lvgl_port_lock(0)) {
        printf("[Display] ERROR: LVGL lock timeout\n");
        return;  // Timeout
    }

    lv_obj_t *scr = lv_disp_get_scr_act(g_disp);
    if (scr == NULL) {
        lvgl_port_unlock();
        return;
    }
    
    // Clear screen and create game over message
    lv_obj_clean(scr);
    // mark HUD labels as gone so they get recreated on next update
    label_score = NULL;
    label_highscore = NULL;
    label_title = NULL;
        
    lv_obj_t *label_go = lv_label_create(scr);
    lv_label_set_text(label_go, "GAME OVER");
    lv_obj_align(label_go, LV_ALIGN_CENTER, 0, -20);

    lv_obj_t *label_score_go = lv_label_create(scr);
    char buf[64];
    snprintf(buf, sizeof(buf), "Score: %lu\nHigh: %lu", final_score, highscore);
    lv_label_set_text(label_score_go, buf);
    lv_obj_align(label_score_go, LV_ALIGN_CENTER, 0, 10);
        
    lvgl_port_unlock();
}
