#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>

#define max 250

int main() {
    char S[max];
    char F[max];
    char Output[max];
    int count = 0;

    fgets(S, max, stdin);
    fgets(F, max, stdin);
    

    printf("%s", S);
    printf("%s", F);

    for (int i = 0; i < max; i++) {
        if (F[i] == 'x') {
            Output[i] = tolower(S[count]);
            count++;
        }
        else if (F[i] == 'X') {
            Output[i] = toupper(S[count]);
            count++;
        }
        else Output[i] = F[i];
    }

    printf("%s", Output);

    return 0;
}
