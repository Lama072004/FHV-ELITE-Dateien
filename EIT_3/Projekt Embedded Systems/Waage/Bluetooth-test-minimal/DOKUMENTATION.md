# Waage Projekt - Dokumentation

## Projektübersicht

Dieses Projekt implementiert eine digitale Waage mit Bluetooth-Kommunikation auf einem ESP32-Mikrocontroller (Board: FACD0008). Die Waage liest ADC-Werte von einem Wägesensor, verarbeitet diese mit einem EMA-Filter und sendet die Gewichtsdaten über Bluetooth.

---

## Hardware-Konfiguration

### Verwendete Pins

| Pin | Funktion | Beschreibung |
|-----|----------|--------------|
| GPIO4 | ADC Input | Analoger Eingang für Wägesensor (ADC1 Channel 3) |
| GPIO42 | TARE Button | Taster zum Nullen der Waage (Pull-up aktiviert) |

### ADC-Spezifikationen

- **ADC Unit**: ADC_UNIT_1 (kann parallel zu WiFi verwendet werden)
- **ADC Channel**: Channel 3 (GPIO4)
- **Bitwidth**: 12-bit (0-4095)
- **Attenuation**: 11 dB (0-3.3V Messbereich)

---

## Software-Architektur

### Dateistruktur

```
main/
├── src/
│   ├── main.c          # Hauptprogramm mit ADC und Filterung
│   ├── Bluetooth.c     # Bluetooth-Kommunikation
│   └── Bluetooth.h     # Bluetooth Header
└── ...
```

---

## main.c - Detaillierte Beschreibung

### Konfigurationskonstanten

```c
#define ADC_CHANNEL ADC_CHANNEL_3  // GPIO4
#define ADC_UNIT    ADC_UNIT_1
#define TARE_PIN    GPIO_NUM_42
```

### Kalibrierungswerte

```c
#define ADC_ZERO 1        // ADC-Wert bei 0g
#define ADC_MAX  5000     // ADC-Wert bei 5000g
#define WEIGHT_MAX 5000.0 // Maximales Gewicht in Gramm
```

**Erklärung**: Die Waage wird linear zwischen zwei Punkten kalibriert:
- Bei 0g liefert der Sensor ADC-Wert 1
- Bei 5000g liefert der Sensor ADC-Wert 5000

### EMA-Filter (Exponential Moving Average)

```c
#define EMA_ALPHA 0.01  // Glättungsfaktor
```

**Funktionsweise**:
- Der EMA-Filter glättet verrauschte ADC-Werte
- Formel: `EMA_neu = α × ADC_roh + (1-α) × EMA_alt`
- **α = 0.01**: Stark geglättet (langsame Reaktion, wenig Rauschen)
- **α = 0.1**: Weniger geglättet (schnelle Reaktion, mehr Rauschen)

**Vorteile**:
- Einfache Implementierung
- Geringer Speicherbedarf (nur ein Wert)
- Effektive Rauschunterdrückung

### Funktionen

#### `init_adc()`

```c
void init_adc(adc_oneshot_unit_handle_t *adc_unit)
```

**Zweck**: Initialisiert den ADC-Kanal

**Ablauf**:
1. Erstellt ADC Unit 1
2. Konfiguriert Channel 3 (GPIO4)
3. Setzt 12-bit Auflösung
4. Setzt 11dB Dämpfung (0-3.3V Bereich)

---

#### `init_gpio()`

```c
void init_gpio(void)
```

**Zweck**: Konfiguriert den TARE-Button

**Konfiguration**:
- GPIO42 als Eingang
- Pull-up Widerstand aktiviert
- Button gegen GND geschaltet (aktiv LOW)

---

#### `adc_to_grams()`

```c
double adc_to_grams(int adc_val)
```

**Zweck**: Konvertiert gefilterten ADC-Wert in Gramm

**Ablauf**:
1. Subtrahiert Tare-Offset (Nullpunkt)
2. Begrenzt Wert auf gültigen Bereich
3. Lineare Umrechnung: `Gewicht = (ADC - ZERO) × WEIGHT_MAX / (MAX - ZERO)`

**Beispiel**:
- ADC = 2500, Tare = 0
- Gewicht = 2499 × 5000 / 4999 ≈ 2500g

---

### Hauptprogramm (`app_main()`)

#### Initialisierung

```c
bluetooth_init();           // Startet Bluetooth
adc_oneshot_unit_handle_t adc1;
init_adc(&adc1);           // Initialisiert ADC
init_gpio();               // Initialisiert GPIO

// EMA mit erstem ADC-Wert initialisieren
adc_oneshot_read(adc1, ADC_CHANNEL, &adc_raw);
double ema = (double)adc_raw;
```

**Wichtig**: Der EMA-Filter wird mit dem ersten gemessenen Wert initialisiert, um Einschwingvorgänge zu vermeiden.

---

#### Haupt-Loop (50 Hz)

```c
for(;;){
    // 1. ADC lesen
    adc_oneshot_read(adc1, ADC_CHANNEL, &adc_raw);
    
    // 2. EMA-Filter anwenden
    ema = EMA_ALPHA * (double)adc_raw + (1.0 - EMA_ALPHA) * ema;
    int adc_filtered = (int)(ema + 0.5);
    
    // 3. Tare-Button prüfen
    if(gpio_get_level(TARE_PIN) == 0){
        tare_offset = adc_filtered;
    }
    
    // 4. Gewicht berechnen
    double weight = adc_to_grams(adc_filtered);
    
    // 5. Logging (jeden 10. Wert)
    if(log_counter >= 10){
        ESP_LOGI(TAG,"ADC roh=%d, gefiltert=%d, Gewicht=%.2f g", ...);
    }
    
    // 6. Bluetooth senden
    bluetooth_send_int((int32_t)(weight*100));
    
    // 7. 20ms warten (50 Hz)
    vTaskDelay(pdMS_TO_TICKS(20));
}
```

**Ablauf erklärt**:

1. **ADC lesen**: Rohdaten vom Sensor einlesen
2. **Filtern**: EMA-Algorithmus anwenden für stabile Werte
3. **Tare**: Bei Tastendruck aktuellen Wert als Nullpunkt setzen
4. **Umrechnen**: ADC-Wert in Gramm konvertieren
5. **Loggen**: Nur jeden 10. Durchlauf loggen (Konsolenausgabe reduzieren)
6. **Senden**: Gewicht × 100 über Bluetooth (2 Dezimalstellen als Integer)
7. **Timing**: 20ms Verzögerung = 50 Messungen pro Sekunde

---

## Bluetooth.c - Detaillierte Beschreibung

### Bluetooth-Architektur

Das Projekt verwendet **ESP32 Classic Bluetooth** im Serial Port Profile (SPP) Modus.

### Konfigurationskonstanten

```c
#define SPP_SERVER_NAME "ESP32_SPP_SERVER"
#define DEVICE_NAME "ESP32_Waage"
```

---

### GAP Event Handler

```c
static void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, 
                          esp_bt_gap_cb_param_t *param)
```

**Zweck**: Verarbeitet Bluetooth GAP (Generic Access Profile) Events

**Wichtige Events**:

#### `ESP_BT_GAP_AUTH_CMPL_EVT`
- Wird ausgelöst nach Pairing-Prozess
- Prüft ob Authentifizierung erfolgreich war
- Loggt Geräte-Adresse des verbundenen Clients

**Beispiel-Log**:
```
authentication success: ESP32_Waage
```

---

### SPP Event Handler

```c
static void esp_spp_cb(esp_spp_cb_event_t event, 
                       esp_spp_cb_param_t *param)
```

**Zweck**: Verarbeitet SPP (Serial Port Profile) Events

#### Wichtige Events:

##### `ESP_SPP_INIT_EVT`
```c
esp_spp_start_srv(sec_mask, role_slave, slot, SPP_SERVER_NAME);
```
- Startet SPP-Server nach Initialisierung
- Wartet auf Client-Verbindungen

##### `ESP_SPP_SRV_OPEN_EVT`
```c
connection_handle = param->srv_open.handle;
ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_OPEN_EVT");
```
- Wird bei erfolgreicher Verbindung ausgelöst
- Speichert Connection Handle für späteres Senden

##### `ESP_SPP_CLOSE_EVT`
```c
connection_handle = 0;
ESP_LOGI(SPP_TAG, "ESP_SPP_CLOSE_EVT");
```
- Wird bei Verbindungsabbruch ausgelöst
- Setzt Handle zurück (keine Verbindung)

##### `ESP_SPP_WRITE_EVT`
- Bestätigt erfolgreiches Senden von Daten
- Loggt Anzahl gesendeter Bytes

##### `ESP_SPP_DATA_IND_EVT`
- Empfängt Daten vom Client
- Aktuell nur Logging implementiert

---

### Initialisierung (`bluetooth_init()`)

```c
void bluetooth_init(void)
```

**Ablauf**:

1. **NVS initialisieren**
   ```c
   nvs_flash_init();
   ```
   - Non-Volatile Storage für Bluetooth-Konfiguration

2. **Controller konfigurieren**
   ```c
   esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
   esp_bt_controller_init(&bt_cfg);
   esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
   ```
   - Initialisiert Bluetooth Controller
   - Aktiviert Classic Bluetooth (nicht BLE)

3. **Bluedroid Stack starten**
   ```c
   esp_bluedroid_init();
   esp_bluedroid_enable();
   ```
   - Startet Bluetooth-Stack

4. **Callbacks registrieren**
   ```c
   esp_bt_gap_register_callback(esp_bt_gap_cb);
   esp_spp_register_callback(esp_spp_cb);
   ```
   - Verknüpft Event Handler

5. **SPP initialisieren**
   ```c
   esp_spp_init(esp_spp_mode);
   ```
   - Startet Serial Port Profile

6. **Gerät sichtbar machen**
   ```c
   esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
   esp_bt_dev_set_device_name(DEVICE_NAME);
   ```
   - Gerät erscheint als "ESP32_Waage" in Bluetooth-Suche

---

### Daten senden (`bluetooth_send_int()`)

```c
void bluetooth_send_int(int32_t value)
```

**Zweck**: Sendet Integer-Wert über Bluetooth (wenn verbunden)

**Ablauf**:
```c
if(connection_handle != 0){
    char buffer[16];
    int len = snprintf(buffer, sizeof(buffer), "%ld\n", (long)value);
    esp_spp_write(connection_handle, len, (uint8_t*)buffer);
}
```

**Format**:
- Integer als ASCII-String
- Mit Newline (`\n`) abgeschlossen
- Beispiel: `250000\n` (entspricht 2500.00g)

**Sicherheit**:
- Prüft `connection_handle != 0` (Verbindung aktiv?)
- Sendet nur wenn Client verbunden ist

---

## Datenfluss

```
Wägesensor (analog)
    ↓
GPIO4 / ADC1 Channel 3
    ↓
ADC Rohwert (12-bit: 0-4095)
    ↓
EMA-Filter (α=0.01)
    ↓
Gefilterter ADC-Wert
    ↓
Tare-Korrektur (Offset subtrahieren)
    ↓
Lineare Umrechnung (ADC → Gramm)
    ↓
Gewicht in Gramm
    ↓
× 100 (2 Dezimalstellen als Integer)
    ↓
Bluetooth SPP (ASCII String)
    ↓
Client-Gerät (z.B. Smartphone/PC)
```

---

## Timing-Diagramm

```
Zeit:    0ms   20ms   40ms   60ms   80ms   100ms
         |     |      |      |      |      |
ADC:     ●─────●──────●──────●──────●──────●
Filter:  ●─────●──────●──────●──────●──────●
BT:      ●─────●──────●──────●──────●──────●
Log:     ●─────────────────────────●

Legende:
● = Aktion ausgeführt
─ = Warten
```

- **Sampling-Rate**: 50 Hz (alle 20ms)
- **Logging-Rate**: 5 Hz (jeder 10. Wert)
- **Bluetooth-Rate**: 50 Hz (mit jedem Sample)

---

## Verwendung

### Verbindung herstellen

1. ESP32 einschalten
2. Bluetooth auf Client-Gerät aktivieren
3. Nach "ESP32_Waage" suchen
4. Verbindung herstellen
5. SPP-Port öffnen

### Kalibrierung

1. **Nullpunkt setzen**: Waage leer, TARE-Button drücken
2. **Bekanntes Gewicht**: 5kg auflegen
3. **ADC-Wert ablesen**: Im Log nachsehen
4. **Konstanten anpassen**: `ADC_MAX` im Code aktualisieren
5. **Neu kompilieren**: Code flashen

### Datenformat empfangen

Empfangene Daten sind ASCII-Strings:
```
250000\n    → 2500.00g
100050\n    → 1000.50g
5\n         → 0.05g
```

Umrechnung: `Gewicht_g = empfangener_Wert / 100.0`

---

## Fehlerbehebung

### Problem: Keine Bluetooth-Verbindung

**Lösung**:
- NVS Flash löschen: `idf.py erase-flash`
- Bluetooth-Cache auf Client löschen
- Gerät neu pairen

### Problem: Instabile Werte

**Lösung**:
- EMA_ALPHA verkleinern (z.B. 0.005)
- Elektrische Störquellen entfernen
- ADC-Referenzspannung prüfen

### Problem: Falsche Gewichtswerte

**Lösung**:
- Kalibrierung überprüfen
- `ADC_ZERO` und `ADC_MAX` anpassen
- Tare-Funktion verwenden

---

## Erweiterungsmöglichkeiten

### 1. Mehrere Gewichtseinheiten
```c
enum unit { GRAM, KILOGRAM, POUND };
double convert_weight(double grams, enum unit target);
```

### 2. Autom. Tare bei Stabilität
```c
if(abs(weight_current - weight_previous) < 0.1) {
    // Gewicht stabil → Auto-Tare
}
```

### 3. Datenlogging auf SD-Karte
```c
void log_to_sd(double weight, uint32_t timestamp);
```

### 4. WiFi + MQTT
```c
mqtt_publish("waage/gewicht", weight_string);
```

---

## Abhängigkeiten

- **ESP-IDF**: v5.x
- **FreeRTOS**: Enthalten in ESP-IDF
- **Bluetooth Classic**: ESP32 Hardware

---

## Performance

- **CPU-Last**: ~5% (bei 240 MHz)
- **RAM-Nutzung**: ~15 KB (Bluetooth Stack)
- **Stromverbrauch**: ~80-100 mA (BT aktiv)

---

## Lizenz & Autor

**Projekt**: Digitale Waage mit Bluetooth  
**Autor**: Mathias Lampert  
**Hochschule**: FH ELITE-Semester, EIT_3  
**Jahr**: 2024

---

## Änderungshistorie

| Version | Datum | Änderung |
|---------|-------|----------|
| 1.0 | 2024 | Erste Version mit EMA-Filter |
| 1.1 | 2024 | Kalman-Filter entfernt, GPIO4 für ADC |

