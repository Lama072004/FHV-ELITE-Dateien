#include "driver/rmt_tx.h"
#include "musical_score_encoder.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "Globals.h"
#include "ThemeSong.h"

extern EventGroupHandle_t theme_event_group;

#define SPEED THEME_SONG_SPEED

static const char *TAG = "ThemeSong";

/* --------------------------------------------------------------------------
   NOTE DEFINITIONS (frequencies in Hz)
   ------------------------------------------------------------------------ */
#define Pause 0
#define C2  65
#define CS2 69
#define D2  73
#define DS2 78
#define E2  82
#define F2  87
#define FS2 93
#define G2  98
#define GS2 104
#define A2  110
#define AS2 117
#define B2  123

#define C3  131
#define CS3 139
#define D3  147
#define DS3 156
#define E3  165
#define F3  175
#define FS3 185
#define G3  196
#define GS3 208
#define A3  220
#define AS3 233
#define B3  247

#define C4  262
#define CS4 277
#define D4  294
#define DS4 311
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494

#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define B5  988

#define C6  1047
#define CS6 1109
#define D6  1175
#define E6  1319
#define FS6 1480
#define G6  1568
#define GS6 1661
#define A6  1760
#define AS6 1865
#define B6  1976
#define DS6 1245
#define F6  1397

/* --------------------------------------------------------------------------
   Score
   ------------------------------------------------------------------------ */
static const buzzer_musical_score_t score[] = {
    {E4, 500*SPEED}, {B3, 250*SPEED}, {C4, 250*SPEED}, {D4, 500*SPEED},
    {C4, 250*SPEED}, {B3, 250*SPEED}, {A3, 500*SPEED}, {A3, 250*SPEED},
    {C4, 250*SPEED}, {E4, 500*SPEED}, {D4, 250*SPEED}, {C4, 250*SPEED},
    {B3, 500*SPEED}, {B3, 250*SPEED}, {C4, 250*SPEED}, {D4, 500*SPEED},
    {E4, 500*SPEED}, {C4, 500*SPEED}, {A3, 500*SPEED}, {A3, 500*SPEED},
    {Pause, 1000*SPEED},
    {D4, 750*SPEED}, {F4, 250*SPEED}, {A4, 500*SPEED}, {G4, 250*SPEED},
    {F4, 250*SPEED}, {E4, 750*SPEED}, {C4, 250*SPEED}, {E4, 500*SPEED},
    {D4, 250*SPEED}, {C4, 250*SPEED}, {B3, 500*SPEED}, {B3, 250*SPEED},
    {C4, 250*SPEED}, {D4, 500*SPEED}, {E4, 500*SPEED}, {C4, 500*SPEED},
    {A3, 500*SPEED}, {A3, 500*SPEED},
    {Pause, 500*SPEED},{Pause, 1000*SPEED},

    {E5, 500*SPEED}, {B4, 250*SPEED}, {C5, 250*SPEED}, {D5, 500*SPEED},
    {C5, 250*SPEED}, {B4, 250*SPEED}, {A4, 500*SPEED}, {A4, 250*SPEED},
    {C5, 250*SPEED}, {E5, 500*SPEED}, {D5, 250*SPEED}, {C5, 250*SPEED},
    {B4, 500*SPEED}, {B4, 250*SPEED}, {C5, 250*SPEED}, {D5, 500*SPEED},
    {E5, 500*SPEED}, {C5, 500*SPEED}, {A4, 500*SPEED}, {A4, 500*SPEED},
    {Pause, 1000*SPEED},
    {D5, 750*SPEED}, {F5, 250*SPEED}, {A5, 500*SPEED}, {G5, 250*SPEED},
    {F5, 250*SPEED}, {E5, 750*SPEED}, {C5, 250*SPEED}, {E5, 500*SPEED},
    {D5, 250*SPEED}, {C5, 250*SPEED}, {B4, 500*SPEED}, {B4, 250*SPEED},
    {C5, 250*SPEED}, {D5, 500*SPEED}, {E5, 500*SPEED}, {C5, 500*SPEED},
    {A4, 500*SPEED}, {A4, 500*SPEED},
    {Pause, 500*SPEED},{Pause, 1000*SPEED},
};

// ============================================================================
// PLACEHOLDER: Weitere Melodien (vom Benutzer zu implementieren!)
// ============================================================================

/**
 * TEMPLATE für StarWars Theme - Bitte implementieren!
 * Nutze die gleiche Struktur wie die Tetris-Melodie oben
 */
static const buzzer_musical_score_t score_starwars[] = {
    {AS4, 250*SPEED}, {AS4, 250*SPEED}, {AS4, 250*SPEED},
    {F5, 1000*SPEED}, {C6, 1000*SPEED},
    {AS5, 250*SPEED}, {A5, 250*SPEED}, {G5, 250*SPEED}, {F6, 1000*SPEED}, {C6, 500*SPEED},
    {AS5, 250*SPEED}, {A5, 250*SPEED}, {G5, 250*SPEED}, {F6, 1000*SPEED}, {C6, 500*SPEED},
    {AS5, 250*SPEED}, {A5, 250*SPEED}, {AS5, 250*SPEED}, {G5, 1000*SPEED}, {C5, 250*SPEED}, {C5, 250*SPEED}, {C5, 250*SPEED},
    {F5, 1000*SPEED}, {C6, 1000*SPEED},
    {AS5, 250*SPEED}, {A5, 250*SPEED}, {G5, 250*SPEED}, {F6, 1000*SPEED}, {C6, 500*SPEED},

    {AS5, 250*SPEED}, {A5, 250*SPEED}, {G5, 250*SPEED}, {F6, 1000*SPEED}, {C6, 500*SPEED},
    {AS5, 250*SPEED}, {A5, 250*SPEED}, {AS5, 250*SPEED}, {G5, 1000*SPEED}, {C5, 375*SPEED}, {C5, 125*SPEED},
    {D5, 750*SPEED}, {D5, 250*SPEED}, {AS5, 250*SPEED}, {A5, 250*SPEED}, {G5, 250*SPEED}, {F5, 250*SPEED},
    {F5, 250*SPEED}, {G5, 250*SPEED}, {A5, 250*SPEED}, {G5, 500*SPEED}, {D5, 250*SPEED}, {E5, 500*SPEED}, {C5, 375*SPEED}, {C5, 125*SPEED},
    {D5, 750*SPEED}, {D5, 250*SPEED}, {AS5, 250*SPEED}, {A5, 250*SPEED}, {G5, 250*SPEED}, {F5, 250*SPEED},

    {C6, 375*SPEED}, {G5, 125*SPEED}, {G5, 1000*SPEED}, {Pause, 250*SPEED}, {C5, 250*SPEED},
    {D5, 750*SPEED}, {D5, 250*SPEED}, {AS5, 250*SPEED}, {A5, 250*SPEED}, {G5, 250*SPEED}, {F5, 250*SPEED},
    {F5, 250*SPEED}, {G5, 250*SPEED}, {A5, 250*SPEED}, {G5, 500*SPEED}, {D5, 250*SPEED}, {E5, 500*SPEED}, {C6, 375*SPEED}, {C6, 125*SPEED},
    {F6, 500*SPEED}, {DS6, 250*SPEED}, {CS6, 500*SPEED}, {C6, 250*SPEED}, {AS5, 500*SPEED}, {GS5, 250*SPEED}, {G5, 500*SPEED}, {F5, 250*SPEED},
    {C6, 2000*SPEED}
};

static const buzzer_musical_score_t score_mario[] = {
    {E5, 250*SPEED/1.5}, {E5, 250*SPEED/1.5}, {Pause, 250*SPEED/1.5}, {E5, 250*SPEED/1.5}, {Pause, 250*SPEED/1.5}, {C5, 250*SPEED/1.5}, {E5, 250*SPEED/1.5},
    {G5, 500*SPEED/1.5}, {Pause, 500*SPEED/1.5}, {G4, 500*SPEED/1.5}, {Pause, 500*SPEED/1.5},
    {C5, 750*SPEED/1.5}, {G4, 250*SPEED/1.5}, {Pause, 500*SPEED/1.5}, {E4, 750*SPEED/1.5},
    {A4, 500*SPEED/1.5}, {B4, 500*SPEED/1.5}, {AS4, 250*SPEED/1.5}, {A4, 500*SPEED/1.5},
    {G4, 375*SPEED/1.5}, {E5, 375*SPEED/1.5}, {G5, 375*SPEED/1.5}, {A5, 500*SPEED/1.5}, {F5, 250*SPEED/1.5}, {G5, 250*SPEED/1.5},
    {Pause, 250*SPEED/1.5}, {E5, 500*SPEED/1.5}, {C5, 250*SPEED/1.5}, {D5, 250*SPEED/1.5}, {B4, 750*SPEED/1.5},

    {C5, 750*SPEED/1.5}, {G4, 250*SPEED/1.5}, {Pause, 500*SPEED/1.5}, {E4, 750*SPEED/1.5},
    {A4, 500*SPEED/1.5}, {B4, 500*SPEED/1.5}, {AS4, 250*SPEED/1.5}, {A4, 500*SPEED/1.5},
    {G4, 375*SPEED/1.5}, {E5, 375*SPEED/1.5}, {G5, 375*SPEED/1.5}, {A5, 500*SPEED/1.5}, {F5, 250*SPEED/1.5}, {G5, 250*SPEED/1.5},
    {Pause, 250*SPEED/1.5}, {E5, 500*SPEED/1.5}, {C5, 250*SPEED/1.5}, {D5, 250*SPEED/1.5}, {B4, 750*SPEED/1.5},

    {Pause, 500*SPEED/1.5}, {G5, 250*SPEED/1.5}, {FS5, 250*SPEED/1.5}, {F5, 250*SPEED/1.5}, {DS5, 500*SPEED/1.5}, {E5, 250*SPEED/1.5},
    {Pause, 250*SPEED/1.5}, {GS4, 250*SPEED/1.5}, {A4, 250*SPEED/1.5}, {C4, 250*SPEED/1.5}, {Pause, 250*SPEED/1.5}, {A4, 250*SPEED/1.5}, {C5, 250*SPEED/1.5}, {D5, 250*SPEED/1.5},  
    {Pause, 500*SPEED/1.5}, {DS5, 500*SPEED/1.5}, {Pause, 250*SPEED/1.5}, {D5, 750*SPEED/1.5},
    {C5, 1000*SPEED/1.5}, {Pause, 1000*SPEED/1.5}
};

static const buzzer_musical_score_t score_cantinaband[] = {
    {B4, 750*SPEED/1.5}, {E5, 750*SPEED/1.5}, {B4, 750*SPEED/1.5}, {E5, 750*SPEED/1.5},
    {B4, 250*SPEED/1.5}, {E5, 750*SPEED/1.5}, {B4, 250*SPEED/1.5}, {Pause, 250*SPEED/1.5}, {AS4, 250*SPEED/1.5}, {B4, 250*SPEED/1.5},
    {B4, 250*SPEED/1.5}, {AS4, 250*SPEED/1.5}, {B4, 250*SPEED/1.5}, {A4, 250*SPEED/1.5}, {Pause, 250*SPEED/1.5}, {GS4, 250*SPEED/1.5}, {A4, 250*SPEED/1.5}, {G4, 250*SPEED/1.5},
    {G4, 500*SPEED/1.5}, {E4, 1500*SPEED/1.5},

    {B4, 750*SPEED}, {E5, 750*SPEED}, {B4, 750*SPEED}, {E5, 750*SPEED},
    {B4, 250*SPEED/1.5}, {E5, 750*SPEED/1.5}, {B4, 250*SPEED/1.5}, {Pause, 250*SPEED/1.5}, {AS4, 250*SPEED/1.5}, {B4, 250*SPEED/1.5},
    {A4, 750*SPEED/1.5}, {A4, 750*SPEED/1.5}, {GS4, 250*SPEED/1.5}, {A4, 750*SPEED/1.5},
    {D5, 250*SPEED/1.5}, {C5, 750*SPEED/1.5}, {B4, 750*SPEED/1.5}, {A4, 750*SPEED/1.5},

    {B4, 750*SPEED/1.5}, {E5, 750*SPEED/1.5}, {B4, 750*SPEED/1.5}, {E5, 750*SPEED/1.5},
    {B4, 250*SPEED/1.5}, {E5, 750*SPEED/1.5}, {B4, 250*SPEED/1.5}, {Pause, 250*SPEED/1.5}, {AS4, 250*SPEED/1.5}, {B4, 250*SPEED/1.5},
    {D5, 500*SPEED/1.5}, {D5, 750*SPEED/1.5}, {B4, 250*SPEED/1.5}, {A4, 750*SPEED/1.5},
    {G4, 750*SPEED/1.5}, {E4, 1500*SPEED/1.5},

    {E4, 1000*SPEED/1.5}, {G4, 1000*SPEED/1.5},
    {B4, 1000*SPEED/1.5}, {D5, 1000*SPEED/1.5},
    {F5, 750*SPEED/1.5}, {E5, 750*SPEED/1.5}, {AS4, 250*SPEED/1.5}, {AS4, 250*SPEED/1.5}, {B4, 500*SPEED/1.5}, {G4, 500*SPEED/1.5}
};

static const buzzer_musical_score_t score_HarryPotter[] = {
    {Pause, 1000*SPEED/1.3}, {D4, 500*SPEED/1.3},
    {G4, 750*SPEED/1.3}, {AS4, 250*SPEED/1.3}, {A4, 500*SPEED/1.3},
    {G4, 1000*SPEED/1.3}, {D5, 500*SPEED/1.3},
    {C5, 1500*SPEED/1.3},
    {A4, 1500*SPEED/1.3},
    {G4, 750*SPEED/1.3}, {AS4, 250*SPEED/1.3}, {A4, 500*SPEED/1.3},
    {F4, 1000*SPEED/1.3}, {GS4, 500*SPEED/1.3},
    {D4, 2000*SPEED/1.3},
    {D4, 500*SPEED/1.3},

    {G4, 750*SPEED/1.3}, {AS4, 250*SPEED/1.3}, {A4, 500*SPEED/1.3},
    {G4, 1000*SPEED/1.3}, {D5, 500*SPEED/1.3},
    {F5, 1000*SPEED/1.3}, {E5, 500*SPEED/1.3},
    {DS5, 1000*SPEED/1.3}, {B4, 500*SPEED/1.3},
    {DS5, 750*SPEED/1.3}, {D5, 250*SPEED/1.3}, {CS5, 500*SPEED/1.3},
    {CS4, 1000*SPEED/1.3}, {B4, 500*SPEED/1.3},
    {G4, 2000*SPEED/1.3},
    {AS4, 500*SPEED/1.3},

    {D5, 1000*SPEED/1.3}, {AS4, 500*SPEED/1.3},
    {D5, 1000*SPEED/1.3}, {AS4, 500*SPEED/1.3},
    {DS5, 1000*SPEED/1.3}, {D5, 500*SPEED/1.3},
    {CS5, 1000*SPEED/1.3}, {A4, 500*SPEED/1.3},
    {AS4, 750*SPEED/1.3}, {D5, 250*SPEED/1.3}, {CS5, 500*SPEED/1.3},
    {CS4, 1000*SPEED/1.3}, {D4, 500*SPEED/1.3},
    {D5, 2000*SPEED/1.3},
    {Pause, 500*SPEED/1.3}, {AS4, 500*SPEED/1.3}
};

static const buzzer_musical_score_t score_ImperialMarch[] = {
    {A4, 750*SPEED/0.86}, {A4, 750*SPEED/0.86}, {A4, 125*SPEED}, {A4, 125*SPEED}, {A4, 125*SPEED}, {A4, 125*SPEED}, {F4, 250*SPEED}, {Pause, 250*SPEED},
    {A4, 750*SPEED/0.86}, {A4, 750*SPEED/0.86}, {A4, 125*SPEED}, {A4, 125*SPEED}, {A4, 125*SPEED}, {A4, 125*SPEED}, {F4, 250*SPEED}, {Pause, 250*SPEED},
    {A4, 500*SPEED/0.86}, {A4, 500*SPEED/0.86}, {A4, 500*SPEED/0.86}, {F4, 375*SPEED/0.86}, {C5, 125*SPEED/0.86},

    {A4, 500*SPEED/0.86}, {F4, 375*SPEED/0.86}, {C5, 125*SPEED/0.86}, {A4, 1000*SPEED/0.86},
    {E5, 500*SPEED/0.86}, {E5, 500*SPEED/0.86}, {E5, 500*SPEED/0.86}, {F5, 375*SPEED/0.86}, {C5, 125*SPEED/0.86},
    {A4, 500*SPEED/0.86}, {F4, 375*SPEED/0.86}, {C5, 125*SPEED/0.86}, {A4, 1000*SPEED/0.86},

    {A5, 500*SPEED/0.86}, {A4, 375*SPEED/0.86}, {A4, 125*SPEED/0.86}, {A5, 500*SPEED/0.86}, {GS5, 375*SPEED/0.86}, {G5, 125*SPEED/0.86},
    {DS5, 125*SPEED/0.86}, {D5, 125*SPEED/0.86}, {DS5, 250*SPEED/0.86}, {Pause, 250*SPEED/0.86}, {A4, 250*SPEED/0.86}, {DS5, 500*SPEED/0.86}, {D5, 375*SPEED/0.86}, {CS5, 125*SPEED/0.86},

    {C5, 125*SPEED/0.86}, {B4, 125*SPEED/0.86}, {C5, 125*SPEED/0.86}, {Pause, 250*SPEED/0.86}, {F4, 250*SPEED/0.86}, {GS4, 500*SPEED/0.86}, {F4, 375*SPEED/0.86}, {A4, 187*SPEED/0.86},
    {C5, 500*SPEED/0.86}, {A4, 375*SPEED/0.86}, {C5, 125*SPEED/0.86}, {E5, 1000*SPEED/0.86}
};

static const buzzer_musical_score_t score_keyboardCat[] = {
    {Pause, 2000*SPEED/1.33},
    {C4, 500*SPEED/1.33}, {E4, 500*SPEED/1.33}, {G4, 500*SPEED/1.33}, {E4, 500*SPEED/1.33}, 
    {C4, 500*SPEED/1.33}, {E4, 250*SPEED/1.33}, {G4, 750*SPEED/1.33}, {E4, 500*SPEED/1.33},
    {A3, 500*SPEED/1.33}, {C4, 500*SPEED/1.33}, {E4, 500*SPEED/1.33}, {C4, 500*SPEED/1.33},
    {A3, 500*SPEED/1.33}, {C4, 250*SPEED/1.33}, {E4, 750*SPEED/1.33}, {C4, 500*SPEED/1.33},
    {G3, 500*SPEED/1.33}, {B3, 500*SPEED/1.33}, {D4, 500*SPEED/1.33}, {B3, 500*SPEED/1.33},
    {G3, 500*SPEED/1.33}, {B3, 250*SPEED/1.33}, {D4, 750*SPEED/1.33}, {B3, 500*SPEED/1.33},
    {G3, 500*SPEED/1.33}, {G3, 250*SPEED/1.33}, {G3, 750*SPEED/1.33}, {G3, 250*SPEED/1.33}, {G3, 500*SPEED/1.33}, 
    {G3, 500*SPEED/1.33}, {G3, 500*SPEED/1.33}, {G3, 250*SPEED/1.33}, {G3, 500*SPEED/1.33},
    {C4, 500*SPEED/1.33}, {E4, 500*SPEED/1.33}, {G4, 500*SPEED/1.33}, {E4, 500*SPEED/1.33}, 
    {C4, 500*SPEED/1.33}, {E4, 250*SPEED/1.33}, {G4, 750*SPEED/1.33}, {E4, 500*SPEED/1.33},
    {A3, 500*SPEED/1.33}, {C4, 500*SPEED/1.33}, {E4, 500*SPEED/1.33}, {C4, 500*SPEED/1.33},
    {A3, 500*SPEED/1.33}, {C4, 250*SPEED/1.33}, {E4, 750*SPEED/1.33}, {C4, 500*SPEED/1.33},
    {G3, 500*SPEED/1.33}, {B3, 500*SPEED/1.33}, {D4, 500*SPEED/1.33}, {B3, 500*SPEED/1.33},
    {G3, 500*SPEED/1.33}, {B3, 250*SPEED/1.33}, {D4, 750*SPEED/1.33}, {B3, 500*SPEED/1.33},
    {G3, 500*SPEED/1.33},{G3, 3000*SPEED/1.33}
};

/**
 * SILENCE / PAUSE: Leere Melodie (nur Pausen)
 */
static const buzzer_musical_score_t score_silence[] = {
    {Pause, 2000*SPEED},  // 2 Sekunden Stille
};

// ============================================================================
// SONG MANAGEMENT SYSTEM
// ============================================================================

static int current_song_index = THEME_TETRIS;  // Aktuelle Song-Nummer

/* Strukturarray für alle verfügbaren Songs */
typedef struct {
    const buzzer_musical_score_t *score;
    size_t score_size;
    const char *name;
} ThemeSong;

static const ThemeSong available_songs[] = {
    { score, sizeof(score) / sizeof(score[0]), "TETRIS" },
    { score_starwars, sizeof(score_starwars) / sizeof(score_starwars[0]), "STARWARS" },
    { score_cantinaband, sizeof(score_cantinaband) / sizeof(score_cantinaband[0]), "CANTINABAND" },
    { score_ImperialMarch, sizeof(score_ImperialMarch) / sizeof(score_ImperialMarch[0]), "IMPERIALMARCH" },
    { score_mario, sizeof(score_mario) / sizeof(score_mario[0]), "MARIO" },
    { score_HarryPotter, sizeof(score_HarryPotter) / sizeof(score_HarryPotter[0]), "HarryPotter" },
    { score_keyboardCat, sizeof(score_keyboardCat) / sizeof(score_keyboardCat[0]), "keyboardCat" },
    { score_silence, sizeof(score_silence) / sizeof(score_silence[0]), "SILENCE" }
};

#define NUM_AVAILABLE_SONGS (sizeof(available_songs) / sizeof(ThemeSong))
    
/* --------------------------------------------------------------------------
   Task-Funktion für wiederholtes Abspielen (mit Pause-Mechanismus)
   
   VERBESSERUNG: Event-Group Kontrolle ermöglicht Pause/Resume der Musik
   - THEME_RUN_BIT: Musik läuft aktiv
   - THEME_PAUSE_BIT: Musik pausieren (z.B. bei Game Over)
   
   Dies verhindert, dass die Musik bei Spielpausen durchläuft.
   
   WEITERE VERBESSERUNG: Multi-Song Support - Nutzt verfügbare_Songs[] Array
   und aktueller_Song_Index um zwischen Melodien zu wechseln
   -------------------------------------------------------------------------- */
static void ThemeTask(void *arg) {
    ESP_LOGI(TAG, "Theme task started");

    // RMT-Setup
    rmt_channel_handle_t buzzer_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .gpio_num = BUZZER_GPIO_PIN,
        .mem_block_symbols = 64,
        .resolution_hz = RMT_BUZZER_RESOLUTION_HZ,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &buzzer_chan));

    rmt_encoder_handle_t score_encoder = NULL;
    musical_score_encoder_config_t encoder_config = {
        .resolution = RMT_BUZZER_RESOLUTION_HZ
    };
    ESP_ERROR_CHECK(rmt_new_musical_score_encoder(&encoder_config, &score_encoder));
    ESP_ERROR_CHECK(rmt_enable(buzzer_chan));

    while(1) {
        // EVENT-GROUP KONTROL: Warte auf Run-Signal (PAUSE-MECHANISMUS)
        EventBits_t bits = xEventGroupWaitBits(theme_event_group,
                                               THEME_RUN_BIT,
                                               pdFALSE,  // Bit nicht clearen
                                               pdFALSE,  // Alle Bits müssen gesetzt sein
                                               pdMS_TO_TICKS(100));
        
        // Wenn nicht im RUN-Modus, pausieren (z.B. bei Game Over)
        if (!(bits & THEME_RUN_BIT)) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }
        
        // Aktuellen Song aus Array holen
        if (current_song_index < 0 || current_song_index >= NUM_AVAILABLE_SONGS) {
            current_song_index = THEME_TETRIS;  // Fallback
        }
        
        const ThemeSong *current_song = &available_songs[current_song_index];
        ESP_LOGI(TAG, "Playing song: %s", current_song->name);
        
        for (size_t i = 0; i < current_song->score_size; ++i) {
            // Regelmäßig State prüfen, um schnell auf Pause-Signal zu reagieren
            bits = xEventGroupGetBits(theme_event_group);
            if (!(bits & THEME_RUN_BIT)) {
                // Musik wurde pausiert - beende aktuelle Note
                vTaskDelay(pdMS_TO_TICKS(100));
                break;  // Starte neue Schleife von vorne
            }
            
            // Song-Index regelmäßig prüfen (falls während Musik geändert)
            // Wenn geändert, dann zur äußeren Schleife zurück
            if (current_song_index != (int)(current_song - available_songs)) {
                break;
            }
            
            uint32_t freq = current_song->score[i].freq_hz;
            uint32_t dur_ms = current_song->score[i].duration_ms;

            if (freq == 0) {
                vTaskDelay(pdMS_TO_TICKS(dur_ms));
                continue;
            }

            uint64_t prod = (uint64_t)dur_ms * (uint64_t)freq;
            uint32_t loop_count = (uint32_t)(prod / 1000ULL);
            if (loop_count == 0) loop_count = 1;

            rmt_transmit_config_t tx_config = { .loop_count = loop_count };
            ESP_ERROR_CHECK(rmt_transmit(buzzer_chan, score_encoder, &current_song->score[i],
                                        sizeof(buzzer_musical_score_t), &tx_config));
        }
    }
}

/* --------------------------------------------------------------------------
   StartTheme
   ------------------------------------------------------------------------ */
void StartTheme(void) {
    // Task nur einmal erstellen
    static bool task_started = false;
    if (!task_started) {
        xTaskCreate(ThemeTask, "ThemeTask", 4096, NULL, 5, NULL);
        task_started = true;
    }
}
/* --------------------------------------------------------------------------
   theme_pause - Musik pausieren
   
   Diese Funktion nutzt die Event-Group (THEME_RUN_BIT), um die Musik zu pausieren.
   Der ThemeTask wird nicht beendet, sondern wartet auf das RUN-Signal.
   
   VERBESSERUNG: Nicht-destruktive Pause - Task bleibt erhalten und lädt sich nicht neu
   -------------------------------------------------------------------------- */
void theme_pause(void) {
    if (theme_event_group != NULL) {
        xEventGroupClearBits(theme_event_group, THEME_RUN_BIT);
        ESP_LOGI(TAG, "Theme paused");
    }
}

/* --------------------------------------------------------------------------
   theme_resume - Musik fortsetzen
   
   Diese Funktion setzt das THEME_RUN_BIT zurück, damit der ThemeTask wieder aktiv wird.
   -------------------------------------------------------------------------- */
void theme_resume(void) {
    if (theme_event_group != NULL) {
        xEventGroupSetBits(theme_event_group, THEME_RUN_BIT);
        ESP_LOGI(TAG, "Theme resumed");
    }
}

/* --------------------------------------------------------------------------
   theme_next_song - Zur nächsten Melodie wechseln
   
   Zyklischer Wechsel durch alle verfügbaren Songs:
   TETRIS → STARWARS → MARIO → SILENCE → TETRIS → ...
   
   Die aktuelle Musik wird sofort unterbrochen und der neue Song läuft
   in der nächsten ThemeTask-Iteration.
   -------------------------------------------------------------------------- */
void theme_next_song(void) {
    theme_pause();  // Aktuelle Musik sofort pausieren
    vTaskDelay(pdMS_TO_TICKS(50));  // Kurz warten bis Musik gestoppt
    
    current_song_index = (current_song_index + 1) % NUM_AVAILABLE_SONGS;
    
    ESP_LOGI(TAG, "Song switched to: %s (index %d)", 
             available_songs[current_song_index].name, 
             current_song_index);
    
    theme_resume();  // Neue Musik starten
}

/* --------------------------------------------------------------------------
   theme_set_song - Zu einem bestimmten Song wechseln
   
   @param index: Song-Index (0=TETRIS, 1=STARWARS, 2=MARIO, 3=SILENCE)
   
   Wurde für zukünftige Features vorgesehen (z.B. Song-Auswahl über Display)
   -------------------------------------------------------------------------- */
void theme_set_song(int index) {
    if (index >= 0 && index < NUM_AVAILABLE_SONGS) {
        theme_pause();
        vTaskDelay(pdMS_TO_TICKS(50));
        
        current_song_index = index;
        
        ESP_LOGI(TAG, "Song set to: %s (index %d)", 
                 available_songs[current_song_index].name, 
                 current_song_index);
        
        theme_resume();
    } else {
        ESP_LOGW(TAG, "Invalid song index: %d (valid range: 0-%d)", 
                 index, NUM_AVAILABLE_SONGS - 1);
    }
}

/* --------------------------------------------------------------------------
   theme_get_current_song - Aktuellen Song-Index abrufen
   
   @return: Index des aktuellen Songs (0=TETRIS, 1=STARWARS, etc.)
   
   Nützlich für Display-Anzeige "Now Playing: MARIO"
   -------------------------------------------------------------------------- */
int theme_get_current_song(void) {
    return current_song_index;
}