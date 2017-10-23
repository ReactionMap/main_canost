// canonicaliation.c 規範化のメインループ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

//#define TEST_CANONICALIZATION 1

extern int iatm;
extern int new_atm;
extern struct atom *a1st;
extern struct level *top,*down;
extern int equive;
extern struct level *reef;
extern int ore;
extern struct queue *front,*back;
extern FILE *fo;
extern int option;
extern int atoms_in_confine;
extern int *interim_order;
extern struct level *caput,*pes;

int canonicalization( void ){

  int i;
  //  int i,j;

#ifdef TEST_CANONICALIZATION
  printf("canonicalization \n");
#endif

  equive = 2; // a1st[ ZERO ] のために 2 から 

  /* ------- First Bottom-Up Sorting -------- */
  if( first_sort() != EXIT_SUCCESS ){
    printf( " error: Cannot execute first bottom-up sorting \n" );
    exit( EXIT_FAILURE );
  }

  /* ------- Top-Down Sorting ------- */
  if( sort_t_d() != EXIT_SUCCESS ){
    printf( " error: Cannot execute top-down sorting \n" );
    exit( EXIT_FAILURE );
  }

  // orderingの実行
  if( ordering() != EXIT_SUCCESS ){
    printf(" error : Cannot execute ordering \n");
    exit( EXIT_FAILURE );
  }

  if( equive == atoms_in_confine ){ // 全ての原子に異なる順位が付けられたかの判定

    if( option == SYMMETRIC ){

      if( duplicate_levels() != EXIT_SUCCESS ){
	printf(" error : Cannot execute duplicate_level \n");
	exit( EXIT_FAILURE );
      }

    }

    if( set_atom() != EXIT_SUCCESS ){
      printf(" error : Cannot set_atom \n");
      exit( EXIT_FAILURE );
    }

    if( option == SYMMETRIC ){

      for( i=0; i < iatm+new_atm ;i++ ){
	a1st[i].state = UNVISITED;
	a1st[i].grade = interim_order[i];
      }

      if( initialize_second_queue() != EXIT_SUCCESS ){
	printf(" error : Cannot initialize secondary queue \n");
	exit( EXIT_FAILURE );
      }

      reef=caput->next;
      ore=0;
      if( sym_substructure() != EXIT_SUCCESS ){
	printf(" error : Cannot execute sym_substructure \n");
	exit( EXIT_FAILURE );
      }

      if( judge_sym_substruct() != EXIT_SUCCESS ){
	printf(" error : Cannot execute judge_sym_substruct  \n");
	exit( EXIT_FAILURE );
      }

    }

  }else{  // 2層間の結合のみからなる連結成分に分解してその大きさで順位を付ける

    // split_componentの実行
    for( i=0; i < iatm+new_atm ;i++ ){
      a1st[i].state = UNVISITED;
    }

    reef=top->next;
    ore=0;
    while( equive != atoms_in_confine && reef != down ){

      if( split_component() == TRY ){
        // orderingの実行
	if( ordering() != EXIT_SUCCESS ){
	  printf(" error : Cannot execute ordering \n");
	  exit( EXIT_FAILURE );
	}
      }

    }

    if( equive == atoms_in_confine ){ // 全ての原子に異なる順位が付けられたかの判定

      if( option == SYMMETRIC ){

	if( duplicate_levels() != EXIT_SUCCESS ){
	  printf(" error : Cannot execute duplicate_level \n");
	  exit( EXIT_FAILURE );
	}

      }

      if( set_atom() != EXIT_SUCCESS ){
	printf(" error : Cannot set_atom \n");
	exit( EXIT_FAILURE );
      }

      if( option == SYMMETRIC ){

	for( i=0; i < iatm+new_atm ;i++ ){
	  a1st[i].state = UNVISITED;
	  a1st[i].grade = interim_order[i];
	}

	if( initialize_second_queue() != EXIT_SUCCESS ){
	  printf(" error : Cannot initialize secondary queue \n");
	  exit( EXIT_FAILURE );
	}

	reef=caput->next;
	ore=0;
	if( sym_substructure() != EXIT_SUCCESS ){
	  printf(" error : Cannot execute sym_substructure \n");
	  exit( EXIT_FAILURE );
	}

	if( judge_sym_substruct() != EXIT_SUCCESS ){
	  printf(" error : Cannot execute judge_sym_substruct  \n");
	  exit( EXIT_FAILURE );
	}

      }

    }else{ // 成分への分解
      // 成分の大きさで順位付け 同じ大きさの成分は同順位

      for( i=1; i < iatm+new_atm ;i++ ){
	a1st[i].state = UNVISITED;
      }

      reef=top->next;
      ore=0;
      while( equive != atoms_in_confine && reef != down ){

	if( permutable() == TRY ){

	  if( ordering() != EXIT_SUCCESS ){
	    printf(" error : Cannot execute ordering \n");
	    exit( EXIT_FAILURE );
	  }

	}
      
      }

      if( option == SYMMETRIC ){

	if( duplicate_levels() != EXIT_SUCCESS ){
	  printf(" error : Cannot execute duplicate_level \n");
	  exit( EXIT_FAILURE );
	}

      }

      if( equive == atoms_in_confine ){ // 全ての原子に異なる順位が付けられたかの判定

	if( set_atom() != EXIT_SUCCESS ){
	  printf(" error : Cannot set_atom \n");
	  exit( EXIT_FAILURE );
	}

      }else{ // intentinal_orderingへ

	for( i=1; i < iatm+new_atm ;i++ ){
	  a1st[i].state = UNVISITED;
	}

	reef=top->next;
	ore=0;
	if( intentional_ordering() != EXIT_SUCCESS ){
	  printf(" error : Cannot execute intentional_ordering \n");
	  exit( EXIT_FAILURE );
	}

      }

      if( option == SYMMETRIC ){

	for( i=0; i < iatm+new_atm ;i++ ){
	  a1st[i].state = UNVISITED;
	  a1st[i].grade = interim_order[i];
	}

	if( initialize_second_queue() != EXIT_SUCCESS ){
	  printf(" error : Cannot initialize secondary queue \n");
	  exit( EXIT_FAILURE );
	}

	reef=caput->next;
	ore=0;
	if( sym_substructure() != EXIT_SUCCESS ){
	  printf(" error : Cannot execute sym_substructure \n");
	  exit( EXIT_FAILURE );
	}

	if( judge_sym_substruct() != EXIT_SUCCESS ){
	  printf(" error : Cannot execute judge_sym_substruct  \n");
	  exit( EXIT_FAILURE );
	}

      }

    }

  }

  return EXIT_SUCCESS;
 
}
