#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAXSIZE 100

int compare_substrings(const char* a, const char* b, int len) {
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

int main() {
    char input[MAXSIZE];
    printf("Enter the string: ");
    scanf("%s", input);

    for (int i = 0; i < MAXSIZE; i++) {
        input[i] = tolower(input[i]);
    }

    uint32_t length = strlen(input);
    uint32_t maxLen = 0;
    int32_t startIndex = -1;

    for (int len = 1; len < length; len++) {
        for (int i = 0; i + len <= length; i++) {
            for (int j = i + 1; j + len <= length; j++) {
                if (compare_substrings(&input[i], &input[j], len)) {
                    if (len > maxLen) {
                        maxLen = len;
                        startIndex = i;
                    }
                }
            }
        }
    }

    if (maxLen > 0) {
        char result[MAXSIZE];
        strncpy(result, &input[startIndex], maxLen);
        result[maxLen] = '\0';
        printf("%s\n", result);
    }
    else {
        printf("None\n");
    }
}