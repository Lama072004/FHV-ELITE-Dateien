// kap012/listing001.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 64

typedef struct Id3_tag {
  char titel[MAX];
  char kuenstler[MAX];
  char album[MAX];
  short jahr;
  char kommentar[MAX];
  char genere[MAX];
} Id3_t;

void output(Id3_t song) {
  printf("\n\nDer Song den Sie eingaben:\n");
  printf("Titel     :  %s", song.titel);
  printf("Künstler  :  %s", song.kuenstler);
  printf("Album     :  %s", song.album);
  printf("Jahr      :  %hd\n", song.jahr);
  printf("Kommentar :  %s", song.kommentar);
  printf("Genere    :  %s", song.genere);
}

void dump_buffer(FILE *fp) {
  int ch;
  while( (ch = fgetc(fp)) != EOF && ch != '\n' )
    /* Kein Anweisungsblock nötig */ ;
}

int main(void) {
  Id3_t data;
  printf("Titel     :  ");
  if(fgets(data.titel,sizeof(data.titel),stdin)==NULL) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Kuenstler :  ");
  if(fgets(data.kuenstler,sizeof(data.kuenstler),stdin)
         ==NULL){
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Album     :  ");
  if(fgets(data.album,sizeof(data.album),stdin)==NULL) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Jahr      :  ");
  if (scanf("%hd", &data.jahr) != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  dump_buffer(stdin);
  printf("Kommentar :  ");
  if(fgets(data.kommentar,sizeof(data.kommentar),stdin)
         == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Genere    :  ");
  if(fgets(data.genere,sizeof(data.genere),stdin)==NULL) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  // Daten der Struktur ausgeben
  output(data);
  return EXIT_SUCCESS;
}
