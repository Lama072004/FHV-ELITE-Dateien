// kap015/listing004.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINEBUF 1024

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

void countLineOut( FILE *rfp ) {
  char buf[LINEBUF];
  int count = 1;
  while( fgets(buf, LINEBUF, rfp) != NULL ) {
    printf("%3d | ", count);
    fputs( buf, stdout );
    count++;
  }
}

void search( FILE *fp ) {
  char str[LINEBUF], buf[LINEBUF];
  int count = 1;
  printf("Wonach wollen Sie suchen: ");
  if( fgets(str, LINEBUF, stdin) == NULL ) {
    fprintf(stderr, "Fehler bei der Eingabe\n");
    return;
  }
  killNL(str);
  while( fgets(buf, LINEBUF, fp) != NULL ) {
    if(strstr(buf, str) != 0) {
      printf("%3d  :  %s", count, buf);
    }
    count++;
  }
}

void copyFile( FILE *rfp, FILE *wfp ) {
  char buf[LINEBUF];
  while( fgets(buf, LINEBUF, rfp ) != NULL ) {
    if( fputs(buf, wfp) == EOF ) {
      if( ferror(wfp) ) {
        fprintf(stderr, "Fehler beim Schreiben\n");
        return;
      }
    }
  }
}

int main(void) {
  char filename1[LINEBUF], filename2[LINEBUF];
  FILE *wfp = NULL;
  int input = 0;
  printf("Datei zum Lesen: ");
  if( fgets(filename1, LINEBUF, stdin) == NULL ) {
    fprintf(stderr, "Fehler bei der Eingabe\n");
    return EXIT_SUCCESS;
  }
  killNL(filename1);
  FILE *rfp = fopen(filename1, "r");
  if( rfp == NULL ) {
    fprintf(stderr, "Fehler beim Oeffnen\n");
    return EXIT_FAILURE;
  }
  printf("Was wollen Sie mit der Datei machen?\n");
  printf("-1- Zeilen zaehlen (Bildschirmausgabe)\n");
  printf("-2- Zeilen zaehlen (in Datei schreiben)\n");
  printf("-3- Suchen\n");
  printf("-4- Kopieren\n");
  printf("Ihre Auswahl: ");
  if( scanf("%d", &input) != 1 ) {
    fprintf(stderr, "Fehler bei der Eingabe\n");
    return EXIT_FAILURE;
  }
  dump_buffer(stdin);
  switch( input ) {
    case 1 :
    case 2 : if( input == 2 ) {
               printf("Dateiname der Kopie : ");
               if(fgets(filename2,LINEBUF,stdin)==NULL) {
                 fprintf(stderr,"Fehler bei der Eingabe");
                 break;
               }
               killNL(filename2);
               wfp = freopen(filename2, "w", stdout);
               if( wfp == NULL ) {
                 fprintf(stderr, "Fehler bei Oeffnen\n");
                 break;
               }
             }
             countLineOut(rfp);
             break;
    case 3 : rfp = fopen(filename1, "r");
             if( rfp != NULL ) {
               search(rfp);
             }
             break;
    case 4 : printf("Dateiname der Kopie : ");
             if(fgets(filename2, LINEBUF, stdin)==NULL) {
               fprintf(stderr, "Fehler bei der Eingabe\n");
               break;
             }
             killNL(filename2);
             wfp = fopen(filename2, "w");
             if( wfp != NULL ) {
               copyFile( rfp, wfp );
             }
             else {
               fprintf(stderr, "Fehler beim Oeffnen\n");
             }
             break;
  }
  if( rfp !=  NULL )
    fclose(rfp);
  if( wfp != NULL )
    fclose(wfp);
  return EXIT_SUCCESS;
}
