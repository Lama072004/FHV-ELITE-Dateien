#ifndef SCORE_H
#define SCORE_H
#include <stdint.h>

// Score initialisieren
void score_init(void);

// Punkte für gelöschte Reihen hinzufügen
void score_add_lines(int lines);

// Aktuellen Score abrufen
int score_get(void);

// Gesamtzahl der gecleareten Zeilen abrufen
uint32_t score_get_total_lines_cleared(void);


// Highscore (NVS) Funktionen
void score_load_highscore(void);
uint32_t score_get_highscore(void);
void score_update_highscore(void);

// Cleanup: NVS Handle ordnungsgemäß schließen (verhindert Memory Leak)
void score_cleanup(void);

#endif // SCORE_H
