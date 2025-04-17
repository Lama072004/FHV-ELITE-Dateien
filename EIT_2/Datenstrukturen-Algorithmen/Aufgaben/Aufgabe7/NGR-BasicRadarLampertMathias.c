#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PLATE_LEN 12
#define MAX_RADAR_LEN 8

typedef struct {
    char plate[MAX_PLATE_LEN];
    char radarname[MAX_RADAR_LEN];
    long long timestamp;
} RadarReading;

typedef struct {
    char plate[MAX_PLATE_LEN];
    int speed;
} SpeedResult;

int compare_plates(const void *a, const void *b) {
    const SpeedResult *ra = (const SpeedResult *)a;
    const SpeedResult *rb = (const SpeedResult *)b;
    return strcmp(ra->plate, rb->plate);
}

int main() {
    int N;
    scanf("%d", &N);
    
    RadarReading readings[N];
    
    // Input lesen
    for (int i = 0; i < N; i++) {
        scanf("%s %s %lld", readings[i].plate, readings[i].radarname, &readings[i].timestamp);
    }
    
    SpeedResult results[N/2]; // Maximal N/2 Ergebnisse möglich
    int result_count = 0;
    
    // Paare finden und Geschwindigkeiten berechnen
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            if (strcmp(readings[i].plate, readings[j].plate) == 0) {
                if (strcmp(readings[i].radarname, readings[j].radarname) != 0) {
                    // Zeit in Stunden umrechnen (ms -> h)
                    double time_h = (double)(readings[j].timestamp - readings[i].timestamp) / 3600000.0;
                    int speed = (int)(13.0 / time_h); // 13km Distanz
                    
                    if (speed > 130) {
                        strcpy(results[result_count].plate, readings[i].plate);
                        results[result_count].speed = speed;
                        result_count++;
                    }
                    break; // Weiter mit nächstem Kennzeichen
                }
            }
        }
    }
    
    // Ergebnisse sortieren
    qsort(results, result_count, sizeof(SpeedResult), compare_plates);
    
    // Ausgabe
    for (int i = 0; i < result_count; i++) {
        printf("%s %d\n", results[i].plate, results[i].speed);
    }
    
    return 0;
}