# ESP32-S3 Tetris - Systemdokumentation

## 1. System-Übersicht

**Hardware:** ESP32-S3 (Dual-Core @ 240 MHz), 512 KB RAM  
**Software:** ESP-IDF v5.5.1, FreeRTOS v10.5.1  
**Programmiersprache:** C (C11 Standard)  

### Hauptkomponenten:
- **LED-Matrix:** 16x24 WS2812B RGB LEDs (384 LEDs total, 5V, 60mA/LED max)
- **Display:** SSD1306 OLED 128x64 I2C (0x3C, 400 kHz, monochrom)
- **Eingabe:** 4 GPIO-Buttons mit ISR-Handling (Pull-up, aktiv-LOW, 150ms Debounce)
- **Soundausgabe:** LEDC PWM Buzzer für Tetris-Theme (optional)
- **Persistenz:** NVS (Non-Volatile Storage) für Highscore
- **Kommunikation:** UART (115200 baud) für Debug-Logging

### System-Architektur Überblick:
```
┌─────────────────────────────────────────────────────────────────┐
│                     ESP32-S3 Dual-Core                          │
│  ┌──────────────────────┐      ┌──────────────────────┐         │
│  │   Core 0 (PRO_CPU)   │      │   Core 1 (APP_CPU)   │         │
│  │  - FreeRTOS Kernel   │      │  - GameLoop Task     │         │
│  │  - WiFi Stack        │      │  - Theme Task        │         │
│  │  - System Tasks      │      │  - LVGL Updates      │         │
│  └──────────────────────┘      └──────────────────────┘         │
│           │                              │                      │
│           └──────────┬───────────────────┘                      │
│                      │                                          │
│         ┌────────────┴───────────┐                              │
│         │  Shared Resources:     │                              │
│         │  - RAM (512 KB SRAM)   │                              │
│         │  - Flash (SPI)         │                              │
│         │  - Peripherals         │                              │
│         └────────────────────────┘                              │
└─────────────────────────────────────────────────────────────────┘
         │         │          │          │
         │         │          │          │
    ┌────▼───┐  ┌──▼──┐   ┌───▼────┐  ┌──▼──┐
    │ GPIO   │  │ RMT │   │  I2C   │  │ NVS │
    │ ISR    │  │ LED │   │ OLED   │  │Flash│
    └────────┘  └─────┘   └────────┘  └─────┘
```

---

## Hardware-Architektur

### Pin-Belegung

#### LED-Matrix (WS2812B)
- **GPIO 1:** LED-Datenleitung (RMT-Protokoll)
- **Anzahl LEDs:** 384 (16 Breite × 24 Höhe)
- **Protokoll:** WS2812B via ESP-IDF RMT Driver

#### Buttons (Pull-up, aktiv LOW)
- **GPIO 4:** BTN_LEFT (Links bewegen)
- **GPIO 5:** BTN_RIGHT (Rechts bewegen)
- **GPIO 6:** BTN_ROTATE (Block rotieren)
- **GPIO 7:** BTN_FASTER (Schneller fallen lassen)
- **ISR-Trigger:** Negative Flanke (NEGEDGE)
- **Debounce:** 150ms Software-Debounce

#### OLED Display (SSD1306)
- **GPIO 20:** I2C SCL (400 kHz, mit Pull-up)
- **GPIO 21:** I2C SDA (400 kHz, mit Pull-up)
- **Adresse:** 0x3C
- **Auflösung:** 128×64 Monochrom
- **Bibliothek:** LVGL mit ESP-LVGL-Port

#### Buzzer (optional)
- **LEDC PWM:** Für Tetris-Themesong

---

## Software-Architektur

### Modular aufgebautes System

```
┌─────────────────────────────────────────────────────────────┐
│                         app_main()                          │
│  - Hardware-Initialisierung                                 │
│  - Splash Screen                                            │
│  - Game Loop Task starten                                   │
└─────────────────────────────────────────────────────────────┘
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
        ▼                   ▼                   ▼
┌──────────────┐  ┌──────────────────┐  ┌──────────────┐
│ Controls     │  │ GameLoop Task    │  │ Theme Task   │
│ (ISR Queue)  │  │ (5ms Polling)    │  │ (Hintergrund)│
└──────────────┘  └──────────────────┘  └──────────────┘
        │                   │
        │         ┌─────────┴─────────┐
        │         │                   │
        ▼         ▼                   ▼
┌──────────────────────┐    ┌──────────────────┐
│ Block Physics        │    │ Grid Manager     │
│ - Bewegung           │    │ - Kollision      │
│ - Rotation           │    │ - Zeilen löschen │
│ - Fall-Logik         │    │ - Gravity        │
└──────────────────────┘    └──────────────────┘
                │                   │
                └─────────┬─────────┘
                          ▼
                ┌──────────────────┐
                │ Render Engine    │
                │ - LED-Matrix     │
                │ - OLED Display   │
                └──────────────────┘
```

### Module und Dateien

| Modul | Dateien | Verantwortung |
|-------|---------|---------------|
| **main.c** | `main/src/main.c` | System-Initialisierung, Entry Point |
| **GameLoop** | `GameLoop.c/h` | Spiel-Hauptschleife, Zustandsmaschine |
| **Controls** | `Controls.c/h` | Button-ISR, Event-Queue, Debouncing |
| **Blocks** | `Blocks.c/h` | Tetromino-Definitionen, Rotation, Farben |
| **Grid** | `Grid.c/h` | Spielfeld, Kollisionserkennung, Zeilen löschen |
| **Splash** | `Splash.c/h` | Startbildschirm-Animation |
| **Score** | `Score.c/h` | Punkteverwaltung, NVS Highscore |
| **SpeedManager** | `SpeedManager.c/h` | Dynamische Geschwindigkeit |
| **DisplayInit** | `DisplayInit.c/h` | OLED I2C, LVGL Integration |
| **LedMatrixInit** | `LedMatrixInit.c/h` | WS2812B Mapping |
| **ThemeSong** | `ThemeSong.c/h` | Tetris-Musik via PWM |
| **Colors** | `Colors.c` | RGB-Farbtabelle für Blöcke |
| **Globals** | `Globals.h` | Konfigurationsparameter |

---

## Detaillierte Prozessablauf-Diagramme

### 1. System-Start (Boot-Sequenz mit exakten Timings)

```
Zeit    Phase                   Details                                          Status
─────────────────────────────────────────────────────────────────────────────────────────
T+0ms   ┌──────────────────┐
        │ Bootloader Start │  1st Stage Bootloader (ROM)                      [HARDWARE]
        └────────┬─────────┘  - Lädt 2nd Stage Bootloader aus Flash
                 │            - Validiert Signaturen
T+50ms           ▼            - CPU: 40 MHz (Boot-Modus)
        ┌──────────────────┐
        │ 2nd Bootloader   │  ESP-IDF Bootloader                              [FIRMWARE]
        └────────┬─────────┘  - Initialisiert Flash-Mapping
                 │            - Lädt Partition Table
                 │            - Lädt Application Binary
T+100ms          ▼
        ┌──────────────────┐
        │ FreeRTOS Start   │  Kernel Initialization                           [OS]
        └────────┬─────────┘  - CPU auf 240 MHz hochgetaktet
                 │            - Heap-Allokator initialisiert
                 │            - Idle Task erstellt (Prio 0)
T+120ms          ▼
        ┌─────────────────────────────────────────────┐
        │ app_main() Entry Point                      │                       [APP]
        └────────┬────────────────────────────────────┘
                 │
T+150ms          ▼
        ┌─────────────────────────────────────────────┐
        │ setup_led_strip()                           │                       [INIT]
        │ ┌─────────────────────────────────────────┐ │
        │ │ - RMT Channel allokieren (10 MHz)       │ │ → GPIO 1
        │ │ - DMA Buffer erstellen (384 LEDs)       │ │ → ~4.6 KB RAM
        │ │ - Timing-Parameter setzen (WS2812B)     │ │ → 0.4µs/1.25µs/1.25µs
        │ │ - led_strip_clear() + refresh()         │ │ → Alle LEDs AUS
        │ └─────────────────────────────────────────┘ │
        └────────┬────────────────────────────────────┘
T+200ms          ▼
        ┌─────────────────────────────────────────────┐
        │ LedMatrixInit()                             │                       [INIT]
        │ - LED_MATRIX_NUMMER[][] kopieren            │ → 16×24×2 Bytes = 768B
        │ - Matrix-Koordinaten → LED-Nummern Mapping  │ → Verdrahtungs-Layout
        └────────┬────────────────────────────────────┘
T+220ms          ▼
        ┌─────────────────────────────────────────────┐
        │ Random Seed Generation                      │                       [INIT]
        │ - esp_random() aufrufen                     │ → Hardware RNG
        │ - seed_counter inkrementieren               │ → Variation über Reboots
        │ - srand(esp_random() ^ seed_counter)        │ → C stdlib RNG seeden
        └────────┬────────────────────────────────────┘
T+230ms          ▼
        ┌─────────────────────────────────────────────┐
        │ splash_show(SPLASH_DURATION_MS)             │                       [SPLASH]
        │ ┌─────────────────────────────────────────┐ │
        │ │ - Statischen Hintergrund rendern        │ │ → Tetromino-Muster
        │ │ - "TETRIS" Text-Bitmap generieren       │ │ → 3×5 Font, 6 Zeichen
        │ │ - Scroll-Loop starten (40ms pro Frame)  │ │ → 100 Frames @ 25 FPS
        │ │ - Warten auf Button ODER 4s Timeout     │ │ → controls_get_event()
        │ └─────────────────────────────────────────┘ │
        └────────┬────────────────────────────────────┘
                 │ (Button gedrückt oder 4s vergangen)
T+4230ms         ▼
        ┌─────────────────────────────────────────────┐
        │ led_strip_clear() + refresh()               │                       [CLEAR]
        │ - Alle LEDs ausschalten                     │
        └────────┬────────────────────────────────────┘
T+4250ms         ▼
        ┌─────────────────────────────────────────────┐
        │ grid_init() + score_init()                  │                       [GAME]
        │ - grid[24][16] = {0}                        │ → 384 Bytes
        │ - score = 0, lines_cleared = 0              │
        └────────┬────────────────────────────────────┘
T+4260ms         ▼
        ┌─────────────────────────────────────────────┐
        │ score_load_highscore()                      │                       [NVS]
        │ ┌─────────────────────────────────────────┐ │
        │ │ 1. nvs_flash_init()                     │ │ → Flash-Partition mounten
        │ │ 2. nvs_open("tetris", NVS_READWRITE)    │ │ → Namespace öffnen
        │ │ 3. nvs_get_u32("highscore", &hs)        │ │ → Highscore lesen
        │ │ 4. Falls nicht gefunden: highscore = 0  │ │ → Erste Verwendung
        │ └─────────────────────────────────────────┘ │
        └────────┬────────────────────────────────────┘
T+4280ms         ▼
        ┌─────────────────────────────────────────────┐
        │ display_init()                              │                       [I2C]
        │ ┌─────────────────────────────────────────┐ │
        │ │ 1. I2C Bus initialisieren               │ │ → GPIO 20/21, 50 kHz
        │ │ 2. I2C Device Scan (0x08-0x77)          │ │ → Suche nach 0x3C
        │ │ 3. SSD1306 Panel init (falls gefunden)  │ │ → 128×64 Monochrom
        │ │ 4. LVGL Port starten                    │ │ → UI-Framework
        │ │ 5. HUD erstellen (Titel/Score/High)     │ │ → Labels platzieren
        │ │ 6. Highscore anzeigen                   │ │ → "High: XXXX"
        │ └─────────────────────────────────────────┘ │
        │ Note: Graceful Degradation wenn kein Display│ → g_disp = NULL
        └────────┬────────────────────────────────────┘
T+4380ms         ▼
        ┌─────────────────────────────────────────────┐
        │ init_controls()                             │                       [GPIO]
        │ ┌─────────────────────────────────────────┐ │
        │ │ 1. GPIO 4,5,6,7 als Input konfigurieren │ │ → Pull-up aktiviert
        │ │ 2. Interrupt auf NEGEDGE setzen         │ │ → Fallende Flanke
        │ │ 3. FreeRTOS Queue erstellen (8 Events)  │ │ → ISR → Task Kommunikation
        │ │ 4. ISR Service installieren             │ │ → gpio_install_isr_service()
        │ │ 5. Handler für jeden Button registrieren│ │ → gpio_isr_handler_add()
        │ └─────────────────────────────────────────┘ │
        └────────┬────────────────────────────────────┘
T+4400ms         ▼
        ┌─────────────────────────────────────────────┐
        │ StartTheme()                                │                       [AUDIO]
        │ - Theme-Task erstellen (Prio 1, 2048 Stack) │ → Hintergrund-Musik
        │ - LEDC PWM für Buzzer konfigurieren         │ → Optional
        └────────┬────────────────────────────────────┘
T+4420ms         ▼
        ┌─────────────────────────────────────────────┐
        │ start_game_loop()                           │                       [TASK]
        │ - xTaskCreate(game_loop_task)               │ → Prio 5, 4096 Stack
        │ - Task startet in WAIT State                │ → Wartet auf Button
        └────────┬────────────────────────────────────┘
T+4450ms         ▼
        ┌─────────────────────────────────────────────┐
        │ app_main() Idle Loop                        │                       [IDLE]
        │ while(1) { vTaskDelay(1000ms); }            │ → Hält Task am Leben
        └─────────────────────────────────────────────┘
                 │
                 ▼
        ┌─────────────────────────────────────────────┐
        │ System Running (Multi-Tasking)              │                       [RUN]
        │ ┌─────────────────────────────────────────┐ │
        │ │ Core 0: FreeRTOS Kernel, WiFi, System   │ │
        │ │ Core 1: GameLoop Task (5ms Polling)     │ │
        │ │         Theme Task (Hintergrund)        │ │
        │ │         LVGL Timer (UI-Updates)         │ │
        │ └─────────────────────────────────────────┘ │
        └─────────────────────────────────────────────┘

Total Boot Time: ~4.45 Sekunden
Memory Usage at Boot:
  - Heap Free: ~380 KB (von 512 KB total)
  - Stack GameLoop: 4096 Bytes allokiert
  - Stack Theme: 2048 Bytes allokiert
  - DMA Buffers: ~5 KB (RMT + LVGL)
```

---

### 2. Game Loop State Machine (Detailliert mit allen Transitions)

```
                          ╔════════════════════╗
                          ║   BOOT COMPLETE    ║
                          ╚══════════╤═════════╝
                                     │
                                     │ game_loop_task() startet
                                     │ game_running = false
                                     ▼
                          ┌──────────────────────┐
                    ┌────►│   STATE: WAIT        │◄─────┐
                    │     │  (Splash scrollend)  │      │
                    │     └──────────┬───────────┘      │
                    │                │                  │
                    │                │ Button Event     │
                    │                │ (Queue/Polling)  │
                    │                ▼                  │
                    │     ┌──────────────────────┐      │
                    │     │ Transition: WAIT→RUN │      │
                    │     ├──────────────────────┤      │
                    │     │ 1. splash_clear()    │      │
                    │     │ 2. grid_init()       │      │
                    │     │ 3. score_init()      │      │
                    │     │ 4. speed_reset()     │      │
                    │     │ 5. spawn_block()     │      │
                    │     │ 6. game_running=true │      │
                    │     └──────────┬───────────┘      │
                    │                │                  │
                    │                ▼                  │
     Emergency      │     ╔══════════════════════╗      │ Game Over
     Reset (4 BTN)  │     ║  STATE: RUNNING      ║      │ Flag
     1s Hold        │     ║  Hauptspiel-Logik    ║      │
                    │     ╚══════════╤═══════════╝      │
                    │                │                  │
                    │     ┌──────────┴──────────┐       │
                    │     │                     │       │
                    │     ▼                     ▼       │
                    │  ┌─────────────┐    ┌──────────────┐
                    │  │ Input Loop  │    │ Physics Loop │
                    │  │ (5ms Poll)  │    │ (Zeit-basiert│
                    │  ├─────────────┤    ├──────────────┤
                    │  │·BTN_LEFT    │    │·Auto-Fall    │
                    │  │·BTN_RIGHT   │    │ fall_interval│
                    │  │·BTN_ROTATE  │    │·Kollisions-  │
                    │  │·BTN_FASTER  │    │ prüfung      │
                    │  │·Alle 4 BTN? │    │·Block fix?   │
                    │  └──────┬──────┘    └──────┬───────┘
                    │         │                  │
                    │         └────────┬─────────┘
                    │                  │
                    │                  ▼
                    │         ┌─────────────────┐
                    │         │ Block platziert?│──No──┐
                    │         └────────┬────────┘      │
                    │                  │Yes            │
                    │                  ▼               │
                    │         ┌─────────────────┐      │
                    │         │grid_fix_block() │      │
                    │         ├─────────────────┤      │
                    │         │1. In grid[][]   │      │
                    │         │   schreiben     │      │
                    │         │2. LEDs setzen   │      │
                    │         │3. grid_clear_   │      │
                    │         │   full_rows()   │      │
                    │         └────────┬────────┘      │
                    │                  │               │
                    │                  ▼               │
                    │         ┌─────────────────┐      │
                    │         │Zeilen voll?     │──No──┤
                    │         └────────┬────────┘      │
                    │                  │Yes            │
                    │                  ▼               │
                    │         ┌──────────────────────┐ │
                    │         │ Line Clear Sequence  │ │
                    │         ├──────────────────────┤ │
                    │         │1. Zeilen identifizier│ │
                    │         │2. Blink-Animation    │ │
                    │         │   (2×150ms weiß)     │ │
                    │         │3. Zeilen löschen     │ │
                    │         │4. Gravity anwenden   │ │
                    │         │5. Score updaten      │ │
                    │         │   ·1L:+100, 2L:+300  │ │
                    │         │   ·3L:+500, 4L:+800  │ │
                    │         │6. Speed updaten      │ │
                    │         │7. Display updaten    │ │
                    │         └──────────┬───────────┘ │
                    │                    │             │
                    │                    ▼             │
                    │         ┌─────────────────┐      │
                    │         │spawn_block()    │◄─────┘
                    │         ├─────────────────┤
                    │         │1. Typ wählen    │
                    │         │   (0-6 random)  │
                    │         │2. Position suchen
                    │         │   (Mitte bevorzugt)│
                    │         │3. Kollision?    │
                    │         └────────┬────────┘
                    │                  │
                    │       ┌──────────┴──────────┐
                    │       │                     │
                    │       ▼No                   ▼Yes
                    │  ┌─────────┐        ┌──────────────┐
                    │  │ SUCCESS │        │ KEIN PLATZ!  │
                    │  │ Block   │        │ Game Over    │
                    │  │ spawned │        └──────┬───────┘
                    │  └────┬────┘               │
                    │       │                    ▼
                    │       │           ┌─────────────────────┐
                    │       │           │handle_game_over()   │
                    │       │           ├─────────────────────┤
                    │       │           │1. score_update_     │
                    │       │           │   highscore()       │
                    │       │           │   · NVS speichern   │
                    │       │           │2. display_show_     │
                    │       │           │   game_over()       │
                    │       │           │3. Blink-Animation   │
                    │       │           │   (3× rot 300ms)    │
                    │       │           │4. game_over_flag=1  │
                    │       │           └──────────┬──────────┘
                    │       │                      │
                    │       │                      ▼
                    │       │           ┌─────────────────────┐
                    │       │           │ STATE: GAME_OVER    │
                    │       │           │ (Flag-Driven)       │
                    │       │           └──────────┬──────────┘
                    │       │                      │
                    │       │                      ▼
                    │       │           ┌─────────────────────┐
                    │       │           │wait_for_restart()   │
                    │       │           ├─────────────────────┤
                    │       │           │1. Queue drainieren  │
                    │       │           │2. Splash 2s (no btn)│
                    │       │           │3. 500ms drain loop  │
                    │       │           │4. Warte auf Release │
                    │       │           │5. splash_show_      │
                    │       │           │   waiting()         │
                    │       │           │6. Warte auf Button  │
                    │       │           └──────────┬──────────┘
                    │       │                      │
                    │       └──────────────────────┴──────────┐
                    │                                         │
                    └─────────────────────────────────────────┘
                                     │
                              (Loop zurück zu WAIT)

Zusätzliche Transitions:

Emergency Reset Detection (jederzeit während RUNNING):
  ┌─────────────────────────────────────────────┐
  │ if (controls_all_buttons_pressed())         │
  │   vTaskDelay(1000ms)  // 1s Hold-Check      │
  │   if (still_pressed):                       │
  │     → Hard Reset Sequenz (wie Game Over)    │
  │     → Zurück zu WAIT State                  │
  └─────────────────────────────────────────────┘

Rendering (parallel zu State Machine):
  ┌─────────────────────────────────────────────┐
  │ Render Loop (60 FPS, 16ms Intervall)        │
  │ if (current_time - last_render >= 16ms):    │
  │   render_grid()                             │
  │   · Vorherige dynamische Pixel restaurieren │
  │   · Aktuellen Block zeichnen                │
  │   · led_strip_refresh()                     │
  └─────────────────────────────────────────────┘

State Variables:
  - game_running: bool     // false=WAIT, true=RUNNING
  - game_over_flag: int    // 1=GAME_OVER erkannt
  - current_block: struct  // Aktueller Tetromino
  - grid[24][16]: uint8_t  // Fixierte Blöcke
  - score: int            // Aktueller Score
  - total_lines: uint32_t // Geclearte Zeilen (für Speed)
```

---

### 3. Game Loop Task - Mikro-Ebene (Jeden 5ms Zyklus)

```
╔══════════════════════════════════════════════════════════════════════════════╗
║ game_loop_task() - FreeRTOS Task                                             ║
║ Priorität: 5 (High), Stack: 4096 Bytes, Polling: 5ms, CPU: Core 1            ║
╚══════════════════════════════════════════════════════════════════════════════╝

T+0µs   ┌─────────────────────────────────────────────────────────────┐
        │ INITIALIZATION (einmalig beim Task-Start)                   │
        ├─────────────────────────────────────────────────────────────┤
        │ speed_manager_init()        → fall_interval = 400ms         │
        │ grid_init()                 → memset(grid, 0, 384 bytes)    │
        │ score_init()                → score = 0, lines = 0          │
        │ display_reset_and_show_hud()→ LVGL Screen aufbauen          │
        │ game_running = false        → Startet in WAIT State         │
        │ last_fall_time = 0          → Timer-Variablen               │
        │ last_render_time = 0        →                               │
        └─────────────────────────────────────────────────────────────┘
                                │
                                ▼
        ╔═════════════════════════════════════════════════════════════╗
        ║ MAIN LOOP: while(1) { ... vTaskDelay(5ms); }                ║
        ║ Jede Iteration = 1 "Tick" = 5ms Mindestdauer                ║
        ╚═════════════════════════════════════════════════════════════╝
                                │
        ┌───────────────────────┴───────────────────────┐
        │ Loop Iteration N (T = N × 5ms nach Task-Start)│
        └───────────────────────┬───────────────────────┘
                                │
T+0µs                           ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ [1] current_time = xTaskGetTickCount() * portTICK_PERIOD_MS │
        │     Lese aktuelle Zeit in Millisekunden                     │
        │     Typ: uint32_t, Overflow nach ~49.7 Tage                 │
        └─────────────────────────────────────────────────────────────┘
                                │
T+10µs                          ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ [2] EMERGENCY RESET CHECK                                   │
        │ ┌─────────────────────────────────────────────────────────┐ │
        │ │ if (controls_all_buttons_pressed()) {                   │ │
        │ │   // Alle 4 GPIOs prüfen: gpio_get_level() == 0?        │ │
        │ │   vTaskDelay(1000ms);  // 1 Sekunde halten              │ │
        │ │   if (controls_all_buttons_pressed()) {                 │ │
        │ │     printf("[GameLoop] Emergency reset triggered\n");   │ │
        │ │     // Hard Reset Sequenz:                              │ │
        │ │     reset_game_state();                                 │ │
        │ │     led_strip_clear() + refresh();                      │ │
        │ │     game_running = false;                               │ │
        │ │     wait_for_restart();  // Splash + Queue Drain + Wait │ │
        │ │     // Spiel neu starten:                               │ │
        │ │     splash_clear();                                     │ │
        │ │     reset_game_state();                                 │ │
        │ │     spawn_block();                                      │ │
        │ │     game_running = true;                                │ │
        │ │     continue;  // Zurück zu Loop-Anfang                 │ │
        │ │   }                                                     │ │
        │ │ }                                                       │ │
        │ └─────────────────────────────────────────────────────────┘ │
        │ Typische Ausführungszeit: 50µs (wenn nicht gedrückt)        │
        │                          1000ms (wenn 1s halten)            │
        └─────────────────────────────────────────────────────────────┘
                                │
T+60µs                          ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ [3] GAME OVER FLAG CHECK                                    │
        │ ┌─────────────────────────────────────────────────────────┐ │
        │ │ if (game_over_flag) {                                   │ │
        │ │   game_over_flag = 0;                                   │ │
        │ │   game_running = false;                                 │ │
        │ │   display_reset_and_show_hud(highscore);                │ │
        │ │   wait_for_restart();  // Splash + Drain + Wait         │ │
        │ │   // Spiel neu starten:                                 │ │
        │ │   splash_clear();                                       │ │
        │ │   reset_game_state();                                   │ │
        │ │   spawn_block();                                        │ │
        │ │   game_running = true;                                  │ │
        │ │   last_fall_time = current_time;                        │ │
        │ │   last_render_time = current_time;                      │ │
        │ │   continue;                                             │ │
        │ │ }                                                       │ │
        │ └─────────────────────────────────────────────────────────┘ │
        │ Typische Ausführungszeit: 5µs (wenn Flag = 0)               │
        │                          3000ms+ (wenn Flag = 1)            │
        └─────────────────────────────────────────────────────────────┘
                                │
T+65µs                          ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ [4] WAIT STATE CHECK                                        │
        │ ┌─────────────────────────────────────────────────────────┐ │
        │ │ if (!game_running) {                                    │ │
        │ │   // WAIT State: Splash scrollt, warte auf Button       │ │
        │ │   splash_show(SPLASH_DURATION_MS);  // Blockiert!       │ │
        │ │   gpio_num_t ev;                                        │ │
        │ │   while (controls_get_event(&ev)) {}  // Queue drain    │ │
        │ │   vTaskDelay(50ms);                                     │ │
        │ │   // Warte auf Button-Release:                          │ │
        │ │   while (check_button_pressed(ANY)) vTaskDelay(20ms);   │ │
        │ │   // Warte auf frischen Button-Press:                   │ │
        │ │   controls_wait_event(&ev, portMAX_DELAY); // Blockiert!│ │
        │ │   vTaskDelay(50ms);                                     │ │
        │ │   // Spiel starten:                                     │ │
        │ │   splash_clear();                                       │ │
        │ │   reset_game_state();                                   │ │
        │ │   spawn_block();                                        │ │
        │ │   game_running = true;                                  │ │
        │ │   last_fall_time = current_time;                        │ │
        │ │   last_render_time = current_time;                      │ │
        │ │   continue;                                             │ │
        │ │ }                                                       │ │
        │ └─────────────────────────────────────────────────────────┘ │
        │ Typische Ausführungszeit: 10µs (wenn game_running = true)   │
        │                          BLOCKIERT (wenn WAIT State)        │
        └─────────────────────────────────────────────────────────────┘
                                │
                                │ (Ab hier: game_running = true garantiert)
                                │
T+75µs                          ▼
        ╔═════════════════════════════════════════════════════════════╗
        ║ [5] RUNNING STATE - INPUT HANDLING                          ║
        ║ Alle Buttons werden JEDEN Loop geprüft (5ms Polling)        ║
        ╚═════════════════════════════════════════════════════════════╝
                                │
T+80µs                          ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ [5a] BTN_LEFT (Block nach links bewegen)                    │
        │ ┌─────────────────────────────────────────────────────────┐ │
        │ │ if (check_button_pressed(BTN_LEFT)) {                   │ │
        │ │   TetrisBlock tmp = current_block;  // Kopie erstellen  │ │
        │ │   tmp.x--;                          // 1 Spalte links   │ │
        │ │   if (!grid_check_collision(&tmp)) {                    │ │
        │ │     current_block = tmp;  // Bewegung gültig            │ │
        │ │   }                                                     │ │
        │ │   // Sonst: Kollision → Bewegung ignoriert              │ │
        │ │ }                                                       │ │
        │ └─────────────────────────────────────────────────────────┘ │
        │ check_button_pressed():                                     │
        │   - gpio_get_level(GPIO_NUM_4) == 0?  (aktiv-LOW)           │
        │   - Software-Debounce: 150ms seit letztem Press?            │
        │   - Return: true/false                                      │
        │ Ausführungszeit: 15µs (ohne Press), 15µs + Kollision (mit)  │
        └─────────────────────────────────────────────────────────────┘
                                │
T+95µs                          ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ [5b] BTN_RIGHT (Block nach rechts bewegen)                  │
        │ if (check_button_pressed(BTN_RIGHT)) {                      │
        │   tmp = current_block; tmp.x++;                             │
        │   if (!grid_check_collision(&tmp)) current_block = tmp;     │
        │ }                                                           │
        │ Ausführungszeit: 15µs                                       │
        └─────────────────────────────────────────────────────────────┘
                                │
T+110µs                         ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ [5c] BTN_ROTATE (Block rotieren, außer O-Block)             │
        │ ┌─────────────────────────────────────────────────────────┐ │
        │ │ if (check_button_pressed(BTN_ROTATE) &&                 │ │
        │ │     current_block.color != 3) {  // 3 = O-Block         │ │
        │ │   tmp = current_block;                                  │ │
        │ │   rotate_block_90(&tmp);  // Matrix-Transformation      │ │
        │ │   if (!grid_check_collision(&tmp)) {                    │ │
        │ │     current_block = tmp;                                │ │
        │ │   }                                                     │ │
        │ │ }                                                       │ │
        │ └─────────────────────────────────────────────────────────┘ │
        │ rotate_block_90():                                          │
        │   - Transponieren + Spiegeln der 4×4 shape[][] Matrix       │
        │   - Ausführungszeit: ~5µs                                   │
        └─────────────────────────────────────────────────────────────┘
                                │
T+130µs                         ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ [5d] BTN_FASTER (Manuell schneller fallen)                  │
        │ if (check_button_pressed(BTN_FASTER)) {                     │
        │   tmp = current_block; tmp.y++;                             │
        │   if (!grid_check_collision(&tmp)) current_block = tmp;     │
        │ }                                                           │
        │ Ausführungszeit: 15µs                                       │
        └─────────────────────────────────────────────────────────────┘
                                │
T+145µs                         ▼
        ╔═════════════════════════════════════════════════════════════╗
        ║ [6] AUTOMATIC FALL (Zeit-basiert)                           ║
        ╚═════════════════════════════════════════════════════════════╝
                                │
T+150µs                         ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ uint32_t fall_interval = speed_manager_get_fall_interval(); │
        │ // fall_interval: 400ms (Level 0) bis 50ms (Level 10)       │
        │                                                             │
        │ if (current_time - last_fall_time >= fall_interval) {       │
        │   last_fall_time = current_time;  // Timer zurücksetzen     │
        │                                                             │
        │   tmp = current_block;                                      │
        │   tmp.y++;  // 1 Zeile nach unten                           │
        │                                                             │
        │   if (!grid_check_collision(&tmp)) {                        │
        │     // Block kann weiter fallen                             │
        │     current_block = tmp;                                    │
        │   } else {                                                  │
        │     // Kollision unten → Block fixieren                     │
        │     grid_fix_block(&current_block);                         │
        │       ├─► grid[y][x] = color + 1  (für jedes Pixel)         │
        │       ├─► led_strip_set_pixel()   (sofort zeichnen)         │
        │       ├─► led_strip_refresh()                               │
        │       └─► grid_clear_full_rows()                            │
        │             ├─► Volle Zeilen finden                         │
        │             ├─► Blink-Animation (2× weiß)                   │
        │             ├─► Zeilen löschen                              │
        │             ├─► Gravity anwenden (spaltenweise)             │
        │             ├─► score_add_lines(count)                      │
        │             ├─► speed_manager_update_score(total_lines)     │
        │             └─► display_update_score(score, highscore)      │
        │     spawn_block();  // Neuer Block                          │
        │       └─► Falls kein Platz: handle_game_over()              │
        │             └─► game_over_flag = 1 (Loop reagiert nächste Iteration)│
        │   }                                                         │
        │ }                                                           │
        │ Ausführungszeit: 20µs (kein Fall), 50µs+ (bei Fall)         │
        │                  500ms+ (bei Zeilen-Clear + Animation)      │
        └─────────────────────────────────────────────────────────────┘
                                │
T+200µs                         ▼
        ╔═════════════════════════════════════════════════════════════╗
        ║ [7] RENDERING (60 FPS = alle 16ms)                          ║
        ╚═════════════════════════════════════════════════════════════╝
                                │
T+220µs                         ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ if (current_time - last_render_time >= RENDER_INTERVAL_MS) {│
        │   last_render_time = current_time;                          │
        │   render_grid();                                            │
        │   ┌───────────────────────────────────────────────────────┐ │
        │   │ Render-Pipeline (Optimiert):                          │ │
        │   │ 1. Vorherige dynamische Pixel restaurieren            │ │
        │   │    for (i=0; i<prev_dynamic_count; i++):              │ │
        │   │      pos = prev_dynamic_pos[i]                        │ │
        │   │      if grid[pos] > 0: Farbe setzen                   │ │
        │   │      else: Pixel löschen (0,0,0)                      │ │
        │   │                                                       │ │
        │   │ 2. Aktuellen Block zeichnen                           │ │
        │   │    for (by=0; by<4; by++)                             │ │
        │   │      for (bx=0; bx<4; bx++)                           │ │
        │   │        if shape[by][bx]:                              │ │
        │   │          get_block_rgb(color, &r, &g, &b)             │ │
        │   │          r/g/b *= BRIGHTNESS_SCALE                    │ │
        │   │          led_strip_set_pixel(led_num, r, g, b)        │ │
        │   │          prev_dynamic_pos[count++] = [y,x]            │ │
        │   │                                                       │ │
        │   │ 3. LED-Matrix aktualisieren                           │ │
        │   │    led_strip_refresh()                                │ │
        │   │      └─► RMT sendet Daten an WS2812B                  │ │
        │   │      └─► Timing: ~5ms für 384 LEDs                    │ │
        │   └───────────────────────────────────────────────────────┘ │
        │ }                                                           │
        │ Ausführungszeit: 10µs (kein Render), 5000µs (bei Render)    │
        └─────────────────────────────────────────────────────────────┘
                                │
T+5220µs                        ▼
        ┌─────────────────────────────────────────────────────────────┐
        │ [8] TASK DELAY (5ms Mindest-Intervall)                      │
        │ vTaskDelay(pdMS_TO_TICKS(LOOP_INTERVAL_MS));                │
        │ // LOOP_INTERVAL_MS = 5ms                                   │
        │ // Task gibt CPU frei für andere Tasks                      │
        │ // FreeRTOS Scheduler wählt nächsten Task                   │
        └─────────────────────────────────────────────────────────────┘
                                │
                                │ (Zurück zu Loop-Anfang)
                                │
                                ▼

## Render-Pipeline & Display-Aktualisierung

### WS2812B LED Matrix - Timing-Übersicht

**Render-Pipeline** (Game-State → LED-Ausgabe bei 60 FPS):

| Phase                     | Dauer    | Details                                             |
|---------------------------|----------|-----------------------------------------------------|
| Delta-Rendering           | ~0.5ms   | Vorherige Block-Pixel restaurieren (16-32 Pixel)    |
| Block zeichnen            | ~2.0ms   | Aktuelle Block-Position + Farbe + Brightness        |
| RMT-Setup                 | ~0.1ms   | Pixel-Buffer → RMT-Channel (DMA-Vorbereitung)       |
| RMT-Transfer (Blocking)   | ~11.5ms  | 384 LEDs × 24 Bits @ 1.25µs/Bit = 11.52ms           |
| WS2812B-Processing        | ~0.1ms   | Reset-Signal + LED-Aktivierung                      |
| **GESAMT**                | **~14ms**| **Innerhalb 16.67ms @ 60 FPS Target**               |

**CPU-Auslastung**: 2.6ms aktiv (~18%) + 11.5ms RMT-Warten (~82%) = 14ms total

**WS2812B Timing-Spezifikation** (RMT-generiert):
- **Bit 0**: HIGH 0.4µs, LOW 0.85µs (Toleranz: ±150ns)
- **Bit 1**: HIGH 0.8µs, LOW 0.45µs (Toleranz: ±150ns)
- **Reset**: LOW >50µs (Frame-Ende-Signal)
- **Datenformat**: GRB (Green-Red-Blue), 8 Bit pro Kanal
- **Übertragung**: Seriell via GPIO2, Daisy-Chain 384 LEDs

**Optimierungen**:
- ✅ Delta-Rendering aktiv (nur geänderte Pixel)
- ✅ RMT-Hardware + DMA (CPU-Entlastung)
- ❌ Blocking-RMT (Async würde 11.5ms CPU freigeben, benötigt Double-Buffering)

### OLED Display (SSD1306) - Timing-Übersicht

**OLED-Update-Pipeline** (Score/Level/Highscore bei Line-Clear oder Level-Up):

| Phase              | Dauer    | Details                                                        |
|--------------------|----------|----------------------------------------------------------------|
| LVGL Label-Update  | ~0.5ms   | Text-Formatierung + Framebuffer (128×64 @ 1bpp = 1024 Bytes)   |
| Font-Rendering     | ~0.4ms   | Montserrat 14pt, Character → Glyph-Bitmap ("Score: 1234")      |
| I2C-Transfer       | ~2.5ms   | Partial-Update (nur Score-Zeile, ~128 Bytes @ 400kHz)          |
| SSD1306 Update     | ~0.1ms   | GDDRAM → Display-Matrix, automatisch @ 60Hz                    |
| **GESAMT**         | **~3.5ms**| **Nur bei Score-Änderung (~1-2x/min)**                        |

**I2C-Konfiguration**:
- **Adresse**: 0x3C (7-bit)
- **Clock**: 400kHz (Fast Mode)
- **Pins**: SCL: GPIO22, SDA: GPIO21
- **Transfer-Modi**:
  - Partial-Update (Score): ~128 Bytes → 2.5ms
  - Full-Screen (Game Over): 1024 Bytes → 20ms

**Parallelität**:
- ✅ OLED läuft parallel zu LED-Matrix (eigener I2C-Peripheral, blockiert RMT nicht)
- ✅ LVGL in separatem Timer-Task (niedrigere Priorität als GameLoop)
- ✅ Keine Blockierung der Game-Logic
- ✅ CPU-Last <1% (Update selten)

**Timing-Vergleich**: LED-Matrix 14ms @ 60 FPS vs. OLED 3ms @ 0.5 Hz → vernachlässigbar

---

### Performance-Analyse pro Loop-Iteration
═════════════════════════════════════════════════════════════
Szenario                    | CPU-Zeit  | Anteil | Bemerkung
────────────────────────────|-----------|--------|──────────────────────
Minimaler Loop (kein Input) | ~350µs    | 7%     | Nur Checks + Delay
Input-Handling (1 Button)   | ~380µs    | 7.6%   | + Kollisionsprüfung
Auto-Fall (Block bewegt)    | ~400µs    | 8%     | + Physics
Render-Frame (16ms Timer)   | ~5350µs   | 107%   | RMT-Transfer dominiert
Zeilen-Clear (mit Blink)    | ~550ms    | 11000%!| Animation blockiert
Game Over (mit Blink)       | ~1500ms   | 30000%!| 3× Blink-Sequenz

CPU-Auslastung (Durchschnitt):
  - Idle: ~10% (nur Polling, kein Render nötig)
  - Normal: ~25% (60 FPS Rendering)
  - Peak: ~40% (Zeilen-Clear + Render)
```

---

## Event-Flow & ISR-Handling

### ISR → Queue → Task Pipeline (Detailliert)

```
╔═══════════════════════════════════════════════════════════════════════════╗
║                    BUTTON PRESS EVENT FLOW                                ║
║  Von Hardware-Signal bis zur Game-Logic (Mikrosekunden-Präzision)         ║
╚═══════════════════════════════════════════════════════════════════════════╝

T+0ns   ┌───────────────────────────────────────────────────────────────┐
        │ HARDWARE: Button physisch gedrückt                            │
        │ ┌───────────────────────────────────────────────────────────┐ │
        │ │ User drückt BTN_LEFT (GPIO 4)                             │ │
        │ │ • Mechanischer Kontakt schließt                           │ │
        │ │ • Spannung fällt: 3.3V → 0V (Pull-up → GND)               │ │
        │ │ • Signal-Transition: HIGH → LOW (aktiv-LOW Logik)         │ │
        │ │ • Prellen: ~1-10ms mechanisches Bouncing                  │ │
        │ └───────────────────────────────────────────────────────────┘ │
        └───────────────────────────────────────────────────────────────┘
                                    │
T+50ns                              ▼
        ┌───────────────────────────────────────────────────────────────┐
        │ GPIO PERIPHERAL: Negative Flanke erkannt                      │
        │ ┌───────────────────────────────────────────────────────────┐ │
        │ │ GPIO-Einheit im ESP32 erkennt Pegel-Wechsel               │ │
        │ │ • Interrupt-Konfiguration: GPIO_INTR_NEGEDGE              │ │
        │ │ • Flanken-Detektor triggert                               │ │
        │ │ • Interrupt-Signal an CPU-Core                            │ │
        │ └───────────────────────────────────────────────────────────┘ │
        └───────────────────────────────────────────────────────────────┘
                                    │
T+100ns                             ▼
        ┌───────────────────────────────────────────────────────────────┐
        │ CPU: Interrupt-Anforderung                                    │
        │ ┌───────────────────────────────────────────────────────────┐ │
        │ │ CPU unterbricht aktuellen Code (GameLoop oder Idle)       │ │
        │ │ • Kontext speichern (Register auf Stack)                  │ │
        │ │ • Sprung zur ISR-Vektor-Tabelle                           │ │
        │ │ • ISR-Handler Adresse laden                               │ │
        │ │ • Sprung zu gpio_isr_handler()                            │ │
        │ └───────────────────────────────────────────────────────────┘ │
        │ Latenz: ~100-200ns (Hardware-abhängig)                        │
        └───────────────────────────────────────────────────────────────┘
                                    │
T+500ns                             ▼
        ╔═══════════════════════════════════════════════════════════════╗
        ║ ISR EXECUTION: gpio_isr_handler() [IRAM_ATTR]                 ║
        ║ Läuft im INTERRUPT CONTEXT (höchste Priorität)                ║
        ╚═══════════════════════════════════════════════════════════════╝
                                    │
T+600ns                             ▼
        ┌───────────────────────────────────────────────────────────────┐
        │ static void IRAM_ATTR gpio_isr_handler(void* arg) {           │
        │                                                               │
        │   [1] Queue-Gültigkeits-Check (~10ns)                         │
        │       if (s_button_queue == NULL) return;                     │
        │                                                               │
        │   [2] GPIO-Nummer extrahieren (~5ns)                          │
        │       gpio_num_t g = (gpio_num_t)(intptr_t)arg;               │
        │       // arg wurde bei gpio_isr_handler_add() übergeben       │
        │       // Für BTN_LEFT: arg = (void*)GPIO_NUM_4 = (void*)4     │
        │                                                               │
        │   [3] Context-Switch Flag vorbereiten (~5ns)                  │
        │       BaseType_t xHigherPriorityTaskWoken = pdFALSE;          │
        │                                                               │
        │   [4] Event in Queue pushen (~200ns)                          │
        │       xQueueSendFromISR(s_button_queue, &g,                   │
        │                         &xHigherPriorityTaskWoken);           │
        │       // Queue: FIFO, Kapazität 8 Events                      │
        │       // Item-Größe: sizeof(gpio_num_t) = 4 Bytes             │
        │       // Wenn Queue voll: Event wird verworfen                │
        │                                                               │
        │   [5] Context Switch triggern (~50ns)                         │
        │       if (xHigherPriorityTaskWoken) {                         │
        │         portYIELD_FROM_ISR();                                 │
        │         // Weckt höher-priorisierten Task (z.B. GameLoop)     │
        │       }                                                       │
        │ }                                                             │
        │                                                               │
        │ Gesamt-Ausführungszeit: ~300-500ns (typisch)                  │
        │ ISR ist im IRAM → kein Flash-Access → schnell!                │
        └───────────────────────────────────────────────────────────────┘
                                    │
T+1µs                               ▼
        ┌────────────────────────────────────────────────────────────────┐
        │ CPU: Return from Interrupt                                     │
        │ • Kontext wiederherstellen (Register vom Stack)                │
        │ • Zurück zum unterbrochenen Code                               │
        │ • Oder: Context Switch zu gewecktem Task (falls Prio > aktuell)│
        └────────────────────────────────────────────────────────────────┘
                                    │
T+1µs-5ms                           ▼
        ┌───────────────────────────────────────────────────────────────┐
        │ QUEUE STORAGE: Event wartet auf Abholung                      │
        │ ┌───────────────────────────────────────────────────────────┐ │
        │ │ FreeRTOS Queue: s_button_queue                            │ │
        │ │ • Kapazität: 8 Events                                     │ │
        │ │ • Item-Size: 4 Bytes (gpio_num_t)                         │ │
        │ │ • Gesamt-RAM: 8 × 4 = 32 Bytes + Overhead (~64 Bytes)     │ │
        │ │ • FIFO: Ältestes Event zuerst                             │ │
        │ │ • Thread-Safe: Atomare Operationen                        │ │
        │ │                                                           │ │
        │ │ Aktueller Queue-Inhalt (Beispiel):                        │ │
        │ │   [0]: GPIO_NUM_4 (BTN_LEFT)  ← Ältestes                  │ │
        │ │   [1]: GPIO_NUM_6 (BTN_ROTATE)                            │ │
        │ │   [2-7]: leer                                             │ │
        │ └───────────────────────────────────────────────────────────┘ │
        │ Wartezeit bis Abholung: 0µs - 5ms (je nach GameLoop-Timing)   │
        └───────────────────────────────────────────────────────────────┘
                                    │
T+2ms                               ▼
        ╔═══════════════════════════════════════════════════════════════╗
        ║ TASK CONTEXT: GameLoop Task liest Queue                       ║
        ║ controls_get_event() oder check_button_pressed()              ║
        ╚═══════════════════════════════════════════════════════════════╝
                                    │
T+2ms                               ▼
        ┌───────────────────────────────────────────────────────────────┐
        │ METHOD 1: controls_get_event() [Event-Driven]                 │
        │ ┌───────────────────────────────────────────────────────────┐ │
        │ │ bool controls_get_event(gpio_num_t *out_gpio) {           │ │
        │ │   if (s_button_queue == NULL) return false;               │ │
        │ │                                                           │ │
        │ │   gpio_num_t g;                                           │ │
        │ │   // Non-blocking Queue Read (Timeout = 0):               │ │
        │ │   if (xQueueReceive(s_button_queue, &g, 0) == pdTRUE) {   │ │
        │ │     // Event aus Queue geholt                             │ │
        │ │     // SOFTWARE DEBOUNCING (Task-Kontext):                │ │
        │ │     static uint32_t last_pressed[GPIO_NUM_MAX] = {0};     │ │
        │ │     uint32_t now = xTaskGetTickCount()*portTICK_PERIOD_MS;│ │
        │ │                                                           │ │
        │ │     if (now - last_pressed[g] > BUTTON_DEBOUNCE_MS) {     │ │
        │ │       // 150ms seit letztem Event → Event akzeptieren     │ │
        │ │       last_pressed[g] = now;                              │ │
        │ │       *out_gpio = g;                                      │ │
        │ │       printf("[Event] Button %d from queue\n", g);        │ │
        │ │       return true;                                        │ │
        │ │     }                                                     │ │
        │ │     // Innerhalb Debounce-Zeit → Event verwerfen          │ │
        │ │     return false;                                         │ │
        │ │   }                                                       │ │
        │ │   // Queue leer                                           │ │
        │ │   return false;                                           │ │
        │ │ }                                                         │ │
        │ └───────────────────────────────────────────────────────────┘ │
        │ Ausführungszeit: ~10µs (Queue-Read + Debounce-Check)          │
        └───────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
        ┌───────────────────────────────────────────────────────────────┐
        │ METHOD 2: check_button_pressed() [Polling]                    │
        │ ┌───────────────────────────────────────────────────────────┐ │
        │ │ bool check_button_pressed(gpio_num_t gpio) {              │ │
        │ │   // Direkter GPIO-Read (umgeht Queue):                   │ │
        │ │   if (gpio_get_level(gpio) == 0) {  // Aktiv-LOW          │ │
        │ │     // SOFTWARE DEBOUNCING:                               │ │
        │ │     static uint32_t last_pressed[GPIO_NUM_MAX] = {0};     │ │
        │ │     uint32_t now = xTaskGetTickCount()*portTICK_PERIOD_MS;│ │
        │ │                                                           │ │
        │ │     if (now - last_pressed[gpio] > BUTTON_DEBOUNCE_MS) {  │ │
        │ │       last_pressed[gpio] = now;                           │ │
        │ │       printf("[Button] %d pressed\n", gpio);              │ │
        │ │       return true;                                        │ │
        │ │     }                                                     │ │
        │ │   }                                                       │ │
        │ │   return false;                                           │ │
        │ │ }                                                         │ │
        │ └───────────────────────────────────────────────────────────┘ │
        │ Ausführungszeit: ~5µs (GPIO-Read + Debounce-Check)            │
        │ Wird im GameLoop JEDEN 5ms-Tick aufgerufen (Polling)          │
        └───────────────────────────────────────────────────────────────┘
                                    │
T+2.01ms                            ▼
        ┌───────────────────────────────────────────────────────────────┐
        │ GAME LOGIC: Input Processing                                  │
        │ ┌───────────────────────────────────────────────────────────┐ │
        │ │ // Im GameLoop (game_loop_task):                          │ │
        │ │ if (check_button_pressed(BTN_LEFT)) {                     │ │
        │ │   TetrisBlock tmp = current_block;                        │ │
        │ │   tmp.x--;  // 1 Spalte nach links                        │ │
        │ │   if (!grid_check_collision(&tmp)) {                      │ │
        │ │     current_block = tmp;  // Bewegung ausführen           │ │
        │ │   }                                                       │ │
        │ │ }                                                         │ │
        │ └───────────────────────────────────────────────────────────┘ │
        └───────────────────────────────────────────────────────────────┘
                                    │
T+2.02ms                            ▼
        ┌───────────────────────────────────────────────────────────────┐
        │ RENDER: Block an neuer Position zeichnen                      │
        │ • render_grid() wird im nächsten 16ms-Render-Tick aufgerufen  │
        │ • Visuelles Feedback für User: Block bewegt sich              │
        └───────────────────────────────────────────────────────────────┘

╔═══════════════════════════════════════════════════════════════════════════╗
║                    TIMING-ZUSAMMENFASSUNG                                 ║
╚═══════════════════════════════════════════════════════════════════════════╝

Button Press → ISR Entry:           100-200 ns  (Hardware-Latenz)
ISR Execution:                      300-500 ns  (Event in Queue)
ISR → Task wakeup:                  1-10 µs     (Scheduler)
Queue → Game Logic:                 0-5 ms      (Polling-Intervall)
Game Logic → Render:                0-16 ms     (Render-Intervall)
───────────────────────────────────────────────────────────────────────────
GESAMT (Button → Visuelles Feedback): 2-21 ms   (Durchschnitt: ~10ms)

Vergleich:
  - Human Reaction Time: ~200ms (Wahrnehmung → Reaktion)
  - System Latency: ~10ms (Button → Screen)
  - Wahrnehmbar? NEIN (zu schnell für menschliche Wahrnehmung)
  - Gefühl: "Instant Response"

╔═══════════════════════════════════════════════════════════════════════════╗
║                    DEBOUNCING-STRATEGIE                                   ║
╚═══════════════════════════════════════════════════════════════════════════╝

Problem: Mechanisches Button-Prellen
  • Button-Kontakt schließt nicht sofort sauber
  • Signal bounced: HIGH-LOW-HIGH-LOW-... (~1-10ms)
  • Ohne Debouncing: Mehrere Events pro Press

Lösung: Software-Debouncing (150ms Fenster)
  ┌───────────────────────────────────────────────────────────────┐
  │ Timeline (Button-Press mit Prellen):                          │
  │                                                               │
  │ T+0ms:   Button gedrückt                                      │
  │          ↓ ISR triggered                                      │
  │ T+0.5ms: Prellen (Release simuliert)                          │
  │          ↓ ISR triggered (zweites Mal)                        │
  │ T+1ms:   Prellen (Press simuliert)                            │
  │          ↓ ISR triggered (drittes Mal)                        │
  │ T+2ms:   Kontakt stabil LOW                                   │
  │                                                               │
  │ Ohne Debouncing:                                              │
  │   → 3 Events in Queue                                         │
  │   → Block bewegt sich 3× (falsch!)                            │
  │                                                               │
  │ Mit Debouncing (150ms Fenster):                               │
  │   → Erstes Event akzeptiert (T+0ms)                           │
  │   → Zweites Event @ T+0.5ms: VERWORFEN (< 150ms)              │
  │   → Drittes Event @ T+1ms: VERWORFEN (< 150ms)                │
  │   → Nächstes mögliches Event: T+150ms                         │
  │                                                               │
  │ Result: Block bewegt sich 1× (korrekt!)                       │
  └───────────────────────────────────────────────────────────────┘

Debounce-Parameter:
  • BUTTON_DEBOUNCE_MS = 150ms (in Globals.h)
  • Tuning: 100ms zu kurz (Doppel-Events), 200ms zu lang (träge)
  • 150ms: Optimal für responsive Gameplay

╔═══════════════════════════════════════════════════════════════════════════╗
║                    QUEUE OVERFLOW HANDLING                                ║
╚═══════════════════════════════════════════════════════════════════════════╝

Queue Kapazität: 8 Events
  • Typische Nutzung: 0-2 Events gleichzeitig
  • Overflow-Szenario: Extrem schnelles Button-Mashing

Overflow-Verhalten:
  ┌───────────────────────────────────────────────────────────────┐
  │ if (Queue voll):                                              │
  │   xQueueSendFromISR() → pdFALSE (Event wird VERWORFEN)        │
  │   → ISR kehrt sofort zurück                                   │
  │   → Älteste Events bleiben in Queue                           │
  │   → Neuere Events gehen verloren                              │
  │                                                               │
  │ Auswirkung auf Gameplay:                                      │
  │   → Button-Press wird nicht registriert                       │
  │   → User muss erneut drücken                                  │
  │   → Selten problematisch (GameLoop leert Queue alle 5ms)      │
  └───────────────────────────────────────────────────────────────┘

Prevention:
  • Queue-Größe 8 Events (großzügig für 4 Buttons)
  • GameLoop pollt alle 5ms → Queue wird schnell geleert
  • Debouncing reduziert Event-Rate zusätzlich
```

---

### 5. Block Physics & Collision Detection

```
┌─────────────────────────────────────────────┐
│ spawn_block()                               │
│ - Zufälligen Block-Typ wählen (0-6)         │
│ - Spawn-Position finden (Mitte, oben)       │
│ - Kollisionsprüfung beim Spawn              │
│ - Kein Platz? → handle_game_over()          │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ Movement (jeden Loop, 5ms Polling)          │
│ ┌─────────────────────────────────────────┐ │
│ │ BTN_LEFT/RIGHT:                         │ │
│ │ - TetrisBlock tmp = current_block       │ │
│ │ - tmp.x += direction                    │ │
│ │ - if (!grid_check_collision(&tmp))      │ │
│ │     current_block = tmp                 │ │
│ └─────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────┐ │
│ │ BTN_ROTATE:                             │ │
│ │ - rotate_block_90(&tmp)                 │ │
│ │ - if (!grid_check_collision(&tmp))      │ │
│ │     current_block = tmp                 │ │
│ │ - O-Block (Typ 3) rotiert NICHT         │ │
│ └─────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────┐ │
│ │ BTN_FASTER:                             │ │
│ │ - tmp.y++                               │ │
│ │ - if (!grid_check_collision(&tmp))      │ │
│ │     current_block = tmp                 │ │
│ └─────────────────────────────────────────┘ │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ Automatisches Fallen (alle fall_interval)   │
│ - tmp.y++                                   │
│ - if (!grid_check_collision(&tmp))          │
│ │   current_block = tmp                     │
│ - else:                                     │
│ │   grid_fix_block(&current_block)          │
│ │   spawn_block()                           │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ grid_check_collision()                      │
│ - Prüft alle 4×4 Block-Pixel                │
│ - Nur aktive Pixel (shape[y][x] == 1)       │
│ - Kollisionen:                              │
│   • Untere Grenze (gy >= GRID_HEIGHT)       │
│   • Linke/Rechte Wand (gx < 0 || >= WIDTH)  │
│   • Fixierte Blöcke (grid[gy][gx] != 0)     │
│ - Return: true = Kollision                  │
└─────────────────────────────────────────────┘
```

---

### 6. Grid & Row Clearing

```
┌─────────────────────────────────────────────┐
│ grid_fix_block()                            │
│ - Block in grid[][] Array schreiben         │
│ - Farbindex speichern (color + 1)           │
│ - LED-Pixel sofort setzen (statisch)        │
│ - led_strip_refresh()                       │
│ - grid_clear_full_rows() aufrufen           │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ grid_clear_full_rows()                      │
│ ┌─────────────────────────────────────────┐ │
│ │ 1. Volle Zeilen identifizieren          │ │
│ │    - Durchsuche alle Zeilen             │ │
│ │    - Zeile voll wenn alle grid[y][x]!=0 │ │
│ │    - Speichere Zeilen-Indizes           │ │
│ └─────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────┐ │
│ │ 2. Blink-Animation (2× weiß)            │ │
│ │    - Zeile auf weiß (255,255,255)       │ │
│ │    - Delay 150ms                        │ │
│ │    - Zeile zurück auf Original-Farbe    │ │
│ │    - Delay 100ms                        │ │
│ └─────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────┐ │
│ │ 3. Zeilen aus Grid entfernen            │ │
│ │    - grid[y][x] = 0 für volle Zeilen    │ │
│ └─────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────┐ │
│ │ 4. Gravity anwenden (Spaltenweise)      │ │
│ │    - Für jede Spalte x:                 │ │
│ │      • Nicht-null Werte nach unten      │ │
│ │      • Lücken eliminieren               │ │
│ │      • Von unten nach oben packen       │ │
│ └─────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────┐ │
│ │ 5. Grid neu rendern                     │ │
│ │    - led_strip_clear()                  │ │
│ │    - Alle statischen Blöcke zeichnen    │ │
│ │    - led_strip_refresh()                │ │
│ └─────────────────────────────────────────┘ │
│ ┌─────────────────────────────────────────┐ │
│ │ 6. Score & Speed Update                 │ │
│ │    - score_add_lines(remove_count)      │ │
│ │      • 1 Zeile = 100 Punkte             │ │
│ │      • 2 Zeilen = 300 Punkte            │ │
│ │      • 3 Zeilen = 500 Punkte            │ │
│ │      • 4 Zeilen = 800 Punkte (Tetris!)  │ │
│ │    - speed_manager_update_score()       │ │
│ │    - display_update_score()             │ │
│ └─────────────────────────────────────────┘ │
└─────────────────────────────────────────────┘
```

---

### 7. Rendering System

```
┌─────────────────────────────────────────────┐
│ render_grid() - Optimiertes Rendering       │
│ Frequenz: 60 FPS (alle 16ms)                │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ 1. Vorherige dynamische Pixel restaurieren  │
│    - prev_dynamic_pos[][] enthält alte      │
│      Block-Positionen                       │
│    - Für jedes Pixel:                       │
│      • Wenn grid[y][x] > 0: Farbe setzen    │
│      • Wenn grid[y][x] == 0: Pixel löschen  │
│    - Vermeidet led_strip_clear()            │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ 2. Aktuellen Block zeichnen (dynamisch)     │
│    - Für alle 4×4 Block-Pixel:              │
│      • shape[by][bx] == 1?                  │
│      • Position: gx = block.x + bx          │
│      •           gy = block.y + by          │
│      • RGB-Farbe holen (get_block_rgb)      │
│      • Helligkeit skalieren (BRIGHTNESS)    │
│      • led_strip_set_pixel()                │
│      • Position in prev_dynamic_pos[][]     │
│        speichern für nächsten Frame         │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ 3. LED-Matrix aktualisieren                 │
│    - led_strip_refresh()                    │
│    - Sendet Daten via RMT an WS2812B        │
└─────────────────────────────────────────────┘
```

**Optimierung:**
- Nur geänderte Pixel werden geschrieben
- Kein vollständiges `led_strip_clear()` jeden Frame
- Statische Blöcke bleiben erhalten
- Dynamischer Block wird gezeichnet und nächsten Frame restauriert

---

### 8. Speed Progression System

```
┌─────────────────────────────────────────────┐
│ Speed Levels (basierend auf geclearten      │
│ Zeilen)                                     │
├─────────────────────────────────────────────┤
│ Level 0:    0 Zeilen  → 400ms Fall-Intervall│
│ Level 1:    2 Zeilen  → 370ms               │
│ Level 2:    5 Zeilen  → 330ms               │
│ Level 3:   10 Zeilen  → 270ms               │
│ Level 4:   15 Zeilen  → 220ms               │
│ Level 5:   20 Zeilen  → 170ms               │
│ Level 6:   25 Zeilen  → 130ms               │
│ Level 7:   30 Zeilen  → 100ms               │
│ Level 8:   35 Zeilen  →  80ms               │
│ Level 9:   40 Zeilen  →  60ms               │
│ Level Max: 45+ Zeilen →  50ms (Max Speed)   │
└─────────────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────┐
│ speed_manager_update_score()                │
│ - Wird nach grid_clear_full_rows() gerufen  │
│ - Findet passendes Level                    │
│ - Aktualisiert current_fall_interval        │
│ - Log-Ausgabe bei Level-Up                  │
└─────────────────────────────────────────────┘
```

---

### 9. Score & Highscore (NVS Persistence)

```
┌─────────────────────────────────────────────┐
│ Beim Boot: score_load_highscore()           │
│ - NVS Flash initialisieren                  │
│ - NVS Namespace "tetris" öffnen             │
│ - Key "highscore" lesen (uint32_t)          │
│ - Falls nicht vorhanden: highscore = 0      │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ Während Spiel: score_add_lines()            │
│ - Zeilen zu total_lines_cleared addieren    │
│ - Score berechnen:                          │
│   • 1 Zeile  → +100 Punkte                  │
│   • 2 Zeilen → +300 Punkte                  │
│   • 3 Zeilen → +500 Punkte                  │
│   • 4 Zeilen → +800 Punkte (Tetris!)        │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ Bei Game Over: score_update_highscore()     │
│ - if (score > highscore):                   │
│   • highscore = score                       │
│   • NVS speichern (nvs_set_u32)             │
│   • nvs_commit()                            │
│   • Log: "New highscore saved"              │
└─────────────────────────────────────────────┘
```

---

### 10. Display (OLED) - HUD System

```
┌─────────────────────────────────────────────┐
│ display_init()                              │
│ - I2C Bus initialisieren (GPIO 20/21)       │
│ - I2C Device Scan (0x08-0x77)               │
│ - SSD1306 Panel initialisieren              │
│ - LVGL Port starten                         │
│ - HUD erstellen:                            │
│   • label_title: "TETRIS"                   │
│   • label_score: "Score: 0"                 │
│   • label_highscore: "High: 0"              │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ display_update_score()                      │
│ - Wird nach Zeilen-Clear gerufen            │
│ - Aktualisiert label_score Text             │
│ - Aktualisiert label_highscore Text         │
│ - LVGL Refresh (lvgl_port_lock)             │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ display_show_game_over()                    │
│ - Screen löschen (lv_obj_clean)             │
│ - "GAME OVER" zentriert anzeigen            │
│ - Final Score + Highscore anzeigen          │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ display_reset_and_show_hud()                │
│ - Neuer Screen erstellen                    │
│ - HUD Labels neu aufbauen                   │
│ - Highscore aus NVS laden und anzeigen      │
└─────────────────────────────────────────────┘
```

---

## Timing & Performance

### Task-Prioritäten
- **GameLoop Task:** Priorität 5 (hoch)
- **Theme Task:** Priorität 1 (niedrig, Hintergrund)
- **LVGL Timer:** Standard-Priorität
- **Idle Task:** Priorität 0

### Timing-Parameter (aus Globals.h)

| Parameter | Wert | Beschreibung |
|-----------|------|--------------|
| `LOOP_INTERVAL_MS` | 5ms | Game Loop Polling-Rate |
| `RENDER_INTERVAL_MS` | 16ms | LED Refresh (60 FPS) |
| `FALL_INTERVAL_MS` | 400ms (initial) | Block-Fall-Geschwindigkeit |
| `BUTTON_DEBOUNCE_MS` | 150ms | Software-Debounce für Buttons |
| `SPLASH_SCROLL_DELAY_MS` | 40ms | Splash-Text Scroll-Geschwindigkeit |
| `GAME_OVER_BLINK_ON_MS` | 300ms | Blink An-Zeit bei Game Over |
| `GAME_OVER_BLINK_OFF_MS` | 300ms | Blink Aus-Zeit bei Game Over |

### Performance-Metriken
- **Input Latenz:** < 5ms (5ms Polling)
- **Render Latenz:** 16ms (60 FPS)
- **ISR Latenz:** < 1µs (minimales ISR, nur Queue Push)
- **LED Update Zeit:** ~5ms für 384 LEDs via RMT

---

## Datenstrukturen

### TetrisBlock (Block-Datenstruktur)
```c
typedef struct {
    uint8_t shape[4][4];  // 1 = Blockteil, 0 = leer
    int x;                // X-Position (linke obere Ecke)
    int y;                // Y-Position (obere Zeile)
    uint8_t color;        // Farbindex (0-6)
} TetrisBlock;
```

### MATRIX (LED-Mapping)
```c
typedef struct {
    uint16_t LED_Number[24][16];  // LED-Nummern für jede Position
    uint8_t red;                  // Rot-Wert (0-255)
    uint8_t green;                // Grün-Wert (0-255)
    uint8_t blue;                 // Blau-Wert (0-255)
    bool isFilled;                // Ist Pixel gefüllt?
} MATRIX;
```

### Grid (Spielfeld-Array)
```c
uint8_t grid[24][16];  // 0 = leer, 1-7 = Block-Farbe
```

### Block-Typen & Farben
```c
enum BlockType {
    BLOCK_I = 0,  // Cyan
    BLOCK_J = 1,  // Blau
    BLOCK_L = 2,  // Orange
    BLOCK_O = 3,  // Gelb
    BLOCK_S = 4,  // Grün
    BLOCK_T = 5,  // Lila
    BLOCK_Z = 6   // Rot
};
```

---

## Detaillierter Programmablauf

### Boot-Sequenz (0-5 Sekunden)

```
Zeit    Event
─────   ───────────────────────────────────────────────────────────
0.0s    ESP32 Boot, FreeRTOS Start
0.1s    app_main() Entry
0.2s    LED-Strip initialisiert (RMT)
0.3s    LED-Matrix Mapping geladen
0.4s    Random Seed generiert
0.5s    Splash-Animation startet (scrollender Text "TETRIS")
        - Hintergrund: Statische Tetromino-Grafik
        - Vordergrund: Weißer scrollender Text
4.5s    Splash endet nach 4000ms
4.6s    Grid & Score initialisiert
4.7s    I2C Display initialisiert
        - Device Scan (0x08-0x77)
        - SSD1306 gefunden auf 0x3C
        - LVGL initialisiert
        - HUD erstellt (Titel, Score, Highscore)
4.8s    Highscore aus NVS geladen (z.B. 1200)
4.9s    Button-Controls initialisiert
        - GPIO 4,5,6,7 als Input mit Pull-up
        - ISR installiert (Negative Flanke)
        - Event-Queue erstellt (8 Events)
5.0s    Tetris-Theme Task gestartet (Hintergrund-Musik)
5.1s    GameLoop Task gestartet (Priorität 5)
5.2s    System läuft → WAIT State
        - Splash scrollt endlos
        - Wartet auf Button-Press
```

---

### Spiel-Sequenz (normal)

```
Zeit    Event
─────   ───────────────────────────────────────────────────────────
0.0s    Button gedrückt (z.B. BTN_ROTATE)
        - ISR triggered → GPIO_NUM_6 in Queue
        - controls_get_event() liest Queue
        - Debounce-Check: 150ms seit letztem Event?
        - Event akzeptiert

0.01s   Splash beendet (splash_show returns)
        - Queue gedrained (alte Events entfernt)
        - Warten bis alle Buttons released
        - Neues Event warten (controls_wait_event)

0.5s    Neuer Button-Press erkannt
        - splash_clear() → LED-Matrix löschen
        - grid_init() → Grid auf 0 setzen
        - score_init() → Score = 0
        - speed_manager_reset() → 400ms Fall-Intervall
        - display_reset_and_show_hud()
        - spawn_block() → Ersten Block spawnen

0.51s   GameLoop: RUNNING State
        last_fall_time = 0.51s
        last_render_time = 0.51s

0.51s   Loop 1 (LOOP_INTERVAL_MS = 5ms)
+0.005s - Emergency Reset Check: Nein
        - Game Over Check: Nein
        - Running? Ja
        - Input Check:
          • BTN_LEFT: Nein
          • BTN_RIGHT: Nein
          • BTN_ROTATE: Nein
          • BTN_FASTER: Nein
        - Auto-Fall Check: Noch nicht (< 400ms)
        - Render Check: Noch nicht (< 16ms)
        - vTaskDelay(5ms)

0.52s   Loop 2
+0.005s - (gleiche Checks wie Loop 1)

0.526s  Loop 3
        - Render Check: Ja! (16ms vergangen)
        - render_grid() aufgerufen
          • Vorherige dynamische Pixel restaurieren
          • Aktuellen Block zeichnen
          • led_strip_refresh()
        - last_render_time = 0.526s

...     (weitere Loops mit 5ms Intervall)

0.7s    User drückt BTN_RIGHT
        - ISR triggered → GPIO_NUM_5 in Queue
        - Nächster Loop:
          • check_button_pressed(BTN_RIGHT): true
          • tmp = current_block; tmp.x++
          • grid_check_collision(&tmp): false (kein Hindernis)
          • current_block = tmp → Block bewegt sich rechts

0.91s   Auto-Fall Timer abgelaufen (400ms seit 0.51s)
        - tmp = current_block; tmp.y++
        - grid_check_collision(&tmp): false
        - current_block = tmp → Block fällt 1 Zeile

1.31s   Auto-Fall (alle 400ms)
        - Block fällt weiter

...     (Block fällt und wird bewegt)

5.5s    Auto-Fall: Kollision erkannt
        - tmp.y++ würde kollidieren
        - grid_fix_block(&current_block)
          • Block in grid[][] Array schreiben
          • LED-Pixel setzen (statisch)
          • grid_clear_full_rows() aufrufen
            ◦ Zeile 23 (unterste) ist voll!
            ◦ Blink-Animation (2× weiß, 150ms on / 100ms off)
            ◦ Zeile löschen
            ◦ Gravity anwenden (Blöcke fallen)
            ◦ score_add_lines(1) → +100 Punkte
            ◦ speed_manager_update_score(1)
            ◦ display_update_score(100, highscore)
        - spawn_block() → Neuer Block oben

5.51s   Neuer Block spawned
        - Spiel läuft weiter mit neuem Block

...     (Spieler spielt weiter, mehr Zeilen werden gecleart)

20.0s   15 Zeilen gecleart → Speed Level 4
        - fall_interval = 220ms (schneller!)
        - Log: "[SpeedManager] ⚡ LEVEL UP!"

...     (Spiel wird schwieriger)

45.0s   Kein Platz für neuen Block
        - spawn_block() findet keine Position
        - handle_game_over() aufgerufen:
          • score_update_highscore()
            ◦ Highscore in NVS speichern (wenn > alter)
          • display_show_game_over(score, highscore)
          • Blink-Animation (3× rot, 300ms on / 300ms off)
          • game_over_flag = 1

45.5s   Game Over State
        - game_over_flag erkannt
        - Splash anzeigen (2s, Inputs ignoriert)
        - Queue drainieren (500ms kontinuierlich)
        - splash_show_waiting() → warten auf Button
        - Bei Button-Press: Neues Spiel

46.0s   Button gedrückt → Spiel startet neu
        (zurück zu 0.5s Sequenz)
```

---

### Emergency Reset Sequenz

```
Zeit    Event
─────   ───────────────────────────────────────────────────────────
0.0s    Alle 4 Buttons gleichzeitig gedrückt
        - controls_all_buttons_pressed(): true
        - vTaskDelay(1000ms) → 1 Sekunde halten

1.0s    Nach 1s: Immer noch alle gedrückt?
        - controls_all_buttons_pressed(): true
        - Hard Reset ausgelöst!
        - grid_init()
        - score_init()
        - speed_manager_reset()
        - led_strip_clear() + refresh
        - display_reset_and_show_hud()

1.1s    Queue drainieren (alle Events entfernen)
        - while (controls_get_event(&ev)) {}

1.2s    Splash anzeigen (2s, Inputs ignoriert)
        - splash_show_duration(2000)

3.2s    Kontinuierliches Queue-Drain (500ms)
        - while ((now - start) < 500ms):
          • while (controls_get_event(&ev)): drain
          • vTaskDelay(10ms)
          • Log: "Caught straggler event: GPIO X"

3.7s    Warten bis alle Buttons losgelassen
        - while (check_button_pressed(ANY)): delay

3.9s    Splash-Waiting (endlos scrollend)
        - splash_show_waiting()
        - Wartet auf frischen Button-Press

5.0s    Button gedrückt → Spiel startet
        - Normale Spiel-Sequenz beginnt
```

---

## Modul-Beschreibungen

### main.c - System Entry Point
**Verantwortung:** System-Initialisierung und Koordination  
**Wichtige Funktionen:**
- `setup_led_strip()`: WS2812B RMT-Driver initialisieren
- `app_main()`: Haupteinstiegspunkt
  1. LED-Strip Setup
  2. LED-Matrix Mapping laden
  3. Random Seed generieren
  4. Splash-Animation anzeigen
  5. Grid & Score initialisieren
  6. Display initialisieren (I2C, LVGL)
  7. Controls initialisieren (GPIO, ISR)
  8. Theme-Song starten
  9. GameLoop Task starten
  10. Idle Loop (1s Delay)

---

### GameLoop.c - Spiellogik
**Verantwortung:** Hauptspielschleife, Zustandsverwaltung  
**Wichtige Funktionen:**
- `game_loop_task()`: FreeRTOS Task (Priorität 5)
  - Polling-Loop mit 5ms Intervall
  - Zustandsmaschine: WAIT / RUNNING / GAME_OVER
  - Input-Verarbeitung (Buttons)
  - Auto-Fall-Mechanik
  - Rendering (60 FPS)
  - Emergency Reset Erkennung
- `render_grid()`: Optimiertes LED-Rendering
  - Nur geänderte Pixel updaten
  - Statische Blöcke bleiben erhalten
- `spawn_block()`: Neuen Block spawnen
  - Zufälligen Typ wählen
  - Spawn-Position finden (Kollisionsfrei)
  - Game Over wenn kein Platz
- `handle_game_over()`: Game Over Logik
  - Highscore speichern
  - Blink-Animation
  - State-Transition zu WAIT

---

### Controls.c - Button-Input System
**Verantwortung:** GPIO-ISR, Event-Queue, Debouncing  
**Wichtige Funktionen:**
- `init_controls()`: GPIO & ISR Setup
  - GPIO 4,5,6,7 als Input mit Pull-up
  - Interrupt auf Negative Flanke
  - Queue erstellen (8 Events)
  - ISR Handler registrieren
- `gpio_isr_handler()`: ISR-Handler (IRAM)
  - Minimale Verarbeitung im Interrupt
  - GPIO-Nummer in Queue schieben
  - portYIELD_FROM_ISR() bei höherer Priorität
- `controls_get_event()`: Queue-Event auslesen
  - Non-blocking Queue Read
  - Software-Debounce (150ms)
  - Event-Logging (Debug)
- `check_button_pressed()`: GPIO-Polling
  - Direktes gpio_get_level()
  - Software-Debounce per GPIO
- `controls_all_buttons_pressed()`: Reset-Erkennung
  - Prüft alle 4 GPIOs gleichzeitig

---

### Blocks.c - Tetromino-Definitionen
**Verantwortung:** Block-Formen, Rotation, Farben  
**Daten:**
- `blocks[7][4]`: Array aller 7 Block-Typen mit 4 Rotationen
  - I-Block: 4×1 Linie (Cyan)
  - J-Block: L-Form gespiegelt (Blau)
  - L-Block: L-Form (Orange)
  - O-Block: 2×2 Quadrat (Gelb) - rotiert nicht
  - S-Block: S-Form (Grün)
  - T-Block: T-Form (Lila)
  - Z-Block: Z-Form (Rot)
**Funktionen:**
- `assign_block_color()`: Farbe zuweisen
- `get_block_rgb()`: RGB-Werte aus Farbtabelle
- `rotate_block_90()`: Block um 90° drehen (Matrixtransformation)

---

### Grid.c - Spielfeld-Verwaltung
**Verantwortung:** Kollisionserkennung, Zeilen-Clearing, Gravity  
**Daten:**
- `grid[24][16]`: Spielfeld-Array (0 = leer, 1-7 = Farbe)
**Funktionen:**
- `grid_init()`: Grid auf 0 setzen
- `grid_check_collision()`: Kollision prüfen
  - Nur aktive Block-Pixel prüfen
  - Grenzen: Unten, Links, Rechts
  - Fixierte Blöcke
  - Spawn oberhalb erlaubt (y < 0)
- `grid_fix_block()`: Block ins Grid schreiben
  - Farbindex speichern
  - LED-Pixel setzen (statisch)
  - grid_clear_full_rows() aufrufen
- `grid_clear_full_rows()`: Volle Zeilen entfernen
  1. Volle Zeilen identifizieren
  2. Blink-Animation (2× weiß)
  3. Zeilen löschen
  4. Gravity anwenden (spaltenweise)
  5. Grid neu rendern
  6. Score & Speed Update

---

### Splash.c - Startbildschirm
**Verantwortung:** Splash-Animation, Warte-Screen  
**Funktionen:**
- `splash_show()`: Interactive Splash
  - Scrollender Text "TETRIS"
  - Statischer Tetromino-Hintergrund
  - Bricht bei Button-Press ab
- `splash_show_waiting()`: Warte-Splash
  - Scrollt endlos (kein Auto-Break)
  - 500ms Debounce-Timeout
  - Queue-Drain vor Button-Akzeptanz
  - Bricht nur bei frischem Button ab
- `splash_show_duration()`: Timed Splash
  - Feste Dauer (z.B. 2000ms)
  - Ignoriert Button-Presses
  - Für Reset/Game-Over Sequenz
- `splash_clear()`: Splash löschen

---

### Score.c - Punktesystem
**Verantwortung:** Score, Zeilen-Tracking, Highscore (NVS)  
**Funktionen:**
- `score_init()`: Score & Zeilen auf 0
- `score_add_lines()`: Zeilen zu Score
  - 1 Zeile: +100 Punkte
  - 2 Zeilen: +300 Punkte
  - 3 Zeilen: +500 Punkte
  - 4 Zeilen: +800 Punkte (Tetris!)
- `score_load_highscore()`: NVS Highscore laden
  - NVS Flash initialisieren
  - Namespace "tetris" öffnen
  - Key "highscore" lesen
- `score_update_highscore()`: Highscore speichern
  - Nur wenn score > highscore
  - NVS schreiben & committen

---

### SpeedManager.c - Geschwindigkeitssystem
**Verantwortung:** Dynamische Fall-Geschwindigkeit  
**Daten:**
- `speed_levels[]`: Tabelle mit 11 Levels (0-10)
**Funktionen:**
- `speed_manager_init()`: Auf Level 0 zurücksetzen
- `speed_manager_get_fall_interval()`: Aktuelles Intervall
- `speed_manager_update_score()`: Level berechnen
  - Basierend auf geclearten Zeilen
  - Findet passendes Level
  - Log bei Level-Up
- `speed_manager_reset()`: Zurück auf Level 0

---

### DisplayInit.c - OLED-Display
**Verantwortung:** I2C Display, LVGL HUD  
**Funktionen:**
- `display_init()`: I2C & LVGL Setup
  - I2C Bus initialisieren (GPIO 20/21)
  - Device Scan
  - SSD1306 Panel initialisieren
  - LVGL Port starten
  - HUD erstellen (Titel, Score, Highscore)
- `display_update_score()`: Score aktualisieren
- `display_show_game_over()`: Game Over Screen
- `display_reset_and_show_hud()`: HUD neu aufbauen

---

### LedMatrixInit.c - LED-Mapping
**Verantwortung:** LED-Nummern aus MatrixNummer.h laden  
**Funktionen:**
- `LedMatrixInit()`: Matrix-Array initialisieren
  - Kopiert LED_MATRIX_NUMMER[][] in matrix[][]

---

### ThemeSong.c - Tetris-Musik
**Verantwortung:** LEDC PWM Buzzer-Ausgabe  
**Funktionen:**
- `StartTheme()`: Theme-Task starten (Hintergrund)
- Spielt Tetris-Melodie in Endlosschleife

---

### Colors.c - Farbtabelle
**Verantwortung:** Zentrale RGB-Farben für Blöcke  
**Daten:**
- `block_colors[7][3]`: RGB-Werte für 7 Blöcke
  - Werte durch BRIGHTNESSDIV (10) geteilt
  - I: Cyan (0, 255, 80)
  - J: Blau (0, 0, 255)
  - L: Orange (255, 90, 0)
  - O: Gelb (255, 240, 0)
  - S: Grün (0, 255, 0)
  - T: Lila (128, 0, 128)
  - Z: Rot (255, 0, 0)

---

### Globals.h - Konfiguration
**Verantwortung:** Zentrale Konstanten & Typen  
**Wichtige Defines:**
- `LED_WIDTH`, `LED_HEIGHT`: Matrix-Größe (16×24)
- `GRID_WIDTH`, `GRID_HEIGHT`: Spielfeld-Größe (16×24)
- `FALL_INTERVAL_MS`: Start-Fall-Geschwindigkeit (400ms)
- `RENDER_INTERVAL_MS`: Render-Frequenz (16ms = 60fps)
- `LOOP_INTERVAL_MS`: Polling-Frequenz (5ms)
- `BUTTON_DEBOUNCE_MS`: Debounce-Zeit (150ms)
- `GAME_BRIGHTNESS_SCALE`: LED-Helligkeit (125/255)
- `NUM_BLOCKS`: Anzahl Block-Typen (7)

---

## Zusammenfassung

Dieses Tetris-Spiel ist ein komplexes Echtzeit-System mit folgenden Eigenschaften:

1. **Multitasking:** FreeRTOS Tasks für GameLoop, Theme, LVGL
2. **Interrupt-driven Input:** ISR + Queue für responsive Eingabe
3. **Optimiertes Rendering:** 60 FPS ohne Flicker
4. **Persistente Daten:** NVS Highscore-Speicherung
5. **Dynamische Schwierigkeit:** 11 Speed-Levels
6. **Robuste State Machine:** WAIT / RUNNING / GAME_OVER
7. **Emergency Reset:** 4-Button-Kombination für Hard-Reset

**Gesamtarchitektur:** Event-driven, modular, erweiterbar  
**Performance:** Stabil, niedrige Latenz, flüssige Animation  
**Code-Qualität:** Gut strukturiert, aber optimierbar (siehe nächste Phase)

---

*Dokumentation erstellt: 25.11.2025*  
*Autor: Rupp Arian, Lampert Mathias*  
*Projekt: ESP32-S3 Tetris*
