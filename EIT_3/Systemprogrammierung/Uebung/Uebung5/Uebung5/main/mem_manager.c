#include "mem_manager.h"
#include <stdio.h>

// Globaler Speicherbereich
static unsigned char memory[MEMORY_SIZE];

// Header-Größe in Bytes
#define HEADER_SIZE sizeof(BlockHeader)

/* Hilfsfunktion: Zeiger auf Header aus Datenzeiger berechnen */
static BlockHeader* get_header(void *pMem) {
    if (pMem == NULL) {
        return NULL;
    }
    return (BlockHeader*)((unsigned char*)pMem - HEADER_SIZE);
}

/* Hilfsfunktion: Zeiger auf Daten aus Header berechnen */
static void* get_data_ptr(BlockHeader *header) {
    return (void*)((unsigned char*)header + HEADER_SIZE);
}

/* Hilfsfunktion: Speicherblock mit einem Zeichen füllen */
static void mem_fill(void *ptr, unsigned char value, int count) {
    unsigned char *p = (unsigned char*)ptr;
    int i;
    for (i = 0; i < count; i++) {
        p[i] = value;
    }
}

/* Hilfsfunktion: Prüfen ob zwei Blöcke zusammengeführt werden können */
static void merge_free_blocks(void) {
    unsigned char *current = memory;
    unsigned char *end = memory + MEMORY_SIZE;
    
    while (current < end) {
        BlockHeader *header = (BlockHeader*)current;
        
        // Sicherheitsprüfung: Header muss vollständig im Speicher sein
        if (current + HEADER_SIZE > end) {
            break;
        }
        
        // Wenn aktueller Block frei ist
        if (header->is_free) {
            unsigned char *next_pos = current + HEADER_SIZE + header->size;
            
            // Prüfen ob nächster Block existiert und auch frei ist
            if (next_pos < end && next_pos + HEADER_SIZE <= end) {
                BlockHeader *next_header = (BlockHeader*)next_pos;
                
                if (next_header->is_free) {
                    // Blöcke zusammenführen
                    header->size = header->size + HEADER_SIZE + next_header->size;
                    // Nicht weitergehen, nochmal prüfen ob weitere Blöcke folgen
                    continue;
                }
            }
        }
        
        // Zum nächsten Block gehen
        current = current + HEADER_SIZE + header->size;
    }
}

void mem_init(void) {
    BlockHeader *initial_header = (BlockHeader*)memory;
    initial_header->is_free = 1;
    initial_header->size = MEMORY_SIZE - HEADER_SIZE;
}

void* mem_alloc(int size) {
    // Fehlerbehandlung: Ungültige Größe
    if (size <= 0) {
        return NULL;
    }
    
    // Fehlerbehandlung: Größe größer als verfügbarer Speicher
    if (size > MEMORY_SIZE - HEADER_SIZE) {
        return NULL;
    }
    
    unsigned char *current = memory;
    unsigned char *end = memory + MEMORY_SIZE;
    
    // First-Fit-Strategie: Ersten passenden freien Block finden
    while (current < end) {
        BlockHeader *header = (BlockHeader*)current;
        
        // Sicherheitsprüfung
        if (current + HEADER_SIZE > end) {
            break;
        }
        
        // Freien Block gefunden der groß genug ist
        if (header->is_free && header->size >= size) {
            // Block als belegt markieren
            header->is_free = 0;
            
            // Wenn genug Platz übrig ist, Rest-Block erstellen
            // Mindestgröße für Split: HEADER_SIZE + 1 Byte Nutzdaten
            int remaining = header->size - size;
            if (remaining > HEADER_SIZE) {
                // Neuen freien Block für Rest erstellen
                unsigned char *next_pos = current + HEADER_SIZE + size;
                BlockHeader *next_header = (BlockHeader*)next_pos;
                next_header->is_free = 1;
                next_header->size = remaining - HEADER_SIZE;
                
                // Größe des aktuellen Blocks anpassen
                header->size = size;
            }
            
            // Nutzdaten mit 'A' initialisieren
            void *data_ptr = get_data_ptr(header);
            mem_fill(data_ptr, 'A', size);
            
            return data_ptr;
        }
        
        // Zum nächsten Block gehen
        current = current + HEADER_SIZE + header->size;
    }
    
    // Kein passender Block gefunden
    return NULL;
}

void mem_free(void *pMem) {
    // Fehlerbehandlung: NULL-Pointer
    if (pMem == NULL) {
        return;
    }
    
    // Fehlerbehandlung: Zeiger außerhalb des verwalteten Speichers
    if ((unsigned char*)pMem < memory || (unsigned char*)pMem >= memory + MEMORY_SIZE) {
        return;
    }
    
    // Header ermitteln
    BlockHeader *header = get_header(pMem);
    
    // Fehlerbehandlung: Header außerhalb des Speichers
    if ((unsigned char*)header < memory || (unsigned char*)header >= memory + MEMORY_SIZE) {
        return;
    }
    
    // Fehlerbehandlung: Block ist bereits frei
    if (header->is_free) {
        return;
    }
    
    // Block als frei markieren
    header->is_free = 1;
    
    // Angrenzende freie Blöcke zusammenführen
    merge_free_blocks();
}

void mem_dump(void) {
    printf("=== Memory Dump ===\n");
    printf("Total Memory Size: %d Bytes\n", MEMORY_SIZE);
    printf("Header Size: %d Bytes\n\n", (int)HEADER_SIZE);
    
    unsigned char *current = memory;
    unsigned char *end = memory + MEMORY_SIZE;
    int block_number = 0;
    int total_used = 0;
    int total_free = 0;
    
    printf("Offset | State | Size | Content Preview\n");
    printf("-------|-------|------|------------------\n");
    
    while (current < end) {
        BlockHeader *header = (BlockHeader*)current;
        
        // Sicherheitsprüfung
        if (current + HEADER_SIZE > end) {
            printf("Error: Incomplete header at offset %d\n", (int)(current - memory));
            break;
        }
        
        int offset = (int)(current - memory);
        char state = header->is_free ? 'F' : 'A';
        int size = header->size;
        
        printf("%6d | %5c | %4d | ", offset, state, size);
        
        // Content Preview (erste 16 Bytes oder weniger)
        void *data_ptr = get_data_ptr(header);
        int preview_len = size < 16 ? size : 16;
        unsigned char *data = (unsigned char*)data_ptr;
        
        int i;
        for (i = 0; i < preview_len; i++) {
            if (data[i] >= 32 && data[i] <= 126) {
                printf("%c", data[i]);
            } else {
                printf(".");
            }
        }
        if (size > 16) {
            printf("...");
        }
        printf("\n");
        
        // Statistik aktualisieren
        if (header->is_free) {
            total_free += size;
        } else {
            total_used += size;
        }
        
        block_number++;
        
        // Zum nächsten Block gehen
        current = current + HEADER_SIZE + header->size;
        
        // Sicherheit: Endlosschleife vermeiden
        if (block_number > 1000) {
            printf("Error: Too many blocks, possible corruption\n");
            break;
        }
    }
    
    printf("\n=== Statistics ===\n");
    printf("Total Blocks: %d\n", block_number);
    printf("Used Memory: %d Bytes (%d Bytes data + %d Bytes headers)\n", 
           total_used + block_number * (int)HEADER_SIZE, total_used, block_number * (int)HEADER_SIZE);
    printf("Free Memory: %d Bytes\n", total_free);
    printf("==================\n\n");
}
