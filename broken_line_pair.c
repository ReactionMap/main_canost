// broken_line_pair.c 
/*********************************************************
   start に接続している二つの原子は折れ線になっているので
   この角度( altair -- start -- vega )を利用してレベル 2 
   の層の原子の順位を付ける    
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_BROKEN_LINE_PAIR 1

extern struct atom *a1st;
extern int start;
extern int *relative_parent;
extern int *cast_codes;
extern int *confined_bond;

int broken_line_pair( int altair, int vega ){

  int scanned;
  int standard;

  struct bond *bp;

  int iso_type_altair;
  int *comrade_altair;  
  int comNum_altair;
  int axis_type_altair;

  int iso_type_vega;
  int *comrade_vega;
  int comNum_vega;
  int axis_type_vega;

#ifdef TEST_BROKEN_LINE_PAIR
  printf("broken_line_pair \n");
#endif

  bp = NULL;

  scanned = MINUS;

  /*** comrade_***, comNum_*** のメモリ確保 ****/
  if( (comrade_altair=(int *)malloc(TUPLE_SIZE*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in broken_line_pair \n");
    exit( EXIT_FAILURE );
  }
  comNum_altair = 0;

  if( (comrade_vega=(int *)malloc(TUPLE_SIZE*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in broken_line_pair \n");
    exit( EXIT_FAILURE );
  }
  comNum_vega = 0;

  /*** altair と vega の子の順位付け ****/

  /**** altair ****/
  standard = altair;
  iso_type_altair = MINUS;
  axis_type_altair = NOAXIS_TYPE_ONE;

  cast_codes[ standard ] = UK;

  while( confined_bond[ standard ] >= 2 ){
    scanned = standard;
    iso_type_altair = scan_children( scanned, &standard, comrade_altair, &comNum_altair );
    if( iso_type_altair == FIRST_ONE ){
      // start -- scanned -- standard の成す角度
      relative_parent[ scanned ] = start;
      if( linearity( start, scanned , standard ) == FALSE ){ // 一直線でない場合
	// 軸は決定
	axis_type_altair = AXIS_TYPE_ONE;
	break;
      }else{
	cast_codes[ standard ] = UK;
      }

    }else if( iso_type_altair == FIRST_TWO || iso_type_altair == INDIVID_CHILDREN ||
	      iso_type_altair == FIRST_THREE || iso_type_altair == THIRD_THREE ){
      // 軸は決定
      axis_type_altair = AXIS_TYPE_TWO;
      break;

    }else if( iso_type_altair == TIE_TWO || iso_type_altair == TIE_THREE ){
      // 軸は未決定
      axis_type_altair = NOAXIS_TYPE_TWO;
      break;
    }else{
      printf(" error : Error occured in broken_line_pair \n");
      exit( EXIT_FAILURE );
    }

  } // end of while

  // CASTコードをふる
  if( axis_type_altair == AXIS_TYPE_ONE ){
    // vega -- start -- scanned -- standard で standard にCASTコードをふる
    cast_codes[ standard ] = get_dihedral_angle( vega, start, scanned, standard );

  }else{

    if( iso_type_altair == FIRST_THREE || iso_type_altair == THIRD_THREE ||
	iso_type_altair == TIE_TWO || iso_type_altair == TIE_THREE ){ // CASTコードで順位付け

      // vega -- start -- scanned -- bp->alias で bp->alias のCASTコードをふる
      for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	  cast_codes[ bp->alias ] = get_dihedral_angle( vega, start, scanned, bp->alias );
	}
      }

      // 順位付け
      if( set_children( comrade_altair, comNum_altair, NULL, iso_type_altair ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute set_children \n");
	exit( EXIT_FAILURE );
      }

    }else if( iso_type_altair == FIRST_TWO || iso_type_altair == INDIVID_CHILDREN ){

      for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	  cast_codes[ bp->alias ] = UK;
	}
      }

    }

  }

  /**** vega ****/
  standard = vega;
  iso_type_vega = MINUS;
  axis_type_vega = NOAXIS_TYPE_ONE;

  cast_codes[ standard ] = UK;

  while( confined_bond[ standard ] >= 2 ){
    scanned = standard;
    iso_type_vega = scan_children( scanned, &standard, comrade_vega, &comNum_vega );
    if( iso_type_vega == FIRST_ONE ){
      // start -- scanned -- standard の成す角度
      relative_parent[ scanned ] = start;
      if( linearity( start, scanned , standard ) == FALSE ){ // 一直線でない場合
	// 軸は決定
	axis_type_vega = AXIS_TYPE_ONE;
	break;
      }else{
	cast_codes[ standard ] = UK;
      }

    }else if( iso_type_vega == FIRST_TWO || iso_type_vega == INDIVID_CHILDREN ||
	      iso_type_vega == FIRST_THREE || iso_type_vega == THIRD_THREE ){
      // 軸は決定
      axis_type_vega = AXIS_TYPE_TWO;
      break;

    }else if( iso_type_vega == TIE_TWO || iso_type_vega == TIE_THREE ){
      // 軸は未決定
      axis_type_vega = NOAXIS_TYPE_TWO;
      break;
    }else{
      printf(" error : Error occured in broken_line_pair \n");
      exit( EXIT_FAILURE );
    }

  } // end of while

  // CASTコードをふる
  if( axis_type_vega == AXIS_TYPE_ONE ){
    // altair -- start -- scanned -- standard にCASTコードをふる
    cast_codes[ standard ] = get_dihedral_angle( altair, start, scanned, standard );

  }else{

    if( iso_type_vega == FIRST_THREE || iso_type_vega == THIRD_THREE ||
	iso_type_vega == TIE_TWO || iso_type_vega == TIE_THREE ){ // CASTコードで順位付け

      // vega -- start -- scanned -- bp->alias で bp->alias のCASTコードをふる
      for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	  cast_codes[ bp->alias ] = get_dihedral_angle( altair, start, scanned, bp->alias );
	}
      }

      // 順位付け
      if( set_children( comrade_vega, comNum_vega, NULL, iso_type_vega ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute set_children \n");
	exit( EXIT_FAILURE );
      }

    }else if( iso_type_vega == FIRST_TWO || iso_type_vega == INDIVID_CHILDREN ){

      for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	  cast_codes[ bp->alias ] = UK;
	}
      }

    }

  }

  free(comrade_altair);
  free(comrade_vega);

  // レベル 2 の層まで順位確定
  if( apply_cast() != EXIT_SUCCESS ){
    printf(" error : Cannot execute apply_cast \n");
    exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;

}
