# Übung 5 - Lösungsidee und Dokumentation

## Aufgabe 1: Dynamische Speicherverwaltung

### Konzept und Implementierung

Die dynamische Speicherverwaltung verwaltet einen großen Byte-Array (1024 Bytes) und teilt diesen in Blöcke auf. Jeder Block hat einen 4-Byte-Header:
- 2 Bytes: Flag (0 = belegt, 1 = frei)
- 2 Bytes: Größe der Nutzdaten

### Datenstruktur

```c
typedef struct {
    unsigned short is_free;  // 0 = allocated, 1 = free
    unsigned short size;     // Größe ohne Header
} BlockHeader;
```

### Implementierungsdetails

#### mem_init()
- Erstellt einen einzigen freien Block über den gesamten Speicher
- Größe: MEMORY_SIZE - HEADER_SIZE

#### mem_alloc(int size)
- **Strategie**: First-Fit-Algorithmus
- Durchläuft den Speicher und sucht den ersten freien Block mit ausreichender Größe
- **Splitting**: Wenn der gefundene Block größer ist als benötigt (plus Header + 1 Byte), wird er aufgeteilt
- **Initialisierung**: Nutzdaten werden mit 'A' gefüllt (erleichtert Debugging)
- **Fehlerbehandlung**:
  - size <= 0: NULL
  - size > verfügbarer Speicher: NULL
  - Kein passender Block: NULL

#### mem_free(void *pMem)
- Markiert den Block als frei
- Berechnet Header-Adresse aus Datenzeiger
- **Merging**: Ruft merge_free_blocks() auf, um angrenzende freie Blöcke zusammenzuführen
- **Fehlerbehandlung**:
  - NULL-Pointer: sicher ignoriert
  - Pointer außerhalb des Speichers: ignoriert
  - Block bereits frei: ignoriert (verhindert Doppel-Free)

#### merge_free_blocks()
- Hilfsfunktion die den gesamten Speicher durchläuft
- Wenn zwei aufeinanderfolgende Blöcke beide frei sind, werden sie zusammengeführt
- Verhindert Fragmentierung

#### mem_dump()
- Gibt Speicherbelegung auf Konsole aus
- Zeigt für jeden Block:
  - Offset im Speicher
  - Status (A = Allocated, F = Free)
  - Größe
  - Vorschau der ersten 16 Bytes
- Statistik am Ende mit Gesamtbelegung

### Verwendete Techniken (ohne Bibliotheksfunktionen)

1. **Pointer-Arithmetik**: Navigation durch den Speicher
2. **Casting**: Umwandlung zwischen void*, unsigned char* und BlockHeader*
3. **Manuelle String-Operationen**: str_len, str_copy, str_equal in Tests
4. **Manuelle Speicherfüllung**: mem_fill Funktion

### Test-Suite

Die `mem_manager_test.c` enthält 14 umfassende Tests:

1. **test_init**: Initialisierung
2. **test_simple_alloc**: Einfache Allokation mit 'A'-Initialisierung
3. **test_multiple_alloc**: Mehrere Allokationen
4. **test_string_storage**: Speichern von Strings (Beispiel aus Aufgabe)
5. **test_free_and_reuse**: Freigabe und Wiederverwendung
6. **test_merge_free_blocks**: Zusammenführen freier Blöcke
7. **test_invalid_size**: Fehlerbehandlung ungültige Größen
8. **test_out_of_memory**: Fehlerbehandlung Speicher voll
9. **test_double_free**: Fehlerbehandlung Doppel-Free
10. **test_free_null**: Fehlerbehandlung NULL-Free
11. **test_fragmentation**: Fragmentierungsverhalten
12. **test_small_allocations**: Sehr kleine Allokationen
13. **test_sequential_free**: Sequentielle Freigabe aller Blöcke
14. **test_complex_scenario**: Komplexes realistisches Szenario

Alle Tests verwenden **keine Bibliotheksfunktionen** außer printf für Ausgaben.

---

## Aufgabe 2: Multitasked Blink

### Konzept und Implementierung

Das System erstellt 4 FreeRTOS-Tasks:
- 3 LED-Tasks (je einer für LED 0, 1, 2)
- 1 Button-Task zur Geschwindigkeitssteuerung

### Thread-Synchronisation

#### Semaphor 1: led_semaphore
- **Typ**: Mutex (Binary Semaphore)
- **Zweck**: Schützt LED-Zugriff (kritische Region)
- **Verwendet in**: set_led_pixel() und clear_led_pixel()
- **Verhindert**: Race Conditions beim gleichzeitigen LED-Zugriff durch mehrere Tasks

#### Semaphor 2: delay_semaphore
- **Typ**: Mutex (Binary Semaphore)
- **Zweck**: Schützt globale Variable blink_delay_ms
- **Verwendet in**: LED-Tasks (Lesen) und Button-Task (Schreiben)
- **Verhindert**: Inkonsistente Werte beim gleichzeitigen Lese-/Schreibzugriff

### Task-Implementierung

#### LED-Tasks (led_task_0, led_task_1, led_task_2)
```c
while (1) {
    // 1. Delay-Wert lesen (geschützt)
    xSemaphoreTake(delay_semaphore, portMAX_DELAY);
    current_delay = blink_delay_ms;
    xSemaphoreGive(delay_semaphore);
    
    // 2. LED schalten (geschützt durch led_semaphore in set_led_pixel)
    if (state) {
        set_led_pixel(led_index, r, g, b);
    } else {
        clear_led_pixel(led_index);
    }
    
    // 3. Zustand umschalten und warten
    state = !state;
    vTaskDelay(current_delay / portTICK_PERIOD_MS);
}
```

**Eigenschaften**:
- Jeder Task blinkt seine LED unabhängig
- LED 0: Rot (16, 0, 0)
- LED 1: Grün (0, 16, 0)
- LED 2: Blau (0, 0, 16)
- Alle verwenden dieselbe Blink-Geschwindigkeit

#### Button-Task
```c
while (1) {
    // Taster-Zustände lesen
    left_state = gpio_get_level(BUTTON_LEFT_GPIO);
    right_state = gpio_get_level(BUTTON_RIGHT_GPIO);
    
    // Flanken-Erkennung (Pull-up: 0 = gedrückt)
    if (left_state == 0 && last_left_state == 1) {
        // Linker Taster: Verlangsamen
        xSemaphoreTake(delay_semaphore, portMAX_DELAY);
        if (blink_delay_ms < MAX_DELAY_MS) {
            blink_delay_ms += DELAY_STEP_MS;
        }
        xSemaphoreGive(delay_semaphore);
    }
    
    if (right_state == 0 && last_right_state == 1) {
        // Rechter Taster: Beschleunigen
        xSemaphoreTake(delay_semaphore, portMAX_DELAY);
        if (blink_delay_ms > MIN_DELAY_MS) {
            blink_delay_ms -= DELAY_STEP_MS;
        }
        xSemaphoreGive(delay_semaphore);
    }
    
    vTaskDelay(50 / portTICK_PERIOD_MS);  // Polling-Intervall
}
```

**Eigenschaften**:
- Flanken-Erkennung (nur bei fallender Flanke reagieren)
- Linker Taster: Delay +100ms (langsamer)
- Rechter Taster: Delay -100ms (schneller)
- Begrenzung: 100ms bis 5000ms
- Polling alle 50ms

### Kritische Regionen

1. **LED-Operationen**:
   ```c
   xSemaphoreTake(led_semaphore, portMAX_DELAY);
   led_strip_set_pixel(led_strip, led_index, r, g, b);
   led_strip_refresh(led_strip);
   xSemaphoreGive(led_semaphore);
   ```
   Verhindert dass mehrere Tasks gleichzeitig LEDs setzen und refresh aufrufen.

2. **Delay-Zugriff**:
   ```c
   xSemaphoreTake(delay_semaphore, portMAX_DELAY);
   current_delay = blink_delay_ms;  // oder Zuweisung
   xSemaphoreGive(delay_semaphore);
   ```
   Verhindert Race Conditions beim Lesen/Schreiben der Geschwindigkeit.

### GPIO-Konfiguration

- **LEDs**: Addressable LED Strip (WS2812 oder ähnlich) auf BLINK_GPIO
- **Buttons**: GPIO 0 (links) und GPIO 35 (rechts) mit Pull-up
  - Pull-up: High (1) wenn nicht gedrückt
  - Low (0) wenn gedrückt

### Ablauf in app_main()

1. Memory Manager Tests ausführen (Aufgabe 1)
2. Hardware konfigurieren (LEDs und Buttons)
3. Semaphoren erstellen (led_semaphore, delay_semaphore)
4. Tasks erstellen:
   - LED_Task_0, LED_Task_1, LED_Task_2 (Priorität 5)
   - Button_Task (Priorität 6 - höher für bessere Reaktion)
5. System läuft

---

## Build und Ausführung

### Voraussetzungen
- ESP-IDF installiert
- ESP32 oder ESP32-C3 Board

### Build-Befehle
```bash
# Projekt konfigurieren
idf.py menuconfig

# Bauen
idf.py build

# Flashen und Monitor starten
idf.py flash monitor
```

### Erwartete Ausgabe

1. Beim Start: Memory Manager Test-Suite
   - 14 Tests mit detaillierter Ausgabe
   - Memory Dumps zeigen Speicherbelegung
   - Test-Zusammenfassung

2. Nach Tests: Multitasked Blink
   - 3 LEDs blinken im Takt
   - Log-Meldungen bei Button-Drücken
   - Geschwindigkeitsänderungen werden geloggt

---

## Besonderheiten der Implementierung

### Aufgabe 1 (Memory Manager)
- ✅ Keine Bibliotheksfunktionen (außer printf)
- ✅ Umfassende Fehlerbehandlung
- ✅ Automatische Test-Suite
- ✅ Merge von freien Blöcken
- ✅ Block-Splitting
- ✅ Debug-freundlich ('A'-Initialisierung)

### Aufgabe 2 (Multitasked Blink)
- ✅ 3 unabhängige LED-Tasks
- ✅ Semaphor-geschützte LED-Zugriffe
- ✅ Semaphor-geschützte Delay-Variable
- ✅ Button-Task mit Flanken-Erkennung
- ✅ Geschwindigkeitsregelung mit Grenzen
- ✅ Höhere Priorität für Button-Task

---

## Mögliche Erweiterungen

1. **Memory Manager**:
   - Best-Fit oder Worst-Fit statt First-Fit
   - Defragmentierung
   - Speicher-Statistiken zur Laufzeit

2. **Multitasked Blink**:
   - Interrupt-basierte Button-Erkennung statt Polling
   - Verschiedene Blink-Muster
   - Individuelle Geschwindigkeiten pro LED
   - Web-Interface zur Steuerung

---

## Dateien im Projekt

- `mem_manager.h` / `mem_manager.c`: Memory Manager Implementierung
- `mem_manager_test.h` / `mem_manager_test.c`: Automatische Tests
- `blink_example_main.c`: Hauptprogramm mit Multitasked Blink
- `CMakeLists.txt`: Build-Konfiguration (bereits aktualisiert)
- `LOESUNGSIDEE.md`: Diese Datei

---

**Erstellt für Übung 5, Systemprogrammierung**
