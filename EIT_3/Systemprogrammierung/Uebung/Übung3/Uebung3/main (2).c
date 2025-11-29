#include <stdio.h>
#include <stdlib.h>

// ===== BITFIELD IMPLEMENTIERUNG =====

typedef struct {
    unsigned int field1 : 4;
    unsigned int field2 : 6;
    unsigned int field3 : 1;
    unsigned int field4 : 7;
    unsigned int field5 : 2;
    unsigned int field6 : 2;
    unsigned int field7 : 17;
} Bitfield_t;

// Union fuer Bitmaskenzugriff
typedef union {
    Bitfield_t fields;
    unsigned int raw;
} BitfieldUnion_t;

// Bitmasken fuer das 7-Bit Feld (field4)
#define FIELD4_MASK 0x7F0000
#define FIELD4_SHIFT 16

// Funktionen fuer Bitfield
void bitfield_test() {
    printf("=== BITFIELD TEST ===\n");

    BitfieldUnion_t data;
    data.raw = 0;

    // Werte setzen
    data.fields.field1 = 12;    // 4 Bits (max 15)
    data.fields.field2 = 42;    // 6 Bits (max 63)
    data.fields.field3 = 1;     // 1 Bit (max 1)
    data.fields.field4 = 100;   // 7 Bits (max 127)
    data.fields.field5 = 3;     // 2 Bits (max 3)
    data.fields.field6 = 1;     // 2 Bits (max 3)
    data.fields.field7 = 50000; // 17 Bits (max 131071)

    // Werte auslesen
    printf("Field1 (4 bits): %u\n", data.fields.field1);
    printf("Field2 (6 bits): %u\n", data.fields.field2);
    printf("Field3 (1 bit): %u\n", data.fields.field3);
    printf("Field4 (7 bits): %u\n", data.fields.field4);
    printf("Field5 (2 bits): %u\n", data.fields.field5);
    printf("Field6 (2 bits): %u\n", data.fields.field6);
    printf("Field7 (17 bits): %u\n", data.fields.field7);

    // Zugriff per Bitmaskierung auf Field4
    printf("\n--- Bitmaskenzugriff auf Field4 ---\n");
    unsigned int field4_value = (data.raw & FIELD4_MASK) >> FIELD4_SHIFT;
    printf("Field4 via Bitmask: %u\n", field4_value);

    // Field4 per Bitmaskierung setzen
    data.raw &= ~FIELD4_MASK; // Feld loeschen
    data.raw |= (75 << FIELD4_SHIFT); // Neuen Wert setzen
    printf("Field4 nach Setzen via Bitmask: %u\n", data.fields.field4);

    printf("\n");
}

// ===== DYNAMISCHER SPEICHER - VERKETTETE LISTE =====

typedef struct Node_t {
    int data;
    struct Node_t* next;
} Node_t;

typedef struct {
    Node_t* head;
    size_t size;
} List_t;

// Liste erstellen
List_t* list_create() {
    List_t* list = (List_t*)malloc(sizeof(List_t));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

// Element am Anfang einfuegen
int list_insert_front(List_t* list, int value) {
    if (list == NULL) {
        return -1;
    }

    Node_t* new_node = (Node_t*)malloc(sizeof(Node_t));
    if (new_node == NULL) {
        return -1;
    }

    new_node->data = value;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;

    return 0;
}

// Liste anzeigen
void list_display(List_t* list) {
    if (list == NULL || list->head == NULL) {
        printf("Liste ist leer\n");
        return;
    }

    Node_t* current = list->head;
    printf("Liste (%zu Elemente): ", list->size);
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

// Liste loeschen
void list_destroy(List_t* list) {
    if (list == NULL) {
        return;
    }

    Node_t* current = list->head;
    while (current != NULL) {
        Node_t* next = current->next;
        free(current);
        current = next;
    }

    free(list);
}

// ===== CALLBACK IMPLEMENTIERUNG =====

typedef void (*NodeCallback_t)(Node_t*);

// Iterator-Funktion mit Callback
void list_iterate(List_t* list, NodeCallback_t callback) {
    if (list == NULL || callback == NULL) {
        return;
    }

    Node_t* current = list->head;
    while (current != NULL) {
        callback(current);
        current = current->next;
    }
}

// Callback-Funktionen
void print_node(Node_t* node) {
    printf("%d ", node->data);
}

void find_min_callback(Node_t* node) {
    static int min = 0;
    static int first = 1;

    if (first) {
        min = node->data;
        first = 0;
    }
    else if (node->data < min) {
        min = node->data;
    }

    // Letzter Aufruf - Ergebnis ausgeben
    if (node->next == NULL) {
        printf("Minimum: %d\n", min);
        first = 1; // Reset fuer naechsten Aufruf
    }
}

void find_max_callback(Node_t* node) {
    static int max = 0;
    static int first = 1;

    if (first) {
        max = node->data;
        first = 0;
    }
    else if (node->data > max) {
        max = node->data;
    }

    // Letzter Aufruf - Ergebnis ausgeben
    if (node->next == NULL) {
        printf("Maximum: %d\n", max);
        first = 1; // Reset fuer naechsten Aufruf
    }
}

// Testfunktion fuer Callbacks
void callback_test() {
    printf("=== CALLBACK TEST ===\n");

    // Liste erstellen und fuellen
    List_t* list = list_create();
    if (list == NULL) {
        printf("Fehler beim Erstellen der Liste!\n");
        return;
    }

    list_insert_front(list, 10);
    list_insert_front(list, 25);
    list_insert_front(list, 5);
    list_insert_front(list, 42);
    list_insert_front(list, 8);
    list_insert_front(list, 17);

    list_display(list);

    printf("\n--- Callback Ausgabe ---\n");
    printf("Werte: ");
    list_iterate(list, print_node);
    printf("\n");

    printf("\n--- Callback Minimum ---\n");
    list_iterate(list, find_min_callback);

    printf("\n--- Callback Maximum ---\n");
    list_iterate(list, find_max_callback);

    list_destroy(list);
    printf("\n");
}

// ===== TESTFUNKTIONEN =====

void test_bitfield() {
    printf("=== BITFIELD AUSFueHRLICHER TEST ===\n");

    BitfieldUnion_t test;
    test.raw = 0;

    // Grenzwerte testen
    test.fields.field1 = 15;  // Max fuer 4 Bits
    test.fields.field2 = 63;  // Max fuer 6 Bits
    test.fields.field4 = 127; // Max fuer 7 Bits

    printf("Grenzwerte - Field1: %u, Field2: %u, Field4: %u\n",
        test.fields.field1, test.fields.field2, test.fields.field4);

    // ueberlauf testen
    test.fields.field1 = 20; // Sollte auf 4 (20 & 0xF) gesetzt werden
    printf("ueberlauf Test - Field1 (20 -> 4): %u\n", test.fields.field1);

    printf("\n");
}

void test_linked_list() {
    printf("=== VERKETTETE LISTE AUSFueHRLICHER TEST ===\n");

    List_t* list = list_create();
    if (list == NULL) {
        printf("Fehler: Konnte Liste nicht erstellen!\n");
        return;
    }

    // Mehrere Elemente einfuegen
    for (int i = 1; i <= 5; i++) {
        if (list_insert_front(list, i * 10) != 0) {
            printf("Fehler beim Einfuegen von Element %d\n", i);
        }
    }

    list_display(list);

    // Groesse testen
    printf("Listengroesse: %zu (erwartet: 5)\n", list->size);

    list_destroy(list);
    printf("Liste erfolgreich geloescht\n\n");
}

// ===== HAUPTPROGRAMM =====

int main() {
    int choice;

    do {
        printf("=== HAUPTMENUE ===\n");
        printf("1. Bitfield Test\n");
        printf("2. Verkettete Liste Test\n");
        printf("3. Callback Test\n");
        printf("4. Ausfuehrlicher Bitfield Test\n");
        printf("5. Ausfuehrlicher Liste Test\n");
        printf("0. Beenden\n");
        printf("Auswahl: ");

        // Einfache Eingabe fuer Demo
        if (scanf_s("%d", &choice) != 1) {
            printf("Ungueltige Eingabe!\n");
            while (getchar() != '\n'); // Input buffer leeren
            continue;
        }

        switch (choice) {
        case 1:
            bitfield_test();
            break;
        case 2:
            test_linked_list();
            break;
        case 3:
            callback_test();
            break;
        case 4:
            test_bitfield();
            break;
        case 5:
            test_linked_list();
            break;
        case 0:
            printf("Programm beendet.\n");
            break;
        default:
            printf("Ungueltige Auswahl!\n");
            break;
        }

    } while (choice != 0);

    return 0;
}