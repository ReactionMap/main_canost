// set_children.c レベル 2 の原子の順位を固定する関数 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_SET_CHILDREN 1

extern struct atom *a1st;
extern struct level root;
extern struct level *top;

int set_children( int *comrade, int comNum, int *standard, int iso_type ){

#ifdef TEST_SET_CHILDREN
  printf(" set_children \n");
#endif

  if( iso_type == FIRST_THREE || iso_type == THIRD_THREE ){

#ifdef TEST_SET_CHILDREN
    printf(" set_children (1) \n");
#endif

    // castコードに基づく順位付け
    if( geometric_order( comrade, comNum ) != EXIT_SUCCESS ){
      printf(" error : Cannot execute geometric_order \n");
      exit( EXIT_FAILURE );
    }

    if( standard != NULL && iso_type == THIRD_THREE ){
      if( a1st[ comrade[0] ].grade < a1st[ comrade[1] ].grade ){
	*standard = comrade[0];
      }else{
	*standard = comrade[1];
      }
    }

    top = &root;
    if( ordering() != EXIT_SUCCESS ){
      printf(" error : Cannot execute ordering \n");
      exit( EXIT_FAILURE );
    }

  }else if( iso_type == TIE_TWO || iso_type == TIE_THREE ){

#ifdef TEST_SET_CHILDREN
    printf("set_children (2) \n");
#endif

    // castコードに基づく順位付け
    if( geometric_order( comrade, comNum ) != EXIT_SUCCESS ){
      printf(" error : Cannot execute geometric_order \n");
      exit( EXIT_FAILURE );
    }

    if( standard != NULL ){
      if( iso_type == TIE_TWO ){
	if( a1st[ comrade[0] ].grade < a1st[ comrade[1] ].grade ){
	  *standard = comrade[0];
	}else{
	  *standard = comrade[1];
	}
      }

      if( iso_type == TIE_THREE ){
	if( a1st[ comrade[0] ].grade < a1st[ comrade[1] ].grade ){
	  if( a1st[ comrade[0] ].grade < a1st[ comrade[2] ].grade ){
	    *standard = comrade[0];
	  }else{
	    *standard = comrade[2];
	  }
	}else{
	  if( a1st[ comrade[1] ].grade < a1st[ comrade[2] ].grade ){
	    *standard = comrade[1];
	  }else{
	    *standard = comrade[2];
	  }
	}
      }
    }

    top = &root;
    if( ordering() != EXIT_SUCCESS ){
      printf(" error : Cannot execute ordering \n");
      exit( EXIT_FAILURE );
    }

  }else{
    printf(" error : Error occured in set_children \n");
    exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;

}
