# Übung 5 - Kurzzusammenfassung

## Implementierte Aufgaben

### ✅ Aufgabe 1: Dynamische Speicherverwaltung

**Dateien**: `mem_manager.h`, `mem_manager.c`, `mem_manager_test.h`, `mem_manager_test.c`

**Implementierte Funktionen**:
- `void mem_init(void)` - Initialisiert Speicher
- `void* mem_alloc(int size)` - Allokiert Block, gibt Pointer zurück (NULL bei Fehler)
- `void mem_free(void *pMem)` - Gibt Block frei
- `void mem_dump(void)` - Zeigt Speicherbelegung

**Besonderheiten**:
- Header: 4 Bytes (2B Flag + 2B Größe)
- Keine Bibliotheksfunktionen (außer printf in mem_dump)
- First-Fit Allokation mit Block-Splitting
- Automatisches Merge von freien Blöcken
- Initialisierung mit 'A' für Debugging
- 14 automatische Tests mit allen Fehlerfällen

### ✅ Aufgabe 2: Multitasked Blink

**Datei**: `blink_example_main.c`

**Implementierung**:
- 3 FreeRTOS Tasks für 3 LEDs (Rot, Grün, Blau)
- 1 Button-Task für Geschwindigkeitssteuerung
- **led_semaphore**: Schützt LED-Zugriff (set_led_pixel, clear_led_pixel)
- **delay_semaphore**: Schützt globale Delay-Variable (blink_delay_ms)
- Linker Taster: Verlangsamen (+100ms)
- Rechter Taster: Beschleunigen (-100ms)
- Bereich: 100ms - 5000ms

## Build und Test

```bash
# In das Projekt-Verzeichnis wechseln
cd "C:\Users\Mathias Lampert\Documents\Mathias Schule_Studium\FH\ELITE-Semester\EIT_3\Systemprogrammierung\Uebung\Uebung5\Uebung5"

# Bauen
idf.py build

# Flashen und Monitor
idf.py flash monitor
```

## Erwartetes Verhalten

1. Beim Start werden automatisch alle Memory Manager Tests ausgeführt
2. Danach starten die 4 Tasks:
   - LED 0 blinkt rot
   - LED 1 blinkt grün  
   - LED 2 blinkt blau
   - Button-Task wartet auf Tastendruck
3. Taster drücken ändert die Blink-Geschwindigkeit aller LEDs

## Dokumentation

Siehe `LOESUNGSIDEE.md` für detaillierte Erklärungen zu:
- Implementierungskonzepten
- Verwendeten Algorithmen
- Thread-Synchronisation
- Fehlerbehandlung
- Test-Strategie

---

**Hinweis**: GPIO-Pins für Buttons (GPIO 0 und 35) können je nach Hardware angepasst werden.
