#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include <stdbool.h>
#include "led_strip.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// GPIO PIN CONFIGURATION
//////////////////////////////////////////////////////////////////////////////////////////////////
// LED Matrix (WS2812B) Pin
#define LED_GPIO_PIN   GPIO_NUM_1

// Button Pins (Pull-up, active-LOW)
#define BTN_LEFT       GPIO_NUM_4   // Linke Bewegung
#define BTN_RIGHT      GPIO_NUM_5   // Rechte Bewegung
#define BTN_ROTATE     GPIO_NUM_6   // Block rotieren
#define BTN_FASTER     GPIO_NUM_7   // Schneller fallen lassen

// Buzzer/Speaker Pin (RMT)
#define BUZZER_GPIO_PIN GPIO_NUM_14

// I2C Pins für OLED Display (SSD1306)
#define I2C_SDA_PIN    GPIO_NUM_21
#define I2C_SCL_PIN    GPIO_NUM_20

//////////////////////////////////////////////////////////////////////////////////////////////////
// LED MATRIX HARDWARE CONFIGURATION
//////////////////////////////////////////////////////////////////////////////////////////////////
#define LED_WIDTH 16
#define LED_HEIGHT 24
#define LED_STRIP_NUM_LEDS (LED_WIDTH * LED_HEIGHT)  // 384 LEDs total

//////////////////////////////////////////////////////////////////////////////////////////////////
// GAME TIMING CONFIGURATION (all in milliseconds)
//////////////////////////////////////////////////////////////////////////////////////////////////
// Block fall speed: how often a block falls one row (lower = faster)
#define FALL_INTERVAL_MS 300

// Render/refresh frequency: how often LEDs update (lower = smoother, ~60 FPS = 16ms)
#define RENDER_INTERVAL_MS 16

// Main loop frequency: how often input is checked (lower = more responsive, should be <= RENDER_INTERVAL_MS)
#define LOOP_INTERVAL_MS 5

// Splash animation duration: how long the TETRIS startup screen shows
#define SPLASH_DURATION_MS 4000

// Splash scroll delay between frames
// Erhöht auf 80ms für halb so schnelle Laufschrift
#define SPLASH_SCROLL_DELAY_MS 80

//////////////////////////////////////////////////////////////////////////////////////////////////
// I2C & OLED DISPLAY CONFIGURATION (SSD1306)
//////////////////////////////////////////////////////////////////////////////////////////////////
// I2C Bus Configuration
#define I2C_MASTER_NUM         I2C_NUM_0
#define I2C_MASTER_FREQ_HZ     (400 * 1000)  // 400 kHz Fast Mode

// OLED Display Settings
#define OLED_I2C_ADDRESS       0x3C
#define OLED_LCD_H_RES         128
#define OLED_LCD_V_RES         64
#define OLED_LCD_CMD_BITS      8
#define OLED_LCD_PARAM_BITS    8
#define OLED_PIN_NUM_RST       -1  // No hardware reset pin

//////////////////////////////////////////////////////////////////////////////////////////////////
// BUZZER/SPEAKER CONFIGURATION (RMT)
//////////////////////////////////////////////////////////////////////////////////////////////////
#define RMT_BUZZER_RESOLUTION_HZ  1000000  // 1 MHz resolution
#define BUZZER_VOLUME_PERCENT     10        // Lautstärke in Prozent (10-90)
#define THEME_SONG_SPEED          1.5     // Speed multiplier for music

//////////////////////////////////////////////////////////////////////////////////////////////////
// DISPLAY BRIGHTNESS CONFIGURATION
//////////////////////////////////////////////////////////////////////////////////////////////////
// Brightness scale for game blocks (0-255, 128 = 50%, 255 = 100%)
#define GAME_BRIGHTNESS_SCALE 125

// Brightness scale for splash text (0-255)
#define SPLASH_BRIGHTNESS_SCALE 10

// Game Over blink brightness (0-255, 50% = 0x80)
#define GAME_OVER_BLINK_R 128
#define GAME_OVER_BLINK_G 0
#define GAME_OVER_BLINK_B 0

// Game Over blink duration (on time in ms)
#define GAME_OVER_BLINK_ON_MS 300

// Game Over blink duration (off time in ms)
#define GAME_OVER_BLINK_OFF_MS 300

// Number of times to blink on Game Over
#define GAME_OVER_BLINK_COUNT 3

// Line Clear Animation Settings
#define LINE_CLEAR_BLINK_TIMES 2
#define LINE_CLEAR_BLINK_ON_MS 150
#define LINE_CLEAR_BLINK_OFF_MS 150
#define LINE_CLEAR_BLINK_R 128
#define LINE_CLEAR_BLINK_G 128
#define LINE_CLEAR_BLINK_B 128

//////////////////////////////////////////////////////////////////////////////////////////////////
// INPUT DEBOUNCING
//////////////////////////////////////////////////////////////////////////////////////////////////
// Button debounce time in milliseconds (faster block movement during game)
// Reduced so gameplay feels more responsive and start requires shorter press
#define BUTTON_DEBOUNCE_MS 150

//////////////////////////////////////////////////////////////////////////////////////////////////
// GRID & COLLISION CONFIGURATION
//////////////////////////////////////////////////////////////////////////////////////////////////
#define GRID_WIDTH 16
#define GRID_HEIGHT 24

#define NUM_BLOCKS 7
// Brightness divider used to scale down 0-255 color values
#define BRIGHTNESSDIV 10

// Block type indices (use these to refer to colors / block types)
enum BlockType {
    BLOCK_I = 0,
    BLOCK_J = 1,
    BLOCK_L = 2,
    BLOCK_O = 3,
    BLOCK_S = 4,
    BLOCK_T = 5,
    BLOCK_Z = 6
};

// Centralized color table (r,g,b) per block type. Defined in Globals.c
extern const uint8_t block_colors[NUM_BLOCKS][3];

typedef struct {
    uint16_t LED_Number[LED_HEIGHT][LED_WIDTH];
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    bool isFilled;
} MATRIX;

extern MATRIX ledMatrix;
extern led_strip_handle_t led_strip;

//////////////////////////////////////////////////////////////////////////////////////////////////
// FREERTOS SYNCHRONIZATION PRIMITIVES
//////////////////////////////////////////////////////////////////////////////////////////////////
// Semaphore für LED-Strip Zugriff (Schutz vor Race Conditions)
// Priorität: 6 (höchste), Typ: Binary, Timeout: 50ms
extern SemaphoreHandle_t led_strip_semaphore;

// Semaphore für Score & Spielfeld-Zugriff
// Priorität: 4 (mittel), Typ: Binary, Timeout: 100ms
extern SemaphoreHandle_t score_semaphore;

// Semaphore für SpeedManager-State
// Priorität: 3 (niedrig), Typ: Binary, Timeout: 10ms
extern SemaphoreHandle_t speed_semaphore;

// Event-Gruppe für ThemeTask Kontrolle (Pause/Resume)
extern EventGroupHandle_t theme_event_group;
#define THEME_RUN_BIT  (1 << 0)
#define THEME_PAUSE_BIT (1 << 1)

#endif // GLOBALS_H

