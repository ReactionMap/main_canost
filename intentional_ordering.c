// intentional_ordering.c 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_INTENTIONAL_ORDER 1

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern int equive;
extern struct level root;
extern struct level *top;
extern struct level *down;
extern struct level *reef;
extern int ore;
extern int atoms_in_confine;

int intentional_ordering( void ){

  int i;

  int flag_1,flag_2;

  int *caddy; // 同順位の原子集合が入る配列
  int scale; //  同順位の原子集合が入る配列の大きさ
  int higher; // 作為的に順位を高くする原子の番号
  int *class;  //順位を記憶しておくための配列
  int *pile;
  int peak; //  同順位の原子集合の大きさ
  int space; //  同順位の原子集合が入る配列の大きさ

  struct level *lp;
  int pre_ore;

  lp=reef;
  pre_ore=ore;
  scale=0;

  i=0;
  flag_1=0;
  flag_2=0;
  peak=1;
  pile=NULL;
  while( reef != down ){ //最下位のレベルの層になるまで
    flag_1=0;
    flag_2=0;
    pile=reef->member; // 層の原子
    while( ore+1 < reef->member_Num ){ // 層の範囲
      if( !(flag_1) && 
          !( a1st[ pile[ore] ].link == CAVE || a1st[ pile[ore] ].link == STEM ||
             a1st[ pile[ore] ].link == KNOT || a1st[ pile[ore] ].link == TRUNK ) ){
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
  }

  if( peak <= 1 ){ // 同順位の原子の集合が存在しない

    printf("error : Cannot detect co-order atoms \n");
    exit( EXIT_FAILURE );

  }else{ // 同順位の原子の集合を返す

    if( (caddy=(int *)malloc(peak*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in intentional_ordering \n");
      exit( EXIT_FAILURE );
    }

    scale=peak; // 同順位の原子の集合の大きさ
    space=peak; // 同順位の原子の集合の大きさ

    while( peak > 0 ){
      caddy[ --peak ]=pile[i];
      i--;
    }

  }

#ifdef TEST_INTENTIONAL_ORDER
  printf(" scale = %d \n",scale);

  for(i=0;i<scale;i++){
    printf("%d \n",caddy[i]);
  }
#endif

  if( (class=(int *)malloc((iatm+new_atm)*sizeof(int))) == NULL){
    printf("error : Cannot allocate memory in intentional_ordering \n");
    exit( EXIT_FAILURE );
  }

  for(i=1;i<iatm+new_atm;i++){
    class[i] = a1st[i].grade;
  }

  while( scale > 0 ){
    higher=caddy[ --scale ];

    // 作為的にhigherの順位を上げる
    if( raise_rank(higher,caddy,space) != EXIT_SUCCESS ){
      printf("error : Cannot execute raise_rank \n");
      exit( EXIT_FAILURE );
    }

    // 再び順位付けの実行
    if( ordering() != EXIT_SUCCESS ){
	printf("error : Cannot execute ordering \n");
	exit( EXIT_FAILURE );
    }

    // 全ての原子に異なる順位が付けられたかの判定
    if( equive == atoms_in_confine ){
      top = &root;
      reef=top->next;
      ore=0;
      if( set_atom() != EXIT_SUCCESS ){
	printf(" error : Cannot set_atom \n");
	exit( EXIT_FAILURE );
      }

    }else{
      // そうでなければ再び作為的な順位付けへ
      reef=lp;
      ore=pre_ore;
      if( intentional_ordering() != EXIT_SUCCESS ){
	printf("error : Cannot execute intentional_ordering \n");
	exit( EXIT_FAILURE );
      }
    }

    for(i=1;i<iatm+new_atm;i++){
      a1st[i].grade=class[i];
    }

  }

  free(class);
  free(caddy);

  return EXIT_SUCCESS;

}
