#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 25

typedef struct {
    int x, y;
} Point;

int W, H;
char maze[MAX][MAX];
bool visited[MAX][MAX];
Point exits[MAX * MAX];
int exit_count = 0;

// Richtung: rechts, links, unten, oben
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

bool is_inside(int x, int y) {
    return x >= 0 && x < W && y >= 0 && y < H;
}

bool is_exit(int x, int y) {
    return (x == 0 || y == 0 || x == W - 1 || y == H - 1) && maze[y][x] == '.';
}

void add_exit(int x, int y) {
    // Nur hinzufügen, wenn noch nicht vorhanden
    for (int i = 0; i < exit_count; i++) {
        if (exits[i].x == x && exits[i].y == y) return;
    }
    exits[exit_count].x = x;
    exits[exit_count].y = y;
    exit_count++;
}

void bfs(int startX, int startY) {
    Point queue[MAX * MAX];
    int front = 0, back = 0;

    queue[back++] = (Point){startX, startY};
    visited[startY][startX] = true;

    while (front < back) {
        Point p = queue[front++];

        if (is_exit(p.x, p.y)) {
            add_exit(p.x, p.y);
        }

        for (int d = 0; d < 4; d++) {
            int nx = p.x + dx[d];
            int ny = p.y + dy[d];

            if (is_inside(nx, ny) && !visited[ny][nx] && maze[ny][nx] == '.') {
                visited[ny][nx] = true;
                queue[back++] = (Point){nx, ny};
            }
        }
    }
}

int compare_points(const void *a, const void *b) {
    Point *pa = (Point *)a;
    Point *pb = (Point *)b;
    if (pa->x != pb->x) return pa->x - pb->x;
    return pa->y - pb->y;
}

int main() {
    int startX, startY;
    scanf("%d%d", &W, &H);
    scanf("%d%d", &startX, &startY);
    fgetc(stdin);

    for (int i = 0; i < H; i++) {
        fgets(maze[i], sizeof(maze[i]), stdin);
        // Entferne evtl. Zeilenumbruch
        maze[i][strcspn(maze[i], "\n")] = '\0';
    }

    bfs(startX, startY);

    qsort(exits, exit_count, sizeof(Point), compare_points);

    printf("%d\n", exit_count);
    for (int i = 0; i < exit_count; i++) {
        printf("%d %d\n", exits[i].x, exits[i].y);
    }

    return 0;
}
