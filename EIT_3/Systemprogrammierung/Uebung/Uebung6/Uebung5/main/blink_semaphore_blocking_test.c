/* Aufgabe 1: Multitasked Blink Lock
 * Test des Verhaltens wenn Semaphore nicht freigegeben werden
 * 
 * Dieser Code demonstriert:
 * 1. Ewiges Blockieren (portMAX_DELAY)
 * 2. Blockieren mit Timeout
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static const char *TAG = "semaphore_test";

static SemaphoreHandle_t test_semaphore = NULL;

/* Test 1: Task nimmt Semaphor und gibt ihn NIE wieder frei */
static void blocking_task_eternal(void *pvParameters)
{
    ESP_LOGI(TAG, "Blocking Task (eternal) started");
    
    // Semaphor nehmen
    if (xSemaphoreTake(test_semaphore, portMAX_DELAY) == pdTRUE) {
        ESP_LOGI(TAG, "Blocking Task acquired semaphore - will NEVER release it");
        
        // Simuliere Arbeit
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        
        // *** WICHTIG: Semaphor wird ABSICHTLICH NICHT freigegeben ***
        // xSemaphoreGive(test_semaphore);  // <-- Auskommentiert!
        
        ESP_LOGI(TAG, "Blocking Task done (semaphore NOT released)");
    }
    
    // Task beenden
    vTaskDelete(NULL);
}

/* Test 2: Task versucht Semaphor zu nehmen, der nie freigegeben wird (ewiges Warten) */
static void waiting_task_eternal(void *pvParameters)
{
    ESP_LOGI(TAG, "Waiting Task (eternal) started - will wait FOREVER");
    
    // Versuche Semaphor zu nehmen mit unendlichem Timeout
    ESP_LOGI(TAG, "Trying to acquire semaphore with portMAX_DELAY...");
    if (xSemaphoreTake(test_semaphore, portMAX_DELAY) == pdTRUE) {
        // Dieser Code wird NIE erreicht
        ESP_LOGI(TAG, "SUCCESS: Acquired semaphore");
        xSemaphoreGive(test_semaphore);
    } else {
        // Dieser Code wird auch NIE erreicht bei portMAX_DELAY
        ESP_LOGE(TAG, "FAILED: Could not acquire semaphore");
    }
    
    vTaskDelete(NULL);
}

/* Test 3: Task versucht Semaphor zu nehmen mit Timeout */
static void waiting_task_timeout(void *pvParameters)
{
    ESP_LOGI(TAG, "Waiting Task (timeout) started - will wait max 3 seconds");
    
    // Versuche Semaphor zu nehmen mit 3 Sekunden Timeout
    TickType_t timeout = 3000 / portTICK_PERIOD_MS;
    ESP_LOGI(TAG, "Trying to acquire semaphore with 3000ms timeout...");
    
    TickType_t start = xTaskGetTickCount();
    if (xSemaphoreTake(test_semaphore, timeout) == pdTRUE) {
        // Erfolg
        TickType_t end = xTaskGetTickCount();
        ESP_LOGI(TAG, "SUCCESS: Acquired semaphore after %d ms", 
                 (int)((end - start) * portTICK_PERIOD_MS));
        xSemaphoreGive(test_semaphore);
    } else {
        // Timeout
        TickType_t end = xTaskGetTickCount();
        ESP_LOGW(TAG, "TIMEOUT: Could not acquire semaphore after %d ms", 
                 (int)((end - start) * portTICK_PERIOD_MS));
        ESP_LOGW(TAG, "Task can continue with alternative action or error handling");
    }
    
    vTaskDelete(NULL);
}

/* Szenario 1: Ewiges Blockieren demonstrieren */
void test_semaphore_eternal_block(void)
{
    ESP_LOGI(TAG, "\n=== Test 1: Eternal Blocking ===");
    ESP_LOGI(TAG, "Creating semaphore and two tasks...");
    
    test_semaphore = xSemaphoreCreateMutex();
    
    // Task 1: Nimmt Semaphor und gibt ihn nicht frei
    xTaskCreate(blocking_task_eternal, "Blocker", 2048, NULL, 5, NULL);
    
    // Warte kurz damit Blocker-Task starten kann
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
    // Task 2: Wartet ewig auf Semaphor
    xTaskCreate(waiting_task_eternal, "Waiter_Eternal", 2048, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "Tasks created. Waiter will be STUCK forever!");
    ESP_LOGI(TAG, "Expected: Waiter task will never progress (deadlock)");
    
    // Warte 5 Sekunden um Verhalten zu beobachten
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "After 5 seconds: Waiter task is still blocked!");
}

/* Szenario 2: Timeout-Verhalten demonstrieren */
void test_semaphore_timeout(void)
{
    ESP_LOGI(TAG, "\n=== Test 2: Timeout Behavior ===");
    ESP_LOGI(TAG, "Creating new semaphore...");
    
    // Wenn Semaphor existiert, löschen
    if (test_semaphore != NULL) {
        vSemaphoreDelete(test_semaphore);
    }
    
    test_semaphore = xSemaphoreCreateMutex();
    
    // Task 1: Nimmt Semaphor und gibt ihn nicht frei
    xTaskCreate(blocking_task_eternal, "Blocker2", 2048, NULL, 5, NULL);
    
    // Warte kurz
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
    // Task 2: Wartet mit Timeout auf Semaphor
    xTaskCreate(waiting_task_timeout, "Waiter_Timeout", 2048, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "Tasks created. Waiter will timeout after 3 seconds.");
    ESP_LOGI(TAG, "Expected: Waiter task will continue after timeout");
    
    // Warte 5 Sekunden um Verhalten zu beobachten
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "Test completed. Waiter should have timed out.");
}

/* Hauptfunktion zum Ausführen aller Semaphor-Tests */
void run_semaphore_blocking_tests(void)
{
    ESP_LOGI(TAG, "\n========================================");
    ESP_LOGI(TAG, "  Semaphore Blocking Test Suite");
    ESP_LOGI(TAG, "========================================\n");
    
    // Test 1: Ewiges Blockieren
    test_semaphore_eternal_block();
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    // Test 2: Mit Timeout
    test_semaphore_timeout();
    
    ESP_LOGI(TAG, "\n========================================");
    ESP_LOGI(TAG, "  Test Summary");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Test 1 (Eternal): Demonstrated deadlock situation");
    ESP_LOGI(TAG, "Test 2 (Timeout): Demonstrated graceful timeout handling");
    ESP_LOGI(TAG, "========================================\n");
}
