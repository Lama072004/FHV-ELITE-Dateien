#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "mystringutils.h"

int main(int argc, char** argv)
{
    char str[] = "hello this is a test";
    char str2[] = "hello this is a test";
    uint32_t l = mystringlen(str);
    printf("%s, %d\n", str, l);

    toupper(str); // toupper aufrufen, um str zu modifizieren

    printf("%s\n", str); // Modifizierte Zeichenkette ausgeben

    printf("%d\n", mystringcomp(str, str2));


    return 0;
}