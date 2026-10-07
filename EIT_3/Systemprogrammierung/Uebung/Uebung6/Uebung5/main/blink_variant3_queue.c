/* Aufgabe 3 - Variante 3: Queue mit KeyEvent
 * Multitasked Blink mit FreeRTOS Queue
 * 
 * Konzept:
 * - Button-Task sendet KeyEvent-Strukturen an Queue
 * - LED-Tasks empfangen Events aus der Queue
 * - Typsichere Kommunikation durch Struktur
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "queue_variant";

// GPIO Configuration
#define BLINK_GPIO CONFIG_BLINK_GPIO
#define BUTTON_LEFT_GPIO  0
#define BUTTON_RIGHT_GPIO 35

// KeyEvent Struktur für Queue-Kommunikation
typedef enum {
    KEY_EVENT_LEFT_PRESSED,
    KEY_EVENT_RIGHT_PRESSED,
    KEY_EVENT_LEFT_RELEASED,
    KEY_EVENT_RIGHT_RELEASED
} KeyEventType;

typedef struct {
    KeyEventType type;
    uint32_t timestamp;      // Zeit des Events
    uint8_t button_id;       // ID des Buttons (0=links, 1=rechts)
} KeyEvent;

// Semaphor und Queue
static SemaphoreHandle_t led_semaphore = NULL;
static QueueHandle_t key_event_queue = NULL;

#define QUEUE_SIZE 10

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

/* Hilfsfunktion: Event-Typ als String */
static const char* event_type_to_string(KeyEventType type)
{
    switch (type) {
        case KEY_EVENT_LEFT_PRESSED: return "LEFT_PRESSED";
        case KEY_EVENT_RIGHT_PRESSED: return "RIGHT_PRESSED";
        case KEY_EVENT_LEFT_RELEASED: return "LEFT_RELEASED";
        case KEY_EVENT_RIGHT_RELEASED: return "RIGHT_RELEASED";
        default: return "UNKNOWN";
    }
}

/* LED Task 0: Liest Events aus Queue */
static void led_task_0(void *pvParameters)
{
    uint8_t state = 0;
    int local_delay = 1000;
    
    ESP_LOGI(TAG, "LED Task 0 started (reading from queue)");
    
    while (1) {
        KeyEvent event;
        
        // Versuche Event aus Queue zu lesen (non-blocking)
        if (xQueueReceive(key_event_queue, &event, 0) == pdTRUE) {
            ESP_LOGI(TAG, "Task 0: Received %s event (button %d) at %lu ms",
                     event_type_to_string(event.type), event.button_id, event.timestamp);
            
            // Reagiere nur auf PRESSED Events
            if (event.type == KEY_EVENT_LEFT_PRESSED) {
                // Verlangsamen
                if (local_delay < 5000) {
                    local_delay += 100;
                    ESP_LOGI(TAG, "Task 0: Slow down -> delay = %d ms", local_delay);
                }
            } else if (event.type == KEY_EVENT_RIGHT_PRESSED) {
                // Beschleunigen
                if (local_delay > 100) {
                    local_delay -= 100;
                    ESP_LOGI(TAG, "Task 0: Speed up -> delay = %d ms", local_delay);
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
    
    ESP_LOGI(TAG, "LED Task 1 started (reading from queue)");
    
    while (1) {
        KeyEvent event;
        
        if (xQueueReceive(key_event_queue, &event, 0) == pdTRUE) {
            ESP_LOGI(TAG, "Task 1: Received %s event (button %d) at %lu ms",
                     event_type_to_string(event.type), event.button_id, event.timestamp);
            
            if (event.type == KEY_EVENT_LEFT_PRESSED) {
                if (local_delay < 5000) {
                    local_delay += 100;
                    ESP_LOGI(TAG, "Task 1: Slow down -> delay = %d ms", local_delay);
                }
            } else if (event.type == KEY_EVENT_RIGHT_PRESSED) {
                if (local_delay > 100) {
                    local_delay -= 100;
                    ESP_LOGI(TAG, "Task 1: Speed up -> delay = %d ms", local_delay);
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
    
    ESP_LOGI(TAG, "LED Task 2 started (reading from queue)");
    
    while (1) {
        KeyEvent event;
        
        if (xQueueReceive(key_event_queue, &event, 0) == pdTRUE) {
            ESP_LOGI(TAG, "Task 2: Received %s event (button %d) at %lu ms",
                     event_type_to_string(event.type), event.button_id, event.timestamp);
            
            if (event.type == KEY_EVENT_LEFT_PRESSED) {
                if (local_delay < 5000) {
                    local_delay += 100;
                    ESP_LOGI(TAG, "Task 2: Slow down -> delay = %d ms", local_delay);
                }
            } else if (event.type == KEY_EVENT_RIGHT_PRESSED) {
                if (local_delay > 100) {
                    local_delay -= 100;
                    ESP_LOGI(TAG, "Task 2: Speed up -> delay = %d ms", local_delay);
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

/* Button Task: Sendet KeyEvents an Queue */
static void button_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Button Task started (will send events to queue)");
    
    uint8_t last_left_state = 1;
    uint8_t last_right_state = 1;
    
    while (1) {
        uint8_t left_state = gpio_get_level(BUTTON_LEFT_GPIO);
        uint8_t right_state = gpio_get_level(BUTTON_RIGHT_GPIO);
        
        // Linker Taster gedrückt
        if (left_state == 0 && last_left_state == 1) {
            KeyEvent event = {
                .type = KEY_EVENT_LEFT_PRESSED,
                .timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS,
                .button_id = 0
            };
            
            // Sende Event an alle Consumer (3x in Queue)
            ESP_LOGI(TAG, "Button: LEFT pressed - sending to queue");
            for (int i = 0; i < 3; i++) {
                if (xQueueSend(key_event_queue, &event, 0) != pdTRUE) {
                    ESP_LOGW(TAG, "Queue full! Event dropped.");
                }
            }
        }
        
        // Linker Taster losgelassen
        if (left_state == 1 && last_left_state == 0) {
            KeyEvent event = {
                .type = KEY_EVENT_LEFT_RELEASED,
                .timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS,
                .button_id = 0
            };
            ESP_LOGI(TAG, "Button: LEFT released");
            xQueueSend(key_event_queue, &event, 0);
        }
        
        // Rechter Taster gedrückt
        if (right_state == 0 && last_right_state == 1) {
            KeyEvent event = {
                .type = KEY_EVENT_RIGHT_PRESSED,
                .timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS,
                .button_id = 1
            };
            
            ESP_LOGI(TAG, "Button: RIGHT pressed - sending to queue");
            for (int i = 0; i < 3; i++) {
                if (xQueueSend(key_event_queue, &event, 0) != pdTRUE) {
                    ESP_LOGW(TAG, "Queue full! Event dropped.");
                }
            }
        }
        
        // Rechter Taster losgelassen
        if (right_state == 1 && last_right_state == 0) {
            KeyEvent event = {
                .type = KEY_EVENT_RIGHT_RELEASED,
                .timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS,
                .button_id = 1
            };
            ESP_LOGI(TAG, "Button: RIGHT released");
            xQueueSend(key_event_queue, &event, 0);
        }
        
        last_left_state = left_state;
        last_right_state = right_state;
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void run_queue_variant(void)
{
    ESP_LOGI(TAG, "\n========================================");
    ESP_LOGI(TAG, "  Variant 3: Queue with KeyEvent");
    ESP_LOGI(TAG, "========================================\n");
    
    configure_led();
    configure_buttons();
    
    // Mutex für LED-Zugriff
    led_semaphore = xSemaphoreCreateMutex();
    
    // Queue für KeyEvents erstellen
    key_event_queue = xQueueCreate(QUEUE_SIZE, sizeof(KeyEvent));
    
    if (led_semaphore == NULL || key_event_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create semaphore or queue!");
        return;
    }
    
    ESP_LOGI(TAG, "Semaphore and Queue created");
    ESP_LOGI(TAG, "- LED Semaphore: Mutex for LED access");
    ESP_LOGI(TAG, "- KeyEvent Queue: Size=%d, Item size=%d bytes\n", 
             QUEUE_SIZE, sizeof(KeyEvent));
    
    // Tasks erstellen
    xTaskCreate(led_task_0, "LED_Task_0", 2048, NULL, 5, NULL);
    xTaskCreate(led_task_1, "LED_Task_1", 2048, NULL, 5, NULL);
    xTaskCreate(led_task_2, "LED_Task_2", 2048, NULL, 5, NULL);
    xTaskCreate(button_task, "Button_Task", 2048, NULL, 6, NULL);
    
    ESP_LOGI(TAG, "All tasks created. System running...");
    ESP_LOGI(TAG, "Press left button to slow down, right button to speed up");
}
