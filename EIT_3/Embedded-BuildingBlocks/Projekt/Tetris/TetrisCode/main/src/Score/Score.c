#include "Score.h"
#include "Globals.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <stdio.h>

static int score = 0;
static uint32_t total_lines_cleared = 0;
static uint32_t highscore = 0;
// avoid name clash with deprecated typedef 'nvs_handle' in nvs.h
static nvs_handle_t s_nvs_handle = 0;
static bool s_nvs_initialized = false;

extern SemaphoreHandle_t score_semaphore;

#define NVS_NAMESPACE "tetris"
#define NVS_KEY_HIGHSCORE "highscore"

void score_init(void) {
    score = 0;
    total_lines_cleared = 0;
}

void score_add_lines(int lines) {
    // HINWEIS: Wird bereits durch Grid.c mit score_semaphore geschützt
    total_lines_cleared += lines;  // Track total lines
    switch(lines) {
        case 1: score += 100; break;
        case 2: score += 300; break;
        case 3: score += 500; break;
        case 4: score += 800; break;
        default: score += (lines * 300); break;
    }
}

int score_get(void) {
    return score;
}

uint32_t score_get_total_lines_cleared(void) {
    return total_lines_cleared;
}

void score_load_highscore(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &s_nvs_handle);
    if (err != ESP_OK) {
        printf("[Score] Error opening NVS: %s\n", esp_err_to_name(err));
        highscore = 0;
        return;
    }

    s_nvs_initialized = true;

    // Read highscore
    err = nvs_get_u32(s_nvs_handle, NVS_KEY_HIGHSCORE, &highscore);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("[Score] No highscore found, initializing to 0\n");
        highscore = 0;
    } else if (err != ESP_OK) {
        printf("[Score] Error reading highscore: %s\n", esp_err_to_name(err));
        highscore = 0;
    } else {
        printf("[Score] Highscore loaded: %lu\n", highscore);
    }
}

uint32_t score_get_highscore(void) {
    return highscore;
}

void score_update_highscore(void) {
    if (score > highscore) {
        highscore = score;
        if (s_nvs_initialized && s_nvs_handle != 0) {
            esp_err_t err = nvs_set_u32(s_nvs_handle, NVS_KEY_HIGHSCORE, highscore);
            if (err == ESP_OK) {
                err = nvs_commit(s_nvs_handle);
                if (err == ESP_OK) {
                    printf("[Score] New highscore saved: %lu\n", highscore);
                } else {
                    printf("[Score] Error committing highscore: %s\n", esp_err_to_name(err));
                }
            } else {
                printf("[Score] Error writing highscore: %s\n", esp_err_to_name(err));
            }
        }
    }
}

// CLEANUP-FUNKTION: NVS Handle korrekt schließen (verhindert Memory Leak)
void score_cleanup(void) {
    if (s_nvs_initialized && s_nvs_handle != 0) {
        nvs_close(s_nvs_handle);
        s_nvs_handle = 0;
        s_nvs_initialized = false;
        printf("[Score] NVS handle closed\n");
    }
}
