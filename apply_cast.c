// apply_cast.c
/****************************************************************
 まだ, CASTコードがふられていないものに対してCASTコードをふり,
 CASTコードを利用した順位付けをする ordering_one_castを呼び出す．
 その順位付け終了後, CANOSTコードを生成するgenlnr_codeを呼び出す
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_APPLY_CAST 1

extern struct atom *a1st;
extern struct level root;
extern struct level *top,*down;
extern struct level *reef;
extern int ore;
extern int *cast_codes;
extern int lowest;

int apply_cast( void ){

  int i;
  
  int *pile;

  pile=NULL;

#ifdef TEST_APPLY_CAST
  printf(" apply_cast \n");
#endif

  reef=reef->next;
  if( reef != down ){
    pile=reef->member;
    for(i=0; i < reef->member_Num; i++){
      if( cast_codes[ pile[i] ] == MINUS ){ // まだCASTコードがふられていないものに対して
	if( identify_parent( pile[i] ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute identify_parent \n");
	  exit( EXIT_FAILURE );
	}
	if( assign_cast_code( pile[i] ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute assign_cast_code \n");
	  exit( EXIT_FAILURE );
	}
      }
    }
  }

  // CASTコードに基づく順位付け
  if( reef != down && lowest > 4 ){

    ore = 0;
    if( ordering_on_cast() != EXIT_SUCCESS ){
      printf("error : Cannot execute ordering_on_cast \n");
      exit( EXIT_FAILURE );
    }

  }

  // CANOSTコードの生成
  top = &root;
  if( genlnr_code() != EXIT_SUCCESS ){
    printf("error : Cannot generate linear code \n");
    exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;

}
