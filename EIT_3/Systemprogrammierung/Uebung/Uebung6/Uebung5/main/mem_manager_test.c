#include "mem_manager.h"
#include <stdio.h>

/* Automatische Tests für die Memory Manager Bibliothek
 * Testet alle Funktionen ausführlich inklusive Fehlerbehandlung
 */

// Hilfsfunktion: String-Länge ohne Bibliotheksfunktionen
static int str_len(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Hilfsfunktion: String kopieren ohne Bibliotheksfunktionen
static void str_copy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Hilfsfunktion: String vergleichen ohne Bibliotheksfunktionen
static int str_equal(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return 0;
        }
        i++;
    }
    return s1[i] == s2[i];
}

// Test-Zähler
static int test_count = 0;
static int test_passed = 0;
static int test_failed = 0;

// Test-Makros
#define TEST_START(name) \
    printf("\n--- Test %d: %s ---\n", ++test_count, name)

#define TEST_ASSERT(condition, message) \
    if (condition) { \
        printf("  [PASS] %s\n", message); \
        test_passed++; \
    } else { \
        printf("  [FAIL] %s\n", message); \
        test_failed++; \
    }

// Test 1: Initialisierung
void test_init(void) {
    TEST_START("Memory Initialization");
    mem_init();
    printf("  Memory initialized\n");
    mem_dump();
}

// Test 2: Einfache Allokation
void test_simple_alloc(void) {
    TEST_START("Simple Allocation");
    mem_init();
    
    void *p1 = mem_alloc(10);
    TEST_ASSERT(p1 != NULL, "Allocation of 10 bytes succeeded");
    
    if (p1 != NULL) {
        // Prüfen ob mit 'A' initialisiert
        char *data = (char*)p1;
        int all_a = 1;
        for (int i = 0; i < 10; i++) {
            if (data[i] != 'A') {
                all_a = 0;
                break;
            }
        }
        TEST_ASSERT(all_a, "Memory initialized with 'A'");
    }
    
    mem_dump();
}

// Test 3: Multiple Allokationen
void test_multiple_alloc(void) {
    TEST_START("Multiple Allocations");
    mem_init();
    
    void *p1 = mem_alloc(20);
    void *p2 = mem_alloc(30);
    void *p3 = mem_alloc(40);
    
    TEST_ASSERT(p1 != NULL, "First allocation succeeded");
    TEST_ASSERT(p2 != NULL, "Second allocation succeeded");
    TEST_ASSERT(p3 != NULL, "Third allocation succeeded");
    TEST_ASSERT(p1 != p2 && p2 != p3 && p1 != p3, "All pointers are different");
    
    mem_dump();
}

// Test 4: String-Speicherung
void test_string_storage(void) {
    TEST_START("String Storage (like example: Tretboot, Auto)");
    mem_init();
    
    const char *str1 = "Tretboot";
    const char *str2 = "Auto";
    
    int len1 = str_len(str1);
    int len2 = str_len(str2);
    
    void *p1 = mem_alloc(len1 + 1);  // +1 für Null-Terminator
    void *p2 = mem_alloc(len2 + 1);
    
    TEST_ASSERT(p1 != NULL, "Allocation for 'Tretboot' succeeded");
    TEST_ASSERT(p2 != NULL, "Allocation for 'Auto' succeeded");
    
    if (p1 != NULL && p2 != NULL) {
        str_copy((char*)p1, str1);
        str_copy((char*)p2, str2);
        
        TEST_ASSERT(str_equal((char*)p1, str1), "String 1 correctly stored");
        TEST_ASSERT(str_equal((char*)p2, str2), "String 2 correctly stored");
    }
    
    mem_dump();
}

// Test 5: Freigabe und Wiederverwendung
void test_free_and_reuse(void) {
    TEST_START("Free and Reuse");
    mem_init();
    
    void *p1 = mem_alloc(50);
    void *p2 = mem_alloc(50);
    void *p3 = mem_alloc(50);
    
    printf("  After 3 allocations:\n");
    mem_dump();
    
    mem_free(p2);  // Mittleren Block freigeben
    printf("  After freeing middle block:\n");
    mem_dump();
    
    void *p4 = mem_alloc(30);  // Sollte in den freigegebenen Block passen
    TEST_ASSERT(p4 != NULL, "Reallocation in freed space succeeded");
    
    printf("  After reallocating in freed space:\n");
    mem_dump();
}

// Test 6: Zusammenführen von freien Blöcken
void test_merge_free_blocks(void) {
    TEST_START("Merging Free Blocks");
    mem_init();
    
    void *p1 = mem_alloc(20);
    void *p2 = mem_alloc(20);
    void *p3 = mem_alloc(20);
    void *p4 = mem_alloc(20);
    
    printf("  After 4 allocations:\n");
    mem_dump();
    
    mem_free(p2);
    mem_free(p3);
    
    printf("  After freeing blocks 2 and 3 (should merge):\n");
    mem_dump();
}

// Test 7: Fehlerbehandlung - Ungültige Größe
void test_invalid_size(void) {
    TEST_START("Error Handling - Invalid Size");
    mem_init();
    
    void *p1 = mem_alloc(0);
    TEST_ASSERT(p1 == NULL, "Allocation with size 0 returns NULL");
    
    void *p2 = mem_alloc(-10);
    TEST_ASSERT(p2 == NULL, "Allocation with negative size returns NULL");
    
    void *p3 = mem_alloc(MEMORY_SIZE);  // Zu groß (wegen Header)
    TEST_ASSERT(p3 == NULL, "Allocation larger than available memory returns NULL");
}

// Test 8: Fehlerbehandlung - Speicher voll
void test_out_of_memory(void) {
    TEST_START("Error Handling - Out of Memory");
    mem_init();
    
    // Fast gesamten Speicher allokieren
    void *p1 = mem_alloc(MEMORY_SIZE - sizeof(BlockHeader) - 10);
    TEST_ASSERT(p1 != NULL, "Large allocation succeeded");
    
    // Versuchen mehr zu allokieren als übrig ist
    void *p2 = mem_alloc(100);
    TEST_ASSERT(p2 == NULL, "Allocation when out of memory returns NULL");
    
    mem_dump();
}

// Test 9: Fehlerbehandlung - Doppeltes Free
void test_double_free(void) {
    TEST_START("Error Handling - Double Free");
    mem_init();
    
    void *p1 = mem_alloc(50);
    TEST_ASSERT(p1 != NULL, "Initial allocation succeeded");
    
    mem_free(p1);
    printf("  After first free:\n");
    mem_dump();
    
    // Zweites Free sollte sicher ignoriert werden
    mem_free(p1);
    printf("  After second free (should be safe):\n");
    mem_dump();
}

// Test 10: Fehlerbehandlung - NULL-Pointer
void test_free_null(void) {
    TEST_START("Error Handling - Free NULL");
    mem_init();
    
    mem_free(NULL);  // Sollte sicher sein
    printf("  Free NULL pointer (should be safe)\n");
    TEST_ASSERT(1, "Free NULL did not crash");
}

// Test 11: Fragmentierung
void test_fragmentation(void) {
    TEST_START("Fragmentation Test");
    mem_init();
    
    void *blocks[10];
    
    // 10 kleine Blöcke allokieren
    for (int i = 0; i < 10; i++) {
        blocks[i] = mem_alloc(20);
    }
    
    printf("  After 10 allocations:\n");
    mem_dump();
    
    // Jeden zweiten Block freigeben
    for (int i = 0; i < 10; i += 2) {
        mem_free(blocks[i]);
    }
    
    printf("  After freeing every other block:\n");
    mem_dump();
    
    // Versuchen einen großen Block zu allokieren (sollte fehlschlagen wegen Fragmentierung)
    void *big_block = mem_alloc(150);
    TEST_ASSERT(big_block == NULL, "Large allocation fails due to fragmentation");
}

// Test 12: Grenzfälle - Sehr kleine Allokationen
void test_small_allocations(void) {
    TEST_START("Edge Case - Very Small Allocations");
    mem_init();
    
    void *p1 = mem_alloc(1);
    void *p2 = mem_alloc(2);
    void *p3 = mem_alloc(3);
    
    TEST_ASSERT(p1 != NULL, "1-byte allocation succeeded");
    TEST_ASSERT(p2 != NULL, "2-byte allocation succeeded");
    TEST_ASSERT(p3 != NULL, "3-byte allocation succeeded");
    
    mem_dump();
}

// Test 13: Sequentielle Free-Operationen
void test_sequential_free(void) {
    TEST_START("Sequential Free Operations");
    mem_init();
    
    void *p1 = mem_alloc(30);
    void *p2 = mem_alloc(30);
    void *p3 = mem_alloc(30);
    void *p4 = mem_alloc(30);
    
    printf("  Initial state:\n");
    mem_dump();
    
    mem_free(p1);
    mem_free(p2);
    mem_free(p3);
    mem_free(p4);
    
    printf("  After freeing all blocks:\n");
    mem_dump();
    
    // Nach dem Freigeben aller Blöcke sollte wieder der gesamte Speicher verfügbar sein
    void *p5 = mem_alloc(MEMORY_SIZE - sizeof(BlockHeader) - 10);
    TEST_ASSERT(p5 != NULL, "Large allocation after freeing all blocks succeeded");
}

// Test 14: Komplexes Szenario
void test_complex_scenario(void) {
    TEST_START("Complex Scenario");
    mem_init();
    
    const char *strings[] = {"Tretboot", "Auto", "Fahrrad", "Bus", "Zug"};
    void *pointers[5];
    
    // Strings allokieren und speichern
    for (int i = 0; i < 5; i++) {
        int len = str_len(strings[i]);
        pointers[i] = mem_alloc(len + 1);
        if (pointers[i] != NULL) {
            str_copy((char*)pointers[i], strings[i]);
        }
    }
    
    printf("  After storing 5 strings:\n");
    mem_dump();
    
    // Einige Strings freigeben
    mem_free(pointers[1]);  // Auto
    mem_free(pointers[3]);  // Bus
    
    printf("  After freeing 'Auto' and 'Bus':\n");
    mem_dump();
    
    // Neue Strings hinzufügen
    const char *new_str = "Motorrad";
    int new_len = str_len(new_str);
    void *new_ptr = mem_alloc(new_len + 1);
    if (new_ptr != NULL) {
        str_copy((char*)new_ptr, new_str);
    }
    
    printf("  After adding 'Motorrad':\n");
    mem_dump();
    
    // Verbleibende Strings prüfen
    TEST_ASSERT(str_equal((char*)pointers[0], strings[0]), "String 0 still correct");
    TEST_ASSERT(str_equal((char*)pointers[2], strings[2]), "String 2 still correct");
    TEST_ASSERT(str_equal((char*)pointers[4], strings[4]), "String 4 still correct");
    TEST_ASSERT(str_equal((char*)new_ptr, new_str), "New string correctly stored");
}

// Test 15: Heap-Integritätsprüfung mit Checksumme
void test_checksum_integrity(void) {
    TEST_START("Checksum Integrity Check");
    mem_init();
    
    // Normale Allokation - sollte OK sein
    void *p1 = mem_alloc(50);
    TEST_ASSERT(p1 != NULL, "Allocation succeeded");
    
    int corrupt = checkHeap();
    TEST_ASSERT(corrupt == 0, "Heap integrity OK after allocation");
    
    mem_dump();
}

// Test 16: Buffer Overflow Detection mit Checksumme
void test_buffer_overflow_detection(void) {
    TEST_START("Buffer Overflow Detection");
    mem_init();
    
    // Allokiere 20 Bytes
    void *p1 = mem_alloc(20);
    TEST_ASSERT(p1 != NULL, "Allocation succeeded");
    
    // Prüfe Heap - sollte OK sein
    int corrupt_before = checkHeap();
    TEST_ASSERT(corrupt_before == 0, "Heap OK before overflow");
    
    // Simuliere Buffer Overflow: Schreibe über das Ende des Blocks hinaus
    // Dies sollte den Header des nächsten Blocks beschädigen
    char *data = (char*)p1;
    printf("  Simulating buffer overflow...\n");
    for (int i = 0; i < 25; i++) {  // Schreibe 5 Bytes zu viel
        data[i] = 'X';
    }
    
    // Prüfe Heap erneut - sollte jetzt Korruption erkennen
    int corrupt_after = checkHeap();
    TEST_ASSERT(corrupt_after > 0, "Buffer overflow detected by checksum");
    
    mem_dump();
}

// Test 17: Checksum nach Free und Merge
void test_checksum_after_merge(void) {
    TEST_START("Checksum After Merge");
    mem_init();
    
    void *p1 = mem_alloc(30);
    void *p2 = mem_alloc(30);
    void *p3 = mem_alloc(30);
    
    TEST_ASSERT(p1 != NULL && p2 != NULL && p3 != NULL, "All allocations succeeded");
    
    int corrupt1 = checkHeap();
    TEST_ASSERT(corrupt1 == 0, "Heap OK after allocations");
    
    // Free mittleren Block
    mem_free(p2);
    int corrupt2 = checkHeap();
    TEST_ASSERT(corrupt2 == 0, "Heap OK after first free");
    
    // Free ersten Block - sollte mit mittlerem mergen
    mem_free(p1);
    int corrupt3 = checkHeap();
    TEST_ASSERT(corrupt3 == 0, "Heap OK after merge");
    
    mem_dump();
}

// Hauptfunktion für Tests
void run_all_mem_tests(void) {
    printf("\n");
    printf("========================================\n");
    printf("  Memory Manager Test Suite\n");
    printf("========================================\n");
    
    test_init();
    test_simple_alloc();
    test_multiple_alloc();
    test_string_storage();
    test_free_and_reuse();
    test_merge_free_blocks();
    test_invalid_size();
    test_out_of_memory();
    test_double_free();
    test_free_null();
    test_fragmentation();
    test_small_allocations();
    test_sequential_free();
    test_complex_scenario();
    test_checksum_integrity();
    test_buffer_overflow_detection();
    test_checksum_after_merge();
    
    printf("\n");
    printf("========================================\n");
    printf("  Test Summary\n");
    printf("========================================\n");
    printf("Total Tests: %d\n", test_count);
    printf("Assertions Passed: %d\n", test_passed);
    printf("Assertions Failed: %d\n", test_failed);
    printf("========================================\n");
    
    if (test_failed == 0) {
        printf("All tests PASSED!\n");
    } else {
        printf("Some tests FAILED!\n");
    }
    printf("\n");
}
