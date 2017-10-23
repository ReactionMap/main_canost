// ordering_on_cast.c 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_ORDER_ON_CAST 1

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern struct level *top,*down;
extern struct level root;
extern struct level *reef;
extern int ore;
extern int *cast_codes;

int ordering_on_cast( void ){

  int i,j;

  int flag_1,flag_2;

  int *caddy; // 同順位の原子集合が入る配列
  int scale; //  同順位の原子集合が入る配列の大きさ
  int *pile;
  int peak; //  同順位の原子集合の大きさ

#ifdef TEST_ORDER_ON_CAST
  printf("\n ordering_on_cast \n");
#endif

  scale=0;
  pile=NULL;
  caddy=NULL;

  i=0;
  flag_1=0;
  flag_2=0;
  peak=1;
  pile=reef->member; // 層の原子
  while( reef != down ){ //指定されたレベルの層になるまで
    flag_1=0;
    flag_2=0;
    while( ore+1 < reef->member_Num ){ // 層の範囲
      if( !(flag_1) && 
          !( a1st[ pile[ore] ].link == BRANCH || 
//             a1st[ pile[ore] ].link == TRUNK  ||
//             TRUNKは下に複数個の環を持つので,すでに順位は決まっている  
             a1st[ pile[ore] ].link == COLUMN ||
             a1st[ pile[ore] ].link == LOOP ) ){
	ore++;
	continue;
      }
      if( a1st[ pile[ore] ].grade == a1st[ pile[ore+1] ].grade ){ // 同順位
        peak++;  // 同順位の原子の個数
	flag_1=1; // 同順位の頂点が見付かった
	ore++;
      }else{ 
	ore++;
	if( flag_1 ){
	  i=ore-1;
	  flag_2=1; // 同順位の原子の集合の最後
	}
      }
      if( flag_1 && flag_2 ){  // 最も順位の高い同順位の集合が見付けられた
	break;
      }
    }
    if( flag_1 && flag_2 ){ // 最も順位の高い同順位の集合が見付けられた
      break;
    }
    if( flag_1 && (ore+1 == reef->member_Num) ){
      i=ore;
      break;
    }
    reef=reef->next; // 次の層に移る
    ore=0; // 層の先頭から
    if( reef != down ){
      pile=reef->member;
      for(j=0 ; j < reef->member_Num ;j++){
	if( cast_codes[ pile[j] ] == MINUS ){
	  if( identify_parent( pile[j] ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute identify_parent \n");
	    exit( EXIT_FAILURE );
	  }
	  if( assign_cast_code( pile[j] ) != EXIT_SUCCESS ){
	    printf("error : Cannot execute assign_cast_code \n");
	    exit( EXIT_FAILURE );
	  }
	}
      }
    }

  }

  if( peak <= 1 ){ // 同順位の原子の集合が存在しない

    return EXIT_SUCCESS;

  }else{ // 同順位の原子の集合を返す

    if( (caddy=(int *)malloc(peak*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in intentional_ordering \n");
      exit( EXIT_FAILURE );
    }

    scale=peak; // 同順位の原子の集合の大きさ

    while( peak > 0 ){
      caddy[ --peak ]=pile[i];
      i--;
    }

  }

#ifdef TEST_ORDER_ON_CAST
  printf("\n scale = %d \n",scale);

  for(i=0;i<scale;i++){
    printf("atom %d cast_code %d link %d \n",
            caddy[i],cast_codes[ caddy[i] ], a1st[ caddy[i] ].link);
  }
#endif

  if( caddy[0] < iatm ){

    /*
    for(i=0;i<scale;i++){
      if( assign_cast_code( caddy[i] ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute assign_cast_code \n");
	exit( EXIT_FAILURE );
      }
    }
    */

    // castコードに基づく順位付け
    if( geometric_order( caddy,scale ) != EXIT_SUCCESS ){
      printf("error : Cannot execute geometric_order \n");
      exit( EXIT_FAILURE );
    }

    // 上で順位が決まった原子のひとつ上の層からトップダウンにソート
    top=reef->prev; 
    if( sort_t_d() != EXIT_SUCCESS ){
      printf("error : Cannot execute sort_t_b \n");
      exit( EXIT_FAILURE );
    }

    // 再び順位付けの実行
    /*
    if( ordering() != EXIT_SUCCESS ){
      printf("error : Cannot execute ordering \n");
      exit( EXIT_FAILURE );
    }
    */

  }else{
    printf(" error : Error occured in ordering_on_cast \n");
    exit( EXIT_FAILURE );
  }

  // 層別ネットワークの最下位まで到達したかを判断
  if( reef != NULL ){
    if( ordering_on_cast() != EXIT_SUCCESS ){
      printf(" error : Cannot execute intentional_ordering \n");
      exit( EXIT_FAILURE );
    }
  }

  free(caddy);

  return EXIT_SUCCESS;

}
