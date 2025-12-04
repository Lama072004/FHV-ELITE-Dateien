#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

/* Dynamische Speicherverwaltung ohne Bibliotheksfunktionen
 * Memory Manager Header
 */

// Größe des verwalteten Speichers (in Bytes)
#define MEMORY_SIZE 1024

// Header-Struktur für jeden Speicherblock mit Bitfields
// 4 Bytes total: 1 Byte Flags, 2 Bytes Größe, 1 Byte Checksumme
typedef struct {
    // Byte 0: Flags für allocated/free
    unsigned char is_free : 1;      // Bit 0: 0 = belegt (allocated), 1 = frei (free)
    unsigned char reserved : 7;     // Bits 1-7: Reserviert für zukünftige Nutzung
    
    // Bytes 1-2: Größe der Nutzdaten (ohne Header)
    unsigned short size;            // 16 Bit für Größe (max 65535)
    
    // Byte 3: Checksumme über die ersten 3 Bytes
    unsigned char checksum;         // Additive Checksumme modulo 256
} BlockHeader;

// Funktionsprototypen

/* Initialisiert den Speicher mit einem großen freien Block */
void mem_init(void);

/* Reserviert einen Speicherblock der angegebenen Größe
 * Gibt einen Pointer auf den Inhalt zurück (NULL bei Fehler)
 * Inhalt wird mit 'A' initialisiert
 */
void* mem_alloc(int size);

/* Gibt einen zuvor mit mem_alloc reservierten Block wieder frei */
void mem_free(void *pMem);

/* Gibt die Speicherbelegung auf der Konsole aus */
void mem_dump(void);

/* Prüft die Checksummen aller Blöcke im Heap
 * Gibt Anzahl korrupter Blöcke zurück (0 = alles OK)
 */
int checkHeap(void);

#endif // MEM_MANAGER_H
