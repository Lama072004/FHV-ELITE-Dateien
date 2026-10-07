#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_W 10000
#define MAX_H 10000

typedef struct {
    int x;
    int y;
} Point;

bool visited[MAX_W][MAX_H];  // Nur bei sehr großen Feldern kann das kritisch werden

int main() {
    int W, H;
    scanf("%d%d", &W, &H);
    int N;
    scanf("%d", &N);
    int X0, Y0;
    scanf("%d%d", &X0, &Y0);

    int left = 0, right = W - 1;
    int top = 0, bottom = H - 1;

    Point prev = {X0, Y0};
    Point current = {X0, Y0};
    Point next;

    memset(visited, 0, sizeof(visited));
    visited[current.x][current.y] = true;

    char bomb_dir[16];
    int turn = 0;
    int stuck_count = 0;

    while (1) {
        scanf("%s", bomb_dir);

        if (turn == 0) {
            // Erste Bewegung: zur anderen Ecke springen
            next.x = W - 1 - current.x;
            next.y = H - 1 - current.y;
        } else {
            if (strcmp(bomb_dir, "WARMER") == 0) {
                if (current.x != prev.x) {
                    if (current.x > prev.x)
                        left = (prev.x + current.x + 1) / 2;
                    else
                        right = (prev.x + current.x) / 2;
                }
                if (current.y != prev.y) {
                    if (current.y > prev.y)
                        top = (prev.y + current.y + 1) / 2;
                    else
                        bottom = (prev.y + current.y) / 2;
                }
            } else if (strcmp(bomb_dir, "COLDER") == 0) {
                if (current.x != prev.x) {
                    if (current.x > prev.x)
                        right = (prev.x + current.x) / 2;
                    else
                        left = (prev.x + current.x + 1) / 2;
                }
                if (current.y != prev.y) {
                    if (current.y > prev.y)
                        bottom = (prev.y + current.y) / 2;
                    else
                        top = (prev.y + current.y + 1) / 2;
                }
            } else if (strcmp(bomb_dir, "SAME") == 0) {
                if (current.x != prev.x) {
                    int mid = (prev.x + current.x) / 2;
                    left = right = mid;
                }
                if (current.y != prev.y) {
                    int mid = (prev.y + current.y) / 2;
                    top = bottom = mid;
                }
            }

            next.x = (left + right) / 2;
            next.y = (top + bottom) / 2;

            if (next.x == current.x && next.y == current.y) {
                stuck_count++;

                if (stuck_count >= 2) {
                    // Versuche ein unbesuchtes benachbartes Feld
                    bool moved = false;
                    for (int dx = -1; dx <= 1 && !moved; dx++) {
                        for (int dy = -1; dy <= 1 && !moved; dy++) {
                            int nx = current.x + dx;
                            int ny = current.y + dy;
                            if (nx >= 0 && nx < W && ny >= 0 && ny < H && !visited[nx][ny]) {
                                next.x = nx;
                                next.y = ny;
                                moved = true;
                            }
                        }
                    }
                }
            } else {
                stuck_count = 0; // Bewegung erfolgreich
            }
        }

        printf("%d %d\n", next.x, next.y);
        fflush(stdout);

        visited[next.x][next.y] = true;
        prev = current;
        current = next;
        turn++;
    }

    return 0;
}
