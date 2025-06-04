#include <stdio.h>
#include <string.h>

typedef struct {
    char key[1000];
    int value;
    int used;
} Bucket;

int main() {
    int N;
    scanf_s("%d", &N);

    Bucket buckets[26] = { 0 }; // für Buchstaben a–z

    for (int i = 0; i < N; i++) {
        char key[1000];
        int value;
        scanf_s("%s %d", key, (unsigned)_countof(key), &value);

        char first = key[0];
        int index = first - 'a';

        strcpy_s(buckets[index].key, sizeof(buckets[index].key), key);
        buckets[index].value = value;
        buckets[index].used = 1;
    }

    for (int i = 0; i < 26; i++) {
        if (buckets[i].used) {
            printf("%c: %s=%d\n", 'a' + i, buckets[i].key, buckets[i].value);
        }
    }

    return 0;
}
