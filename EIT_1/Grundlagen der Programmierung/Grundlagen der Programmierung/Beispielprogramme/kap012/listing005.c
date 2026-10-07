// kap012/listing005.c
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
  Info_t *info;  // Zeiger als Komponente
} Id3_t;


/*typedef struct Meta_Info {
  Id3_t id3;
  Info_t info;
} Meta_t;*/

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
	 printf("\n\n");
     printf("Titel     :  %s", song->titel);
     printf("Kuenstler :  %s", song->kuenstler);
     printf("Album     :  %s", song->album);
     printf("Jahr      :  %hd\n", song->jahr);
     printf("Kommentar :  %s", song->kommentar);
     printf("Genere    :  %s", song->genere);
	 if(song->info != NULL ) {
       printf("Bitrate   :  %hu\n", song->info->bitrate);
       printf("Spielzeit :  %hu\n", song->info->spielzeit);
       printf("Abtastrate:  %hu\n", song->info->abtastrate);
       printf("Version   :  %s", song->info->version);
     }
   }
}

int main(void) {
  Id3_t data;
  printf("Titel     :  ");
  if( fgets(data.titel, sizeof(data.titel), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Kuenstler :  ");
  if( fgets(data.kuenstler, sizeof(data.kuenstler), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  printf("Album     :  ");
  if( fgets(data.album, sizeof(data.album), stdin) == NULL ) {
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
  if( fgets(data.kommentar, sizeof(data.kommentar), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
	return EXIT_FAILURE;
  }
  printf("Genere    :  ");
  if( fgets(data.genere, sizeof(data.genere), stdin) == NULL ) {
    printf("Fehler bei der Eingabe\n");
	return EXIT_FAILURE;
  }
  data.info = malloc(sizeof(Info_t));
  if(data.info != NULL ) {
    printf("Bitrate   :  ");
    if (scanf("%hu", &data.info->bitrate) != 1 ) {
      printf("Fehler bei der Eingabe\n");
	  return EXIT_FAILURE;
    }
    dump_buffer(stdin);
    printf("Dauer(sec):  ");
    if (scanf("%hu", &data.info->spielzeit) != 1 ) {
      printf("Fehler bei der Eingabe\n");
	  return EXIT_FAILURE;
    }
    dump_buffer(stdin);
    printf("Abtastrate:  ");
    if (scanf("%hu", &data.info->abtastrate) != 1 ) {
      printf("Fehler bei der Eingabe\n");
	  return EXIT_FAILURE;
    }
    dump_buffer(stdin);
    printf("Version   :  ");
    if( fgets(data.info->version, sizeof(data.info->version), stdin) == NULL ) {
      printf("Fehler bei der Eingabe\n");
	  return EXIT_FAILURE;
    }
  }
  else {
    data.info = NULL; 
  }
  print_id3(&data);
  free(data.info);
  return EXIT_SUCCESS;
}



