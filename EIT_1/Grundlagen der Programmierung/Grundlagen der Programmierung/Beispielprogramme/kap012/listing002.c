// kap012/listing002.c
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
    printf("\n\nDer Song den Sie eingaben:\n");
    printf("Titel     :  %s", song->titel);
    printf("Kuenstler :  %s", song->kuenstler);
    printf("Album     :  %s", song->album);
    printf("Jahr      :  %hd\n", song->jahr);
    printf("Kommentar :  %s", song->kommentar);
    printf("Genere    :  %s", song->genere);
  }
}

Id3_t *new_id3(void) {
  Id3_t* data = malloc(sizeof(Id3_t));
  if( data == NULL ) {
    printf("Fehler bei der Speicheranforderung\n");
    return NULL;
  }
  printf("Titel     :  ");
  if(fgets(data->titel,sizeof(data->titel),stdin)==NULL) {
    printf("Fehler bei der Eingabe\n");
    free(data);  // Speicher vorher freigeben
    return NULL; // NULL-Zeiger zurück
  }
  printf("Kuenstler :  ");
  if(fgets( data->kuenstler,
            sizeof(data->kuenstler), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    free(data);
    return NULL;
  }
  printf("Album     :  ");
  if(fgets(data->album,sizeof(data->album),stdin)==NULL) {
    printf("Fehler bei der Eingabe\n");
    free(data);
    return NULL;
  }
  printf("Jahr      :  ");
  if (scanf("%hd", &data->jahr) != 1 ) {
    printf("Fehler bei der Eingabe\n");
    free(data);
    return NULL;
  }
  dump_buffer(stdin);
  printf("Kommentar :  ");
  if(fgets( data->kommentar,
            sizeof(data->kommentar), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    free(data);
    return NULL;
  }
  printf("Genere    :  ");
  if(fgets(data->genere,sizeof(data->genere),stdin)==NULL){
    printf("Fehler bei der Eingabe\n");
    free(data);
    return NULL;
  }
  return data; 
}

int main(void) {
  Id3_t *data = new_id3();
  if( data != NULL ) {
    print_id3(data);
  }
  free(data); // auch im Fall von NULL kein Problem
  return EXIT_SUCCESS;
}
