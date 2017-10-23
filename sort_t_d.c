// sort_t_d.c トップダウンの整列をする関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_SORT_TOP_DOWN 1

#ifdef TEST_SORT_TOP_DOWN
extern FILE *dummyfile;
#endif

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;
extern struct level *top,*down;
extern int equive;
extern int destination;

int sort_t_d( void ){

  int i;
  struct level *lp;
  int *comps; // 新に決まった順位
  int *chain; // ひとつの層の原子
  int lexico;
  int length;
  int upper;
  int lower;

#ifdef TEST_SORT_TOP_DOWN
  printf("sort_t_d \n");
#endif

  lp=NULL;
  comps=NULL;
  chain=NULL;

  destination=1; // トップダウンの整列
  upper=top->member_Num;
  for( lp = top->next ; lp != down ; lp = lp->next ){
    chain=lp->member;
    length=lp->member_Num;
    lower=(lp->next)->member_Num;
    if( update_tuple(chain,length,upper,lower) != EXIT_SUCCESS ){
      printf("error : Cannot execute update tuple \n");
      exit( EXIT_FAILURE );
    }
    if( lexsort(chain,length,upper,lower) != EXIT_SUCCESS ){
      printf(" error : Cannot execute lexicographic sort \n");
      exit( EXIT_FAILURE );
    }
    upper=lp->member_Num;

    // 順位付け    
    lexico=0;
    if( (comps=(int *)realloc(comps,length*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in sort_t_d \n");
      exit( EXIT_FAILURE );
    }
    comps[0] = 0;
    equive++;
    for(i=1;i<length;i++){
      if( a1st[ chain[i-1] ].grade == a1st[ chain[i] ].grade &&
          memcmp( a1st[ chain[i-1] ].tuple_parent,a1st[ chain[i] ].tuple_parent,TUPLE_SIZE*sizeof(int))== 0 &&
          memcmp( a1st[ chain[i-1] ].tuple_child,a1st[ chain[i] ].tuple_child,TUPLE_SIZE*sizeof(int))== 0 ){
	comps[i] = lexico;
	if( !( a1st[ chain[i] ].link == CAVE || a1st[ chain[i] ].link == STEM || 
               a1st[ chain[i] ].link == KNOT || a1st[ chain[i] ].link == TRUNK ) ){ 
	  equive++;
	}
      }else{
        lexico=i;
        comps[i] = lexico;
	equive++;
      }
    }

    for(i=0;i<length;i++){
      a1st[ chain[i] ].grade = comps[i];
    }

  }

  free(comps);

#ifdef TEST_SORT_TOP_DOWN
  dummyfile=fopen("dummy_sort_t_d.dat","w");

  for( lp=top ; lp != down ; lp=lp->next){
    fprintf(dummyfile,"%d \n\n",lp->member_Num );
    chain=lp->member;
    for( i=0 ; i < lp->member_Num ; i++ ){
      fprintf(dummyfile,"%4d %4d %4d\n",chain[i], a1st[ chain[i] ].grade, a1st[ chain[i] ].codeNum );
    }
    fprintf(dummyfile,"\n\n");
  }

  fclose(dummyfile);

  printf("sort_t_d finish \n");
#endif

  return EXIT_SUCCESS;

}
