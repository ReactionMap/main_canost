// duplicate_level.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

extern int start;
extern int iatm;
extern int new_atm;
extern struct atom *a1st;
extern int *interim_order;
extern struct level *top,*down;
extern struct level *caput,*pes;

int duplicate_levels( void ){

  int i;

  int *comp,*comp2;

  struct level *new_level;
  struct level *lp;

  for(i=0; i < iatm+new_atm ; i++ ){
    interim_order[i] = a1st[i].grade;
  }

  if((caput=(struct level *)malloc(sizeof(struct level)))==NULL){
    printf(" error : Cannot allocate memory in duplicate_level \n");
    exit( EXIT_FAILURE );
  }

  caput->member_Num = 1;
  caput->prev = NULL;

  if((caput->member=(int *)calloc(caput->member_Num,sizeof(int)))==NULL){
    printf(" error : Cannot allocate memory in duplicate_level \n");
    exit( EXIT_FAILURE );
  }
  *(caput->member)=start;

  if( top->next != NULL ){

    if((caput->next=(struct level *)malloc(sizeof(struct level)))==NULL){
      printf(" error : Cannot allocate memory in duplicate_level \n");
      exit( EXIT_FAILURE );
    }
 
    caput->next->prev = caput;
    new_level = caput->next;

    for( lp = top->next ; lp != down->next ; lp = lp->next ){
      if((new_level->member=(int *)malloc((lp->member_Num)*sizeof(int)))==NULL){
	printf(" error : Cannot allocate memory in duplicate_level \n");
	exit( EXIT_FAILURE );
      }
      new_level->member_Num = lp->member_Num;

      comp = new_level->member;
      comp2 = lp->member;
      for( i = 0 ; i < lp->member_Num ; i++ ){
	comp[i] = comp2[i];
      }

      if( lp != down ){
	if((new_level->next=(struct level *)malloc(sizeof(struct level)))==NULL){
	  printf(" error : Cannot allocate memory in duplicate_level \n");
	  exit( EXIT_FAILURE );
	}

        new_level->next->prev = new_level;
	new_level = new_level->next;
      }

    }
    new_level->next = NULL;
    pes = new_level;

  }else{
    caput->next = NULL;
    pes = caput;
  }

  return EXIT_SUCCESS;

}
