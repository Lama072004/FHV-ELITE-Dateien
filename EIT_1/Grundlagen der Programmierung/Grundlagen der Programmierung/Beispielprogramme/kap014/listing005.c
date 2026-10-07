// kap014/listing005.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF 256
#define DATAFILE "data.dat"

void killNL( char *str ) {
  size_t p = strlen(str);
  if(str[p-1] == '\n') {
    str[p-1] = '\0';
  }
}

void dump_buffer(FILE *fp) {
  int ch;
  while( (ch = fgetc(fp)) != EOF && ch != '\n' )
  /* Kein Anweisungsblock nötig */ ;
}

typedef struct _plz {
  char ort[BUF];
  unsigned int plz;
} Plz_t;

void newPLZ(void) {
  Plz_t data;
  printf("Ort          :  ");
  if( fgets(data.ort, BUF, stdin) == NULL ) {
    fprintf(stderr, "Fehler bei der Eingabe\n");
    return;
  }
  killNL(data.ort);
  printf("Postleitzahl :  ");
  if( scanf("%u", &data.plz) != 1 ) {
    fprintf(stderr, "Fehler bei der Eingabe\n");
    return;
  }
  dump_buffer(stdin);
  FILE *fp = fopen(DATAFILE, "a+b");
  if( fp == NULL ) {
    printf("Fehler beim Oeffnen: %s\n", DATAFILE);
    exit(EXIT_FAILURE);
  }
  if(fwrite(&data, sizeof(data), 1, fp) != 1) {
    fprintf(stderr,"Fehler beim Schreiben in %s\n",
       DATAFILE);
    fclose(fp);
    return;
  }
  fclose(fp);
}

void printPLZ(void) {
  Plz_t data;
  FILE *fp = fopen(DATAFILE, "r+b");
  if( fp == NULL ) {
    fprintf(stderr, "Fehler beim Oeffnen: %s\n", DATAFILE);
    return;
  }
  while(fread(&data, sizeof(data), 1, fp) == 1 ) {
    printf("\nOrt          : %s\n", data.ort);
    printf("Postleitzahl   : %u\n\n", data.plz);
  }
  fclose(fp);
}

int main(void) {
  int input = 0;
  do {
    printf("-1- Neuer Datensatz\n");
    printf("-2- Datensaetze ausgeben\n");
    printf("-3- Programm beenden\n\n");
    printf("Ihre Auswahl : ");
    if( scanf("%d", &input) != 1 ) {
      fprintf(stderr,"Fehler bei der Eingabe\n");
      input = 0;
    }
    dump_buffer(stdin);
    switch( input ) {
      case 1 : newPLZ( );   break;
      case 2 : printPLZ( ); break;
    }
  }while(input!=3);
  return EXIT_SUCCESS;
}
