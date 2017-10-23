// set_atom.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

//#define TEST_SET_ATOM 1

extern int iatm;
extern int new_atm;
extern struct atom *a1st;
extern struct level root;
extern struct level *top;
extern struct level *reef;
extern int ore;
extern int *cast_codes;
extern int *direct_parent;
extern int *relative_parent;
extern int *relation;
extern struct queue *head,*tail;

int set_atom( void ){

  int i;
  
  int *pile;

  pile=NULL;

#ifdef TEST_SET_ATOM
  printf("set_atom \n");
#endif

  /*** 初期化 ***/
  for(i=1;i<iatm+new_atm;i++){
    direct_parent[i] = MINUS;
    relative_parent[i] = MINUS;
    relation[i] = MINUS;
    cast_codes[i] = MINUS;
  }

  /**** conformation 依存の順位を持つ原子を入れるQueueの初期化 ***/
  head=NULL;
  tail=NULL;
  initialize_primary_queue();

  top = &root;
  reef = top->next;
  if( reef != NULL ){ // レベル 1 以上
    ore = 0;
    if( set_upper_level( ) != EXIT_SUCCESS ){
      printf(" error : Cannot execute set_upper_level \n");
      exit( EXIT_FAILURE );
    }
  }

  return EXIT_SUCCESS;

}
