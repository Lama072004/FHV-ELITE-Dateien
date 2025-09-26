// kap012/listing004.c
#include <stdio.h>
#include <stdlib.h>
#define MAX 64

typedef struct Details {
  unsigned short bitrate;
  unsigned short spielzeit;
  unsigned short abtastrate;
  char version[MAX];
} Info_t;

typedef struct Id3_tag {
  char titel[MAX];
  char kuenstler[MAX];
  char album[MAX];
  short jahr;
  char kommentar[MAX];
  char genere[MAX];
  Info_t info;
} Id3_t;

typedef struct Meta_Info {
  Id3_t id3;
  Info_t info;
} Meta_t;

void dump_buffer(FILE *fp) {
  int ch;
  while( (ch = fgetc(fp)) != EOF && ch != '\n' )
    /* Kein Anweisungsblock nötig */ ;
}

void print_id3(Meta_t* song) {
  if(song == NULL ) {
    printf("Kein Inhalt gefunden\n");
  }
  else {
    printf("\n\n");
    printf("Titel     :  %s", song->id3.titel);
    printf("Kuenstler :  %s", song->id3.kuenstler);
    printf("Album     :  %s", song->id3.album);
    printf("Jahr      :  %hd\n", song->id3.jahr);
    printf("Kommentar :  %s", song->id3.kommentar);
    printf("Genere    :  %s", song->id3.genere);
    printf("Bitrate   :  %hu\n", song->info.bitrate);
    printf("Spielzeit :  %hu\n", song->info.spielzeit);
    printf("Abtastrate:  %hu\n", song->info.abtastrate);
    printf("Version   :  %s", song->info.version);
  }
}

int main(void) {
  Meta_t data;
  printf("Titel     :  ");
  if( fgets(data.id3.titel,
            sizeof(data.id3.titel), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Kuenstler :  ");
  if( fgets(data.id3.kuenstler,
            sizeof(data.id3.kuenstler), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Album     :  ");
  if( fgets(data.id3.album,
            sizeof(data.id3.album), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Jahr      :  ");
  if (scanf("%hd", &data.id3.jahr) != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  dump_buffer(stdin);
  printf("Kommentar :  ");
  if( fgets(data.id3.kommentar,
            sizeof(data.id3.kommentar), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Genere    :  ");
  if( fgets(data.id3.genere,
            sizeof(data.id3.genere), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Bitrate   :  ");
  if (scanf("%hu", &data.info.bitrate) != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  dump_buffer(stdin);
  printf("Dauer(sec):  ");
  if (scanf("%hu", &data.info.spielzeit) != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  dump_buffer(stdin);
  printf("Abtastrate:  ");
  if (scanf("%hu", &data.info.abtastrate) != 1 ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  dump_buffer(stdin);
  printf("Version   :  ");
  if( fgets(data.info.version,
            sizeof(data.info.version), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  print_id3(&data);
  return EXIT_SUCCESS;
}
