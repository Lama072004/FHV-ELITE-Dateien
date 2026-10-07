/* Aufgabe 3 - Variante 1: Producer/Consumer mit Semaphor
 * Multitasked Blink mit Producer/Consumer Pattern
 * 
 * Konzept:
 * - Button-Task (Producer) erzeugt Events
 * - LED-Tasks (Consumer) konsumieren Events
 * - Synchronisation über Counting Semaphore
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "pc_semaphore";

// GPIO Configuration
#define BLINK_GPIO CONFIG_BLINK_GPIO
#define BUTTON_LEFT_GPIO  0
#define BUTTON_RIGHT_GPIO 35

// Semaphoren
static SemaphoreHandle_t led_semaphore = NULL;      // Schützt LED-Zugriff
static SemaphoreHandle_t event_semaphore = NULL;    // Counting Semaphore für Events

// Delay-Werte
#define MIN_DELAY_MS 100
#define MAX_DELAY_MS 5000
#define DELAY_STEP_MS 100

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

/* LED Task 0: Consumer - wartet auf Events und blinkt entsprechend */
static void led_task_0(void *pvParameters)
{
    uint8_t state = 0;
    int local_delay = 1000;  // Lokale Variable statt globaler
    
    ESP_LOGI(TAG, "LED Task 0 (Consumer) started");
    
    while (1) {
        // Warte auf Event vom Producer
        if (xSemaphoreTake(event_semaphore, 0) == pdTRUE) {
            // Event empfangen - ändere Geschwindigkeit
            local_delay = (local_delay == 1000) ? 500 : 1000;
            ESP_LOGI(TAG, "Task 0: Delay changed to %d ms", local_delay);
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

/* LED Task 1: Consumer */
static void led_task_1(void *pvParameters)
{
    uint8_t state = 0;
    int local_delay = 1000;
    
    ESP_LOGI(TAG, "LED Task 1 (Consumer) started");
    
    while (1) {
        if (xSemaphoreTake(event_semaphore, 0) == pdTRUE) {
            local_delay = (local_delay == 1000) ? 500 : 1000;
            ESP_LOGI(TAG, "Task 1: Delay changed to %d ms", local_delay);
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

/* LED Task 2: Consumer */
static void led_task_2(void *pvParameters)
{
    uint8_t state = 0;
    int local_delay = 1000;
    
    ESP_LOGI(TAG, "LED Task 2 (Consumer) started");
    
    while (1) {
        if (xSemaphoreTake(event_semaphore, 0) == pdTRUE) {
            local_delay = (local_delay == 1000) ? 500 : 1000;
            ESP_LOGI(TAG, "Task 2: Delay changed to %d ms", local_delay);
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

/* Button Task: Producer - erzeugt Events für alle Consumer */
static void button_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Button Task (Producer) started");
    
    uint8_t last_left_state = 1;
    uint8_t last_right_state = 1;
    
    while (1) {
        uint8_t left_state = gpio_get_level(BUTTON_LEFT_GPIO);
        uint8_t right_state = gpio_get_level(BUTTON_RIGHT_GPIO);
        
        // Bei Tastendruck: Signale an ALLE Consumer senden
        if (left_state == 0 && last_left_state == 1) {
            ESP_LOGI(TAG, "Producer: Left button pressed - sending events to all consumers");
            // Gebe Semaphor 3x frei (für 3 Consumer)
            xSemaphoreGive(event_semaphore);
            xSemaphoreGive(event_semaphore);
            xSemaphoreGive(event_semaphore);
        }
        
        if (right_state == 0 && last_right_state == 1) {
            ESP_LOGI(TAG, "Producer: Right button pressed - sending events to all consumers");
            xSemaphoreGive(event_semaphore);
            xSemaphoreGive(event_semaphore);
            xSemaphoreGive(event_semaphore);
        }
        
        last_left_state = left_state;
        last_right_state = right_state;
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void run_producer_consumer_semaphore(void)
{
    ESP_LOGI(TAG, "\n========================================");
    ESP_LOGI(TAG, "  Variant 1: Producer/Consumer with Semaphore");
    ESP_LOGI(TAG, "========================================\n");
    
    configure_led();
    configure_buttons();
    
    // Mutex für LED-Zugriff
    led_semaphore = xSemaphoreCreateMutex();
    
    // Counting Semaphore für Events (Initial: 0)
    event_semaphore = xSemaphoreCreateCounting(10, 0);
    
    if (led_semaphore == NULL || event_semaphore == NULL) {
        ESP_LOGE(TAG, "Failed to create semaphores!");
        return;
    }
    
    ESP_LOGI(TAG, "Semaphores created successfully");
    ESP_LOGI(TAG, "- LED Semaphore: Mutex for LED access");
    ESP_LOGI(TAG, "- Event Semaphore: Counting (max=10) for events\n");
    
    // Tasks erstellen
    xTaskCreate(led_task_0, "LED_Consumer_0", 2048, NULL, 5, NULL);
    xTaskCreate(led_task_1, "LED_Consumer_1", 2048, NULL, 5, NULL);
    xTaskCreate(led_task_2, "LED_Consumer_2", 2048, NULL, 5, NULL);
    xTaskCreate(button_task, "Button_Producer", 2048, NULL, 6, NULL);
    
    ESP_LOGI(TAG, "All tasks created. System running...");
    ESP_LOGI(TAG, "Press buttons to change blink speed (Producer sends to all Consumers)");
}
