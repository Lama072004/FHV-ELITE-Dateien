/**
 * @file Controls.c
 * @brief Button-Input-System mit ISR und Event-Queue
 * 
 * Dieses Modul implementiert ein interrupt-basiertes Button-System:
 * - GPIO-ISR erkennt Button-Presses (negative Flanke)
 * - Events werden in FreeRTOS Queue gepusht
 * - Software-Debouncing verhindert Prellen
 * - Polling-Funktionen für direkten GPIO-Zugriff
 */

#include "Controls.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdint.h>
#include <stdbool.h>
#include "Globals.h"
#include "freertos/queue.h"
#include <stdio.h>

// ============================================================================
// PRIVATE VARIABLEN
// ============================================================================

/** @brief FreeRTOS Queue für Button-Events aus ISR (Kapazität: 8 Events) */
static QueueHandle_t s_button_queue = NULL;

// ============================================================================
// HELPER FUNKTIONEN (REDUNDANZEN ELIMINATED)
// ============================================================================

/**
 * @brief Helper-Funktion: Button-Name für Logging ermitteln
 * 
 * REDUNDANZ ELIMINATED: Button-Name Lookup war vorher in mehreren
 * Funktionen wiederholt. Jetzt konsolidiert in EINER Stelle!
 */
static const char* get_button_name(gpio_num_t gpio) {
    if (gpio == BTN_LEFT) return "LEFT";
    else if (gpio == BTN_RIGHT) return "RIGHT";
    else if (gpio == BTN_ROTATE) return "ROTATE";
    else if (gpio == BTN_FASTER) return "FASTER";
    else return "UNKNOWN";
}

// ============================================================================
// BUTTON INPUT FUNKTIONEN
// ============================================================================

/**
 * @brief ISR-Handler für Button-Events (läuft im Interrupt-Kontext)
 * 
 * Dieser Handler wird bei jedem Button-Press getriggert und läuft im IRAM
 * für minimale Latenz. Die Verarbeitung ist bewusst minimal:
 * - GPIO-Nummer in Queue pushen
 * - Context Switch triggern falls höher-prioritärer Task wartet
 * 
 * WICHTIG: Keine zeitintensiven Operationen oder Blocking-Calls hier!
 * Die eigentliche Debouncing-Logik erfolgt im Task-Kontext.
 * 
 * @param arg GPIO-Nummer (cast von void*)
 */
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    // Queue-Check (sollte nie NULL sein nach init_controls)
    if (s_button_queue == NULL) return;
    
    // GPIO-Nummer aus Argument extrahieren
    gpio_num_t g = (gpio_num_t)(intptr_t)arg;
    
    // Flag für Context Switch
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // Event in Queue pushen (non-blocking ISR-Version)
    xQueueSendFromISR(s_button_queue, &g, &xHigherPriorityTaskWoken);
    
    // Context Switch durchführen falls höher-prioritärer Task geweckt wurde
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

/**
 * @brief Initialisiert das Button-Control-System
 * 
 * Diese Funktion richtet die GPIO-Pins für die 4 Buttons ein:
 * 1. GPIO-Konfiguration (Input, Pull-up, Interrupt bei negativer Flanke)
 * 2. Event-Queue erstellen für ISR-to-Task Kommunikation
 * 3. ISR-Service installieren
 * 4. ISR-Handler für jeden Button registrieren
 * 
 * Die Buttons sind aktiv-LOW (gedrückt = 0V, nicht gedrückt = 3.3V via Pull-up)
 * ISR wird bei negativer Flanke (Button-Press) getriggert
 */
void init_controls(void) {
    // GPIO-Konfiguration für alle 4 Buttons
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BTN_LEFT) | (1ULL << BTN_RIGHT) |
                        (1ULL << BTN_ROTATE) | (1ULL << BTN_FASTER),
        .mode = GPIO_MODE_INPUT,              // Input-Modus
        .pull_up_en = GPIO_PULLUP_ENABLE,     // Pull-up aktivieren (Button aktiv-LOW)
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE        // Interrupt bei fallender Flanke (3.3V → 0V)
    };
    gpio_config(&io_conf);

    // FreeRTOS Queue für Button-Events erstellen (8 Events Kapazität)
    if (s_button_queue == NULL) {
        s_button_queue = xQueueCreate(8, sizeof(gpio_num_t));
    }

    // GPIO ISR-Service installieren (globaler Service für alle GPIOs)
    gpio_install_isr_service(0);
    
    // ISR-Handler für jeden Button registrieren
    // Der Handler läuft im IRAM (schneller Zugriff) und pusht GPIO-Nummer in Queue
    gpio_isr_handler_add(BTN_LEFT, gpio_isr_handler, (void*)(intptr_t)BTN_LEFT);
    gpio_isr_handler_add(BTN_RIGHT, gpio_isr_handler, (void*)(intptr_t)BTN_RIGHT);
    gpio_isr_handler_add(BTN_ROTATE, gpio_isr_handler, (void*)(intptr_t)BTN_ROTATE);
    gpio_isr_handler_add(BTN_FASTER, gpio_isr_handler, (void*)(intptr_t)BTN_FASTER);
}

/**
 * @brief Prüft ob ein Button aktuell gedrückt ist (Polling-Methode)
 * 
 * Diese Funktion liest den GPIO-Pin direkt aus und implementiert
 * Software-Debouncing um Button-Prellen zu verhindern.
 * 
 * Debouncing-Logik:
 * - Letzter Press-Zeitpunkt wird pro GPIO gespeichert
 * - Neuer Press wird nur akzeptiert wenn BUTTON_DEBOUNCE_MS vergangen
 * - Verhindert mehrfache Events durch mechanisches Prellen
 * 
 * @param gpio GPIO-Nummer des zu prüfenden Buttons
 * @return true wenn Button gedrückt und Debounce-Zeit verstrichen
 * @return false wenn Button nicht gedrückt oder innerhalb Debounce-Zeit
 */
bool check_button_pressed(gpio_num_t gpio) {
    // Statisches Array für letzten Press-Zeitpunkt pro GPIO
    // (bleibt zwischen Funktionsaufrufen erhalten)
    static uint32_t last_pressed_time[GPIO_NUM_MAX] = {0};
    
    // Button ist aktiv-LOW (gedrückt = 0V)
    if (gpio_get_level(gpio) == 0) {
        // Aktuelle Zeit in Millisekunden
        uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
        
        // Debounce-Check: Ist genug Zeit seit letztem Press vergangen?
        if (now - last_pressed_time[gpio] > BUTTON_DEBOUNCE_MS) {
            // Zeitstempel aktualisieren
            last_pressed_time[gpio] = now;
            
            // Debug-Logging: Button-Namen mit HELPER-FUNKTION ermitteln (REDUNDANZ ELIMINATED)
            printf("[Button] %s pressed (GPIO %d)\n", get_button_name(gpio), gpio);
            
            return true;
        }
    }
    return false;
}

/**
 * @brief Liest Button-Event aus ISR-Queue (non-blocking)
 * 
 * Diese Funktion ist die primäre Methode um Button-Events zu konsumieren:
 * 1. Event aus Queue lesen (non-blocking, Timeout = 0)
 * 2. Software-Debouncing im Task-Kontext durchführen
 * 3. Event nur akzeptieren wenn Debounce-Zeit verstrichen
 * 
 * Vorteile gegenüber Polling:
 * - Event-driven (keine CPU-Zeit verschwendet)
 * - ISR-Queue puffert Events während Task beschäftigt
 * - Zuverlässigeres Timing als reines Polling
 * 
 * @param out_gpio Pointer zum Speichern der GPIO-Nummer
 * @return true wenn Event vorhanden und Debounce OK
 * @return false wenn keine Events oder innerhalb Debounce-Zeit
 */
bool controls_get_event(gpio_num_t *out_gpio) {
    // Queue existiert?
    if (s_button_queue == NULL) return false;
    
    gpio_num_t g;
    
    // Versuche Event aus Queue zu lesen (non-blocking, Timeout = 0)
    if (xQueueReceive(s_button_queue, &g, 0) == pdTRUE) {
        // Event empfangen → Software-Debouncing im Task-Kontext
        static uint32_t last_pressed_time[GPIO_NUM_MAX] = {0};
        uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
        
        // Debounce-Check: Ist genug Zeit seit letztem Event für dieses GPIO vergangen?
        if (now - last_pressed_time[g] > BUTTON_DEBOUNCE_MS) {
            // Event akzeptieren
            last_pressed_time[g] = now;
            
            // Debug-Logging
            const char *btn_name = "UNKNOWN";
            if (g == BTN_LEFT) btn_name = "LEFT";
            else if (g == BTN_RIGHT) btn_name = "RIGHT";
            else if (g == BTN_ROTATE) btn_name = "ROTATE";
            else if (g == BTN_FASTER) btn_name = "FASTER";
            printf("[Event] %s event from queue (GPIO %d)\n", btn_name, g);
            
            *out_gpio = g;
            return true;
        }
        // Event innerhalb Debounce-Zeit → verwerfen
        return false;
    }
    // Keine Events in Queue
    return false;
}

/**
 * @brief Wartet auf Button-Event aus Queue (blocking mit Timeout)
 * 
 * Wie controls_get_event(), aber mit Timeout:
 * - Blockiert Task bis Event verfügbar oder Timeout abgelaufen
 * - Nützlich für Warte-Zustände (z.B. Splash-Screen, Pause)
 * - Software-Debouncing wird auch hier angewendet
 * 
 * @param out_gpio Pointer zum Speichern der GPIO-Nummer
 * @param ticks_to_wait Timeout in FreeRTOS Ticks (portMAX_DELAY = unendlich)
 * @return true wenn Event empfangen und Debounce OK
 * @return false bei Timeout oder Debounce-Verwerfung
 */
bool controls_wait_event(gpio_num_t *out_gpio, TickType_t ticks_to_wait) {
    if (s_button_queue == NULL) return false;
    
    gpio_num_t g;
    // Warte auf Event (blocking mit Timeout)
    if (xQueueReceive(s_button_queue, &g, ticks_to_wait) == pdTRUE) {
        // Software-Debouncing
        static uint32_t last_pressed_time[GPIO_NUM_MAX] = {0};
        uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
        
        if (now - last_pressed_time[g] > BUTTON_DEBOUNCE_MS) {
            last_pressed_time[g] = now;
            *out_gpio = g;
            return true;
        }
    }
    return false;
}

/**
 * @brief Prüft ob alle 4 Buttons gleichzeitig gedrückt sind
 * 
 * Wird für Emergency Reset verwendet:
 * - Alle 4 Buttons gedrückt + 1 Sekunde halten = Hard Reset
 * - Direktes GPIO-Polling ohne Debouncing
 * - Aktiv-LOW: gedrückt = 0V
 * 
 * @return true wenn alle 4 Buttons aktuell gedrückt
 * @return false wenn mindestens 1 Button nicht gedrückt
 */
bool controls_all_buttons_pressed(void) {
    // Alle 4 GPIOs müssen LOW sein (Buttons sind aktiv-LOW)
    if (gpio_get_level(BTN_LEFT) == 0 && 
        gpio_get_level(BTN_RIGHT) == 0 &&
        gpio_get_level(BTN_ROTATE) == 0 && 
        gpio_get_level(BTN_FASTER) == 0) {
        return true;
    }
    return false;
}

void controls_disable_isr(void) {
    printf("[Controls] Disabling ISR for all buttons\n");
    gpio_isr_handler_remove(BTN_LEFT);
    gpio_isr_handler_remove(BTN_RIGHT);
    gpio_isr_handler_remove(BTN_ROTATE);
    gpio_isr_handler_remove(BTN_FASTER);
    // Disable GPIO interrupts completely
    gpio_intr_disable(BTN_LEFT);
    gpio_intr_disable(BTN_RIGHT);
    gpio_intr_disable(BTN_ROTATE);
    gpio_intr_disable(BTN_FASTER);
}

void controls_enable_isr(void) {
    printf("[Controls] Enabling ISR for all buttons\n");
    // Re-enable GPIO interrupts
    gpio_intr_enable(BTN_LEFT);
    gpio_intr_enable(BTN_RIGHT);
    gpio_intr_enable(BTN_ROTATE);
    gpio_intr_enable(BTN_FASTER);
    gpio_isr_handler_add(BTN_LEFT, gpio_isr_handler, (void*)(intptr_t)BTN_LEFT);
    gpio_isr_handler_add(BTN_RIGHT, gpio_isr_handler, (void*)(intptr_t)BTN_RIGHT);
    gpio_isr_handler_add(BTN_ROTATE, gpio_isr_handler, (void*)(intptr_t)BTN_ROTATE);
    gpio_isr_handler_add(BTN_FASTER, gpio_isr_handler, (void*)(intptr_t)BTN_FASTER);
}
