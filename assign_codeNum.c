// assign_codeNum.c CANOSTコードのコード番号をふる関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;
extern int canost_codes;

// #define TEST_ASSIGN_CODENUM 1

#ifdef TEST_ASSIGN_CODENUM
extern FILE *dummyfile;
#endif

int assign_codeNum( void ){
 
  int i,j;

  FILE *f_prior;
  char in_line[MAX_LINE];
  char codename[5];

#ifdef TEST_ASSIGN_CODENUM
  printf("assign_codeNum \n");
#endif

  // CANOSTコードの優先順位を登録したcode.priorを開く
  if((f_prior=fopen("code.prior","r"))==NULL){
    printf(" Cannot read priority of CANOST codes \n");
    exit( EXIT_FAILURE );
  }

  j=0;
  while( TRUE ){
    if( fgets(in_line,MAX_LINE,f_prior) == NULL ){
      break;
    }
    sscanf(in_line,"%s",codename);
    j++;
    // 該当するコードを見付けコード番号(順位)をふる
    for( i=1 ; i < iatm ; i++){
      if(strcmp(codename,a1st[i].code)==0){
	a1st[i].codeNum=j;
      }
    }
  }
  canost_codes=j;

  //  printf("%s \n",codename);

  fclose(f_prior);

#ifdef TEST_ASSIGN_CODENUM
  dummyfile=fopen("dummy_assign_codeNum.dat","w");

  for( i=1 ; i < iatm ; i++){
    fprintf(dummyfile,"%d %s %5d\n",i,a1st[i].code,a1st[i].codeNum);
  }

  fclose(dummyfile);
#endif

  return EXIT_SUCCESS; 

}

