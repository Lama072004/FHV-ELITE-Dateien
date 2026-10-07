#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    double start;
    double end;
} Interval;

int cmp_intervals(const void* a, const void* b) {
    double diff = ((Interval*)a)->end - ((Interval*)b)->end;
    return (diff > 0) - (diff < 0);
}

int min_transmitters(int p, int r, int* coords) {
    Interval intervals[100];
    int count = 0;

    for (int i = 0; i < p; i++) {
        int x = coords[2 * i];
        int y = coords[2 * i + 1];

        if (y > r) {
            return -1;
        }

        double dx = sqrt((double)r * r - (double)y * y);
        intervals[count].start = x - dx;
        intervals[count].end = x + dx;
        count++;
    }

    qsort(intervals, count, sizeof(Interval), cmp_intervals);

    int result = 0;
    double last_covered = -1e9;

    for (int i = 0; i < count; i++) {
        if (last_covered < intervals[i].start) {
            last_covered = intervals[i].end;
            result++;
        }
    }

    return result;
}

int main() {
    int n;
    scanf("%d", &n);
    getchar(); // consume the newline after n

    for (int i = 0; i < n; i++) {
        char line[2048];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "Fehler beim Lesen der Eingabezeile.\n");
            continue;
        }

        // Tokenisieren
        int values[202]; // p + r + maximal 100 Punkte * 2
        int count = 0;
        char* token = strtok(line, " \n");
        while (token != NULL) {
            values[count++] = atoi(token);
            token = strtok(NULL, " \n");
        }

        if (count < 2) {
            printf("-1\n"); // nicht genug Daten
            continue;
        }

        int p = values[0];
        int r = values[1];

        if (count < 2 + 2 * p) {
            printf("-1\n"); // unvollständige Koordinaten
            continue;
        }

        int* coords = &values[2];
        int result = min_transmitters(p, r, coords);
        printf("%d\n", result);
    }

    return 0;
}
