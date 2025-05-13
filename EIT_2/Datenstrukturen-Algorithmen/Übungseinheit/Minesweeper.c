#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX 20

// === Datenstrukturen ===
typedef struct {
    int x, y;
} Pos;

// === Globale Variablen ===
char grid[MAX][MAX];
bool safe[MAX][MAX] = {false};
bool is_mine[MAX][MAX] = {false};

int h, w, nb;
Pos mines[400];
int mine_count = 0;

// Bewegungsrichtungen (8 Nachbarn)
int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

// === Hilfsfunktionen ===
bool in_bounds(int x, int y) {
    return x >= 0 && x < w && y >= 0 && y < h;
}

// Markiere angrenzende Felder von '.' als sicher
void mark_safe_from_dots() {
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (grid[y][x] == '.')
                for (int d = 0; d < 8; d++) {
                    int nx = x + dx[d], ny = y + dy[d];
                    if (in_bounds(nx, ny))
                        safe[ny][nx] = true;
                }
}

// Füge Mine zur Liste hinzu, wenn noch nicht markiert
void add_mine(int x, int y) {
    if (!is_mine[y][x]) {
        is_mine[y][x] = true;
        mines[mine_count++] = (Pos){x, y};
    }
}

// Deduktives Finden sicherer Felder und Minen
void deduce_mines() {
    bool changed;
    do {
        changed = false;

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                if (grid[y][x] >= '0' && grid[y][x] <= '8') {
                    int required = grid[y][x] - '0';
                    int known = 0;
                    Pos unknowns[8];
                    int u_count = 0;

                    // Untersuche Nachbarn
                    for (int d = 0; d < 8; d++) {
                        int nx = x + dx[d], ny = y + dy[d];
                        if (in_bounds(nx, ny)) {
                            if (is_mine[ny][nx]) known++;
                            else if (grid[ny][nx] == '?' && !safe[ny][nx])
                                unknowns[u_count++] = (Pos){nx, ny};
                        }
                    }

                    int need = required - known;

                    if (need == 0 && u_count > 0) {
                        // Alle verbleibenden ? sind sicher
                        for (int i = 0; i < u_count; i++)
                            safe[unknowns[i].y][unknowns[i].x] = true;
                        changed = true;
                    } else if (need == u_count && need > 0) {
                        // Alle verbleibenden ? sind Minen
                        for (int i = 0; i < u_count; i++)
                            add_mine(unknowns[i].x, unknowns[i].y);
                        changed = true;
                    }
                }
            }
        }
    } while (changed);
}

// Vergleichsfunktion für qsort
int compare(const void *a, const void *b) {
    Pos *pa = (Pos *)a, *pb = (Pos *)b;
    if (pa->x != pb->x) return pa->x - pb->x;
    return pa->y - pb->y;
}

// === Hauptprogramm ===
int main() {
    // Eingabe: Höhe, Breite, Anzahl Minen
    scanf("%d %d", &h, &w);
    scanf("%d", &nb);
    getchar();  // Zeilenumbruch einlesen

    // Spielfeld einlesen
    for (int i = 0; i < h; i++)
        fgets(grid[i], MAX, stdin);

    // 1. Schritt: sichere Felder durch '.' identifizieren
    mark_safe_from_dots();

    // 2. Schritt: deduktive Analyse
    deduce_mines();

    // 3. Schritt: restliche Minen auf nicht-sichere ? verteilen
    int remaining = nb - mine_count;
    for (int y = 0; y < h && remaining > 0; y++) {
        for (int x = 0; x < w && remaining > 0; x++) {
            if (grid[y][x] == '?' && !safe[y][x] && !is_mine[y][x]) {
                add_mine(x, y);
                remaining--;
            }
        }
    }

    // Ausgabe sortieren und drucken
    qsort(mines, mine_count, sizeof(Pos), compare);

    for (int i = 0; i < mine_count; i++)
        printf("%d %d\n", mines[i].x, mines[i].y);

    return 0;
}
