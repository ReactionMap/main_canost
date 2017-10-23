// equive_search

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

//#define TEST_EQUIVE_SEARCH 1

#define CTRL_J 10

extern int iatm;

int equive_search( char *outfilename ){

  int i,j,k;
  char **ca_codes;
  char code;

  char in_line[MAX_LINE];

  int *eq_atoms;

  FILE *outfile;

  if( (ca_codes=(char **)calloc(iatm,sizeof(char *)))==NULL ){
    printf(" error : Cannot allocate memory in equive_search \n");
    exit( EXIT_FAILURE );
  }

  for(i=0;i < iatm ;i++){
    if( (ca_codes[i]=(char *)calloc(10*iatm,sizeof(char)))==NULL ){
      printf(" error : Cannot allocate memory in equive_search \n");
      exit( EXIT_FAILURE );
    }
  }

  if((outfile = fopen(outfilename,"r")) == NULL){
    printf(" error : Cannot open output-file \n");
    exit( EXIT_FAILURE );
  }
  
  for(i=1; i< iatm ; i++){

    if( fgets(in_line,MAX_LINE,outfile) == NULL ){
      printf(" error : Cannot read file (1) \n");
      exit( EXIT_FAILURE );
    } // 一行読み捨て

    j=0;
    while(TRUE){
      code = getc(outfile);
      if( code == CTRL_J ){
	ca_codes[i][j]='\0';
	break;
      }else{
	ca_codes[i][j]=code;
      }
      j++;
    } // 規範的表記の読み込み

    if( fgets(in_line,MAX_LINE,outfile) == NULL ){
      printf(" error : Cannot read file (2) \n");
      exit( EXIT_FAILURE );
    } // 一行読み捨て

    while(TRUE){
      code = getc(outfile);
      if( code == CTRL_J ){
	break;
      }
    } // 規範的番号の読み捨て

    getc(outfile); // 改行読み捨て

  }

#ifdef TEST_EQUIVE_SEARCH
  for(i=1;i<iatm;i++){
    printf("%s \n",ca_codes[i]);
  }
#endif

  fclose(outfile);

  if((outfile=fopen(outfilename,"a")) == NULL){
    printf(" error : Cannot open output-file \n");
    exit( EXIT_FAILURE );
  }

  if( (eq_atoms=(int *)calloc(iatm,sizeof(int)))==NULL ){
    printf(" error : Cannot allocate memory in equive_search \n");
    exit( EXIT_FAILURE );
  }

  for(i=1; i < iatm ; i++){
    eq_atoms[i]=i;
  }

  fprintf(outfile,"--- equivalent atoms --- \n");
  k=0;
  for(i=1; i < iatm ;i++){
    if( eq_atoms[i] == i ){
      k++;
      fprintf(outfile,"%d - %d ",k,i);
      for(j=i+1; j < iatm ; j++){
	if( eq_atoms[j] == j ){
	  if( strcmp(ca_codes[i],ca_codes[j]) == 0 ){
	    fprintf(outfile,"%d ",j);
	    eq_atoms[j]=i;
	  }
	}
      }
      fprintf(outfile,"\n");
    }
  }

#ifdef TEST_EQUIVE_SEARCH
  for(i=1;i<iatm;i++){
    printf("%d %d \n",i,eq_atoms[i]);
  }
#endif

  for(i=0;i<iatm;i++){
    free(ca_codes[i]);
  }

  free(ca_codes);

  free(eq_atoms);

  fclose(outfile);

  return EXIT_SUCCESS;

}
