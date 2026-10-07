#ifndef SPLASH_H
#define SPLASH_H

#include <stdint.h>

/**
 * @brief Initialize LED state before splash display
 * 
 * Clears all LEDs and validates that the LED subsystem is ready.
 * Must be called before splash_show() to ensure consistent display after reset.
 * This function is idempotent and safe to call multiple times.
 */
void splash_init(void);

// Show the splash animation on the LED matrix. duration_ms controls how long the
// splash remains visible (use SPLASH_DURATION_MS constant from Globals.h).
void splash_show(uint32_t duration_ms);

// Clear the splash image from the LEDs (used when the game starts)
void splash_clear(void);

// Show the splash animation for a fixed duration in milliseconds.
// During this call inputs are ignored; the function returns when the time
// has elapsed so the caller can then wait for a fresh button press.
void splash_show_duration(uint32_t duration_ms);

// Show the splash animation and wait for a button press (blocks until pressed).
// This function loops continuously and only returns when a button is pressed.
// Used after reset/gameover splash_show_duration to wait for player input.
void splash_show_waiting(void);

#endif // SPLASH_H
