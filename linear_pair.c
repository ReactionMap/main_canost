// linear_pair.c
/************************************************************
  start に接続している二つの原子は直線になっている.
  したがって, altair, vega のそれぞれの先を探索して
  軸となる結合を見つけ, 見つけた軸を用いて順位を付ける関数
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_LINEAR_PAIR 1

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern int start;
extern struct level root;
extern struct level *top;
extern struct level *reef;
extern int *relative_parent;
extern int *cast_codes;
extern int *confined_bond;

int linear_pair( int altair, int vega ){

  int i,j;

  int *class;
  int *caddy;
  int scale;
  int higher;

  int scanned;
  int standard;

  int iso_type_altair;
  int iso_type_vega;

  struct bond *bp;

  int child_altair;
  int grand_child_altair;
  int *comrade_altair;  
  int comNum_altair;
  int axis_type_altair;

  int child_vega;
  int grand_child_vega;
  int *comrade_vega;
  int comNum_vega;
  int axis_type_vega;

#ifdef TEST_LINEAR_PAIR
  printf("\n linear_pair: altair %d vega %d\n", altair, vega);
#endif

  /** 初期化 ***/
  bp=NULL;

  caddy = NULL;

  child_vega = MINUS;
  grand_child_vega = MINUS;

  /*** comrade_***, comNum_*** のメモリ確保 ****/
  if( (comrade_altair=(int *)malloc(TUPLE_SIZE*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in linear_pair \n");
    exit( EXIT_FAILURE );
  }
  comNum_altair = 0;

  if( (comrade_vega=(int *)malloc(TUPLE_SIZE*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in linear_pair \n");
    exit( EXIT_FAILURE );
  }
  comNum_vega = 0;

  /*** altair と vega の子の順位付け ****/

  /**** altair ****/
  standard = altair;
  iso_type_altair = MINUS;
  axis_type_altair = NOAXIS_TYPE_ONE;
  child_altair = MINUS;
  grand_child_altair = MINUS;

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
	child_altair = scanned;
	grand_child_altair = standard;
#ifdef TEST_LINEAR_PAIR
	printf("altair : AXIS_TYPE_ONE \n");
#endif
        break;
      }else{
	cast_codes[ standard ] = UK;
      }

    }else if( iso_type_altair == FIRST_TWO || iso_type_altair == INDIVID_CHILDREN ||
              iso_type_altair == FIRST_THREE || iso_type_altair == THIRD_THREE ){
      // 軸は決定
      axis_type_altair = AXIS_TYPE_TWO;
      child_altair = scanned;
      grand_child_altair = standard;
#ifdef TEST_LINEAR_PAIR
	printf("altair : AXIS_TYPE_TWO \n");
#endif
      break;

    }else if( iso_type_altair == TIE_TWO || iso_type_altair == TIE_THREE ){
      // 軸は未決定
      axis_type_altair = NOAXIS_TYPE_TWO;
      child_altair = scanned;
#ifdef TEST_LINEAR_PAIR
	printf("altair : NOAXIS_TYPE_ONE \n");
#endif
      break;
    }else{
      printf(" error : Error occured in linear_pair \n");
      exit( EXIT_FAILURE );
    }

  } // end of while

  if( axis_type_altair == AXIS_TYPE_ONE ){
    cast_codes[ grand_child_altair ] = get_dihedral_angle( ZERO,start,child_altair,grand_child_altair );

  }else{

    if( iso_type_altair == FIRST_THREE || iso_type_altair == THIRD_THREE ){ // 残り二つに順位を付ける

      // ZERO -- start -- child_altair -- bp->alias で bp->alias にCASTコードをふる
      for( bp=a1st[ child_altair ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ child_altair ].layer+1 ){
	  cast_codes[ bp->alias ] = get_dihedral_angle( ZERO, start, child_altair, bp->alias );
	}
      }

      // 順位を確定
      if( set_children( comrade_altair, comNum_altair, NULL, iso_type_altair ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute set_children \n");
	exit( EXIT_FAILURE );
      }

    }else if( iso_type_altair == FIRST_TWO || iso_type_altair == INDIVID_CHILDREN ){

      for( bp=a1st[ child_altair ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ child_altair ].layer+1 ){
	  cast_codes[ bp->alias ] = UK;
	}
      }

    }

  }

  /**** vega ****/
  standard = vega;
  iso_type_vega = MINUS;
  axis_type_vega = NOAXIS_TYPE_ONE;
  child_vega = MINUS;
  grand_child_vega = MINUS;

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
	child_vega = scanned;
	grand_child_vega = standard;
#ifdef TEST_LINEAR_PAIR
	printf("vega : AXIS_TYPE_ONE \n");
#endif
        break;
      }else{
	cast_codes[ standard ] = UK;
      }

    }else if( iso_type_vega == FIRST_TWO || iso_type_vega == INDIVID_CHILDREN ||
              iso_type_vega == FIRST_THREE || iso_type_vega == THIRD_THREE ){
      // 軸は決定
      axis_type_vega = AXIS_TYPE_TWO;
      child_vega = scanned;
      grand_child_vega = standard;
#ifdef TEST_LINEAR_PAIR
	printf("vega : AXIS_TYPE_TWO \n");
#endif
      break;

    }else if( iso_type_vega == TIE_TWO || iso_type_vega == TIE_THREE ){
      // 軸は未決定
      axis_type_vega = NOAXIS_TYPE_TWO;
      child_vega = scanned;
#ifdef TEST_LINEAR_PAIR
	printf("vega : NOAXIS_TYPE_TWO \n");
#endif
      break;
    }else{
      printf(" error : Error occured in linear_pair \n");
      exit( EXIT_FAILURE );
    }

  } // end of while

  if( axis_type_vega == AXIS_TYPE_ONE ){

    if( axis_type_altair == AXIS_TYPE_ONE || axis_type_altair == AXIS_TYPE_TWO ){
      // grand_child_altair -- child_altair -- child_vega -- grand_child_vega で grand_child_vega のCASTコードをふる
      cast_codes[ grand_child_vega ] = get_dihedral_angle( grand_child_altair, child_altair, child_vega, grand_child_vega);

    }else{

      if( axis_type_altair == NOAXIS_TYPE_TWO ){

    	// grand_child_vega -- child_vega -- child_altair -- bp->alias で bp->alias のCASTコードをふる
	for( bp=a1st[ child_altair ].adj_list ; bp != NULL ; bp = bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ child_altair ].layer+1 ){
	    cast_codes[ bp->alias ] = get_dihedral_angle( grand_child_vega, child_vega, child_altair, bp->alias );
	  }
	}

	// 順位を付ける
	if( set_children( comrade_altair, comNum_altair, NULL, iso_type_altair ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute set_children \n");
	  exit( EXIT_FAILURE );
	}

      }

    }

    // レベル 2 までの層の順位確定
    if( apply_cast() != EXIT_SUCCESS ){
      printf(" error : Cannot execute apply_cast \n");
      exit( EXIT_FAILURE );
    }

  }else if( axis_type_vega == AXIS_TYPE_TWO ){

    if( iso_type_vega == FIRST_TWO || iso_type_vega == INDIVID_CHILDREN ){ // 順位は確定

      if( axis_type_altair == NOAXIS_TYPE_TWO ){ // child_altair の子に順位が付いていない場合

	// grand_child_vega -- child_vega -- child_altair -- bp->alias で bp->alias のCASTコードをふる
	for( bp=a1st[ child_altair ].adj_list ; bp != NULL ; bp = bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ child_altair ].layer+1 ){
	    cast_codes[ bp->alias ] = get_dihedral_angle( grand_child_vega, child_vega, child_altair, bp->alias );
	  }
	}

	// 順位を付ける
	if( set_children( comrade_altair, comNum_altair, NULL, iso_type_altair ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute set_children \n");
	  exit( EXIT_FAILURE );
	}

      }

    }else{ // iso_type_vega == FIRST_THREE, THIRD_THREE
      if( axis_type_altair == AXIS_TYPE_ONE || axis_type_altair == AXIS_TYPE_TWO ){

	 // grand_child_altair -- child_altair -- child_vega -- bp->alias で bp->alias のCASTコードをふる
	for( bp=a1st[ child_vega ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ child_vega ].layer+1 ){
	    cast_codes[ bp->alias ] = get_dihedral_angle( grand_child_altair,child_altair, child_vega, bp->alias );
	  }
	}

	// 順位を付ける
	if( set_children( comrade_vega, comNum_vega, &standard, iso_type_vega ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute set_children \n");
	  exit( EXIT_FAILURE );
	}

	grand_child_vega = standard;

      }else{

	  // ZERO -- start  -- child_vega -- bp->alias で bp->alias のCASTコードをふる
	  for( bp=a1st[ child_vega ].adj_list ; bp != NULL ; bp=bp->next ){
	    if( a1st[ bp->alias ].layer == a1st[ child_vega ].layer+1 ){
	      cast_codes[ bp->alias ] = get_dihedral_angle( ZERO,start, child_vega, bp->alias );
	    }
	  }

	  // 順位を付ける
	  if( set_children( comrade_vega, comNum_vega, &standard, iso_type_vega ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute set_children \n");
	    exit( EXIT_FAILURE );
	  }

	  grand_child_vega = standard;

	  if( axis_type_altair == NOAXIS_TYPE_TWO ){ // child_altair の複数ある子には順位が付いていない

	  // grand_child_vega -- child_vega -- child_altair -- bp->alias でCASTコードをふる
	    for( bp=a1st[ child_altair ].adj_list ; bp != NULL ; bp = bp->next ){
	      if( a1st[ bp->alias ].layer == a1st[ child_altair ].layer+1 ){
		cast_codes[ bp->alias ] = get_dihedral_angle( grand_child_vega, child_vega, child_altair, bp->alias );
	      }
	    }

	  // 順位を付ける
	    if( set_children( comrade_altair, comNum_altair, NULL, iso_type_altair ) != EXIT_SUCCESS ){
	      printf(" error : Cannot execute set_children \n");
	      exit( EXIT_FAILURE );
	    }

	  }

      }
    }

    // レベル 2 までの層の順位確定
    if( apply_cast() != EXIT_SUCCESS ){
      printf(" error : Cannot execute apply_cast \n");
      exit( EXIT_FAILURE );
    }

  }else if( axis_type_vega == NOAXIS_TYPE_TWO ){
    if( axis_type_altair == AXIS_TYPE_ONE || axis_type_altair == AXIS_TYPE_TWO ){

      // grand_child_altair -- child_altair -- child_vega -- bp->alias で bp->alias のCASTコードをふる
      for( bp=a1st[ child_vega ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ child_vega ].layer+1 ){
	  cast_codes[ bp->alias ] = get_dihedral_angle( grand_child_altair,child_altair, child_vega, bp->alias );
	}
      }

      // 順位を付ける
      if( set_children( comrade_vega, comNum_vega, &standard, iso_type_vega ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute set_children \n");
	exit( EXIT_FAILURE );
      }

      grand_child_vega = standard;

      // レベル 2 までの層の順位確定
      if( apply_cast() != EXIT_SUCCESS ){
	printf(" error : Cannot execute apply_cast \n");
	exit( EXIT_FAILURE );
      }

    }else if( axis_type_altair == NOAXIS_TYPE_ONE ){ // altair 側は一直線 従って vega 側は全列挙

      if( (class=(int *)malloc((iatm+new_atm)*sizeof(int)))==NULL){
	printf(" error : Cannot allocate memory in set_upper_level \n");
	exit( EXIT_FAILURE );
      }

      for(i=1;i<iatm+new_atm;i++){
	class[i]=a1st[i].grade;
      }

      if( iso_type_vega == TIE_TWO ){
	scale = 2;
      }else{
	scale = 3;
      }

      if( (caddy=(int *)malloc(scale*sizeof(int)))==NULL){
	printf(" error : Cannot allocate memory in set_upper_level \n");
	exit( EXIT_FAILURE );
      }

      for( i=0 ; i < comNum_vega ; i++){
	caddy[i] = comrade_vega[i];
      }

      for(i=0;i<scale;i++){

	for( j=0 ; j < comNum_vega ; j++){
	  // ZERO -- start -- child_vega -- caddy[j] で caddy[j] にCASTコードをふる
	  cast_codes[ caddy[j] ] = get_dihedral_angle( ZERO, start, child_vega, caddy[j] );
	}

	higher = caddy[i];

	grand_child_vega = higher;

        // 作為的にhigherの順位を上げて順位付け
	if( raise_and_order( higher, caddy, scale ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute raise_and_order \n");
	  exit( EXIT_FAILURE );
	}

	if( iso_type_vega == TIE_THREE ){
	  // FIRST_THREEになったことを確認
	  if( scan_children( child_vega, &standard, comrade_vega, &comNum_vega ) != FIRST_THREE ){
	    printf(" error : Error occured in linear_pair \n");
	    exit( EXIT_FAILURE );
	  }

	  // 順位を付ける
	  if( set_children( comrade_vega, comNum_vega, NULL, FIRST_THREE ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute set_children \n");
	    exit( EXIT_FAILURE );
	  }

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

    }else{ // axis_type_altair == NOAXIS_TYPE_TWO
      if( (class=(int *)malloc((iatm+new_atm)*sizeof(int)))==NULL){
	printf(" error : Cannot allocate memory in linear_pair \n");
	exit( EXIT_FAILURE );
      }

      for(i=1;i<iatm+new_atm;i++){
	class[i]=a1st[i].grade;
      }

      if( iso_type_altair == TIE_TWO ){
	scale = 2;
      }else{
	scale = 3;
      }

      if( (caddy=(int *)malloc(scale*sizeof(int)))==NULL){
	printf(" error : Cannot allocate memory in set_upper_level \n");
	exit( EXIT_FAILURE );
      }

      for( i=0 ; i < comNum_altair ; i++){
	caddy[i] = comrade_altair[i];
      }

      for(i=0;i<scale;i++){

	for( j=0 ; j < comNum_altair ; j++){
	  // ZERO -- start -- child_altair -- caddy[j] で caddy[j] にCASTコードをふる
	  cast_codes[ caddy[j] ] = get_dihedral_angle( ZERO, start, child_altair, caddy[j] );
	}

	higher = caddy[i];

	grand_child_altair = higher;

        // 作為的にhigherの順位を上げて順位付け
	if( raise_and_order( higher, caddy, scale ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute raise_and_order \n");
	  exit( EXIT_FAILURE );
	}

	if( iso_type_altair == TIE_THREE ){

	  // FIRST_THREEになったことを確認
	  if( scan_children( child_altair, &standard, comrade_altair, &comNum_altair ) != FIRST_THREE ){
	    printf(" error : Error occured in linear_pair \n");
	    exit( EXIT_FAILURE );
	  }

	  // 順位を付ける
	  if( set_children( comrade_altair, comNum_altair, NULL ,FIRST_THREE ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute set_children \n");
	    exit( EXIT_FAILURE );
	  }

	}

	// grand_child_altair -- child_altair -- child_vaga -- bp->alias でCASTコードをふる
	for( bp=a1st[ child_vega ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ child_vega ].layer+1 ){
	    cast_codes[ bp->alias ] = get_dihedral_angle( grand_child_altair,child_altair, child_vega, bp->alias );
	  }
	}

	// 順位を付ける
	if( set_children( comrade_vega, comNum_vega, &standard, iso_type_vega ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute set_children \n");
	  exit( EXIT_FAILURE );
	}

	grand_child_vega = standard;

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

  }else{

    // レベル 2 の層まで順位確定
    if( apply_cast() != EXIT_SUCCESS ){
      printf(" error : Cannot execute apply_cast \n");
      exit( EXIT_FAILURE );
    }

  }

  free(comrade_altair);
  free(comrade_vega);

  return EXIT_SUCCESS;

}
