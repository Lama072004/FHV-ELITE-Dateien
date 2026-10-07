# ESP32-S3 Tetris - Projekt-Präsentation
## 5-8 Minuten

---

## 1. Projekt-Übersicht (1 min)

**Ziel:** Vollständiges Tetris-Spiel auf ESP32-S3 mit LED-Matrix

**Hardware:**
- ESP32-S3 (Dual-Core @ 240 MHz)
- WS2812B LED-Matrix: 16×24 (384 LEDs)
- SSD1306 OLED Display (Score/Highscore)
- 4 Buttons für Steuerung

**Software:**
- ESP-IDF v5.5.1 (FreeRTOS)
- C Programming
- ~2000 Zeilen Code

---

## 2. Ursprüngliche Planung (1 min)

### Was war geplant?

✅ **Core Features (umgesetzt):**
- 7 Tetromino-Typen mit Rotation
- Kollisionserkennung
- Zeilen-Clear mit Score-System
- LED-Matrix Rendering
- Button-Steuerung

❌ **Nice-to-Have (nicht umgesetzt):**
- WiFi-Connectivity
- Webserver für Remote-Play
- Next-Block Preview
- Pause-Funktion

### Zeitplan:
- **Geplant:** 3 Wochen
- **Tatsächlich:** 4 Wochen (Debugging + Optimierung)

### Entwicklungsprozess:
- **Hackathon:** Ein großer Teil des Projekts entstand während eines Hackathons
- **Iterative Entwicklung:** Viele Probleme wurden durch direktes Nutzerfeedback identifiziert
- **User Testing:** Nutzerrezensionen führten zu wichtigen Verbesserungen:
  - Button-Debouncing wurde von 100ms auf 150ms erhöht
  - Emergency Reset Feature (4-Button-Kombination) wurde hinzugefügt
  - Graceful Degradation für fehlendes OLED Display implementiert
  - Delta-Rendering zur Behebung des LED-Flackerns

---

## 3. Architektur (1 min)

### System-Design

```
┌─────────────────────────────────────────┐
│         ESP32-S3 (FreeRTOS)             │
│                                         │
│  ┌─────────────┐    ┌─────────────┐    │
│  │ GameLoop    │    │ Controls    │    │
│  │ Task (5ms)  │◄───┤ (ISR+Queue) │    │
│  └──────┬──────┘    └─────────────┘    │
│         │                               │
│         ├──► RMT ──► LED Matrix         │
│         ├──► I2C ──► OLED Display       │
│         └──► NVS ──► Highscore          │
└─────────────────────────────────────────┘
```

### Modul-Struktur:
- **GameLoop:** State Machine, Rendering (60 FPS)
- **Controls:** Button-ISR, Debouncing
- **Grid:** Kollision, Zeilen-Clear, Gravity
- **Blocks:** Tetromino-Definitionen, Rotation

---

## 4. Wichtigste Code-Snippets (2 min)

### 4.1 ISR + Queue für Buttons

```c
// ISR: Minimale Verarbeitung, nur Queue Push
static void IRAM_ATTR gpio_isr_handler(void* arg) {
    gpio_num_t gpio = (gpio_num_t)(intptr_t)arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // Event in Queue schieben
    xQueueSendFromISR(s_button_queue, &gpio, 
                      &xHigherPriorityTaskWoken);
    
    // Context Switch wenn höhere Priorität
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

// GameLoop liest Queue mit Debouncing
bool controls_get_event(gpio_num_t *out_gpio) {
    gpio_num_t g;
    if (xQueueReceive(s_button_queue, &g, 0) == pdTRUE) {
        // Software-Debounce: 150ms seit letztem Event?
        uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
        if (now - last_pressed[g] > BUTTON_DEBOUNCE_MS) {
            last_pressed[g] = now;
            *out_gpio = g;
            return true;
        }
    }
    return false;
}
```

**Warum ISR + Queue?**
- ISR ultra-schnell (300-500ns)
- Debouncing im Task-Kontext (nicht im Interrupt)
- Keine verlorenen Events

---

### 4.2 Kollisionserkennung

```c
bool grid_check_collision(const TetrisBlock *block) {
    for (int by = 0; by < 4; by++) {
        for (int bx = 0; bx < 4; bx++) {
            // Nur aktive Block-Pixel prüfen
            if (block->shape[by][bx] == 0) continue;
            
            int gx = block->x + bx;
            int gy = block->y + by;
            
            // Grenz-Checks
            if (gx < 0 || gx >= GRID_WIDTH) return true;
            if (gy >= GRID_HEIGHT) return true;
            
            // Spawn-Zone erlauben (y < 0 für hohe Blöcke)
            if (gy < 0) continue;
            
            // Kollision mit fixierten Blöcken?
            if (grid[gy][gx] != 0) return true;
        }
    }
    return false;  // Keine Kollision
}
```

**Besonderheit:** Spawn-Zone oberhalb Grid (y < 0) erlaubt

---

### 4.3 Delta-Rendering (Optimierung)

```c
void render_grid() {
    // Phase 1: Vorherige Block-Position restaurieren
    for (int i = 0; i < prev_dynamic_count; i++) {
        int pos = prev_dynamic_pos[i];
        int y = pos / GRID_WIDTH;
        int x = pos % GRID_WIDTH;
        
        if (grid[y][x] > 0) {
            // Grid-Pixel: Farbe setzen
            uint8_t r, g, b;
            get_block_rgb(grid[y][x] - 1, &r, &g, &b);
            led_strip_set_pixel(led_num, r, g, b);
        } else {
            // Leer: Pixel löschen
            led_strip_set_pixel(led_num, 0, 0, 0);
        }
    }
    
    // Phase 2: Aktuellen Block zeichnen
    int dynamic_count = 0;
    for (int by = 0; by < 4; by++) {
        for (int bx = 0; bx < 4; bx++) {
            if (current_block.shape[by][bx] == 0) continue;
            
            int gx = current_block.x + bx;
            int gy = current_block.y + by;
            
            // RGB mit Brightness-Skalierung
            uint8_t r, g, b;
            get_block_rgb(current_block.color, &r, &g, &b);
            r = (r * BRIGHTNESS_SCALE) / 255;
            g = (g * BRIGHTNESS_SCALE) / 255;
            b = (b * BRIGHTNESS_SCALE) / 255;
            
            led_strip_set_pixel(led_num, r, g, b);
            prev_dynamic_pos[dynamic_count++] = gy * GRID_WIDTH + gx;
        }
    }
    prev_dynamic_count = dynamic_count;
    
    // Phase 3: RMT-Transfer
    led_strip_refresh();  // 11.5ms für 384 LEDs
}
```

**Performance:** ~70% weniger LED-Operationen vs. Full-Refresh

---

### 4.4 Zeilen-Clear mit Gravity

```c
void grid_clear_full_rows() {
    // Schritt 1: Volle Zeilen finden
    int full_rows[4];
    int remove_count = 0;
    for (int y = 0; y < GRID_HEIGHT; y++) {
        bool full = true;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] == 0) {
                full = false;
                break;
            }
        }
        if (full) full_rows[remove_count++] = y;
    }
    
    if (remove_count == 0) return;
    
    // Schritt 2: Blink-Animation (2× weiß)
    for (int blink = 0; blink < 2; blink++) {
        for (int i = 0; i < remove_count; i++) {
            int y = full_rows[i];
            for (int x = 0; x < GRID_WIDTH; x++) {
                led_strip_set_pixel(led_num, 255, 255, 255);
            }
        }
        led_strip_refresh();
        vTaskDelay(pdMS_TO_TICKS(150));
        
        // Restore original colors...
    }
    
    // Schritt 3: Zeilen löschen
    for (int i = 0; i < remove_count; i++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[full_rows[i]][x] = 0;
        }
    }
    
    // Schritt 4: Gravity (spaltenweise)
    for (int x = 0; x < GRID_WIDTH; x++) {
        int target_y = GRID_HEIGHT - 1;
        for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
            if (grid[y][x] != 0) {
                grid[target_y][x] = grid[y][x];
                if (target_y != y) grid[y][x] = 0;
                target_y--;
            }
        }
    }
    
    // Schritt 5: Score Update
    score_add_lines(remove_count);  // 1L:100, 4L:800
    speed_manager_update_score(total_lines_cleared);
}
```

**Key:** Spaltenweise Gravity statt zeilenweise (verhindert Lücken)

---

## 5. Technische Herausforderungen (1.5 min)

### Problem 1: Button-Prellen
**Symptom:** Block bewegt sich mehrfach pro Press  
**Ursache:** Mechanisches Prellen (1-10ms Bouncing)  
**Lösung:** 
- Hardware: Pull-up Widerstände
- Software: 150ms Debounce-Filter
```c
if (now - last_pressed[gpio] > 150) {
    // Event akzeptieren
}
```

---

### Problem 2: LED-Flackern
**Symptom:** LEDs flackern bei Block-Bewegung  
**Ursache:** `led_strip_clear()` jeden Frame → Alle LEDs kurz aus  
**Lösung:** Delta-Rendering
- Nur geänderte Pixel updaten
- Statische Blöcke bleiben erhalten
- Vorherige Block-Position in Array speichern

---

### Problem 3: RMT-Timing WS2812B
**Symptom:** Falsche Farben, LEDs reagieren nicht  
**Ursache:** WS2812B benötigt präzise Timing (±150ns)  
**Lösung:** 
- ESP-IDF RMT-Driver nutzen (Hardware-generierte Pulse)
- Timing aus Datasheet: Bit 0: 0.4µs HIGH + 0.85µs LOW
- DMA für automatischen Transfer (CPU-Entlastung)

---

### Problem 4: Gravity nach Zeilen-Clear
**Symptom:** "Schwebende" Blöcke nach Zeilen-Löschen  
**Ursache:** Zeilen nach oben kopieren lässt Lücken  
**Lösung:** Spaltenweise Gravity
```c
// Nicht-null Werte von unten nach oben packen
for (int x = 0; x < WIDTH; x++) {
    int write_y = HEIGHT - 1;
    for (int read_y = HEIGHT - 1; read_y >= 0; read_y--) {
        if (grid[read_y][x] != 0) {
            grid[write_y--][x] = grid[read_y][x];
        }
    }
}
```

---

### Problem 5: I2C Display not found
**Symptom:** Boot hängt bei Display-Init  
**Ursache:** Display nicht angeschlossen oder falsche Adresse  
**Lösung:** Graceful Degradation
```c
if (i2c_master_probe(I2C_NUM_0, 0x3C, 1000) != ESP_OK) {
    ESP_LOGW(TAG, "No display found, running without OLED");
    g_disp = NULL;
    return;  // Spiel läuft weiter ohne Display
}
```

---

## 6. Performance-Optimierungen (1 min)

### Implementiert:
✅ **Delta-Rendering:** Nur geänderte Pixel (~70% Reduktion)  
✅ **Hardware-Beschleunigung:** RMT + DMA für LEDs  
✅ **ISR + Queue:** Minimale Interrupt-Zeit (300-500ns)  
✅ **Task-Prioritäten:** GameLoop Prio 5 (höchste)

### Messwerte:
| Szenario          | CPU-Zeit | Anteil |
|-------------------|----------|--------|
| Minimal Loop      | 350 µs   | 7%     |
| Render Frame      | 5350 µs  | 107%   |
| CPU Idle          | ~10%     |        |
| CPU Normal        | ~25%     | 60 FPS |

### Nicht implementiert (Zukunft):
❌ **Async RMT:** Non-blocking LED-Transfer (würde 11.5ms CPU freigeben)  
❌ **Animation-Task:** Zeilen-Clear in separatem Task (GameLoop nicht blockieren)

---

## 7. Besondere Features (0.5 min)

### 1. Emergency Reset
- Alle 4 Buttons gleichzeitig 1s halten
- Hard Reset ohne Neustart des ESP32

### 2. NVS Highscore-Persistenz
- Highscore überlebt Power-Off
- Automatisches Speichern bei neuem Rekord

### 3. Dynamic Speed Progression
- 11 Levels (400ms → 50ms Fall-Intervall)
- Basierend auf geclearten Zeilen
- Logarithmische Progression

### 4. Graceful Degradation
- Spiel läuft ohne OLED Display
- Fehlende Hardware wird erkannt und überbrückt

---

## 8. Ergebnisse & Lessons Learned (1 min)

### ✅ Was funktioniert gut:
- **Responsive Input:** 10ms Latenz (Button → Screen)
- **Smooth Rendering:** 60 FPS ohne Flackern
- **Stabile Performance:** ~25% CPU-Auslastung
- **Robuster Code:** Keine Crashes, Graceful Degradation

### 🔧 Was verbessert werden könnte:
- **Async RMT:** Für non-blocking LED-Updates
- **Next-Block Preview:** Auf OLED anzeigen
- **Pause-Funktion:** State PAUSED hinzufügen
- **Statistiken:** Tracking von Tetris-Count, Max Combo

### 📚 Lessons Learned:
1. **Hardware-Beschleunigung nutzen:** RMT/DMA statt Bit-Banging
2. **ISR minimieren:** Nur Queue-Push, Verarbeitung im Task
3. **Debugging wichtig:** 30% der Zeit war Debugging (Prellen, Timing)
4. **Graceful Degradation:** System sollte mit fehlender Hardware umgehen
5. **Delta-Rendering:** Massive Performance-Verbesserung (~70%)

---

## 9. Demo-Video / Live-Demo (optional, 1 min)

**Zeigen:**
- ✅ Boot-Sequenz (Splash-Animation)
- ✅ Block-Bewegung (Links/Rechts/Rotation)
- ✅ Zeilen-Clear Animation (Blink weiß)
- ✅ Score auf OLED Display
- ✅ Speed-Progression (schneller Fall)
- ✅ Game Over (Blink rot)
- ✅ Highscore-Speicherung

---

## 10. Zusammenfassung (0.5 min)

**Projekt erfolgreich abgeschlossen:**
- ✅ Vollständiges Tetris mit 7 Block-Typen
- ✅ 60 FPS smooth Rendering auf 384 LEDs
- ✅ Responsive Controls (10ms Latenz)
- ✅ Persistente Highscore (NVS)
- ✅ Dynamic Speed Progression
- ✅ ~2000 Zeilen C Code in 11 Modulen

**Technologie-Stack:**
- ESP32-S3 + ESP-IDF v5.5.1
- FreeRTOS Multi-Tasking
- RMT Hardware-Beschleunigung
- I2C OLED Display (LVGL)

**Projekt-Dauer:** 4 Wochen  
**Code-Zeilen:** ~2000  
**GitHub:** [Link einfügen]

---

## Fragen?

**Kontakt:**
- Rupp Arian
- Lampert Mathias
- [E-Mail / Discord]

---

**Anhang: Repository-Struktur**

```
TetrisCode/
├── main/
│   ├── src/
│   │   ├── main.c              # Entry Point
│   │   ├── GameLoop/
│   │   │   └── GameLoop.c      # State Machine, Rendering
│   │   ├── Controls/
│   │   │   └── Controls.c      # ISR + Queue
│   │   ├── PlayingField/
│   │   │   ├── Blocks.c        # Tetromino-Definitionen
│   │   │   └── Grid.c          # Kollision, Zeilen-Clear
│   │   ├── Score/
│   │   │   └── Score.c         # NVS Highscore
│   │   └── init/
│   │       ├── DisplayInit.c   # I2C OLED
│   │       └── LedMatrixInit.c # WS2812B Setup
│   └── hdr/
│       └── Globals.h           # Konfiguration
└── SYSTEM_DOKUMENTATION.md     # 8-Seiten Doku
```
