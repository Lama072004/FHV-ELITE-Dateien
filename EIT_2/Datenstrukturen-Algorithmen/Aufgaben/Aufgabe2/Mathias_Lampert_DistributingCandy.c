#include <stdio.h>

// Bubble-Sort-Funktion zum Sortieren des Arrays
void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    int candies[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", &candies[i]);
    }

    // Sortiere das Array mit Bubble-Sort
    bubbleSort(candies, n);

    int min_unfairness = 200;

    for (int i = 0; i <= n - m; i++) {
        int current_unfairness = candies[i + m - 1] - candies[i];
        if (current_unfairness < min_unfairness) {
            min_unfairness = current_unfairness;
        }
    }
    printf("%d\n", min_unfairness);
    return 0;
}