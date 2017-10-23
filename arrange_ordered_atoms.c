// arrange_ordered_atoms.c 
/****************************************************
 レベル 1 の層は完全に順位が決定済み 
 それを利用したレベル 2 の層の順位付けをする関数
*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_ARRANGE_ORDERED_ATOM 1

extern struct atom *a1st;
extern int start;
extern int *cast_codes;
extern int *relative_parent;
extern int *relation;
extern int *confined_bond;

int arrange_ordered_atoms( int *ranker, int rank ){

  int i;

  struct bond *bp;

  int *comrade;
  int comNum;

  int iso_type;
  int scanned;
  int standard;
  int new_standard;

#ifdef TEST_ARRANGE_ORDERED_ATOM
  printf(" arrange_ordered_atom \n");
#endif

  bp = NULL;

  iso_type = MINUS;
  scanned = MINUS;
  standard = MINUS;
  new_standard = MINUS;

  if( (comrade=(int *)malloc(TUPLE_SIZE*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in arrange_ordered_atoms \n");
    exit( EXIT_FAILURE );
  }
  comNum = 0;
  
  for( i=0; i < rank ; i++ ){
    scanned = MINUS;
    new_standard = ranker[i];
    cast_codes[ new_standard ] = UK;

    if( i == 0 ){
      standard = ranker[1];
    }else{
      standard = ranker[0];
    }

    while( confined_bond[ new_standard ] >= 2 ){
      scanned = new_standard;
      iso_type = scan_children( scanned, &new_standard, comrade, &comNum );

      if( iso_type == FIRST_ONE ){
        // start -- scanned -- new_standard の成す角度
	relative_parent[ scanned ] = start;

        if( linearity( start, scanned, new_standard ) == FALSE ){ // 一直線ではない
	  cast_codes[ new_standard ] = get_dihedral_angle( standard, start, scanned, new_standard );
	  break;
	}else{ // 一直線
	  cast_codes[ new_standard ] = UK;
	}

      }else{

        if( iso_type == FIRST_THREE || iso_type == THIRD_THREE ||
            iso_type == TIE_TWO || iso_type == TIE_THREE ){

          // standard -- start -- scanned -- bp->alias で bp->alias にCASTコードをふる
	  for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	    if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	      cast_codes[ bp->alias ] = get_dihedral_angle( standard, start, scanned, bp->alias );
	    }
	  }

          // 順位を付ける
	  if( set_children( comrade, comNum, NULL, iso_type ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute set_children \n");
	    exit( EXIT_FAILURE );
	  }

	}else{

	  for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	    if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	      cast_codes[ bp->alias ] = UK;
	    }
	  }

	}

	break;

      }

    } // end of while( confined_bond[ new_standard ] >= 2 )

  }

  free(comrade);

  // レベル 2 の層まで順位確定
  if( apply_cast() != EXIT_SUCCESS ){
    printf(" error : Cannot execute apply_cast \n");
    exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;

}
