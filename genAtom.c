// genAtom.c 仮想的な原子を生成する

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;
extern struct atom *va;
extern int new_atm;
extern int canost_codes;

// dad,mum 仮想的な原子の結合する原子
// stair 仮想的な原子の属す層のレベル
int genAtom( int dad, int mum, int stair ){

  if( new_atm%10 == 0 ){
    // va は仮想的な原子 10 個単位でメモリ確保
    if( (va=(struct atom *)realloc(va,(new_atm+10)*sizeof(struct atom))) == NULL ){
      printf(" error: Cannot allocate memory in genAtom \n" );
    }
  }

  va[new_atm].xco=0; // 3次元座標は全て 0
  va[new_atm].yco=0;
  va[new_atm].zco=0;

  va[new_atm].nbnd=2; // 結合している原子は 2 個

  strcpy(va[new_atm].atm_type,"VA"); // 原子種は VA 原子番号は 0
  va[new_atm].atmNum=0;
  
  strcpy(va[new_atm].code,"RING"); // CANOSTコードは RING CANOSTコード番号は canost_codes
  va[new_atm].codeNum=canost_codes;

  va[new_atm].state=VISITED; // 探索済みとする
    
  va[new_atm].grade=0; // 順位は 0

  va[new_atm].layer=stair; // 属す層のレベル

  va[new_atm].link=KNOT; // 環の結び目なので KNOT

  /*** 二本の結合を作成 ***/
  // 結合 1
  if( (va[new_atm].adj_list=(struct bond *)malloc(sizeof(struct bond)))==NULL ){
    printf(" error : Cannot allocate memory in genAtom \n");
    exit( EXIT_FAILURE );
  }
  (va[new_atm].adj_list)->second=dad; // 親は dad
  (va[new_atm].adj_list)->alias=dad;
  (va[new_atm].adj_list)->btype=0;
  (va[new_atm].adj_list)->bstereo=0;
  (va[new_atm].adj_list)->ring=0;

  // 結合 2
  if( ((va[new_atm].adj_list)->next=(struct bond *)malloc(sizeof(struct bond)))==NULL ){
    printf(" error : Cannot allocate memory in genAtom \n");
    exit( EXIT_FAILURE );
  }
  va[new_atm].last=(va[new_atm].adj_list)->next;
  (va[new_atm].last)->second=mum; // 親は mum
  (va[new_atm].last)->alias=mum;
  (va[new_atm].last)->btype=0;
  (va[new_atm].last)->bstereo=0;
  (va[new_atm].last)->ring=0;
  (va[new_atm].last)->next=NULL;

  return EXIT_SUCCESS;

}
