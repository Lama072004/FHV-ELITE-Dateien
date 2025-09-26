// kap012/listing003.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 64
#define SIZE 3

typedef struct Id3_tag {
  char titel[MAX];
  char kuenstler[MAX];
  char album[MAX];
  short jahr;
  char kommentar[MAX];
  char genere[MAX];
} Id3_t;

void dump_buffer(FILE *fp) {
  int ch;
  while( (ch = fgetc(fp)) != EOF && ch != '\n' )
    /* Kein Anweisungsblock nötig */ ;
}

void print_id3(Id3_t* song) {
  if(song == NULL ) {
    printf("Kein Inhalt gefunden\n");
  }
  else {
    printf("\n\nAusgabe der ID3-Daten:\n");
    printf("Titel     :  %s", song->titel);
    printf("Kuenstler :  %s", song->kuenstler);
    printf("Album     :  %s", song->album);
    printf("Jahr      :  %hd\n", song->jahr);
    printf("Kommentar :  %s", song->kommentar);
    printf("Genere    :  %s", song->genere);
  }
}

int main(void) {
  Id3_t data[SIZE];
  for(int i=0; i<SIZE; i++) {
    printf("Titel     :  ");
    if(fgets( data[i].titel,
              sizeof(data[i].titel),stdin) == NULL ) {
      printf("Fehler bei der Eingabe\n");
      return EXIT_FAILURE;
    }
    printf("Kuenstler :  ");
    if(fgets(data[i].kuenstler,
             sizeof(data[i].kuenstler), stdin) == NULL ) {
      printf("Fehler bei der Eingabe\n");
      return EXIT_FAILURE;
    }
    printf("Album     :  ");
    if( fgets(data[i].album,
              sizeof(data[i].album), stdin) == NULL ) {
      printf("Fehler bei der Eingabe\n");
      return EXIT_FAILURE;
    }
    printf("Jahr      :  ");
    if (scanf("%hd", &data[i].jahr) != 1 ) {
      printf("Fehler bei der Eingabe\n");
     return EXIT_FAILURE;
    }
    dump_buffer(stdin);
    printf("Kommentar :  ");
    if( fgets(data[i].kommentar,
              sizeof(data[i].kommentar), stdin) == NULL ) {
      printf("Fehler bei der Eingabe\n");
      return EXIT_FAILURE;
    }
    printf("Genere    :  ");
    if( fgets(data[i].genere,
              sizeof(data[i].genere), stdin) == NULL ) {
      printf("Fehler bei der Eingabe\n");
      return EXIT_FAILURE;
    }
  }
  for(int i = 0; i<SIZE; i++ ) {
    print_id3(&data[i]);
  }
  return EXIT_SUCCESS;
}
