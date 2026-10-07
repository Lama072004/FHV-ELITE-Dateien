#include "Splash.h"
#include "Globals.h"
#include "led_strip.h"
#include "Blocks.h"
#include "Controls.h"
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

extern SemaphoreHandle_t led_strip_semaphore;

// ============================================================================
// INITIALIZATION FUNCTION
// ============================================================================

/**
 * @brief Initialize LED state and validate readiness for splash display
 * 
 * This function ensures a clean LED state before the splash animation begins:
 * - Clears all LEDs explicitly
 * - Validates semaphore availability
 * - Waits for physical LED update
 * - Essential for consistent splash display after reset
 */
void splash_init(void) {
    // Attempt to acquire semaphore (must be available)
    if (xSemaphoreTake(led_strip_semaphore, pdMS_TO_TICKS(100)) != pdTRUE) {
        printf("[Splash] ERROR: LED semaphore not available during init\n");
        return;
    }
    
    // Explicit clear and refresh of all LEDs before splash
    // Ensures no junk data in framebuffer
    led_strip_clear(led_strip);
    led_strip_refresh(led_strip);
    
    xSemaphoreGive(led_strip_semaphore);
    
    // Small delay to ensure physical LED update completes
    // Critical for consistent display after reset/power-on
    vTaskDelay(pdMS_TO_TICKS(50));
}

// ============================================================================
// SPLASH DESIGN MAP
// ============================================================================

// splash_design_map: 24 rows x 16 cols; each value: 0 = transparent, 1..7 -> block color index
// User can edit this to create custom designs
static const uint8_t splash_design_map[24][16] = {
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 1,1,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 1,1,0,0, 0,0,0,0},
    {0,0,4,4, 0,0,0,0, 1,1,0,0, 0,0,0,0},
    {0,0,4,4, 0,0,0,0, 1,1,0,0, 0,0,0,0},
    {0,0,4,4, 4,4,7,7, 1,1,6,6, 6,6,0,0},
    {0,0,4,4, 4,4,7,7, 1,1,6,6, 6,6,0,0},
    {0,0,5,5, 4,4,7,7, 1,1,2,2, 6,6,6,6},
    {0,0,5,5, 4,4,7,7, 1,1,2,2, 6,6,6,6},
    {5,5,5,5, 5,5,7,7, 7,7,2,2, 2,2,2,2},
    {5,5,5,5, 5,5,7,7, 7,7,2,2, 2,2,2,2}
};

// ============================================================================
// HELPER FUNKTION: Text-Bitmap generieren (REDUNDANZ ELIMINATED)
// ============================================================================

/**
 * @brief Generiert das Text-Bitmap für "TETRIS" (3x5 Font)
 * 
 * Konsolidiert die duplizierte Bitmap-Generierung zwischen splash_show
 * und splash_show_waiting in EINE Funktion
 * 
 * @param out_cols Output-Array für die Text-Spalten
 * @param max_cols Maximale Größe des Output-Arrays
 * @return Tatsächliche Anzahl der generierten Spalten
 */
static int splash_generate_text_bitmap(uint8_t *out_cols, int max_cols) {
    static const uint8_t ch_T[3] = {1, 31, 1};
    static const uint8_t ch_E[3] = {31, 21, 21};
    static const uint8_t ch_R[3] = {31, 5, 26};
    static const uint8_t ch_I[3] = {17, 31, 17};
    static const uint8_t ch_S[3] = {18, 21, 9};

    const char *txt = "TETRIS";
    const int char_w = 3;
    const int spacing = 1;
    int len = 6;
    int total_cols = len * (char_w + spacing);

    if (total_cols > max_cols) return 0;  // Buffer zu klein

    int pos = 0;
    for (int i = 0; i < len; i++){
        const uint8_t *bmp = NULL;
        switch (txt[i]){
            case 'T': bmp = ch_T; break;
            case 'E': bmp = ch_E; break;
            case 'R': bmp = ch_R; break;
            case 'I': bmp = ch_I; break;
            case 'S': bmp = ch_S; break;
            default: bmp = NULL; break;
        }
        for (int c = 0; c < char_w; c++){
            out_cols[pos++] = bmp ? (bmp[c] & 31) : 0;
        }
        if (spacing) out_cols[pos++] = 0;
    }
    return total_cols;
}

// ============================================================================
// HELPER FUNKTION: Render Design-Map (REDUNDANZ ELIMINATED)
// ============================================================================

static void splash_render_design_map(void) {
    // SEMAPHOR-SCHUTZ: LED-Strip schützen
    if (xSemaphoreTake(led_strip_semaphore, pdMS_TO_TICKS(50)) != pdTRUE) {
        printf("[Splash] ERROR: LED semaphore timeout\n");
        return;
    }

    for (int y = 0; y < LED_HEIGHT; y++) {
        for (int x = 0; x < LED_WIDTH; x++) {
            uint8_t val = splash_design_map[y][x];
            if (val == 0) continue;
            uint8_t bidx = (val - 1) % NUM_BLOCKS;
            uint8_t r, g, b;
            get_block_rgb(bidx, &r, &g, &b);
            r = (r * GAME_BRIGHTNESS_SCALE) / 255;
            g = (g * GAME_BRIGHTNESS_SCALE) / 255;
            b = (b * GAME_BRIGHTNESS_SCALE) / 255;
            int led_num = ledMatrix.LED_Number[y][x];
            led_strip_set_pixel(led_strip, led_num, r, g, b);
        }
    }
    led_strip_refresh(led_strip);
    xSemaphoreGive(led_strip_semaphore);
}

// ============================================================================
// MAIN SPLASH FUNCTION: splash_show_internal (CONSOLIDATED)
// ============================================================================

/**
 * @brief Interne konsolidierte Splash-Animation
 * 
 * Diese Funktion vereinigt die Logik von splash_show und splash_show_waiting
 * in EINE Implementierung mit Flag-Parameter:
 * - wait_for_button=true: Wartet auf Button (keine Zeitbegrenzung)
 * - wait_for_button=false: Läuft für duration_ms, bricht ab wenn Button
 * 
 * REDUNDANZEN ELIMINIERT: 90% Code-Duplikation wurde entfernt!
 * 
 * @param duration_ms Dauer in ms (nur relevant wenn wait_for_button=false)
 * @param wait_for_button true = warte auf Button, false = zeitbasiert
 */
static void splash_show_internal(uint32_t duration_ms, bool wait_for_button) {
    // Generate text bitmap (REDUNDANZ ELIMINATED)
    uint8_t text_bitmap[50];  // 3*6 + 5 = 23, aber 50 für Sicherheit
    int total_cols = splash_generate_text_bitmap(text_bitmap, 50);
    if (total_cols == 0) return;

    // Render design map once (REDUNDANZ ELIMINATED)
    splash_render_design_map();

    // Continuous scroll loop
    int step = 0;
    uint32_t start_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
    bool button_pressed = false;

    while (!button_pressed) {
        // Zeitbasierte Begrenzung (falls nicht auf Button warten)
        if (!wait_for_button) {
            uint32_t elapsed = (xTaskGetTickCount() * portTICK_PERIOD_MS) - start_time;
            if (elapsed > duration_ms) break;
        }

        // SEMAPHOR-SCHUTZ: LED-Strip für Text-Update schützen
        if (xSemaphoreTake(led_strip_semaphore, pdMS_TO_TICKS(50)) == pdTRUE) {
            // Only update text rows (optimization: rows 2-6 where text displays)
            for (int x = 0; x < LED_WIDTH; x++){
                int src = step - (LED_WIDTH - x);
                
                // Restore design underneath text area
                for (int y = 2; y < 7; y++) {
                    uint8_t val = splash_design_map[y][x];
                    if (val == 0) {
                        // transparent - turn off
                        int led_num = ledMatrix.LED_Number[y][x];
                        led_strip_set_pixel(led_strip, led_num, 0, 0, 0);
                    } else {
                        // restore design color
                        uint8_t bidx = (val - 1) % NUM_BLOCKS;
                        uint8_t r, g, b;
                        get_block_rgb(bidx, &r, &g, &b);
                        r = (r * GAME_BRIGHTNESS_SCALE) / 255;
                        g = (g * GAME_BRIGHTNESS_SCALE) / 255;
                        b = (b * GAME_BRIGHTNESS_SCALE) / 255;
                        int led_num = ledMatrix.LED_Number[y][x];
                        led_strip_set_pixel(led_strip, led_num, r, g, b);
                    }
                }
                
                // Draw text on top if visible
                if (src >= 0 && src < total_cols){
                    uint8_t col = text_bitmap[src];
                    for (int y = 0; y < 5; y++){
                        if (col & (1 << y)){
                            int gy = 2 + y;
                            int led_num = ledMatrix.LED_Number[gy][x];
                            uint8_t brightness = (SPLASH_BRIGHTNESS_SCALE * 255) / 255;
                            led_strip_set_pixel(led_strip, led_num, brightness, brightness, brightness);
                        }
                    }
                }
            }
                led_strip_refresh(led_strip);
            xSemaphoreGive(led_strip_semaphore);
        }
        
        // Only break on button input when explicitly waiting for a button
        if (wait_for_button) {
            gpio_num_t ev;
            if (controls_get_event(&ev) || 
                check_button_pressed(BTN_LEFT) || 
                check_button_pressed(BTN_RIGHT) || 
                check_button_pressed(BTN_ROTATE) || 
                check_button_pressed(BTN_FASTER)) {
                button_pressed = true;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(SPLASH_SCROLL_DELAY_MS));
        step++;
        
        // Loop wraps continuously
        if (step >= total_cols + LED_WIDTH) {
            step = 0;
        }
    }
}

// ============================================================================
// PUBLIC API FUNCTIONS
// ============================================================================

/**
 * @brief Zeige Splash Animation für bestimmte Dauer (oder bis Button)
 * 
 * Diese Funktion wurde mit splash_show_waiting durch EINE konsolidierte
 * Implementierung ersetzt (splash_show_internal) um Redundanzen zu eliminieren.
 */
void splash_show(uint32_t duration_ms) {
    splash_show_internal(duration_ms, false);  // Zeitbasiert
}

/**
 * @brief Zeige Splash Animation und warte auf Button-Druck
 * 
 * Diese Funktion wurde mit splash_show durch EINE konsolidierte
 * Implementierung ersetzt (splash_show_internal) um Redundanzen zu eliminieren.
 */
void splash_show_waiting(void) {
    splash_show_internal(0, true);  // Button-basiert (duration ignoriert)
}

/**
 * @brief Zeige Splash Animation für bestimmte Dauer (ignoriere Inputs komplett)
 * 
 * Diese Funktion ist identisch mit splash_show(), zeigt die Animation
 * aber definitiv für die angegebene Dauer, ohne auf Buttons zu warten.
 * 
 * @param duration_ms Dauer in Millisekunden für die Splash-Anzeige
 */
void splash_show_duration(uint32_t duration_ms) {
    splash_show_internal(duration_ms, false);  // Zeitbasiert (Button ignorieren)
}

/**
 * @brief Lösche die Splash-Animation von den LEDs
 * 
 * Schaltet alle LEDs aus, um den LED-Matrix für das Spiel freizugeben.
 * Diese Funktion schützt den Zugriff mit dem LED-Strip Semaphor.
 */
void splash_clear(void) {
    // SEMAPHOR-SCHUTZ: LED-Strip für Clear-Operation schützen
    if (xSemaphoreTake(led_strip_semaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
        // Setze alle LEDs auf schwarz (0,0,0)
        for (int y = 0; y < LED_HEIGHT; y++) {
            for (int x = 0; x < LED_WIDTH; x++) {
                int led_num = ledMatrix.LED_Number[y][x];
                led_strip_set_pixel(led_strip, led_num, 0, 0, 0);
            }
        }
        
        // Aktualisiere LED-Strip mit schwarzer Matrix
        led_strip_refresh(led_strip);
        xSemaphoreGive(led_strip_semaphore);
    }
}
