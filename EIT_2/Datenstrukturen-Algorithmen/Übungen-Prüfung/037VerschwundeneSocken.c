#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 10
#define MAX_COLOR 21

typedef struct {
    int size;
    char color[MAX_COLOR];
    int count;
} Sock;

int compare(const void* a, const void* b) {
    Sock* s1 = (Sock*)a;
    Sock* s2 = (Sock*)b;
    if (s1->size != s2->size)
        return s1->size - s2->size;
    return strcmp(s1->color, s2->color);
}

int main() {
    int N;
    scanf("%d", &N);

    Sock sockList[MAX];
    int sockCount = 0;

    for (int i = 0; i < N; i++) {
        char type[10], color[MAX_COLOR];
        int size;
        scanf("%s %d %s", type, &size, color);

        if (strcmp(type, "sock") == 0) {
            // Check if sock already in list
            bool found = false;
            for (int j = 0; j < sockCount; j++) {
                if (sockList[j].size == size && strcmp(sockList[j].color, color) == 0) {
                    sockList[j].count++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                sockList[sockCount].size = size;
                strcpy(sockList[sockCount].color, color);
                sockList[sockCount].count = 1;
                sockCount++;
            }
        }
    }

    // Sammle verlorene Socken
    Sock verloren[MAX];
    int vCount = 0;

    for (int i = 0; i < sockCount; i++) {
        if (sockList[i].count % 2 != 0) {
            verloren[vCount++] = sockList[i];
        }
    }

    // Sortiere
    qsort(verloren, vCount, sizeof(Sock), compare);

    // Ausgabe
    printf("%d\n", vCount);
    for (int i = 0; i < vCount; i++) {
        printf("%d %s\n", verloren[i].size, verloren[i].color);
    }

    return 0;
}