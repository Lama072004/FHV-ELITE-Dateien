/* Blink Example - Multitasked with FreeRTOS
 * Aufgabe 2: Multitasked Blink
 * - 3 Tasks für 3 LEDs
 * - Semaphor für LED-Zugriff
 * - Button-Task für Geschwindigkeitssteuerung
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "mem_manager_test.h"

static const char *TAG = "multitask_blink";

/* GPIO Configuration */
#define BLINK_GPIO CONFIG_BLINK_GPIO

// GPIO-Pins für Buttons (anpassen je nach Hardware)
#define BUTTON_LEFT_GPIO  0   // Linker Taster
#define BUTTON_RIGHT_GPIO 35  // Rechter Taster

// Semaphoren für Thread-Synchronisation
static SemaphoreHandle_t led_semaphore = NULL;      // Schützt LED-Zugriff
static SemaphoreHandle_t delay_semaphore = NULL;    // Schützt Delay-Variable

// Globale Variable für Blink-Geschwindigkeit (in Millisekunden)
static int blink_delay_ms = 1000;  // Standard: 1 Sekunde

// Mindest- und Maximalwerte für Delay
#define MIN_DELAY_MS 100
#define MAX_DELAY_MS 5000
#define DELAY_STEP_MS 100

static uint8_t s_led_state = 0;

#ifdef CONFIG_BLINK_LED_STRIP

static led_strip_handle_t led_strip;

/* Setzt eine bestimmte LED im Strip (0, 1, oder 2) auf eine Farbe */
static void set_led_pixel(int led_index, uint8_t r, uint8_t g, uint8_t b)
{
    /* Semaphor nehmen für kritische Region */
    if (xSemaphoreTake(led_semaphore, portMAX_DELAY) == pdTRUE) {
        led_strip_set_pixel(led_strip, led_index, r, g, b);
        led_strip_refresh(led_strip);
        /* Semaphor wieder freigeben */
        xSemaphoreGive(led_semaphore);
    }
}

/* Schaltet eine bestimmte LED aus */
static void clear_led_pixel(int led_index)
{
    /* Semaphor nehmen für kritische Region */
    if (xSemaphoreTake(led_semaphore, portMAX_DELAY) == pdTRUE) {
        led_strip_set_pixel(led_strip, led_index, 0, 0, 0);
        led_strip_refresh(led_strip);
        /* Semaphor wieder freigeben */
        xSemaphoreGive(led_semaphore);
    }
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Configuring addressable LED strip with 3 LEDs!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 3, // 3 LEDs für 3 Tasks
    };
#if CONFIG_BLINK_LED_STRIP_BACKEND_RMT
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
#elif CONFIG_BLINK_LED_STRIP_BACKEND_SPI
    led_strip_spi_config_t spi_config = {
        .spi_bus = SPI2_HOST,
        .flags.with_dma = true,
    };
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
#else
#error "unsupported LED strip backend"
#endif
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

#elif CONFIG_BLINK_LED_GPIO

/* Für GPIO-LEDs: Einfachere Variante ohne LED-Strip */
static void set_led_pixel(int led_index, uint8_t r, uint8_t g, uint8_t b)
{
    /* Bei GPIO wird nur eine LED unterstützt */
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
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

#else
#error "unsupported LED type"
#endif

/* Button-Konfiguration */
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
    ESP_LOGI(TAG, "Buttons configured on GPIO %d and %d", BUTTON_LEFT_GPIO, BUTTON_RIGHT_GPIO);
}

/* Task 1: Blinkt LED 0 (Rot) */
static void led_task_0(void *pvParameters)
{
    uint8_t state = 0;
    ESP_LOGI(TAG, "LED Task 0 started");
    
    while (1) {
        /* Aktuellen Delay-Wert lesen (geschützt durch Semaphor) */
        int current_delay;
        if (xSemaphoreTake(delay_semaphore, portMAX_DELAY) == pdTRUE) {
            current_delay = blink_delay_ms;
            xSemaphoreGive(delay_semaphore);
        }
        
        /* LED schalten */
        if (state) {
            set_led_pixel(0, 16, 0, 0);  // Rot
        } else {
            clear_led_pixel(0);
        }
        
        state = !state;
        vTaskDelay(current_delay / portTICK_PERIOD_MS);
    }
}

/* Task 2: Blinkt LED 1 (Grün) */
static void led_task_1(void *pvParameters)
{
    uint8_t state = 0;
    ESP_LOGI(TAG, "LED Task 1 started");
    
    while (1) {
        /* Aktuellen Delay-Wert lesen (geschützt durch Semaphor) */
        int current_delay;
        if (xSemaphoreTake(delay_semaphore, portMAX_DELAY) == pdTRUE) {
            current_delay = blink_delay_ms;
            xSemaphoreGive(delay_semaphore);
        }
        
        /* LED schalten */
        if (state) {
            set_led_pixel(1, 0, 16, 0);  // Grün
        } else {
            clear_led_pixel(1);
        }
        
        state = !state;
        vTaskDelay(current_delay / portTICK_PERIOD_MS);
    }
}

/* Task 3: Blinkt LED 2 (Blau) */
static void led_task_2(void *pvParameters)
{
    uint8_t state = 0;
    ESP_LOGI(TAG, "LED Task 2 started");
    
    while (1) {
        /* Aktuellen Delay-Wert lesen (geschützt durch Semaphor) */
        int current_delay;
        if (xSemaphoreTake(delay_semaphore, portMAX_DELAY) == pdTRUE) {
            current_delay = blink_delay_ms;
            xSemaphoreGive(delay_semaphore);
        }
        
        /* LED schalten */
        if (state) {
            set_led_pixel(2, 0, 0, 16);  // Blau
        } else {
            clear_led_pixel(2);
        }
        
        state = !state;
        vTaskDelay(current_delay / portTICK_PERIOD_MS);
    }
}

/* Task 4: Überwacht Taster und steuert Blink-Geschwindigkeit */
static void button_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Button Task started");
    
    uint8_t last_left_state = 1;   // Pull-up: High wenn nicht gedrückt
    uint8_t last_right_state = 1;
    
    while (1) {
        /* Taster-Zustände lesen */
        uint8_t left_state = gpio_get_level(BUTTON_LEFT_GPIO);
        uint8_t right_state = gpio_get_level(BUTTON_RIGHT_GPIO);
        
        /* Linker Taster: Verlangsamen (Delay erhöhen) */
        if (left_state == 0 && last_left_state == 1) {  // Fallende Flanke
            if (xSemaphoreTake(delay_semaphore, portMAX_DELAY) == pdTRUE) {
                if (blink_delay_ms < MAX_DELAY_MS) {
                    blink_delay_ms += DELAY_STEP_MS;
                    ESP_LOGI(TAG, "Slowing down: delay = %d ms", blink_delay_ms);
                }
                xSemaphoreGive(delay_semaphore);
            }
        }
        
        /* Rechter Taster: Beschleunigen (Delay verringern) */
        if (right_state == 0 && last_right_state == 1) {  // Fallende Flanke
            if (xSemaphoreTake(delay_semaphore, portMAX_DELAY) == pdTRUE) {
                if (blink_delay_ms > MIN_DELAY_MS) {
                    blink_delay_ms -= DELAY_STEP_MS;
                    ESP_LOGI(TAG, "Speeding up: delay = %d ms", blink_delay_ms);
                }
                xSemaphoreGive(delay_semaphore);
            }
        }
        
        last_left_state = left_state;
        last_right_state = right_state;
        
        /* Polling-Intervall */
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting Multitasked Blink Example");
    
    /* Zuerst: Memory Manager Tests ausführen */
    ESP_LOGI(TAG, "Running Memory Manager Tests...");
    run_all_mem_tests();
    ESP_LOGI(TAG, "Memory Manager Tests completed\n");
    
    /* Hardware konfigurieren */
    configure_led();
    configure_buttons();
    
    /* Semaphoren erstellen */
    led_semaphore = xSemaphoreCreateMutex();
    delay_semaphore = xSemaphoreCreateMutex();
    
    if (led_semaphore == NULL || delay_semaphore == NULL) {
        ESP_LOGE(TAG, "Failed to create semaphores!");
        return;
    }
    
    ESP_LOGI(TAG, "Semaphores created successfully");
    
    /* Tasks erstellen */
    xTaskCreate(led_task_0, "LED_Task_0", 2048, NULL, 5, NULL);
    xTaskCreate(led_task_1, "LED_Task_1", 2048, NULL, 5, NULL);
    xTaskCreate(led_task_2, "LED_Task_2", 2048, NULL, 5, NULL);
    xTaskCreate(button_task, "Button_Task", 2048, NULL, 6, NULL);
    
    ESP_LOGI(TAG, "All tasks created. System running...");
    ESP_LOGI(TAG, "Press left button to slow down, right button to speed up");
}
