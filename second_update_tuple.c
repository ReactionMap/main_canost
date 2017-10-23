//second_update_tuple.c tupleを更新する関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

extern struct atom *a1st;

// chain : ひとつの層中の原子の配列
// length : 層中の原子の個数
int second_update_tuple( int *chain, int length, int lower ){

  int i,j;

  struct bond *bp;

  // tupleの再初期化
  for(i=0;i<length;i++){
    for(j=0;j<TUPLE_SIZE;j++){
      if( lower > 0 ){
	a1st[ chain[i] ].tuple_child[j]=lower;
      }else{
	a1st[ chain[i] ].tuple_child[j]=0;
      }
    }
  }

  for(i=0;i<length;i++){
    for( bp = a1st[ chain[i] ].adj_list; bp != NULL ; bp=bp->next ){
      if( a1st[ bp->alias ].layer == (a1st[ chain[i] ].layer+1) ){
        // 挿入ソートにより子に関するtupleを更新
	j=TUPLE_SIZE-2;
	while( (j >= 0) && ( a1st[ chain[i] ].tuple_child[j] > a1st[ bp->alias ].grade ) ){
	  a1st[ chain[i] ].tuple_child[ j+1 ]=a1st[ chain[i] ].tuple_child[j];
	  j--;
	}
	a1st[ chain[i] ].tuple_child[j+1]=a1st[ bp->alias ].grade;
      }
    }
  }

  return EXIT_SUCCESS;

}


