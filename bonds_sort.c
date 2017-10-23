// bonds_sort.c 各原子の持つ結合を順位順に整列

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

//#define TEST_BONDS_SORT 1

extern int iatm;
extern struct atom *a1st;
extern int *confined_bond;
extern int boundary;

int bonds_sort( void ){

  int i;

  struct bond *new_bond;
  struct bond *competitor;
  struct bond *target;
  struct bond *pre_target;

#ifdef TEST_BONDS_SORT
  printf("bonds_sort \n");
#endif

  for( i=1; i < iatm ;i++ ){
    if( a1st[i].layer <= boundary && confined_bond[i] > 1 ){
      new_bond = a1st[i].adj_list->next;
      a1st[i].adj_list->next = NULL;

      while( new_bond != NULL ){
	competitor = new_bond;
	new_bond = new_bond->next;
	competitor->next = NULL;
        target = a1st[i].adj_list;
	pre_target = a1st[i].adj_list;
	while( target != NULL ){
	  if(  a1st[ competitor->alias ].grade < a1st[ target->alias ].grade ){
	    if( target == a1st[i].adj_list && pre_target == a1st[i].adj_list ){
	      a1st[i].adj_list = competitor;
	    }else{
	      pre_target->next = competitor;
	    }
	    competitor->next = target;
	    break;
	  }else{
	    pre_target = target;
	    target=target->next;
	  }
	}
	if( target == NULL ){
	  pre_target->next = competitor;
	}
      }

      target = a1st[i].adj_list;
      while( target->next != NULL ){
	target = target->next;
      }
      a1st[i].last = target;

    }

#ifdef TEST_BONDS_SORT
    target = a1st[i].adj_list;
    printf("%d :",i);
    while( target != NULL ){
      printf(" %d",target->alias );
      target=target->next;
    }
    printf(" last %d",a1st[i].last->alias);
    printf("\n");
#endif

  }

  return EXIT_SUCCESS;

}
