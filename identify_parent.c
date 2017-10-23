// identify_parent.c 親を決める

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_IDENTIFY_PARENT 1

extern int iatm;
extern int new_atm;
extern struct atom *a1st;
extern int *direct_parent;
extern int *relative_parent;
extern int *relation;

int identify_parent( int child ){

  struct bond *bp;

  for( bp=a1st[ child ].adj_list; bp!= NULL ; bp=bp->next ){
    if( a1st[ child ].layer == a1st[ bp->alias ].layer+1 ){
      if( direct_parent[ child ] == MINUS ||
	  a1st[ direct_parent[ child ] ].grade > a1st[ bp->alias ].grade ){
	direct_parent[ child ] = bp->alias;
	relative_parent[ child ] = bp->alias;
	relation[ child ] = bp->btype;
      }
    }
  }

#ifdef TEST_IDENTIFY_PARENT
  printf("direct_parent %d relation %d child %d \n",direct_parent[ child ],relation[ child ],child);
#endif

  return EXIT_SUCCESS;

}
