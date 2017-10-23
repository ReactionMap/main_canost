// distinguish_competitive_atoms2.c 
/*********************************************************************
 レベル 1 の層の四つの原子の内, 二つの原子にそれぞれ異なる
 順位が付いていて, 残り二つの原子には順位が付いていない．
 順位の付いていない二つの原子のなす角(pivots[0] -- start -- pivots[1])
 を利用して, その他の二つの原子及びその先の原子に順位を付ける関数
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_DIST_COMP_TWO 1

extern struct atom *a1st;
extern int start;
extern struct level root;
extern struct level *top;
extern struct level *reef;
extern int *cast_codes;
extern int *direct_parent;
extern int *relative_parent;
extern int *confined_bond;

int distinguish_competitive_atoms2( int *pivots, int *competitor, int compe_type ){

  int i;

  struct bond *bp;

  int iso_type;
  int standard;
  int scanned;
  int new_standard;

  int standard_compe;

  int *comrade;
  int comNum;

#ifdef TEST_DIST_COMP_TWO
  printf("distinguish_competitive_atoms2 \n");
#endif

  if( (comrade=(int *)malloc(TUPLE_SIZE*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in distinguish_competitive_atoms2 \n");
    exit( EXIT_FAILURE );
  }
  comNum = 0;

  standard = MINUS;

  // CASTコードをふる
  // pivots[1]
  cast_codes[ pivots[1] ] = get_dihedral_angle( ZERO, pivots[0], start, pivots[1] );
  // competitor
  cast_codes[ competitor[0] ] = get_dihedral_angle( ZERO, pivots[0], start, competitor[0] );
  cast_codes[ competitor[1] ] = get_dihedral_angle( ZERO, pivots[0], start, competitor[1] );

  // pivots[0] の direct_parent を一時的に MINUS に変更
  direct_parent[ pivots[0] ] = MINUS;

#ifdef TEST_DIST_COMP_TWO
  printf("distinguish_competitive_atoms2 (1) \n");
  printf("pivots 0 %5d \n", pivots[0]);
  printf("pivots 1 %5d \n", pivots[1]);
  printf("competitor 0 %5d \n", competitor[0]);
  printf("competitor 1 %5d \n", competitor[1]);
#endif

  // 順位付け
  if( set_children( competitor, 2, &standard_compe, compe_type ) != EXIT_SUCCESS ){
    printf(" error : Cannot execute set_children \n");
    exit( EXIT_FAILURE );
  }

  // top, reef の再初期化
  top = &root;
  reef = top->next;

  if( ordering() != EXIT_SUCCESS ){
    printf(" error : Cannot execute ordering \n");
    exit( EXIT_FAILURE );
  }

  // pivots[0] の direct_parent を start に戻す
  direct_parent[ pivots[0] ] = start;

  /*** pivots[0] の子の順位付け ***/
  // 軸の選択
  if( compe_type == FIRST_THREE ){
    standard = pivots[1];
  }else{
    standard = standard_compe;
  }

  scanned = MINUS;
  new_standard = pivots[0];

  cast_codes[ new_standard ] = UK;

  while( confined_bond[ new_standard ] >= 2 ){
    scanned = new_standard;
    iso_type = scan_children( scanned, &new_standard, comrade, &comNum );

#ifdef TEST_DIST_COMP_TWO
    printf("distinguish_competitive_atoms2 (2) \n");
#endif

    if( iso_type == FIRST_ONE ){
      // start -- scanned -- new_standard の成す角度
      relative_parent[ scanned ] = start;
 
      if( linearity( start, scanned, new_standard ) == FALSE ){ // 直線でない
	cast_codes[ new_standard ] = get_dihedral_angle( standard, start, scanned, new_standard );
	break;
      }else{ 
	cast_codes[ new_standard ] = UK;
      }

    }else{

#ifdef TEST_DIST_COMP_TWO
      printf("distinguish_competitive_atoms2 (3) \n");
#endif

      if( iso_type == FIRST_THREE || iso_type == THIRD_THREE ||
	  iso_type == TIE_TWO || iso_type == TIE_THREE ){

        // standard -- start -- scanned -- bp->alias で bp->alias のCASTコードをふる
	for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	    cast_codes[ bp->alias ] = get_dihedral_angle( standard, start, scanned, bp->alias );
	  }
	}

        // 順位付け
	if( set_children( comrade, comNum, &new_standard, iso_type ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute set_children \n");
	  exit( EXIT_FAILURE );
	}

      }else{

	for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	    cast_codes[ bp->alias ] = get_dihedral_angle( standard, start, scanned, bp->alias );
	  }
	}

      }

      break;

    }

  } // end of while( confined_bond[ new_standard ] >= 2 )

  /*** pivots[1] の子の順位付け ***/
  standard = pivots[0];

  scanned = MINUS;
  new_standard = pivots[1];

  cast_codes[ new_standard ] = UK;

  while( confined_bond[ new_standard ] >= 2 ){
    scanned = new_standard;
    iso_type = scan_children( scanned, &new_standard, comrade, &comNum );

    if( iso_type == FIRST_ONE ){
      // start -- scanned -- new_standard の成す角度
      relative_parent[ scanned ] = start;
 
      if( linearity( start, scanned, new_standard ) == FALSE ){ // 直線でない
	cast_codes[ new_standard ] = get_dihedral_angle( standard, start, scanned, new_standard );
	break;
      }else{
	cast_codes[ new_standard ] = UK;
      }

    }else{

      if( iso_type == FIRST_THREE || iso_type == THIRD_THREE ||
	  iso_type == TIE_TWO || iso_type == TIE_THREE ){

        // standard -- start -- scanned -- bp->alias で bp->alias のCASTコードをふる
	for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	    cast_codes[ bp->alias ] = get_dihedral_angle( standard, start, scanned, bp->alias );
	  }
	}

        // 順位付け
	if( set_children( comrade, comNum, &new_standard, iso_type ) != EXIT_SUCCESS ){
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

  /*** competitor の子の順位付け ***/
  standard = pivots[0];

  for( i=0; i < 2 ; i++ ){
    scanned = MINUS;
    new_standard = competitor[i];

    cast_codes[ new_standard ] = UK;

    while( confined_bond[ new_standard ] >= 2 ){
      scanned = new_standard;
      iso_type = scan_children( scanned, &new_standard, comrade, &comNum );

      if( iso_type == FIRST_ONE ){
        // start -- scanned -- new_standard の成す角度
	relative_parent[ scanned ] = start;
 
        if( linearity( start, scanned, new_standard ) == FALSE ){ // 直線でない
	  cast_codes[ new_standard ] = get_dihedral_angle( standard, start, scanned, new_standard );
	  break;
	}else{
	  cast_codes[ new_standard ] = UK;
	}

      }else{

        if( iso_type == FIRST_THREE || iso_type == THIRD_THREE ||
	    iso_type == TIE_TWO || iso_type == TIE_THREE ){

          // standard -- start -- scanned -- bp->alias で bp->alias のCASTコードをふる
	  for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	    if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	      cast_codes[ bp->alias ] = get_dihedral_angle( standard, start, scanned, bp->alias );
	    }
	  }

          // 順位付け
	  if( set_children( comrade, comNum, &new_standard, iso_type ) != EXIT_SUCCESS ){
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
