#include <stdio.h>

int main() {
    int AmountNumbers;
    scanf_s("%d", &AmountNumbers);

    int numbers[100];  // Maximal 100 Elemente gem‰ﬂ Einschr‰nkung
    for (int i = 0; i < AmountNumbers; i++) {
        scanf_s("%d", &numbers[i]);
    }

    int AmountRange;
    scanf_s("%d", &AmountRange);

    for (int i = 0; i < AmountRange; i++) {
        int startRange, endRange;
        scanf_s("%d %d", &startRange, &endRange);

        int sum = 0;
        for (int j = startRange; j <= endRange; j++) {
            sum += numbers[j];
        }

        printf("%d\n", sum);
    }

    return 0;
}
