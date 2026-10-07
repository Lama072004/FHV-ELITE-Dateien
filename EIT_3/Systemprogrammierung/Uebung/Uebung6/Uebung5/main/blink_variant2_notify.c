/* Aufgabe 3 - Variante 2: Event Notification
 * Multitasked Blink mit Task Notifications
 * 
 * Konzept:
 * - Button-Task sendet Notifications direkt an LED-Tasks
 * - Verwendet xTaskNotify() und xTaskNotifyWait()
 * - Keine Semaphore oder Queues für Event-Kommunikation
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "event_notify";

// GPIO Configuration
#define BLINK_GPIO CONFIG_BLINK_GPIO
#define BUTTON_LEFT_GPIO  0
#define BUTTON_RIGHT_GPIO 35

// Semaphor nur für LED-Zugriff
static SemaphoreHandle_t led_semaphore = NULL;

// Task Handles für Notifications
static TaskHandle_t led_task_0_handle = NULL;
static TaskHandle_t led_task_1_handle = NULL;
static TaskHandle_t led_task_2_handle = NULL;

// Event-Typen als Notification Values
#define EVENT_SPEED_UP    0x01
#define EVENT_SLOW_DOWN   0x02

#ifdef CONFIG_BLINK_LED_STRIP
static led_strip_handle_t led_strip;

static void set_led_pixel(int led_index, uint8_t r, uint8_t g, uint8_t b)
{
    if (xSemaphoreTake(led_semaphore, portMAX_DELAY) == pdTRUE) {
        led_strip_set_pixel(led_strip, led_index, r, g, b);
        led_strip_refresh(led_strip);
        xSemaphoreGive(led_semaphore);
    }
}

static void clear_led_pixel(int led_index)
{
    if (xSemaphoreTake(led_semaphore, portMAX_DELAY) == pdTRUE) {
        led_strip_set_pixel(led_strip, led_index, 0, 0, 0);
        led_strip_refresh(led_strip);
        xSemaphoreGive(led_semaphore);
    }
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Configuring LED strip");
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 3,
    };
#if CONFIG_BLINK_LED_STRIP_BACKEND_RMT
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
#elif CONFIG_BLINK_LED_STRIP_BACKEND_SPI
    led_strip_spi_config_t spi_config = {
        .spi_bus = SPI2_HOST,
        .flags.with_dma = true,
    };
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
#endif
    led_strip_clear(led_strip);
}
#elif CONFIG_BLINK_LED_GPIO
static void set_led_pixel(int led_index, uint8_t r, uint8_t g, uint8_t b)
{
    if (led_index == 0) {
        if (xSemaphoreTake(led_semaphore, portMAX_DELAY) == pdTRUE) {
            gpio_set_level(BLINK_GPIO, (r > 0 || g > 0 || b > 0) ? 1 : 0);
            xSemaphoreGive(led_semaphore);
        }
    }
}

static void clear_led_pixel(int led_index)
{
    if (led_index == 0) {
        if (xSemaphoreTake(led_semaphore, portMAX_DELAY) == pdTRUE) {
            gpio_set_level(BLINK_GPIO, 0);
            xSemaphoreGive(led_semaphore);
        }
    }
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Configuring GPIO LED");
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}
#endif

static void configure_buttons(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BUTTON_LEFT_GPIO) | (1ULL << BUTTON_RIGHT_GPIO),
        .pull_down_en = 0,
        .pull_up_en = 1,
    };
    gpio_config(&io_conf);
}

/* LED Task 0: Wartet auf Notifications */
static void led_task_0(void *pvParameters)
{
    uint8_t state = 0;
    int local_delay = 1000;
    
    ESP_LOGI(TAG, "LED Task 0 started (waiting for notifications)");
    
    while (1) {
        uint32_t notification_value;
        
        // Warte auf Notification (mit 0 Timeout = non-blocking)
        if (xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, 0) == pdTRUE) {
            // Notification empfangen
            if (notification_value & EVENT_SPEED_UP) {
                if (local_delay > 100) {
                    local_delay -= 100;
                    ESP_LOGI(TAG, "Task 0: Speed up -> delay = %d ms", local_delay);
                }
            }
            if (notification_value & EVENT_SLOW_DOWN) {
                if (local_delay < 5000) {
                    local_delay += 100;
                    ESP_LOGI(TAG, "Task 0: Slow down -> delay = %d ms", local_delay);
                }
            }
        }
        
        // LED blinken
        if (state) {
            set_led_pixel(0, 16, 0, 0);  // Rot
        } else {
            clear_led_pixel(0);
        }
        
        state = !state;
        vTaskDelay(local_delay / portTICK_PERIOD_MS);
    }
}

/* LED Task 1 */
static void led_task_1(void *pvParameters)
{
    uint8_t state = 0;
    int local_delay = 1000;
    
    ESP_LOGI(TAG, "LED Task 1 started (waiting for notifications)");
    
    while (1) {
        uint32_t notification_value;
        
        if (xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, 0) == pdTRUE) {
            if (notification_value & EVENT_SPEED_UP) {
                if (local_delay > 100) {
                    local_delay -= 100;
                    ESP_LOGI(TAG, "Task 1: Speed up -> delay = %d ms", local_delay);
                }
            }
            if (notification_value & EVENT_SLOW_DOWN) {
                if (local_delay < 5000) {
                    local_delay += 100;
                    ESP_LOGI(TAG, "Task 1: Slow down -> delay = %d ms", local_delay);
                }
            }
        }
        
        if (state) {
            set_led_pixel(1, 0, 16, 0);  // Grün
        } else {
            clear_led_pixel(1);
        }
        
        state = !state;
        vTaskDelay(local_delay / portTICK_PERIOD_MS);
    }
}

/* LED Task 2 */
static void led_task_2(void *pvParameters)
{
    uint8_t state = 0;
    int local_delay = 1000;
    
    ESP_LOGI(TAG, "LED Task 2 started (waiting for notifications)");
    
    while (1) {
        uint32_t notification_value;
        
        if (xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, 0) == pdTRUE) {
            if (notification_value & EVENT_SPEED_UP) {
                if (local_delay > 100) {
                    local_delay -= 100;
                    ESP_LOGI(TAG, "Task 2: Speed up -> delay = %d ms", local_delay);
                }
            }
            if (notification_value & EVENT_SLOW_DOWN) {
                if (local_delay < 5000) {
                    local_delay += 100;
                    ESP_LOGI(TAG, "Task 2: Slow down -> delay = %d ms", local_delay);
                }
            }
        }
        
        if (state) {
            set_led_pixel(2, 0, 0, 16);  // Blau
        } else {
            clear_led_pixel(2);
        }
        
        state = !state;
        vTaskDelay(local_delay / portTICK_PERIOD_MS);
    }
}

/* Button Task: Sendet Notifications an LED-Tasks */
static void button_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Button Task started (will send notifications)");
    
    uint8_t last_left_state = 1;
    uint8_t last_right_state = 1;
    
    while (1) {
        uint8_t left_state = gpio_get_level(BUTTON_LEFT_GPIO);
        uint8_t right_state = gpio_get_level(BUTTON_RIGHT_GPIO);
        
        // Linker Taster: Verlangsamen
        if (left_state == 0 && last_left_state == 1) {
            ESP_LOGI(TAG, "Button: Sending SLOW_DOWN notification to all tasks");
            xTaskNotify(led_task_0_handle, EVENT_SLOW_DOWN, eSetBits);
            xTaskNotify(led_task_1_handle, EVENT_SLOW_DOWN, eSetBits);
            xTaskNotify(led_task_2_handle, EVENT_SLOW_DOWN, eSetBits);
        }
        
        // Rechter Taster: Beschleunigen
        if (right_state == 0 && last_right_state == 1) {
            ESP_LOGI(TAG, "Button: Sending SPEED_UP notification to all tasks");
            xTaskNotify(led_task_0_handle, EVENT_SPEED_UP, eSetBits);
            xTaskNotify(led_task_1_handle, EVENT_SPEED_UP, eSetBits);
            xTaskNotify(led_task_2_handle, EVENT_SPEED_UP, eSetBits);
        }
        
        last_left_state = left_state;
        last_right_state = right_state;
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void run_event_notification_variant(void)
{
    ESP_LOGI(TAG, "\n========================================");
    ESP_LOGI(TAG, "  Variant 2: Event Notification");
    ESP_LOGI(TAG, "========================================\n");
    
    configure_led();
    configure_buttons();
    
    // Nur Mutex für LED-Zugriff
    led_semaphore = xSemaphoreCreateMutex();
    
    if (led_semaphore == NULL) {
        ESP_LOGE(TAG, "Failed to create semaphore!");
        return;
    }
    
    ESP_LOGI(TAG, "LED Semaphore created");
    ESP_LOGI(TAG, "Using xTaskNotify() for event communication\n");
    
    // Tasks erstellen und Handles speichern
    xTaskCreate(led_task_0, "LED_Task_0", 2048, NULL, 5, &led_task_0_handle);
    xTaskCreate(led_task_1, "LED_Task_1", 2048, NULL, 5, &led_task_1_handle);
    xTaskCreate(led_task_2, "LED_Task_2", 2048, NULL, 5, &led_task_2_handle);
    xTaskCreate(button_task, "Button_Task", 2048, NULL, 6, NULL);
    
    ESP_LOGI(TAG, "All tasks created. System running...");
    ESP_LOGI(TAG, "Press left button to slow down, right button to speed up");
}
