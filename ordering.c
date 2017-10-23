// ordering.c 整列手続きの繰り返し

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

//#define TEST_ORDERING 1

extern int equive,pre_equive;

int ordering( void ){

#ifdef TEST_ORDERING
  int i;

  i=0;

  printf("ordering \n");
#endif

  // 規範化のための整列手続き
  equive = 2; // a1st[ ZERO ] のために 2 から
  do{
    pre_equive = equive;
    equive = 2; // a1st[ ZERO ] のために 2 から

#ifdef TEST_ORDERING
    printf("iteration %d \n",++i);
#endif

    /* ------- Bottom-Up Sorting -------- */
    if( sort_b_u() != EXIT_SUCCESS ){
      printf("error: Cannot execute bottom-up sorting \n" );
      exit( EXIT_FAILURE );
    }

    /* ------- Top-Down Sorting ------- */
    if( sort_t_d() != EXIT_SUCCESS ){
      printf("error: Cannot execute top-down sorting \n" );
      exit( EXIT_FAILURE );
    }

  }while( equive != pre_equive );

  return EXIT_SUCCESS;

}
