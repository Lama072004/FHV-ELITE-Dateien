# Übung 6 - Systemprogrammierung

## Übersicht

Diese Übung erweitert Übung 5 mit fortgeschrittenen Konzepten der Systemprogrammierung:

1. **Semaphor-Blocking-Tests**: Untersuchung von Deadlock-Situationen
2. **Erweiterte Speicherverwaltung**: Header mit Checksumme zur Buffer-Overflow-Erkennung
3. **Multitasked Producer/Consumer**: Drei Varianten der Task-Kommunikation

## Projektstruktur

```
Uebung5/
├── main/
│   ├── blink_example_main.c              # Hauptprogramm mit Varianten-Auswahl
│   ├── mem_manager.c/h                   # Memory Manager mit Checksumme
│   ├── mem_manager_test.c/h              # Automatische Tests
│   ├── blink_semaphore_blocking_test.c/h # Aufgabe 1: Blocking Tests
│   ├── blink_variant1_semaphore.c        # Aufgabe 3.1: Semaphore
│   ├── blink_variant2_notify.c           # Aufgabe 3.2: Notifications
│   ├── blink_variant3_queue.c            # Aufgabe 3.3: Queue
│   └── blink_variants.h                  # Header für alle Varianten
├── LOESUNGSIDEE_UEBUNG6.md               # Vollständige Dokumentation
└── README_UEBUNG6.md                     # Diese Datei
```

## Quick Start

### 1. Variante auswählen

Öffnen Sie `main/blink_example_main.c` und setzen Sie **genau EIN** Define auf 1:

```c
#define RUN_ORIGINAL_VERSION 0           // Original (Übung 5)
#define RUN_SEMAPHORE_BLOCKING_TEST 0    // Aufgabe 1
#define RUN_VARIANT_1_SEMAPHORE 0        // Aufgabe 3.1
#define RUN_VARIANT_2_NOTIFY 0           // Aufgabe 3.2
#define RUN_VARIANT_3_QUEUE 1            // Aufgabe 3.3 ← Standard
```

### 2. Build und Flash

```bash
# Projekt bauen
idf.py build

# Auf ESP32 flashen und Monitor starten
idf.py flash monitor

# Monitor beenden: Ctrl+]
```

### 3. Beobachten

- **Memory Tests** werden automatisch zuerst ausgeführt
- Dann startet die gewählte Variante
- LEDs beginnen zu blinken
- Buttons drücken um Geschwindigkeit zu ändern

## Aufgaben

### Aufgabe 1: Semaphor-Blocking

**Ziel**: Untersuche was passiert wenn Semaphore nicht freigegeben werden

**Datei**: `blink_semaphore_blocking_test.c`

**Tests**:
1. Ewiges Blockieren (portMAX_DELAY) → Deadlock
2. Blockieren mit Timeout → Task kann weitermachen

**Erkenntnisse**:
- Immer Timeouts verwenden
- Deadlocks vermeiden
- Fehlerbehandlung wichtig

### Aufgabe 2: Erweiterte Speicherverwaltung

**Ziel**: Header mit Bitfields und Checksumme implementieren

**Dateien**: `mem_manager.c/h`, `mem_manager_test.c`

**Änderungen**:
- Bitfield-basierter Header (4 Bytes)
- Additive Checksumme (modulo 256)
- `checkHeap()` Funktion
- Buffer-Overflow-Detection Tests

**Features**:
```c
typedef struct {
    unsigned char is_free : 1;    // 1 Bit
    unsigned char reserved : 7;   // 7 Bits
    unsigned short size;          // 16 Bits
    unsigned char checksum;       // 8 Bits (über erste 3 Bytes)
} BlockHeader;
```

**Tests**:
- Test 15: Normale Checksummen-Prüfung
- Test 16: Buffer Overflow Detection (schreibt über Block-Ende hinaus)
- Test 17: Checksumme nach Merge-Operation

### Aufgabe 3: Producer/Consumer-Varianten

**Ziel**: Drei verschiedene Kommunikationsmechanismen implementieren

#### Variante 1: Counting Semaphore

**Datei**: `blink_variant1_semaphore.c`

**Konzept**: Producer sendet Events via Semaphore

```c
// Producer
xSemaphoreGive(event_semaphore);  // × 3 für alle Consumer

// Consumer
if (xSemaphoreTake(event_semaphore, 0) == pdTRUE) {
    // Event empfangen
}
```

**Vorteile**: Einfach, mehrere Consumer
**Nachteile**: Keine Typ-Information

#### Variante 2: Task Notifications

**Datei**: `blink_variant2_notify.c`

**Konzept**: Direkte Task-zu-Task Benachrichtigung

```c
// Producer
xTaskNotify(led_task_handle, EVENT_SPEED_UP, eSetBits);

// Consumer
if (xTaskNotifyWait(0x00, 0xFFFFFFFF, &value, 0) == pdTRUE) {
    if (value & EVENT_SPEED_UP) { /* ... */ }
}
```

**Vorteile**: Schnell, bis 32 Bit Daten
**Nachteile**: Task Handles nötig, wenig Pufferung

#### Variante 3: Queue mit KeyEvent

**Datei**: `blink_variant3_queue.c`

**Konzept**: Typsichere Events über Queue

```c
typedef struct {
    KeyEventType type;
    uint32_t timestamp;
    uint8_t button_id;
} KeyEvent;

// Producer
KeyEvent event = { .type = KEY_EVENT_LEFT_PRESSED, ... };
xQueueSend(key_event_queue, &event, 0);

// Consumer
KeyEvent event;
if (xQueueReceive(key_event_queue, &event, 0) == pdTRUE) {
    // Event verarbeiten
}
```

**Vorteile**: Typsicher, FIFO, erweiterbar
**Nachteile**: Etwas mehr Overhead

## Hardware-Anforderungen

- **ESP32** oder **ESP32-C3** Board
- **LED Strip** (WS2812 oder ähnlich) auf GPIO 8 (konfigurierbar)
- **2 Buttons**:
  - Linker Button: GPIO 0 (meist BOOT-Button)
  - Rechter Button: GPIO 35 (oder anpassen)

## GPIO-Konfiguration

In `sdkconfig.defaults` bzw. `menuconfig`:

```
CONFIG_BLINK_GPIO=8
CONFIG_BLINK_LED_STRIP=y
CONFIG_BLINK_LED_STRIP_BACKEND_RMT=y
```

## Erwartete Ausgabe

### Memory Manager Tests (immer zuerst)

```
========================================
  Memory Manager Test Suite
========================================

--- Test 1: Memory Initialization ---
...
--- Test 16: Buffer Overflow Detection ---
  Simulating buffer overflow...
CORRUPTION DETECTED at Block 1 (Offset 24):
  Expected checksum: 0x2A
  Actual checksum:   0x58
  [PASS] Buffer overflow detected by checksum
...
Total Tests: 17
Assertions Passed: 45
All tests PASSED!
```

### Variante 3: Queue (Beispiel)

```
========================================
  Variant 3: Queue with KeyEvent
========================================

LED Task 0 started (reading from queue)
Button Task started (will send events to queue)

Button: LEFT pressed - sending to queue
Task 0: Received LEFT_PRESSED event (button 0) at 5234 ms
Task 0: Slow down -> delay = 1100 ms
```

## Vergleich der Varianten

| Aspekt | Semaphore | Notify | Queue |
|--------|-----------|--------|-------|
| Komplexität | ⭐ | ⭐⭐ | ⭐⭐⭐ |
| Geschwindigkeit | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐ |
| Typ-Information | ❌ | ✅ (32 Bit) | ✅ (Struct) |
| Pufferung | ⭐⭐⭐ | ⭐ | ⭐⭐⭐⭐ |
| Empfehlung | Einfach | Schnell | Professionell |

## Debugging-Tipps

### Monitor-Output

```bash
# Mit Filter (nur eigene Logs)
idf.py monitor --print-filter="multitask_blink:I,pc_semaphore:I,event_notify:I,queue_variant:I"

# Ohne Filter (alles)
idf.py monitor
```

### Häufige Probleme

1. **LEDs blinken nicht**
   - GPIO-Pin korrekt konfiguriert?
   - LED-Typ in menuconfig richtig?
   - Stromversorgung ausreichend?

2. **Buttons reagieren nicht**
   - GPIO-Pins richtig? (GPIO 0 meist BOOT-Button)
   - Pull-up richtig konfiguriert?
   - Polling-Interval angepasst?

3. **Compilation-Fehler**
   - Alle Dateien in CMakeLists.txt?
   - ESP-IDF Version kompatibel?
   - `idf.py fullclean` und neu bauen

## Weiterführende Dokumentation

Siehe `LOESUNGSIDEE_UEBUNG6.md` für:
- Detaillierte Implementierungsbeschreibung
- Schritt-für-Schritt Erklärungen
- Code-Beispiele mit Kommentaren
- Vergleichstabellen
- Best Practices
- Mögliche Erweiterungen

## Testen verschiedener Varianten

```bash
# 1. Variante 1 testen
# Ändere in blink_example_main.c: RUN_VARIANT_1_SEMAPHORE auf 1
idf.py build flash monitor

# 2. Variante 2 testen  
# Ändere in blink_example_main.c: RUN_VARIANT_2_NOTIFY auf 1
idf.py build flash monitor

# 3. Variante 3 testen
# Ändere in blink_example_main.c: RUN_VARIANT_3_QUEUE auf 1
idf.py build flash monitor

# 4. Blocking Tests
# Ändere in blink_example_main.c: RUN_SEMAPHORE_BLOCKING_TEST auf 1
idf.py build flash monitor
```

## Code-Qualität

- ✅ Keine Bibliotheksfunktionen (außer ESP-IDF und printf)
- ✅ Umfassende Fehlerbehandlung
- ✅ Automatische Test-Suite
- ✅ Ausführliche Kommentare
- ✅ Logging für Debugging
- ✅ Konfigurierbar über Defines

## Lessons Learned

1. **Semaphore**: Immer mit Timeout, sonst Deadlock-Gefahr
2. **Checksummen**: Buffer Overflows früh erkennen
3. **Task-Kommunikation**: Richtige Methode für Problem wählen
4. **Lokale Variablen**: Keine globalen Variablen wenn möglich
5. **Error Handling**: Return-Werte immer prüfen

## Lizenz

Erstellt für Übungsblatt 6, Systemprogrammierung  
FH Kurs, Dezember 2024

## Kontakt

Bei Fragen zur Implementierung:
- Siehe Kommentare im Code
- Lese LOESUNGSIDEE_UEBUNG6.md
- ESP-IDF Dokumentation: https://docs.espressif.com/

---

**Viel Erfolg! 🚀**
