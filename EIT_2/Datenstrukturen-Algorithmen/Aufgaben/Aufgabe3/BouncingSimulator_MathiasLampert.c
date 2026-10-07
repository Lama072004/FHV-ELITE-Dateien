#include <stdio.h>

int main() {
    int w, h, n;
    scanf("%d %d %d", &w, &h, &n);

    char grid[h + 2][w + 2];

    // Initialize grid with spaces.
    for (int i = 0; i < h + 2; i++) {
        for (int j = 0; j < w + 2; j++) {
            grid[i][j] = ' ';
        }
    }

    // Set boundaries using '#'.
    for (int i = 0; i < h + 2; i++) {
        grid[i][0] = '#';
        grid[i][w + 1] = '#';
    }
    for (int j = 0; j < w + 2; j++) {
        grid[0][j] = '#';
        grid[h + 1][j] = '#';
    }

    // Special case: single column.
    if (w == 1) {
        int y = 1, dy = 1;
        while (n > 0) {
            // Mark current cell (incrementing if already marked)
            if (grid[y][1] == ' ')
                grid[y][1] = '1';
            else if (grid[y][1] < '9')
                grid[y][1]++;

            int ny = y + dy;
            // If the next cell is out-of-bound, bounce:
            if (ny < 1 || ny > h) {
                dy = -dy;
                y = y + dy;
            } else {
                y = ny;
            }
            n--;
        }
    }
    // Special case: single row.
    else if (h == 1) {
        int x = 1, dx = 1;
        while (n > 0) {
            if (grid[1][x] == ' ')
                grid[1][x] = '1';
            else if (grid[1][x] < '9')
                grid[1][x]++;

            int nx = x + dx;
            if (nx < 1 || nx > w) {
                dx = -dx;
                x = x + dx;
            } else {
                x = nx;
            }
            n--;
        }
    }
    // General case: grid with more than one row and column.
    else {
        int x = 1, y = 1, dx = 1, dy = 1;
        while (n > 0) {
            if (grid[y][x] == ' ') {
                grid[y][x] = '1';
            } else if (grid[y][x] < '9') {
                grid[y][x]++;
            }

            int nx = x + dx;
            int ny = y + dy;

            int bounced = 0;
            if (nx < 1 || nx > w) {
                dx = -dx;
                bounced = 1;
            }
            if (ny < 1 || ny > h) {
                dy = -dy;
                bounced = 1;
            }

            if (bounced) {
                n--;
            }
            x += dx;
            y += dy;
        }
    }

    // Print the final grid.
    for (int i = 0; i < h + 2; i++) {
        for (int j = 0; j < w + 2; j++) {
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }

    return 0;
}
