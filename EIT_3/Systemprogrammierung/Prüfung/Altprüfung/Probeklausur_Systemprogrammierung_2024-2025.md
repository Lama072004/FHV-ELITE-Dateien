# Probeklausur Systemprogrammierung 2024/2025

Beachte Unterschied zwischen Pseudocode und C. Verwenden Sie für die Beantwortung möglichst den eingeplanten Platz.

---

## Aufgabe 1.) Datentypen (1 + 2 + 1 Punkte)

1.1 Weshalb wird empfohlen `stdint.h` bzw. `inttypes.h` zu inkludieren und diese Typen zu verwenden?

1.2 Was ist Padding? Unter welchen Umständen hat Padding in C einen Einfluss auf die Programmierung?

1.3 Was ist Alignment und wie hängt es mit Padding zusammen?

---

## Aufgabe 2.) Lieblingsfarbe (3 Punkte)

Definieren Sie eine RGB-Farbe (Komponenten sind 8-bittig) über eine `struct`.  
Definieren Sie auch eine `union`, um auf die Farbe als 32-Bit Wert (`raw`) zuzugreifen.

Schreiben Sie eine Funktion in C, welche sowohl den RGB- als auch den `raw`-Wert einer als Parameter übergebenen Farbe per `printf()` in der Konsole ausgibt.

Beispiel (als Hinweis, Pseudocode/C gemischt erlaubt):

```c
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a; // optional, z.B. für Alignment oder Alpha
} RGBColor;

typedef union {
    RGBColor color;
    uint32_t raw;
} ColorUnion;

void printColor(const ColorUnion *c) {
    printf("R=%u G=%u B=%u A=%u, raw=0x%08X\n",
           c->color.r, c->color.g, c->color.b, c->color.a, c->raw);
}
```

---

## Aufgabe 3.) Bitmaskierung (2 + 4 Punkte)

3.1 Erklären Sie die Unterschiede bei der Verwendung von Bitmaskierung und Bitfields. Geben Sie ein Beispiel analog in beiden Varianten an.

3.2 Bei der seriellen Kommunikation (RS-232) ist ein gerades Paritätsbit gesetzt, wenn die Anzahl gesetzter Bits in einem Byte inklusive Paritätsbit gerade ist (also ohne dass Paritätsbit ungerade ist).  
Schreiben Sie eine Funktion in C, welche als Parameter ein Byte (8 Bit unsigned) erhält und das berechnete Paritätsbit für gerade Parität zurückgibt.

Beispielimplementierung (Hinweis):

```c
#include <stdint.h>
#include <stdbool.h>

bool even_parity_bit(uint8_t byte) {
    // Anzahl gesetzter Bits zählen (Kernidee: Paritätsberechnung)
    uint8_t v = byte;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    // v & 1 ist die Parität (1 = ungerade Anzahl Einsen)
    // Für gerade Parität muss Paritätsbit so gesetzt werden, dass Gesamtanzahl gerade wird.
    return (v & 1) ? 1 : 0; // Beispiel: Rückgabewert 1 bedeutet Paritätsbit setzen
}
```

---

## Aufgabe 4.) Dynamische Speicherverwaltung (5 Punkte)

In der dynamischen Speicherverwaltung der Übungsblätter 2/3/4 fehlt eine wesentliche Funktion: `mem_get_Sizes(…)`. Diese Funktion soll die Größe des allokierten und die Größe des freien Speichers zurückgeben. Dazu müssen alle Speicherblöcke durchwandert und deren Größe entsprechend gezählt werden. Der Speicher, den die Header verbrauchen, wird jedenfalls zum allokierten Speicher gezählt. Die Checksumme kann beim Durchlaufen ignoriert werden.

Struktur der Header:

```c
struct Header {
    uint8_t flags;    // 0x01: allocated, sonst free; alle anderen Bits RFU
    uint8_t checksum; // XOR-Checksum
    uint16_t length;  // Anzahl Datenbytes
};
```

Der Heap ist im globalen Array, wie folgt:

```c
#define HEAPSIZE 1023
uint8_t gHeap[HEAPSIZE];
```

Aufgabe: Implementieren Sie `mem_get_Sizes(...)`, die den gesamten belegten und freien Speicher (in Bytes) zurückliefert. Beim Zählen werden Header-Größen zu den allokierten Bytes gezählt, Checksum kann ignoriert werden.

---

## Aufgabe 5.) Multitasking (2 + 2 + 4 + 2 + 2 Punkte)

### 5.1 Semaphor
Was ist ein Semaphor? Wie funktioniert er? Was bedeuten die Aufrufe `P()` und `V()`?

### 5.2 Semaphor in FreeRTOS
Erklären Sie folgende FreeRTOS-Semaphor-Funktionen: Was bedeuten die Parameter, was die Rückgabetypen?

- `SemaphoreHandle_t xSemaphoreCreateCounting(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount);`

- `xSemaphoreGive(SemaphoreHandle_t xSemaphore);`

- `xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait);`

### 5.3 Producer/Consumer
Gegeben sind zwei Taskfunktionen, eine für einen Producer und eine für einen Consumer. Der Producer soll alle 30 ms ein Ereignis "Produzieren" erzeugen und an den Consumer senden. Der Consumer soll alle 30 ms per `printf()` auf die Konsole ausgeben.

```c
void ProducerTaskFunc(void * pvParameters) {
    // Implementierung: alle 30ms produzieren und an Consumer senden
}

void ConsumerTaskFunc(void * pvParameters) {
    // Implementierung: alle 30ms empfangen und per printf ausgeben
}
```

(Pseudocode ist hier erlaubt — Synchronisation z.B. per Queue oder Semaphore)

### 5.4 Scheduling
Skizzieren Sie das Scheduling in einem Diagramm. Nehmen Sie an, dass eine Zeitscheibe (System Tick) 10 ms beträgt. Ein Idle-Task mit niedriger Priorität konsumiert Prozessorzeit, wenn alle anderen Tasks blockiert sind.

(Hier ist eine einfache Timeline mit 10 ms Slots zu zeichnen; z.B. Task A, Task B, Idle, etc.)

### 5.5 Producer/Consumer mit Produkten
Angenommen, der Producer möchte Produkte vom Typ `struct Product` an den Consumer senden. Welche Datenstruktur ist dann geeignet? Skizzieren Sie eine entsprechende Änderung des Producer/Consumer-Codes (Pseudocode ist hier erlaubt).

Beispielskizze:

```c
typedef struct {
    int id;
    char name[32];
    // weitere Felder
} Product;

// Verwendung einer Queue in FreeRTOS:
QueueHandle_t productQueue = xQueueCreate(10, sizeof(Product));

void ProducerTaskFunc(void * pvParameters) {
    Product p;
    // erzeugen...
    xQueueSend(productQueue, &p, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(30));
}

void ConsumerTaskFunc(void * pvParameters) {
    Product p;
    for (;;) {
        if (xQueueReceive(productQueue, &p, pdMS_TO_TICKS(30)) == pdTRUE) {
            printf("Produkt id=%d name=%s\n", p.id, p.name);
        }
    }
}
```

---

Ende der Probeklausur.