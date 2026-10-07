# Übung 6 - Lösungsidee und Dokumentation

## Überblick

Diese Übung erweitert Übung 5 mit folgenden Aufgaben:
1. **Semaphor-Blocking-Tests**: Untersuchung des Verhaltens wenn Semaphore nicht freigegeben werden
2. **Erweiterte Speicherverwaltung**: Header mit Bitfields und Checksumme zur Korruptionserkennung
3. **Multitasked Producer/Consumer**: Drei verschiedene Kommunikationsvarianten für Task-Synchronisation

---

## Aufgabe 1: Multitasked Blink Lock

### Ziel
Untersuchen des Verhaltens wenn reservierte Semaphore nicht mehr freigegeben werden:
- Fall 1: Semaphor "ewig" blockiert (portMAX_DELAY)
- Fall 2: Definierter Timeout

### Implementierung

Datei: `blink_semaphore_blocking_test.c/h`

#### Szenario 1: Ewiges Blockieren

**Blocking Task**:
```c
if (xSemaphoreTake(test_semaphore, portMAX_DELAY) == pdTRUE) {
    ESP_LOGI(TAG, "Acquired semaphore - will NEVER release it");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // xSemaphoreGive(test_semaphore);  // <-- ABSICHTLICH auskommentiert!
}
```

**Waiting Task (Eternal)**:
```c
ESP_LOGI(TAG, "Trying to acquire semaphore with portMAX_DELAY...");
if (xSemaphoreTake(test_semaphore, portMAX_DELAY) == pdTRUE) {
    // Dieser Code wird NIE erreicht
    ESP_LOGI(TAG, "SUCCESS: Acquired semaphore");
}
```

**Beobachtetes Verhalten**:
- Blocking Task nimmt Semaphor und gibt ihn nicht frei
- Waiting Task wartet unendlich lange
- **Deadlock**: Waiting Task macht keinen Fortschritt mehr
- System ist blockiert (andere Tasks laufen weiter, aber Waiting Task ist "stuck")

#### Szenario 2: Timeout-Verhalten

**Waiting Task (Timeout)**:
```c
TickType_t timeout = 3000 / portTICK_PERIOD_MS;
TickType_t start = xTaskGetTickCount();

if (xSemaphoreTake(test_semaphore, timeout) == pdTRUE) {
    ESP_LOGI(TAG, "SUCCESS: Acquired semaphore after %d ms", ...);
} else {
    ESP_LOGW(TAG, "TIMEOUT: Could not acquire semaphore after %d ms", ...);
    // Task kann mit Fehlerbehandlung fortfahren
}
```

**Beobachtetes Verhalten**:
- Task wartet maximal 3000ms
- Nach Timeout wird pdFALSE zurückgegeben
- Task kann Fehlerbehandlung ausführen
- **Kein Deadlock**: Task macht weiter (mit alternativer Aktion oder Fehlerbehandlung)

### Erkenntnisse

| Aspekt | portMAX_DELAY | Mit Timeout |
|--------|---------------|-------------|
| Blockierung | Unendlich | Maximal Timeout-Dauer |
| Rückgabewert | Nie (wartet ewig) | pdFALSE nach Timeout |
| Fehlerbehandlung | Nicht möglich | Möglich |
| Deadlock-Gefahr | Sehr hoch | Vermeidbar |
| Empfehlung | Vermeiden | Bevorzugt |

**Best Practices**:
1. Immer Timeouts verwenden in Produktivcode
2. Semaphore IMMER freigeben (auch bei Fehlern)
3. RAII-ähnliche Muster verwenden (z.B. im finally-Block)
4. Logging für Debug-Zwecke
5. Watchdog-Timer als Absicherung

---

## Aufgabe 2: Dynamische Speicherverwaltung (Erweitert)

### Neue Header-Struktur mit Bitfields

**Alter Header (Übung 5)**:
```c
typedef struct {
    unsigned short is_free;  // 2 Bytes
    unsigned short size;     // 2 Bytes
} BlockHeader;  // Total: 4 Bytes
```

**Neuer Header (Übung 6)**:
```c
typedef struct {
    // Byte 0: Flags
    unsigned char is_free : 1;      // Bit 0
    unsigned char reserved : 7;     // Bits 1-7
    
    // Bytes 1-2: Größe
    unsigned short size;            // 16 Bit (max 65535)
    
    // Byte 3: Checksumme
    unsigned char checksum;         // Additive Checksumme mod 256
} BlockHeader;  // Total: 4 Bytes
```

### Bitfield-Vorteile

1. **Speichereffizienz**: 1 Bit statt 16 Bit für is_free
2. **Erweiterbarkeit**: 7 reserved Bits für zukünftige Flags
3. **Typsicherheit**: Compiler prüft Bitfeld-Zugriffe
4. **Dokumentation**: Struktur zeigt klar die Aufteilung

### Checksumme-Implementierung

#### Berechnung
```c
static unsigned char calculate_checksum(BlockHeader *header) {
    unsigned char *bytes = (unsigned char*)header;
    unsigned int sum = 0;
    sum += bytes[0];  // Flag byte
    sum += bytes[1];  // Size low byte
    sum += bytes[2];  // Size high byte
    return (unsigned char)(sum % 256);
}
```

**Additive Checksumme**:
- Einfachste Form einer Prüfsumme
- Summe der ersten 3 Bytes
- Modulo 256 (passt in 1 Byte)
- Erkennt: Bit-Flips, Überschreibungen, einfache Korruption
- Erkennt NICHT: Vertauschungen, komplexe Fehler

#### Setzen der Checksumme
```c
static void set_checksum(BlockHeader *header) {
    header->checksum = calculate_checksum(header);
}
```

Wird aufgerufen:
- Nach `mem_init()`
- Nach `mem_alloc()` (für allokierten und Rest-Block)
- Nach `mem_free()`
- Nach `merge_free_blocks()`

#### Verifizieren der Checksumme
```c
static int verify_checksum(BlockHeader *header) {
    return (header->checksum == calculate_checksum(header));
}
```

### checkHeap() Funktion

```c
int checkHeap(void) {
    int block_number = 0;
    int corrupt_blocks = 0;
    
    // Durchlaufe alle Blöcke
    while (current < end) {
        BlockHeader *header = (BlockHeader*)current;
        
        // Checksumme prüfen
        if (!verify_checksum(header)) {
            printf("CORRUPTION DETECTED at Block %d (Offset %d)\n", 
                   block_number, offset);
            printf("  Expected checksum: 0x%02X\n", calculate_checksum(header));
            printf("  Actual checksum:   0x%02X\n", header->checksum);
            corrupt_blocks++;
        }
        
        block_number++;
        current = current + HEADER_SIZE + header->size;
    }
    
    return corrupt_blocks;  // 0 = alles OK
}
```

**Funktionalität**:
- Durchläuft alle Blöcke im Heap
- Prüft Checksumme jedes Headers
- Meldet korrupte Blöcke mit Details
- Gibt Anzahl korrupter Blöcke zurück

### Buffer Overflow Detection Tests

#### Test 1: Normale Allokation (sollte OK sein)
```c
void test_checksum_integrity(void) {
    mem_init();
    void *p1 = mem_alloc(50);
    int corrupt = checkHeap();
    // Erwartet: corrupt == 0
}
```

#### Test 2: Buffer Overflow simulieren
```c
void test_buffer_overflow_detection(void) {
    mem_init();
    void *p1 = mem_alloc(20);  // 20 Bytes allokieren
    
    // Prüfe Heap - sollte OK sein
    int corrupt_before = checkHeap();  // = 0
    
    // Simuliere Overflow: Schreibe über Block-Ende hinaus
    char *data = (char*)p1;
    for (int i = 0; i < 25; i++) {  // 5 Bytes zu viel!
        data[i] = 'X';
    }
    
    // Prüfe erneut - sollte jetzt Korruption erkennen
    int corrupt_after = checkHeap();  // > 0
    // Header des nächsten Blocks wurde überschrieben!
}
```

**Funktionsweise**:
```
Speicherlayout vor Overflow:
[Header1|20 Bytes Daten][Header2|Rest...]
        ^ p1 zeigt hier

Nach Schreiben von 25 Bytes:
[Header1|XXXXXXXXXXXXXXXXXXXXXXXXX]...
                         ^^^ Überschreibt Header2!

Header2.checksum ist jetzt falsch!
```

#### Test 3: Checksum nach Merge
```c
void test_checksum_after_merge(void) {
    mem_init();
    void *p1 = mem_alloc(30);
    void *p2 = mem_alloc(30);
    void *p3 = mem_alloc(30);
    
    mem_free(p2);  // Mittleren freigeben
    mem_free(p1);  // Ersten freigeben -> Merge!
    
    int corrupt = checkHeap();  // Sollte 0 sein
    // Checksumme des gemergten Blocks muss korrekt sein
}
```

### Änderungen an bestehenden Funktionen

**mem_init()**:
```c
void mem_init(void) {
    BlockHeader *initial_header = (BlockHeader*)memory;
    initial_header->is_free = 1;
    initial_header->reserved = 0;  // NEU
    initial_header->size = MEMORY_SIZE - HEADER_SIZE;
    set_checksum(initial_header);  // NEU
}
```

**mem_alloc()** - Checksumme für beide Blöcke:
```c
// Block als belegt markieren
header->is_free = 0;

if (remaining > HEADER_SIZE) {
    // Rest-Block erstellen
    next_header->is_free = 1;
    next_header->reserved = 0;  // NEU
    next_header->size = remaining - HEADER_SIZE;
    set_checksum(next_header);  // NEU
    
    header->size = size;
}

set_checksum(header);  // NEU
```

**mem_free()** - Checksumme aktualisieren:
```c
header->is_free = 1;
set_checksum(header);  // NEU
merge_free_blocks();
```

**merge_free_blocks()** - Nach Merge:
```c
if (next_header->is_free) {
    header->size = header->size + HEADER_SIZE + next_header->size;
    set_checksum(header);  // NEU
}
```

### Vorteile der Checksummen-Implementierung

| Vorteil | Beschreibung |
|---------|--------------|
| **Buffer Overflow Detection** | Erkennt Überschreibungen durch zu große Schreibzugriffe |
| **Debugging** | Hilft bei der Fehlersuche (wo wurde korrupt?) |
| **Datenintegrität** | Zusätzliche Sicherheitsschicht |
| **Geringer Overhead** | Nur 1 Byte pro Block + Berechnungszeit |
| **Einfach** | Additive Checksumme ist leicht zu implementieren |

### Grenzen der Implementierung

| Limitation | Beschreibung |
|------------|--------------|
| **Nicht kryptografisch** | Erkennt keine böswilligen Manipulationen |
| **Einfache Kollisionen** | Vertauschungen werden nicht erkannt |
| **Nur Header** | Nutzdaten werden nicht geprüft |
| **Overhead** | Bei jedem Zugriff Checksumme berechnen/prüfen |

---

## Aufgabe 3: Multitasked Producer/Consumer Blink

### Konzeptänderungen gegenüber Übung 5

**Übung 5**:
- Globale Variable `blink_delay_ms`
- Semaphor `delay_semaphore` zum Schutz
- Alle Tasks lesen/schreiben dieselbe Variable

**Übung 6**:
- **Lokale** Variablen in jedem LED-Task
- Kein `delay_semaphore` mehr
- Event-basierte Kommunikation vom Button-Task zu LED-Tasks
- 3 verschiedene Implementierungsvarianten

### Variante 1: Producer/Consumer mit Semaphor

Datei: `blink_variant1_semaphore.c`

#### Architektur

```
Button Task (Producer)          LED Tasks (Consumer 0, 1, 2)
      |                                  |
      |  xSemaphoreGive() × 3           |
      +-------------------------------->|
                Counting Semaphore       |
                  (max=10)               |
                                         v
                              xSemaphoreTake() non-blocking
                              Ändere lokale Delay-Variable
```

#### Semaphor-Konfiguration
```c
// Counting Semaphore: Maximal 10 Events, initial 0
event_semaphore = xSemaphoreCreateCounting(10, 0);
```

**Eigenschaften**:
- Zählt Events (nicht binär)
- Initial: 0 (keine Events)
- Maximum: 10 (bis zu 10 Events können "aufgestaut" werden)

#### Producer (Button Task)
```c
static void button_task(void *pvParameters)
{
    while (1) {
        // Taster lesen
        if (left_state == 0 && last_left_state == 1) {
            ESP_LOGI(TAG, "Producer: Left button pressed - sending events");
            
            // Sende Event an ALLE 3 Consumer
            xSemaphoreGive(event_semaphore);
            xSemaphoreGive(event_semaphore);
            xSemaphoreGive(event_semaphore);
        }
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
```

**Funktionsweise**:
- Bei Tastendruck: 3× `xSemaphoreGive()`
- Jeder Call erhöht Semaphor-Zähler um 1
- Consumer können Events "abholen"

#### Consumer (LED Tasks)
```c
static void led_task_0(void *pvParameters)
{
    int local_delay = 1000;  // Lokale Variable!
    
    while (1) {
        // Versuche Event zu empfangen (non-blocking)
        if (xSemaphoreTake(event_semaphore, 0) == pdTRUE) {
            // Event empfangen - ändere Geschwindigkeit
            local_delay = (local_delay == 1000) ? 500 : 1000;
            ESP_LOGI(TAG, "Task 0: Delay changed to %d ms", local_delay);
        }
        
        // LED blinken mit lokalem Delay
        if (state) {
            set_led_pixel(0, 16, 0, 0);
        } else {
            clear_led_pixel(0);
        }
        
        state = !state;
        vTaskDelay(local_delay / portTICK_PERIOD_MS);
    }
}
```

**Funktionsweise**:
- Jeder Task hat eigene `local_delay` Variable
- `xSemaphoreTake()` mit Timeout=0 (non-blocking)
- Wenn Event da: Delay ändern
- Wenn kein Event: Weiterblinken

#### Vorteile
- ✅ Einfaches Konzept
- ✅ Mehrere Consumer möglich
- ✅ Events können "gepuffert" werden (bis max=10)
- ✅ Non-blocking für Consumer

#### Nachteile
- ❌ Keine Typ-Information (nur "ein Event ist da")
- ❌ Keine Parameter (welcher Button?)
- ❌ Producer muss wissen wie viele Consumer es gibt (3× Give)

---

### Variante 2: Event Notification

Datei: `blink_variant2_notify.c`

#### Architektur

```
Button Task                     LED Tasks (0, 1, 2)
      |                              |
      |  xTaskNotify()              |
      +----------------------------->|
      |  (Event-Typ als Value)      |
      |                              v
      |                   xTaskNotifyWait()
      |                   Lese Event-Typ
      |                   Ändere Delay entsprechend
```

#### Event-Typen als Bit-Flags
```c
#define EVENT_SPEED_UP    0x01
#define EVENT_SLOW_DOWN   0x02
```

#### Task Handles speichern
```c
static TaskHandle_t led_task_0_handle = NULL;
static TaskHandle_t led_task_1_handle = NULL;
static TaskHandle_t led_task_2_handle = NULL;

// Bei Task-Erstellung:
xTaskCreate(led_task_0, "LED_Task_0", 2048, NULL, 5, &led_task_0_handle);
```

#### Producer (Button Task)
```c
static void button_task(void *pvParameters)
{
    while (1) {
        // Linker Taster: Verlangsamen
        if (left_state == 0 && last_left_state == 1) {
            ESP_LOGI(TAG, "Button: Sending SLOW_DOWN notification");
            xTaskNotify(led_task_0_handle, EVENT_SLOW_DOWN, eSetBits);
            xTaskNotify(led_task_1_handle, EVENT_SLOW_DOWN, eSetBits);
            xTaskNotify(led_task_2_handle, EVENT_SLOW_DOWN, eSetBits);
        }
        
        // Rechter Taster: Beschleunigen
        if (right_state == 0 && last_right_state == 1) {
            ESP_LOGI(TAG, "Button: Sending SPEED_UP notification");
            xTaskNotify(led_task_0_handle, EVENT_SPEED_UP, eSetBits);
            xTaskNotify(led_task_1_handle, EVENT_SPEED_UP, eSetBits);
            xTaskNotify(led_task_2_handle, EVENT_SPEED_UP, eSetBits);
        }
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
```

**xTaskNotify() Parameter**:
- `xTaskHandle`: Ziel-Task
- `ulValue`: Event-Typ (als Bit-Maske)
- `eAction`: `eSetBits` = Bits werden ODER-verknüpft

#### Consumer (LED Tasks)
```c
static void led_task_0(void *pvParameters)
{
    int local_delay = 1000;
    
    while (1) {
        uint32_t notification_value;
        
        // Warte auf Notification (non-blocking)
        if (xTaskNotifyWait(0x00, 0xFFFFFFFF, &notification_value, 0) == pdTRUE) {
            // Notification empfangen
            if (notification_value & EVENT_SPEED_UP) {
                if (local_delay > 100) {
                    local_delay -= 100;
                    ESP_LOGI(TAG, "Task 0: Speed up -> %d ms", local_delay);
                }
            }
            if (notification_value & EVENT_SLOW_DOWN) {
                if (local_delay < 5000) {
                    local_delay += 100;
                    ESP_LOGI(TAG, "Task 0: Slow down -> %d ms", local_delay);
                }
            }
        }
        
        // LED blinken
        // ... (wie Variante 1)
        
        vTaskDelay(local_delay / portTICK_PERIOD_MS);
    }
}
```

**xTaskNotifyWait() Parameter**:
- `ulBitsToClearOnEntry`: Bits vor Warten löschen (0x00 = keine)
- `ulBitsToClearOnExit`: Bits nach Empfang löschen (0xFFFFFFFF = alle)
- `pulNotificationValue`: Output - empfangener Wert
- `xTicksToWait`: Timeout (0 = non-blocking)

#### Vorteile
- ✅ **Typ-Information**: Event-Typ wird mitgesendet
- ✅ **Effizient**: Direkter Task-zu-Task, kein Objekt dazwischen
- ✅ **Schnell**: Schneller als Queues oder Semaphore
- ✅ **Flexibel**: Bis zu 32 Bit Daten

#### Nachteile
- ❌ Task Handles müssen bekannt sein
- ❌ Nur 1 Notification pro Task gleichzeitig (überschreiben möglich)
- ❌ Weniger Pufferung als Queues

#### Anwendungsfall
- Ideal für einfache Event-Benachrichtigungen
- Gut wenn wenige Events gleichzeitig kommen
- Perfekt für "Display neu zeichnen", "Button gedrückt", etc.

---

### Variante 3: Queue mit KeyEvent

Datei: `blink_variant3_queue.c`

#### Architektur

```
Button Task                     LED Tasks (0, 1, 2)
      |                              |
      |  xQueueSend(KeyEvent)       |
      +----------------------------->|
      |  (Struct mit Typ, Timestamp)|
      |                              v
      |                   xQueueReceive(KeyEvent)
      |                   Verarbeite Event-Daten
      |                   Ändere Delay
                          
         Queue (FIFO)
    [Event1][Event2][Event3]...
```

#### KeyEvent Struktur
```c
typedef enum {
    KEY_EVENT_LEFT_PRESSED,
    KEY_EVENT_RIGHT_PRESSED,
    KEY_EVENT_LEFT_RELEASED,
    KEY_EVENT_RIGHT_RELEASED
} KeyEventType;

typedef struct {
    KeyEventType type;
    uint32_t timestamp;      // Zeit des Events (ms)
    uint8_t button_id;       // 0=links, 1=rechts
} KeyEvent;
```

**Vorteile der Struktur**:
- Typsicher (Compiler prüft)
- Erweiterbar (weitere Felder hinzufügen)
- Selbstdokumentierend
- Enthält alle relevanten Informationen

#### Queue Erstellung
```c
#define QUEUE_SIZE 10
key_event_queue = xQueueCreate(QUEUE_SIZE, sizeof(KeyEvent));
```

**Parameter**:
- `uxQueueLength`: Maximale Anzahl Elemente (10)
- `uxItemSize`: Größe eines Elements (sizeof(KeyEvent))

#### Producer (Button Task)
```c
static void button_task(void *pvParameters)
{
    while (1) {
        uint8_t left_state = gpio_get_level(BUTTON_LEFT_GPIO);
        
        // Linker Taster gedrückt
        if (left_state == 0 && last_left_state == 1) {
            KeyEvent event = {
                .type = KEY_EVENT_LEFT_PRESSED,
                .timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS,
                .button_id = 0
            };
            
            ESP_LOGI(TAG, "Button: LEFT pressed - sending to queue");
            
            // Sende an alle 3 Consumer (3× in Queue)
            for (int i = 0; i < 3; i++) {
                if (xQueueSend(key_event_queue, &event, 0) != pdTRUE) {
                    ESP_LOGW(TAG, "Queue full! Event dropped.");
                }
            }
        }
        
        // Linker Taster losgelassen
        if (left_state == 1 && last_left_state == 0) {
            KeyEvent event = {
                .type = KEY_EVENT_LEFT_RELEASED,
                .timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS,
                .button_id = 0
            };
            xQueueSend(key_event_queue, &event, 0);
        }
        
        // Analog für rechten Taster...
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
```

**xQueueSend() Parameter**:
- `xQueue`: Queue Handle
- `pvItemToQueue`: Pointer auf zu sendende Daten (wird kopiert!)
- `xTicksToWait`: Timeout (0 = non-blocking)

#### Consumer (LED Tasks)
```c
static void led_task_0(void *pvParameters)
{
    int local_delay = 1000;
    
    while (1) {
        KeyEvent event;
        
        // Versuche Event aus Queue zu lesen
        if (xQueueReceive(key_event_queue, &event, 0) == pdTRUE) {
            ESP_LOGI(TAG, "Task 0: Received %s event (button %d) at %lu ms",
                     event_type_to_string(event.type), 
                     event.button_id, 
                     event.timestamp);
            
            // Reagiere nur auf PRESSED Events
            if (event.type == KEY_EVENT_LEFT_PRESSED) {
                // Verlangsamen
                if (local_delay < 5000) {
                    local_delay += 100;
                    ESP_LOGI(TAG, "Task 0: Slow down -> %d ms", local_delay);
                }
            } else if (event.type == KEY_EVENT_RIGHT_PRESSED) {
                // Beschleunigen
                if (local_delay > 100) {
                    local_delay -= 100;
                    ESP_LOGI(TAG, "Task 0: Speed up -> %d ms", local_delay);
                }
            }
        }
        
        // LED blinken
        // ... (wie Variante 1)
        
        vTaskDelay(local_delay / portTICK_PERIOD_MS);
    }
}
```

**xQueueReceive() Parameter**:
- `xQueue`: Queue Handle
- `pvBuffer`: Pointer wohin Daten geschrieben werden
- `xTicksToWait`: Timeout (0 = non-blocking)

#### Hilfsfunktion für Logging
```c
static const char* event_type_to_string(KeyEventType type)
{
    switch (type) {
        case KEY_EVENT_LEFT_PRESSED: return "LEFT_PRESSED";
        case KEY_EVENT_RIGHT_PRESSED: return "RIGHT_PRESSED";
        case KEY_EVENT_LEFT_RELEASED: return "LEFT_RELEASED";
        case KEY_EVENT_RIGHT_RELEASED: return "RIGHT_RELEASED";
        default: return "UNKNOWN";
    }
}
```

#### Vorteile
- ✅ **Typsicher**: Compiler-geprüfte Strukturen
- ✅ **Flexibel**: Beliebige Datenstrukturen möglich
- ✅ **FIFO-Garantie**: Events kommen in Reihenfolge an
- ✅ **Pufferung**: Bis zu QUEUE_SIZE Events gepuffert
- ✅ **Erweiterbar**: Struktur kann erweitert werden
- ✅ **Selbstdokumentierend**: Event-Typen als Enum
- ✅ **Zusätzliche Daten**: Timestamp, Button-ID, etc.

#### Nachteile
- ❌ Speicher-Overhead (Queue + kopierte Strukturen)
- ❌ Etwas langsamer als Task Notifications
- ❌ Queue kann voll werden (Events werden verworfen)

#### Anwendungsfall
- **Ideal** für komplexe Event-Systeme
- Gut wenn Event-Daten wichtig sind
- Perfekt wenn FIFO-Reihenfolge wichtig ist
- Standard-Lösung in professionellem Code

---

## Vergleich der drei Varianten

| Aspekt | Variante 1: Semaphore | Variante 2: Notify | Variante 3: Queue |
|--------|-----------------------|--------------------|-------------------|
| **Komplexität** | Einfach | Mittel | Höher |
| **Typ-Information** | Nein | Ja (32 Bit) | Ja (Struktur) |
| **Daten-Menge** | Keine | Bis 32 Bit | Beliebig |
| **Pufferung** | Bis max (10) | 1 pro Task | QUEUE_SIZE (10) |
| **Geschwindigkeit** | Schnell | Sehr schnell | Mittel |
| **Speicher** | Minimal | Minimal | Moderat |
| **FIFO-Garantie** | Nein | Nein | Ja |
| **Mehrere Consumer** | Ja | Ja (broadcast) | Ja |
| **Empfehlung** | Einfache Events | Single Events | Komplexe Daten |

### Wann welche Variante?

#### Variante 1 (Semaphore)
- Nur "Event ist da" Information nötig
- Sehr einfacher Code gewünscht
- Mehrere Consumer sollen benachrichtigt werden
- Keine Daten müssen übertragen werden

**Beispiel**: "Neuzeichnen!", "Aufwachen!", "Start!"

#### Variante 2 (Task Notify)
- Schnelle Reaktion wichtig
- Bis zu 32 Bit Daten
- Direkte Task-zu-Task Kommunikation
- Nicht viele Events gleichzeitig

**Beispiel**: "Button 3 gedrückt", "Sensor hat Wert 42", "Mode = 5"

#### Variante 3 (Queue)
- Komplexe Daten übertragen
- Reihenfolge wichtig (FIFO)
- Events müssen gepuffert werden
- Professioneller, wartbarer Code

**Beispiel**: "KeyEvent mit Timestamp", "Sensor-Daten-Paket", "Command mit Parametern"

---

## Testdurchführung und Ergebnisse

### Konfiguration in blink_example_main.c

```c
/* Setzen Sie genau EINE der folgenden Defines auf 1: */
#define RUN_ORIGINAL_VERSION 0           // Übung 5
#define RUN_SEMAPHORE_BLOCKING_TEST 0    // Aufgabe 1
#define RUN_VARIANT_1_SEMAPHORE 0        // Aufgabe 3.1
#define RUN_VARIANT_2_NOTIFY 0           // Aufgabe 3.2
#define RUN_VARIANT_3_QUEUE 1            // Aufgabe 3.3 (DEFAULT)
```

### Build und Flash

```bash
# Projekt bauen
idf.py build

# Flashen und Monitor starten
idf.py flash monitor

# Zum Beenden: Ctrl+]
```

### Erwartete Ausgabe

#### Memory Manager Tests (alle Varianten)
```
========================================
  Memory Manager Test Suite
========================================

--- Test 1: Memory Initialization ---
  Memory initialized
=== Memory Dump ===
...

--- Test 15: Checksum Integrity Check ---
  [PASS] Allocation succeeded
SUCCESS: All 2 blocks have valid checksums!

--- Test 16: Buffer Overflow Detection ---
  [PASS] Allocation succeeded
  [PASS] Heap OK before overflow
  Simulating buffer overflow...
CORRUPTION DETECTED at Block 1 (Offset 24):
  Expected checksum: 0x2A
  Actual checksum:   0x58
  [PASS] Buffer overflow detected by checksum

========================================
  Test Summary
========================================
Total Tests: 17
Assertions Passed: 45
Assertions Failed: 0
========================================
All tests PASSED!
```

#### Aufgabe 1: Semaphore Blocking Test
```
========================================
  Semaphore Blocking Test Suite
========================================

=== Test 1: Eternal Blocking ===
Blocking Task (eternal) started
Blocking Task acquired semaphore - will NEVER release it
Waiting Task (eternal) started - will wait FOREVER
Trying to acquire semaphore with portMAX_DELAY...
... (Task bleibt hängen)
After 5 seconds: Waiter task is still blocked!

=== Test 2: Timeout Behavior ===
Blocking Task started
Waiting Task (timeout) started - will wait max 3 seconds
Trying to acquire semaphore with 3000ms timeout...
TIMEOUT: Could not acquire semaphore after 3001 ms
Task can continue with alternative action or error handling
```

#### Aufgabe 3, Variante 1: Producer/Consumer
```
========================================
  Variant 1: Producer/Consumer with Semaphore
========================================

LED Task 0 (Consumer) started
LED Task 1 (Consumer) started
LED Task 2 (Consumer) started
Button Task (Producer) started

Producer: Left button pressed - sending events to all consumers
Task 0: Delay changed to 500 ms
Task 1: Delay changed to 500 ms
Task 2: Delay changed to 500 ms
```

#### Aufgabe 3, Variante 2: Event Notification
```
========================================
  Variant 2: Event Notification
========================================

LED Task 0 started (waiting for notifications)
LED Task 1 started (waiting for notifications)
LED Task 2 started (waiting for notifications)
Button Task started (will send notifications)

Button: Sending SLOW_DOWN notification to all tasks
Task 0: Slow down -> delay = 1100 ms
Task 1: Slow down -> delay = 1100 ms
Task 2: Slow down -> delay = 1100 ms

Button: Sending SPEED_UP notification to all tasks
Task 0: Speed up -> delay = 1000 ms
Task 1: Speed up -> delay = 1000 ms
Task 2: Speed up -> delay = 1000 ms
```

#### Aufgabe 3, Variante 3: Queue
```
========================================
  Variant 3: Queue with KeyEvent
========================================

LED Task 0 started (reading from queue)
LED Task 1 started (reading from queue)
LED Task 2 started (reading from queue)
Button Task started (will send events to queue)

Button: LEFT pressed - sending to queue
Task 0: Received LEFT_PRESSED event (button 0) at 5234 ms
Task 0: Slow down -> delay = 1100 ms
Task 1: Received LEFT_PRESSED event (button 0) at 5234 ms
Task 1: Slow down -> delay = 1100 ms
Task 2: Received LEFT_PRESSED event (button 0) at 5234 ms
Task 2: Slow down -> delay = 1100 ms

Button: LEFT released
Button: RIGHT pressed - sending to queue
Task 0: Received RIGHT_PRESSED event (button 1) at 6891 ms
Task 0: Speed up -> delay = 1000 ms
...
```

---

## Dateien im Projekt

### Neue Dateien (Übung 6)

| Datei | Beschreibung |
|-------|--------------|
| `blink_semaphore_blocking_test.c/h` | Aufgabe 1: Semaphor-Blocking-Tests |
| `blink_variant1_semaphore.c` | Aufgabe 3.1: Producer/Consumer mit Semaphor |
| `blink_variant2_notify.c` | Aufgabe 3.2: Task Notification Variante |
| `blink_variant3_queue.c` | Aufgabe 3.3: Queue mit KeyEvent |
| `blink_variants.h` | Header für alle drei Varianten |
| `LOESUNGSIDEE_UEBUNG6.md` | Diese Dokumentation |

### Geänderte Dateien

| Datei | Änderung |
|-------|----------|
| `mem_manager.h` | Header mit Bitfields und Checksumme |
| `mem_manager.c` | Checksummen-Funktionen, checkHeap() |
| `mem_manager_test.c` | Neue Tests für Buffer Overflow Detection |
| `blink_example_main.c` | Auswahl zwischen Varianten |

---

## Lessons Learned / Erkenntnisse

### Aufgabe 1: Semaphore
1. **Immer Timeouts verwenden**: portMAX_DELAY kann zu Deadlocks führen
2. **Fehlerbehandlung**: Bei Timeout alternative Aktionen einplanen
3. **Debugging**: Logging hilft beim Erkennen von Blockierungen
4. **Best Practice**: RAII-Pattern (immer im finally freigeben)

### Aufgabe 2: Checksummen
1. **Trade-off**: Sicherheit vs. Performance (Checksumme kostet Zeit)
2. **Früherkennung**: Buffer Overflows werden sofort erkannt
3. **Debugging-Hilfe**: Korrupte Blöcke zeigen Fehlerquelle
4. **Grenzen**: Nur einfache Fehler werden erkannt

### Aufgabe 3: Task-Kommunikation
1. **Keine One-Size-Fits-All**: Verschiedene Probleme brauchen verschiedene Lösungen
2. **Simplicity First**: Einfachste Lösung die funktioniert wählen
3. **Scalability**: Queues skalieren besser für komplexe Systeme
4. **Performance**: Task Notifications sind am schnellsten
5. **Maintainability**: Queues mit Strukturen sind am wartbarsten

### FreeRTOS Best Practices
1. **Non-blocking bevorzugen**: Timeout=0 wo möglich
2. **Lokale Variablen**: Keine globalen Variablen wenn möglich
3. **Task Prioritäten**: Button-Task höher als LED-Tasks
4. **Stack Size**: 2048 Bytes reichen für diese Tasks
5. **Error Handling**: Immer Return-Werte prüfen

---

## Mögliche Erweiterungen

### Aufgabe 1
- Watchdog-Timer implementieren
- Deadlock-Detection-System
- Automatische Recovery-Mechanismen

### Aufgabe 2
- CRC32 statt additive Checksumme
- Checksumme über Nutzdaten
- Memory Scrubbing (regelmäßige Prüfung)
- Korrektur-Mechanismen

### Aufgabe 3
- Hybrid-Ansatz: Beste Aspekte kombinieren
- Priority-Queue für Events
- Event-Filter (Tasks nur für bestimmte Events)
- Statistiken (Events pro Sekunde, etc.)
- Web-Interface zur Steuerung

---

## Anhang: Verwendete FreeRTOS APIs

### Semaphore
```c
xSemaphoreCreateMutex()           // Binary Semaphore erstellen
xSemaphoreCreateCounting(max, 0)  // Counting Semaphore erstellen
xSemaphoreTake(sem, timeout)      // Semaphor nehmen
xSemaphoreGive(sem)                // Semaphor freigeben
```

### Task Notifications
```c
xTaskNotify(handle, value, action) // Notification senden
xTaskNotifyWait(clearEntry, clearExit, &value, timeout) // Warten auf Notification
```

### Queues
```c
xQueueCreate(size, itemSize)      // Queue erstellen
xQueueSend(queue, &item, timeout) // Item in Queue senden
xQueueReceive(queue, &item, timeout) // Item aus Queue empfangen
```

### Tasks
```c
xTaskCreate(function, name, stackSize, param, priority, &handle) // Task erstellen
vTaskDelay(ticks)                  // Task schlafen legen
xTaskGetTickCount()                // Aktuelle Tick-Count
```

### Time
```c
portTICK_PERIOD_MS                 // Millisekunden pro Tick (meist 10ms)
portMAX_DELAY                      // Unendlicher Timeout
```

---

**Erstellt für Übung 6, Systemprogrammierung**  
**Datum: Dezember 2024**
