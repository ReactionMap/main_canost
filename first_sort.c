// first_sort.c 1回目のボトムアップの整列をする関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_FIRST_SORT 1

#ifdef TEST_FIRST_SORT
extern FILE *dummyfile;
#endif

extern int iatm;
extern int ibnd;
extern int new_atm;
extern struct atom *a1st;
extern struct bond *b1st;
extern struct level *top,*down;

int first_sort( void ){

  int i;
  struct level *lp;
  int *comps; // 新に決まった順位
  int *chain; // ひとつの層の原子
  int lexico;
  int length;
  int lower;

#ifdef TEST_FIRST_SORT
  printf("first_sort \n");
#endif

  lp=NULL;
  chain=NULL;
  comps=NULL;

  lower=0;
  for( lp = down->prev ; lp->prev != NULL ; lp = lp->prev ){
    chain=lp->member;
    length=lp->member_Num;

    if( first_update_tuple(chain,length,lower) != EXIT_SUCCESS ){
      printf("error : Cannot execute update tuple \n");
      exit( EXIT_FAILURE );
    }
    if( first_lexsort(chain,length,lower) != EXIT_SUCCESS ){
      printf(" error : Cannot execute lexicographic sort \n");
      exit( EXIT_FAILURE );
    }

    // 順位付け
    lexico=0;
    if( (comps=(int *)realloc(comps,length*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in first_sort \n");
      exit( EXIT_FAILURE );
    }
    comps[0] = 0;
    for(i=1;i<length;i++){
      /*
      if(  a1st[ chain[i-1] ].codeNum == a1st[ chain[i] ].codeNum &&
           tuple_cmp( a1st[ chain[i-1] ].tuple_child,a1st[ chain[i] ].tuple_child ) == 0 ){
      */
      if( a1st[ chain[i-1] ].codeNum == a1st[ chain[i] ].codeNum  &&
          tuple_cmp( a1st[ chain[i-1] ].tuple_child,a1st[ chain[i] ].tuple_child )==0  &&
          a1st[ chain[i-1] ].link == a1st[ chain[i] ].link ){
	comps[i] = lexico;
      }else{
        lexico=i;
        comps[i] = lexico;
      }
    }
    for(i=0;i<length;i++){
      a1st[ chain[i] ].grade = comps[i];
    }

    if( second_update_tuple(chain,length,lower) != EXIT_SUCCESS ){
      printf("error : Cannot execute update tuple \n");
      exit( EXIT_FAILURE );
    }

    lower=lp->member_Num;

  }

  free(comps);

#ifdef TEST_FIRST_SORT
  dummyfile=fopen("dummy_first_sort.dat","w");

  for( lp=top ; lp->next != NULL ; lp=lp->next){
    fprintf(dummyfile,"%d \n\n",lp->member_Num );
    chain=lp->member;
    for( i=0 ; i < lp->member_Num ; i++ ){
      fprintf(dummyfile,"%4d %4d %4d\n",chain[i],a1st[ chain[i] ].grade, a1st[ chain[i] ].codeNum );
    }
    fprintf(dummyfile,"\n\n");
  }

  fclose(dummyfile);
#endif

  return EXIT_SUCCESS;

}
