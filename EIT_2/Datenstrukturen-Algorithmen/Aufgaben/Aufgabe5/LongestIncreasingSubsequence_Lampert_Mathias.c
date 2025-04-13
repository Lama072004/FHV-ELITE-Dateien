#include <stdio.h>

#define MAX_SIZE 2568

int main() {
    int AmountOfNumbers;
    scanf("%d", &AmountOfNumbers);
    
    int Input[MAX_SIZE];
    for (int i = 0; i < AmountOfNumbers; i++) {
        scanf("%d", &Input[i]);
    }

    int OutputArray[MAX_SIZE];
    int LongestIncreasingSubsequence = 1;

    for (int i = 0; i < AmountOfNumbers; i++) {
        OutputArray[i] = 1;
        for (int j = 0; j < i; j++) {
            if (Input[i] > Input[j] && OutputArray[i] < OutputArray[j] + 1) {
                OutputArray[i] = OutputArray[j] + 1;
            }
        }
        if (OutputArray[i] > LongestIncreasingSubsequence) {
            LongestIncreasingSubsequence = OutputArray[i];
        }
    }

    printf("%d\n", LongestIncreasingSubsequence);

    return 0;
}