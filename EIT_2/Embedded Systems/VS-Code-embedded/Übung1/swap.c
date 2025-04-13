#include <stdint.h>
#include <stdio.h>

// Funktion zum Vertauschen der Werte von zwei uint32_t-Variablen
void swap(uint32_t* a, uint32_t* b)
{
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

// Funktion zum Vertauschen der Adressen von zwei uint32_t-Zeigern
void swap2(uint32_t** a, uint32_t** b)
{
    uint32_t* temp = *a;
    *a = *b;
    *b = temp;
}

// Funktion zum Anzeigen und Vertauschen der Werte
void Werte(uint32_t a, uint32_t b)
{
    // Werte vor dem Vertauschen
    printf("Before swap:\n");
    printf("a: %d\n", a);
    printf("b: %d\n\n", b);

    // Werte vertauschen
    swap(&a, &b);
    printf("After swap:\n");
    printf("a: %d\n", a);
    printf("b: %d\n\n", b);
}

// Funktion zum Anzeigen und Vertauschen der Adressen
void Adressen(uint32_t* ptrA, uint32_t* ptrB)
{
    // Adressen vor dem Vertauschen
    printf("Before address swap:\n");
    printf("ptrA points to value: %d, address: %p\n", *ptrA, (void*)ptrA);
    printf("ptrB points to value: %d, address: %p\n\n", *ptrB, (void*)ptrB);

    // Adressen vertauschen
    swap2(&ptrA, &ptrB);
    printf("After address swap:\n");
    printf("ptrA points to value: %d, address: %p\n", *ptrA, (void*)ptrA);
    printf("ptrB points to value: %d, address: %p\n\n", *ptrB, (void*)ptrB);
}

int main(int argc, char** argv)
{
    uint32_t a = 10;
    uint32_t b = 20;

    // Zeiger auf a und b
    uint32_t* ptrA = &a;
    uint32_t* ptrB = &b;

    // Werte anzeigen und vertauschen
    Werte(a, b);

    // Adressen anzeigen und vertauschen
    Adressen(ptrA, ptrB);

    return 0;
}