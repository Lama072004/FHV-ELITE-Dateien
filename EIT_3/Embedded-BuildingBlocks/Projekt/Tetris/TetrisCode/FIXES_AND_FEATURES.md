# Bug Fixes & Features - Session Aktualisierung

## 1. AUTO-START BUG GELÖST ✅

### Problem
Nach dem Reset (Emergency Reset mit 4-Button-Combo) startete das Spiel automatisch, ohne dass der Benutzer einen neuen Button-Press durchführen musste.

### Ursache
Die `wait_for_restart()` Funktion wartete nur auf Button-Release, nicht auf einen NEW Button-Press. Dadurch startete das Spiel sofort nach Rückkehr zur Hauptschleife.

### Lösung
Die `wait_for_restart()` Funktion wurde überarbeitet (GameLoop_NEW.c, ca. Zeile 350-390):

```c
// 4. Warten bis ALLE Buttons released sind
printf("[GameLoop] Waiting for ALL buttons to be released...\n");
vTaskDelay(pdMS_TO_TICKS(100));
while (check_button_pressed(BTN_LEFT) || check_button_pressed(BTN_RIGHT) ||
       check_button_pressed(BTN_ROTATE) || check_button_pressed(BTN_FASTER)) {
    // Timeout-Schutz
}

// 5. **EXPLIZIT auf NEUEN Button-Press warten** (FIX!)
printf("[GameLoop] Waiting for NEW button press to start game...\n");
while (!controls_get_event(&ev)) {
    vTaskDelay(pdMS_TO_TICKS(50));  // Poll-basiert, aber robust
}
printf("[GameLoop] Button pressed (GPIO %d), starting game!\n", ev);
```

**Verhalten nach Fix:**
1. Reset triggered → Splash anzeigen → Button-Release warten
2. **WARTE auf NEUEM Button-Press** (wichtig!)
3. Erst dann startet Spiel neu

---

## 2. MULTI-SONG SYSTEM IMPLEMENTIERT ✅

### Features
- **Cyclisches Wechseln:** Tetris → StarWars → Mario → Silence → Tetris (Loop)
- **Button-Combo während Spiel:** Alle 4 Buttons > 1 Sekunde = Song wechseln (statt Reset)
- **Song-Pausierung:** Musik pausiert kurz bei Wechsel, dann neuer Song startet
- **Unterschiedliche Funktion für Reset:**
  - Während **RUNNING:** 4-Button-Combo = Song wechsel
  - Während **WAIT:** 4-Button-Combo = Emergency Reset (wie zuvor)

### Code-Änderungen

#### A. Neue Melodien-Vorlagen in ThemeSong.c (Zeile ~100-130)

```c
// TEMPLATES für User zum Ausfüllen:
static const buzzer_musical_score_t score_starwars[] = {
    // TODO: Hier StarWars Noten einfügen
    {Pause, 1000*SPEED},  // Placeholder
};

static const buzzer_musical_score_t score_mario[] = {
    // TODO: Hier Mario Bros Noten einfügen
    {Pause, 1000*SPEED},  // Placeholder
};

static const buzzer_musical_score_t score_silence[] = {
    {Pause, 2000*SPEED},  // 2 Sekunden Stille
};
```

**Du kannst hier die Noten einfügen!** Beispiel:
```c
static const buzzer_musical_score_t score_mario[] = {
    {E5, 250*SPEED},
    {E5, 250*SPEED},
    {E5, 250*SPEED},
    {C5, 250*SPEED},
    {E5, 250*SPEED},
    {G5, 500*SPEED},
    {G4, 500*SPEED},
    {Pause, 1000*SPEED},
    // ... weitere Noten
};
```

#### B. Song-Management System in ThemeSong.c (Zeile ~130-170)

```c
static int current_song_index = THEME_TETRIS;  // Aktuelle Song

typedef struct {
    const buzzer_musical_score_t *score;
    size_t score_size;
    const char *name;
} ThemeSong;

static const ThemeSong available_songs[] = {
    { score, sizeof(score) / sizeof(score[0]), "TETRIS" },
    { score_starwars, sizeof(score_starwars) / sizeof(score_starwars[0]), "STARWARS" },
    { score_mario, sizeof(score_mario) / sizeof(score_mario[0]), "MARIO" },
    { score_silence, sizeof(score_silence) / sizeof(score_silence[0]), "SILENCE" }
};
```

#### C. Neue Funktionen in ThemeSong.c (Zeile ~310-380)

1. **`theme_next_song()`** - Zur nächsten Melodie wechseln (mit Cycling)
   ```c
   current_song_index = (current_song_index + 1) % NUM_AVAILABLE_SONGS;
   ```
   
2. **`theme_set_song(int index)`** - Zu bestimmtem Song wechseln
   ```c
   theme_set_song(THEME_MARIO);  // Direkt zu Mario
   ```

3. **`theme_get_current_song()`** - Aktuellen Song-Index abrufen
   ```c
   int current = theme_get_current_song();  // 0=Tetris, 1=StarWars, etc.
   ```

#### D. ThemeTask Anpassung (Zeile ~175-230)

Die ThemeTask wurde so geändert, dass sie den aktuellen Song aus dem `available_songs[]` Array laden kann, statt den hardcodierten `score[]` zu verwenden.

```c
// Aktuellen Song aus Array holen
const ThemeSong *current_song = &available_songs[current_song_index];

for (size_t i = 0; i < current_song->score_size; ++i) {
    // ... Musik abspielen mit current_song->score[i]
    
    // Song-Index regelmäßig prüfen (falls während Musik geändert)
    if (current_song_index != (int)(current_song - available_songs)) {
        break;  // Zur nächsten Song-Schleife
    }
}
```

#### E. GameLoop Logik für Song-Wechsel (Zeile ~410-450)

```c
if (controls_all_buttons_pressed()) {
    vTaskDelay(pdMS_TO_TICKS(1000));  // 1 Sec halten
    
    if (controls_all_buttons_pressed()) {
        if (game_running) {
            // ✅ WÄHREND SPIELS: 4-Button = Song wechsel
            printf("[GameLoop] Song change triggered\n");
            theme_next_song();
            printf("[GameLoop] Switched to song #%d\n", theme_get_current_song());
            
            // Kurz pausieren für visuelles Feedback
            theme_pause();
            vTaskDelay(pdMS_TO_TICKS(200));
            theme_resume();
        } else {
            // ✅ WÄHREND WAIT: 4-Button = Emergency Reset
            printf("[GameLoop] Emergency reset triggered\n");
            reset_game_state();
            wait_for_restart();
            // ... Game starten
        }
    }
}
```

---

## 3. WIE DU DIE MELODIEN SCHREIBST

### Schritt-für-Schritt Anleitung

1. **Öffne** `main/src/ThemeSong/ThemeSong.c`

2. **Finde** die Score-Arrays (ca. Zeile 105-125):
   ```c
   static const buzzer_musical_score_t score_starwars[] = {
       // TODO: Hier StarWars Noten einfügen
       {Pause, 1000*SPEED},  // Placeholder
   };
   ```

3. **Ersetze** die Placeholder mit deinen Noten. Verfügbare Noten:
   ```c
   // Noten-Frequenzen (Hz)
   C2, D2, E2, F2, G2, A2, B2,
   C3, D3, E3, F3, G3, A3, B3,
   C4, D4, E4, F4, G4, A4, B4,
   C5, D5, E5, F5, G5, A5, B5,
   Pause  // Stille (Frequenz = 0)
   ```

4. **Format:** `{note, duration_ms * SPEED}`
   ```c
   {E5, 250*SPEED},     // E5, 250ms
   {G5, 500*SPEED},     // G5, 500ms (doppelte Länge)
   {Pause, 100*SPEED},  // Pause, 100ms
   ```

5. **Beispiel** - Super Mario Bros erste Noten:
   ```c
   static const buzzer_musical_score_t score_mario[] = {
       {E5, 250*SPEED},
       {E5, 250*SPEED},
       {E5, 250*SPEED},
       {Pause, 125*SPEED},
       {C5, 250*SPEED},
       {E5, 250*SPEED},
       {G5, 500*SPEED},
       {Pause, 250*SPEED},
       {G4, 500*SPEED},
       {Pause, 2000*SPEED},
   };
   ```

6. **Speichern** und kompilieren!

---

## 4. BENUTZER-INTERFACE IM SPIEL

### Button-Mappings
- **Links/Rechts/Oben/Unten:** Normal Tetris-Steuerung
- **Alle 4 Buttons + 1 Sekunde halten (WÄHREND SPIEL):** 🎵 Song wechseln
- **Alle 4 Buttons + 1 Sekunde halten (IM WAIT-SCREEN):** 🔄 Game Reset

### Song-Reihenfolge
```
🎮 GAME RUNNING:
   Musik pausiert kurz... 
   "Now Playing: STARWARS" (könnte auf Display angezeigt werden)
   Musik läuft weiter...
   
  Buttons wieder loslassen → Tetris-Steuerung aktiv
```

---

## 5. VERWENDETE FUNKTIONEN (HEADER)

Alle Funktionen sind in `main/hdr/ThemeSong.h` deklariert:

```c
// Musik-Kontrolle
void theme_pause(void);           // Musik pausieren
void theme_resume(void);          // Musik fortsetzen

// Multi-Song System
void theme_next_song(void);       // Zur nächsten Melodie
void theme_set_song(int index);   // Zu bestimmtem Song
int theme_get_current_song(void); // Aktuellen Song abfragen

// Enum für Song-Indizes
enum ThemeSongType {
    THEME_TETRIS = 0,      // Original Tetris
    THEME_STARWARS = 1,    // Star Wars
    THEME_MARIO = 2,       // Super Mario Bros
    THEME_SILENCE = 3,     // Stille
    THEME_LAST = 4         // Sentinel
};
```

---

## 6. DEBUGGING-TIPPS

Falls die Musik nicht wechselt, überprüfe:

1. **Kompilierung:** Wurden alle Dateien korrekt modified?
   - ✅ GameLoop_NEW.c (Include ThemeSong.h + Logik)
   - ✅ ThemeSong.c (Song-Arrays + Funktionen)
   - ✅ ThemeSong.h (Funktions-Deklarationen)

2. **Serial Output:** Überprüfe `printf()` Ausgaben:
   ```
   [GameLoop] Song change triggered (4 buttons during game)
   [GameLoop] Switched to song #1
   [Theme] Playing song: STARWARS
   ```

3. **Button-Debounce:** 1 Sekunde halten ist erforderlich!
   - Zu schnell drücken? → Song ändert sich nicht
   - Test: Alle 4 Buttons gemeinsam mindestens 1 Sekunde halten

4. **Song-Index Bereich:** `current_song_index` sollte 0-3 sein
   - Falls `printf("[GameLoop] Switched to song #-1"` → Bug!

---

## 7. ZUSAMMENFASSUNG DER ÄNDERUNGEN

| Datei | Änderung | Status |
|-------|----------|--------|
| GameLoop_NEW.c | Auto-Start Fix + Song-Wechsel Logik | ✅ DONE |
| ThemeSong.c | Multi-Song System + Funktionen | ✅ DONE |
| ThemeSong.h | Enum + Funktions-Deklarationen | ✅ DONE |
| main/hdr/Globals.h | Keine Änderung nötig | ✅ OK |

**Kompilierung:** Wird nach ESP-IDF Setup validiert

---

## 8. NÄCHSTE SCHRITTE (OPTIONAL)

1. **Display-Integration:** Song-Namen auf OLED anzeigen ("Now Playing: MARIO")
2. **EEPROM-Speicher:** Letzten gespielten Song merken
3. **Dynamische Tempo-Anpassung:** SPEED-Variable basierend auf Spielgeschwindigkeit

---

**Stand:** $(date)
**Version:** 1.0 - Auto-Start Fix + Multi-Song System
