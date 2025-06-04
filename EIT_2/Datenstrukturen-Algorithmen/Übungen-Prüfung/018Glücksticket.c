#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define MaxTickets 10

int checkTicket(int n) {
    int Ziffer[6] = { 0 };
    int count = 5;
    int erste3Ziffer = 0;
    int letzte3Ziffer = 0;

    while (n > 0 && count >= 0) {
        Ziffer[count] = (n % 10);
        n /= 10;
        count--;
    }

    for (int i = 0; i < 3; i++) {
        erste3Ziffer += Ziffer[i];
    }

    for (int i = 3; i < 6; i++) {
        letzte3Ziffer += Ziffer[i];
    }

    if (erste3Ziffer == letzte3Ziffer)
        return 1;
    else
        return 0;
}

int main() {
    int N = MaxTickets;
    int funktioniertTicket = 0;
    int TicketNumber = 0;

    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        scanf("%d", &TicketNumber);

        funktioniertTicket = checkTicket(TicketNumber);

        if (funktioniertTicket == 1)
            printf("true\n");
        else
            printf("false\n");
    }

    return 0;
}