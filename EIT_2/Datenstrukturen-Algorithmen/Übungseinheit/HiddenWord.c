#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 40

// Directions: right, left, down, up, and 4 diagonals
int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
int dy[8] = {1, -1, 0, 0, 1, -1, 1, -1};

char words[MAX][MAX + 1];
char grid[MAX][MAX + 1];
bool marked[MAX][MAX];

int word_count, height, width;

// Checks if the word matches starting at (x, y) in direction d
bool matches(int x, int y, int d, const char *word) {
    int len = strlen(word);
    for (int k = 0; k < len; k++) {
        int nx = x + dx[d] * k;
        int ny = y + dy[d] * k;
        if (nx < 0 || ny < 0 || nx >= height || ny >= width) return false;
        if (grid[nx][ny] != word[k]) return false;
    }
    return true;
}

// Marks all letters of the found word
void mark_word(int x, int y, int d, int len) {
    for (int k = 0; k < len; k++) {
        int nx = x + dx[d] * k;
        int ny = y + dy[d] * k;
        marked[nx][ny] = true;
    }
}

// Finds and marks the word (and its reverse) in the grid
void find_and_mark_word(const char *word) {
    int len = strlen(word);
    char reversed[MAX + 1];
    for (int i = 0; i < len; i++) reversed[i] = word[len - 1 - i];
    reversed[len] = '\0';

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int d = 0; d < 8; d++) {
                if (matches(i, j, d, word)) {
                    mark_word(i, j, d, len);
                    return;
                }
                if (matches(i, j, d, reversed)) {
                    mark_word(i, j, d, len);
                    return;
                }
            }
        }
    }
}

int main() {
    // Read the number of words
    scanf("%d", &word_count);
    // Read each word
    for (int i = 0; i < word_count; i++) {
        scanf("%s", words[i]);
    }
    // Read grid dimensions
    scanf("%d%d", &height, &width);

    // Read the grid
    for (int i = 0; i < height; i++) {
        scanf("%s", grid[i]);
    }
    // Initialize marked array
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            marked[i][j] = false;
        }
    }
    // Find and mark each word in the grid
    for (int i = 0; i < word_count; i++) {
        find_and_mark_word(words[i]);
    }
    // Collect and print the remaining letters
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!marked[i][j]) {
                putchar(grid[i][j]);
            }
        }
    }

    putchar('\n');
    return 0;
}