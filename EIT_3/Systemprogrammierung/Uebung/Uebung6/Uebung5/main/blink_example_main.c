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
#include "blink_semaphore_blocking_test.h"
#include "blink_variants.h"

/* Konfiguration: Welche Variante soll ausgeführt werden?
 * Setzen Sie genau EINE der folgenden Defines auf 1:
 */
#define RUN_ORIGINAL_VERSION 0           // Original Multitasked Blink (Übung 5)
#define RUN_SEMAPHORE_BLOCKING_TEST 0    // Aufgabe 1: Semaphor Blocking Tests
#define RUN_VARIANT_1_SEMAPHORE 0        // Aufgabe 3, Variante 1: Producer/Consumer
#define RUN_VARIANT_2_NOTIFY 0           // Aufgabe 3, Variante 2: Task Notifications
#define RUN_VARIANT_3_QUEUE 1            // Aufgabe 3, Variante 3: Queue (DEFAULT)

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
    ESP_LOGI(TAG, "\n========================================");
    ESP_LOGI(TAG, "  Übung 6 - Systemprogrammierung");
    ESP_LOGI(TAG, "========================================\n");
    
    /* Zuerst: Memory Manager Tests ausführen (Aufgabe 2) */
    ESP_LOGI(TAG, "Running Memory Manager Tests (Aufgabe 2)...");
    run_all_mem_tests();
    ESP_LOGI(TAG, "Memory Manager Tests completed\n");
    
    /* Verzögerung zwischen Tests */
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    /* Wähle welche Variante ausgeführt werden soll */
#if RUN_ORIGINAL_VERSION
    ESP_LOGI(TAG, "Starting ORIGINAL Multitasked Blink (Übung 5)");
    
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
    
#elif RUN_SEMAPHORE_BLOCKING_TEST
    ESP_LOGI(TAG, "Starting SEMAPHORE BLOCKING TESTS (Aufgabe 1)");
    run_semaphore_blocking_tests();
    
#elif RUN_VARIANT_1_SEMAPHORE
    ESP_LOGI(TAG, "Starting VARIANT 1 - Producer/Consumer with Semaphore (Aufgabe 3.1)");
    run_producer_consumer_semaphore();
    
#elif RUN_VARIANT_2_NOTIFY
    ESP_LOGI(TAG, "Starting VARIANT 2 - Event Notification (Aufgabe 3.2)");
    run_event_notification_variant();
    
#elif RUN_VARIANT_3_QUEUE
    ESP_LOGI(TAG, "Starting VARIANT 3 - Queue with KeyEvent (Aufgabe 3.3)");
    run_queue_variant();
    
#else
    #error "Please select exactly ONE variant to run by setting the corresponding define to 1"
#endif
}
