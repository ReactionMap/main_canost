// distinguish_competitive_atoms.c 
/*******************************************************
 基準となる原子を利用して, その他の順位の付いていない
 原子(2,3個)の順位を決める関数
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_DIST_COMP_ONE 1

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern int start;
extern struct level root;
extern struct level *top;
extern struct level *reef;
extern int *cast_codes;
extern int *direct_parent;
extern int *relative_parent;
extern int *relation;
extern int *confined_bond;

int distinguish_competitive_atoms( int pivot, int *competitor, int compeNum ){

  int i,j;

  struct bond *bp;

  int *caddy; // 同順位の原子集合が入る配列
  int scale; //  同順位の原子集合が入る配列の大きさ
  int higher; // 作為的に順位を高くする原子の番号
  int *class;  //順位を記憶しておくための配列

  int scanned;
  int iso_type;
  int standard;
  int new_scanned;
  int new_standard;

  int compe_type;

  int axis_type;

  int iso_type_compe;
  int standard_compe;

  int *comrade;
  int comNum;

  int *twins;

#ifdef TEST_DIST_COMP_ONE
  printf(" distinguish_competitive_atoms \n");
#endif

  // TIE_*** の判定
  if( compeNum == 2 ){
    compe_type = TIE_TWO;
  }else if( compeNum == 3 ){
    compe_type = TIE_THREE;
  }else{
    printf(" error : Error ocuured in distinguish_competitive_atoms \n");
    exit( EXIT_FAILURE );
  }

  // 同順位の子が入る配列を用意
  if( (comrade=(int *)malloc(TUPLE_SIZE*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in distinguish_competitive_atoms \n");
    exit( EXIT_FAILURE );
  }
  comNum = 0;

  /*** 初期化 ***/
  scanned = MINUS;
  iso_type = MINUS;
  axis_type = NOAXIS_TYPE_ONE;
  standard = pivot;
  new_standard = MINUS;

  cast_codes[ standard ] = UK;

  while( confined_bond[ standard ] >= 2 ){
    scanned = standard;
    iso_type = scan_children( scanned, &standard, comrade, &comNum );
#ifdef TEST_DIST_COMP_ONE
    printf("scanned %d standard %d iso_type %d \n",scanned,standard,iso_type);
#endif
    if( iso_type == FIRST_ONE ){
      // start -- scanned -- standard の成す角度
      relative_parent[ scanned ] = start;

      if( linearity( start, scanned , standard ) == FALSE ){ //  一直線でない場合
	// 軸が決定
	axis_type = AXIS_TYPE_ONE;
#ifdef TEST_DIST_COMP_ONE
	printf("AXIS_TYPE_ONE \n");
#endif
	break;
      }else{
	cast_codes[ standard ] = UK;
      }

    }else{
      if( iso_type == FIRST_TWO || iso_type == INDIVID_CHILDREN ||
	  iso_type == FIRST_THREE || iso_type == THIRD_THREE ){
	// 軸が決定
	axis_type = AXIS_TYPE_TWO;
#ifdef TEST_DIST_COMP_ONE
	printf("AXIS_TYPE_TWO \n");
#endif
	break;

      }else if( iso_type == TIE_TWO || iso_type == TIE_THREE ){
	// 軸が未決定
	axis_type = NOAXIS_TYPE_TWO;
#ifdef TEST_DIST_COMP_ONE
	printf("NOAXIS_TYPE_TWO \n");
#endif
	break;

      }else{
        printf(" error : Error occured in distinguish_competitive_atoms \n");
	exit( EXIT_FAILURE );
      }

    }

  } // end of while

  if( axis_type == AXIS_TYPE_ONE || axis_type == AXIS_TYPE_TWO ){ // 軸がある
    // pivot の direct_parent を一時的に MINUS に設定
    direct_parent[ pivot ] = MINUS;

    // competitor のCASTコードをふる
    for( i=0; i < compeNum ; i++ ){
      cast_codes[ competitor[i] ] =  get_dihedral_angle( standard, scanned, start, competitor[i] );
    }

    // pivot -- start の結合関係は start -- pivot 
    relation[ start ] = relation[ pivot ];

    // competitor の順位付け
    if( set_children( competitor, compeNum, &standard_compe, compe_type ) != EXIT_SUCCESS ){
      printf(" error : Cannot execute set_children \n");
      exit( EXIT_FAILURE );
    }

    // pivot の direct_parent を start に戻す
    direct_parent[ pivot ] = start;

    // 元に戻す
    relation[ start ] = MINUS;

    if( axis_type == AXIS_TYPE_TWO ){

      if( iso_type == FIRST_THREE || iso_type == THIRD_THREE ){
        // standard_compe -- start -- scanned -- bp->alias で bp->alias にCASTコードをふる
	for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	    cast_codes[ bp->alias ] = get_dihedral_angle( standard_compe, start, scanned, bp->alias );
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

    }

    for( i=0; i < compeNum ;i++ ){
      new_scanned = MINUS;
      new_standard = competitor[i];

      cast_codes[ new_standard ] = UK;

      while( confined_bond[ new_standard ] >= 2 ){
	new_scanned = new_standard;
	iso_type_compe = scan_children( new_scanned, &new_standard, comrade, &comNum );

        if( iso_type_compe == FIRST_ONE ){
	  // start -- new_scanned -- new_standard の成す角度
	  relative_parent[ new_scanned ] = start;

	  if( linearity( start, new_scanned, new_standard ) == FALSE ){ // 一直線ではない
	    cast_codes[ new_standard ] = get_dihedral_angle( pivot, start, new_scanned, new_standard );
	    break;
	  }else{
	    cast_codes[ new_standard ] = UK;
	  }

	}else{

           if( iso_type_compe == FIRST_THREE || iso_type_compe == THIRD_THREE ||
	       iso_type_compe == TIE_TWO || iso_type_compe == TIE_THREE ){

             // pivot -- start -- new_scanned -- bp->alias で bp->alias にCASTコードをふる
	     for( bp=a1st[ new_scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	       if( a1st[ bp->alias ].layer == a1st[ new_scanned ].layer+1 ){
		 cast_codes[ bp->alias ] = get_dihedral_angle( pivot, start, new_scanned, bp->alias );
	       }
	     }

             // 順位を付ける
	     if( set_children( comrade, comNum, NULL, iso_type_compe ) != EXIT_SUCCESS ){
	       printf(" error : Cannot execute set_children \n");
	       exit( EXIT_FAILURE );
	     }

	   }else{

	     for( bp=a1st[ new_scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	       if( a1st[ bp->alias ].layer == a1st[ new_scanned ].layer+1 ){
		 cast_codes[ bp->alias ] = UK;
	       }
	     }

	   }

	   break;

	}

      } // end of while( confined_bond[ new_standard ] >= 2 )

    }

    // レベル 2 の層まで順位確定
    if( apply_cast() != EXIT_SUCCESS ){
      printf(" error : Cannot execute apply_cast \n");
      exit( EXIT_FAILURE );
    }

  }else{ // axis_type == NOAXIS_TYPE_***  軸がない

    if( (class=(int *)malloc((iatm+new_atm)*sizeof(int)))==NULL){
      printf(" error : Cannot allocate memory in distinguish_competitive_atoms \n");
      exit( EXIT_FAILURE );
    }

    for(i=1;i<iatm+new_atm;i++){
      class[i]=a1st[i].grade;
    }

    scale = compeNum;

    if( (caddy=(int *)malloc(scale*sizeof(int)))==NULL){
      printf(" error : Cannot allocate memory in distinguish_competitive_atoms \n");
      exit( EXIT_FAILURE );
    }

    for(i=0; i < scale ;i++){
      caddy[i] = competitor[i];
    }

    for(i=0;i<scale;i++){

      for(j=0; j < scale ;j++){
	// ZERO -- pivot -- start -- caddy[j] で caddy[j] にCASTコードをふる
	cast_codes[ caddy[j] ] = get_dihedral_angle( ZERO, pivot, start, caddy[j] );
      }

      higher = caddy[i];

      standard_compe = higher;

      if( raise_and_order( higher, caddy, scale ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute raise_and_order \n");
	exit( EXIT_FAILURE );
      }

      if( compe_type == TIE_THREE ){

        // pivot の direct_parent を一時的に MINUS に設定
	direct_parent[ pivot ] = MINUS;

	if( (twins=(int *)malloc(2*sizeof(int)))==NULL){
	  printf(" error : Cannot allocate memory in distinguish_competitive_atoms \n");
	  exit( EXIT_FAILURE );
	}

	if( i == 0 ){
	  twins[0] = caddy[1];
	  twins[1] = caddy[2];
	}else if( i == 1 ){
	  twins[0] = caddy[0];
	  twins[1] = caddy[2];
	}else{
	  twins[0] = caddy[0];
	  twins[1] = caddy[1];
	}

        // 順位を付ける
	if( set_children( twins, 2, NULL, FIRST_THREE ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute set_children \n");
	  exit( EXIT_FAILURE );
	}

	free(twins);

        // pivot の direct_parent を start に戻す
	direct_parent[ pivot ] = start;

      }

      if( ordering() != EXIT_SUCCESS ){
	printf(" error : Cannot execute ordering \n");
	exit( EXIT_FAILURE );
      }

      // pivot の先の原子に順位が付いていないので順位を付ける
      if( axis_type == NOAXIS_TYPE_TWO ){
        // standard_compe -- start -- scanned -- bp->alias で bp->alias にCASTコードをふる
	for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	    cast_codes[ bp->alias ] = get_dihedral_angle( standard_compe, start, scanned, bp->alias );
	  }
	}

        // 順位を付ける
	if( set_children( comrade, comNum, NULL, iso_type ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute set_children \n");
	  exit( EXIT_FAILURE );
	}

      }

      // competitor の先の原子に順位を付ける
      for( j=0; j < compeNum ;j++ ){
	new_scanned = MINUS;
	new_standard = competitor[j];

	cast_codes[ new_standard ] = UK;
	while( confined_bond[ new_standard ] >= 2 ){
	  new_scanned = new_standard;
	  iso_type_compe = scan_children( new_scanned, &new_standard, comrade, &comNum );

	  if( iso_type_compe == FIRST_ONE ){
	    // start -- new_scanned -- new_standard の成す角度
	    relative_parent[ new_scanned ] = start;

	    if( linearity( start, new_scanned, new_standard ) == FALSE ){ // 一直線ではない
	      cast_codes[ new_standard ] = get_dihedral_angle( pivot, start, new_scanned, new_standard );
	      break;
	    }else{
	      cast_codes[ new_standard ] = UK;
	    }

	  }else{

            if( iso_type_compe == FIRST_THREE || iso_type_compe == THIRD_THREE ||
		iso_type_compe == TIE_TWO || iso_type_compe == TIE_THREE ){

              // pivot -- start -- new_scanned -- bp->alias で bp->alias にCASTコードをふる
	      for( bp=a1st[ new_scanned ].adj_list ; bp != NULL ; bp=bp->next ){
		if( a1st[ bp->alias ].layer == a1st[ new_scanned ].layer+1 ){
		  cast_codes[ bp->alias ] = get_dihedral_angle( pivot, start, new_scanned, bp->alias );
		}
	      }

              // 順位を付ける
	      if( set_children( comrade, comNum, NULL, iso_type_compe ) != EXIT_SUCCESS ){
		printf(" error : Cannot execute set_children \n");
		exit( EXIT_FAILURE );
	      }

	    }else{

	      for( bp=a1st[ new_scanned ].adj_list ; bp != NULL ; bp=bp->next ){
		if( a1st[ bp->alias ].layer == a1st[ new_scanned ].layer+1 ){
		  cast_codes[ bp->alias ] = UK;
		}
	      }

	    }

	    break;

	  }

        } // end of while( confined_bond[ new_standard ] >= 2 )

      }

      // レベル 2 の層まで順位確定
      if( apply_cast() != EXIT_SUCCESS ){
	printf(" error : Cannot execute apply_cast \n");
	exit( EXIT_FAILURE );
      }

      for( j=1 ; j < iatm+new_atm ; j++){
	a1st[j].grade = class[j];
	cast_codes[j] = MINUS;
      }

    }

    free(class);
    free(caddy);

  }

  return EXIT_SUCCESS;

}
