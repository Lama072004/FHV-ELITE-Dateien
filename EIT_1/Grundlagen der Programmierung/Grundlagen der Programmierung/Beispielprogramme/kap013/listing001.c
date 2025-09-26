// kap013/listing001.c
#include <stdio.h>
#include <stdlib.h>

void dump_buffer(FILE *fp) {
  int ch;
  while( (ch = fgetc(fp)) != EOF && ch != '\n' )
  /* Kein Anweisungsblock nötig */ ;
}

struct knoten {
  int wert;
  struct knoten *next;
};

typedef struct knoten Knoten_t;
typedef struct knoten* KnotenPtr_t;
KnotenPtr_t anfang = NULL;

void einfuegenKnoten( KnotenPtr_t neu ) {
  KnotenPtr_t hilfZeiger;
  if( anfang == NULL ) {
    anfang = neu;
    neu->next = NULL;
  }
  else {
    hilfZeiger = anfang;
    while(hilfZeiger->next != NULL) {
      hilfZeiger = hilfZeiger->next;
    }
    hilfZeiger->next = neu;
    neu->next = NULL;
  }
}

void neuerKnoten( void ) {
  KnotenPtr_t neu = malloc(sizeof(Knoten_t));
  if( neu == NULL ) {
    printf("Kein Speicher vorhanden!?\n");
    return;
  }
  printf("Neuen Wert fuer Knoten eingeben: ");
  if( scanf("%d", &neu->wert) != 1 ) {
    dump_buffer(stdin);
    printf("Fehler bei der Eingabe\n");
    free(neu); // Speicher wieder freigeben
    return;
  }
  dump_buffer(stdin);
  einfuegenKnoten( neu );
}

void loescheKnoten( int val ) {
  KnotenPtr_t hilfZeiger1;
  KnotenPtr_t hilfZeiger2;
  if( anfang != NULL ) {
    if( anfang->wert == val ) {
      hilfZeiger1 = anfang->next;
      free(anfang);
      anfang = hilfZeiger1;
    }
    else {
      hilfZeiger1 = anfang;
      while( hilfZeiger1->next != NULL ) {
        hilfZeiger2 = hilfZeiger1->next;
        if( hilfZeiger2->wert == val ) {
          hilfZeiger1->next = hilfZeiger2->next;
          free(hilfZeiger2);
          break;
        }
        hilfZeiger1 = hilfZeiger2;
      }  // Ende while
    }  // Ende else
  }  // Ende if
}

void knotenAuflisten( void ) {
  KnotenPtr_t hilfZeiger = anfang;
  printf("Elemente in der Liste:\n");
  while( hilfZeiger != NULL ) {
    printf("\t -> %d\n", hilfZeiger->wert);
    hilfZeiger = hilfZeiger->next;
  }
}

int main(void) {
  int wahl = 0, val = 0;
  do {
    printf(" -1- Neues Element hinzufuegen\n");
    printf(" -2- Element loeschen\n");
    printf(" -3- Alle Elemente auflisten\n");
    printf(" -4- Programmende\n");
    printf(" Ihre Auswahl : ");
    if( scanf("%d", &wahl) != 1 ) {
      printf("Fehlerhafte Auswahl\n");
      wahl = 0;
      dump_buffer(stdin);
    } 
    switch( wahl ) {
      case 1 : neuerKnoten(); break;
      case 2 : if( anfang == NULL ) {
                 printf("Liste ist leer!\n");
               }
               else {
                 printf("Wert zum Loeschen : ");
                 if( scanf("%d", &val) != 1 ) {
                   printf("Fehler bei der Eingabe\n");
                 }
                 else {
                   loescheKnoten( val );
                 }
               }
               break;
      case 3 : if( anfang == NULL ) {
                 printf("Liste ist leer!\n");
               }
               else {
                 knotenAuflisten();
               }
               break;
    }
  }while( wahl != 4 );
  return EXIT_SUCCESS;
}
