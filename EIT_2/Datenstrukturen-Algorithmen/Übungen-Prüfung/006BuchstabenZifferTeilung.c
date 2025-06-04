#include <stdio.h>
#include <ctype.h>

int main() {
    char input[257];
    fgets(input, sizeof(input), stdin);

    int letters = 0;
    int digits = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            letters++;
        }
        else if (isdigit(input[i])) {
            digits++;
        }
    }

    int result = letters / digits;
    printf("%d\n", result);

    return 0;
}
