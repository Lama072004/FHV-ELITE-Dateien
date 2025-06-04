#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int Quadratzahl(int A, int B){
    
    int anzahl;
    for(int i = 1; i<B; i++){
        if((i*i)<=B && (i*i)>=A){
            anzahl +=1;
        }
    }
    return anzahl;
}

int main()
{
    int A;
    int B;
    int Anzahl;
    
    scanf("%d %d",&A,&B);
    Anzahl = Quadratzahl(A,B);
    printf("%d", Anzahl);
    return 0;
}