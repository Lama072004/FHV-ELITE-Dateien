#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

/* Dynamische Speicherverwaltung ohne Bibliotheksfunktionen
 * Memory Manager Header
 */

// Größe des verwalteten Speichers (in Bytes)
#define MEMORY_SIZE 1024

// Header-Struktur für jeden Speicherblock
// 4 Bytes: 2 Bytes Flag (frei/belegt), 2 Bytes Länge
typedef struct {
    unsigned short is_free;  // 0 = belegt (allocated), 1 = frei (free)
    unsigned short size;     // Größe der Nutzdaten (ohne Header)
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

#endif // MEM_MANAGER_H
