#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Function to check if a character is a digit
bool IsDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

// Function to perform Luhn algorithm validation
bool IsValidCard(const char* card) {
    int sum = 0, count = 0;

    // Traverse from the end, skipping non-digits
    for (int i = strlen(card) - 1; i >= 0; i--) {
        if (IsDigit(card[i])) {
            int digit = card[i] - '0';

            // Double every second digit
            if (count % 2 == 1) {
                digit *= 2;
                if (digit > 9) {
                    digit -= 9;
                }
            }

            sum += digit;
            count++;
        }
    }

    return (sum % 10 == 0);
}

int main() {
    int n;
    scanf("%d ", &n);  // Read the number of cards

    for (int i = 0; i < n; i++) {
        char card[21];
        fgets(card, sizeof(card), stdin);

        if (IsValidCard(card)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }

    return 0;
}
