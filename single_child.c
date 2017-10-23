//single_child.c 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_SINGLE_CHILD 1

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern int start;
extern struct level root;
extern struct level *top;
extern struct level *down;
extern struct level *reef;
extern int *cast_codes;

int single_child( int scanned ){

  int i,j;

  int *caddy;
  int scale;
  int higher;
  int *class;

  int iso_type;
  int standard;

  int *pile_two;

  struct level *lv_two;

  int *comrade;
  int comNum;

#ifdef TEST_SINGLE_CHILD
  printf(" single_child \n");
#endif

  lv_two = NULL;
  pile_two = NULL;

  // lv_two を reef->next に設定
  lv_two = reef->next;
  if( lv_two != NULL ){
    pile_two = lv_two->member;
  }

  // 同順位の子が入る配列を用意
  if( (comrade=(int *)malloc(TUPLE_SIZE*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in single_child \n");
    exit( EXIT_FAILURE );
  }

  comNum = 0;

  // scan_children の実行
  iso_type = scan_children( scanned, &standard , comrade, &comNum );

  if( iso_type == NOCHILD ){

    return EXIT_SUCCESS;

  }else if( iso_type == FIRST_ONE ){

    if( linearity( start, scanned , standard ) == TRUE ){  // 一直線の場合

      reef = reef->next;

      if( reef->next != down ){

	if( set_upper_level( ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute set_upper_level \n");
	  exit( EXIT_FAILURE );
	}

      }else{

	if( apply_cast() != EXIT_SUCCESS ){
	  printf(" error : Cannot execute apply_cast \n");
	  exit( EXIT_FAILURE );
	}

      }

    }else{
      if( apply_cast() != EXIT_SUCCESS ){
	printf(" error : Cannot execute apply_cast \n");
	exit( EXIT_FAILURE );
      }
    }

  }else if( iso_type == FIRST_TWO || iso_type == INDIVID_CHILDREN ){

    if( apply_cast() != EXIT_SUCCESS ){
      printf(" error : Cannot execute apply_cast \n");
      exit( EXIT_FAILURE );
    }

  }else if( iso_type == FIRST_THREE || iso_type == THIRD_THREE ){

    for( i=0 ; i < lv_two->member_Num ; i++){
      cast_codes[ pile_two[i] ] = get_dihedral_angle( ZERO, start, scanned, pile_two[i] );
    }

    if( set_children( comrade, comNum, NULL, iso_type ) != EXIT_SUCCESS ){
      printf(" error : Cannot execute set_children \n");
      exit( EXIT_FAILURE );
    }

    if( apply_cast() != EXIT_SUCCESS ){
      printf(" error : Cannot execute apply_cast \n");
      exit( EXIT_FAILURE );
    }

  }else if( iso_type == TIE_TWO ){

    if( (class=(int *)malloc((iatm+new_atm)*sizeof(int)))==NULL){
      printf(" error : Cannot allocate memory in single_child \n");
      exit( EXIT_FAILURE );
    }

    for(i=1;i<iatm+new_atm;i++){
      class[i]=a1st[i].grade;
    }

    scale = 2; // pile_two は 2 個の原子からなる

    if( (caddy=(int *)malloc(scale*sizeof(int)))==NULL){
      printf(" error : Cannot allocate memory in single_child \n");
      exit( EXIT_FAILURE );
    }

    for(i=0; i < scale ;i++){
      caddy[i] = pile_two[i];
    }

    for(i=0;i<scale;i++){
      higher = caddy[i];

      if( raise_and_order( higher, caddy, scale ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute raise_and_order \n");
	exit( EXIT_FAILURE );
      }

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

  }else if( iso_type == TIE_THREE ){

    if( (class=(int *)malloc((iatm+new_atm)*sizeof(int)))==NULL){
      printf(" error : Cannot allocate memory in single_child \n");
      exit( EXIT_FAILURE );
    }

    for(i=1;i<iatm+new_atm;i++){
      class[i]=a1st[i].grade;
    }

    scale = 3; // pile_two は 3 個の原子からなる

    if( (caddy=(int *)malloc(scale*sizeof(int)))==NULL){
      printf(" error : Cannot allocate memory in single_child \n");
      exit( EXIT_FAILURE );
    }

    for(i=0; i < scale ;i++){
      caddy[i] = pile_two[i];
    }

    for( i=0 ; i < scale ; i++ ){
      higher = caddy[i];

      for(j=0; j < scale ;j++){
	cast_codes[ caddy[j] ] = get_dihedral_angle( ZERO, start, scanned, caddy[j] );
      }

      if( raise_and_order( higher, caddy, scale ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute raise_and_order \n");
	exit( EXIT_FAILURE );
      }

      if( scan_children( scanned, &standard , comrade, &comNum ) != FIRST_THREE ){
	printf(" error : Error occured in single_child \n");
	exit( EXIT_FAILURE );
      }

      if( set_children( comrade, comNum, NULL, FIRST_THREE ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute set_children \n");
	exit( EXIT_FAILURE );
      }

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

  }else{
    printf(" error : Error occured in set_upper_level \n");
    exit( EXIT_FAILURE );
  }

  free(comrade);

  return EXIT_SUCCESS;

}
