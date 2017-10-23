//update_tuple.c tupleを更新する関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_UPDATE_TUPLE 1

extern int iatm;
extern struct atom *a1st;
extern int destination;

// chain : ひとつの層中の原子の配列
// length : 層中の原子の個数
int update_tuple(int *chain,int length,int upper,int lower){

  int i,j;

  struct bond *bp;

#ifdef TEST_UPDATE_TUPLE
  //  printf("update_tuple \n");

  /*
  printf("upper = %d lower = %d \n",upper,lower);
  for(i=0;i<length;i++){
    printf("%d ",chain[i]);
  }
  printf("\n");
  */
#endif

  // tupleの再初期化
  for(i=0;i<length;i++){
    for(j=0;j<TUPLE_SIZE;j++){
      if( upper > 0 ){
	a1st[ chain[i] ].tuple_parent[j]=upper;
      }else{
	a1st[ chain[i] ].tuple_parent[j]=0;
      }
      if( lower > 0 ){
	a1st[ chain[i] ].tuple_child[j]=lower;
      }else{
	a1st[ chain[i] ].tuple_child[j]=0;
      }
    }
  }

  for(i=0;i<length;i++){
    for( bp = a1st[ chain[i] ].adj_list; bp != NULL ; bp=bp->next){
      if( a1st[ bp->alias ].layer == (a1st[ chain[i] ].layer-1) ){
        // 挿入ソートにより親に関するtupleを更新
	j=TUPLE_SIZE-2;
	while( (j >= 0) && (a1st[ chain[i] ].tuple_parent[j] > a1st[ bp->alias ].grade) ){
	  a1st[ chain[i] ].tuple_parent[ j+1 ] = a1st[ chain[i] ].tuple_parent[j];
	  j--;
	}
	a1st[ chain[i] ].tuple_parent[j+1] = a1st[ bp->alias ].grade;
      }else if( a1st[ bp->alias ].layer == (a1st[ chain[i] ].layer+1) ){
        // 挿入ソートにより子に関するtupleを更新
	j=TUPLE_SIZE-2;
	while( (j >= 0) && (a1st[ chain[i] ].tuple_child[j] > a1st[ bp->alias ].grade) ){
	  a1st[ chain[i] ].tuple_child[ j+1 ] = a1st[ chain[i] ].tuple_child[j];
	  j--;
	}
	a1st[ chain[i] ].tuple_child[j+1]=a1st[ bp->alias ].grade;
      }else{ // 指定されたレベル外の原子なので tuple に入れない
	continue;
      }
    }
  }

  // 全列挙を行うために以下の方法では不具合が生じるので当面使用しない 
  /*
  for(i=0;i<length;i++){
    for(j=0;j<TUPLE_SIZE;j++){
      if( destination == 1 ){
	if( upper > 0 ){
	  a1st[ chain[i] ].tuple_parent[j]=upper;
	}else{
	  a1st[ chain[i] ].tuple_parent[j]=0;
	}
      }else{
	if( lower > 0 ){
	  a1st[ chain[i] ].tuple_child[j]=lower;
	}else{
	  a1st[ chain[i] ].tuple_child[j]=0;
	}
      }
    }
  }

  for(i=0;i<length;i++){
    if( destination == 1 ){  // トップダウンの整列におけるtupleの更新  
      for( bp = a1st[ chain[i] ].adj_list; bp != NULL ; bp=bp->next){
        if( a1st[ bp->alias ].layer == (a1st[ chain[i] ].layer-1) ){
          // 挿入ソートにより親に関するtupleを更新
          j=TUPLE_SIZE-2;
	  while( (j >= 0) && (a1st[ chain[i] ].tuple_parent[j] > a1st[ bp->alias ].grade) ){
	    a1st[ chain[i] ].tuple_parent[ j+1 ]=a1st[ chain[i] ].tuple_parent[j];
	    j--;
	  }
          a1st[ chain[i] ].tuple_parent[j+1]=a1st[ bp->alias ].grade;
	}
      }
    }else{  // ボトムアップの整列におけるtupleの更新
      for( bp = a1st[ chain[i] ].adj_list; bp != NULL ; bp=bp->next){
	if( a1st[ bp->alias ].layer == (a1st[ chain[i] ].layer+1) ){
          // 挿入ソートにより子に関するtupleを更新
	  j=TUPLE_SIZE-2;
	  while( (j >= 0) && (a1st[ chain[i] ].tuple_child[j] > a1st[ bp->alias ].grade) ){
	    a1st[ chain[i] ].tuple_child[ j+1 ]=a1st[ chain[i] ].tuple_child[j];
	    j--;
	  }
	  a1st[ chain[i] ].tuple_child[j+1]=a1st[ bp->alias ].grade;
	}
      }
    }
  }
  */

#ifdef TEST_UPDATE_TUPLE
  for(i=0;i<length;i++){
    printf("%d : ",chain[i]);
    for(j=0;j<TUPLE_SIZE;j++){
      printf("%d ",a1st[ chain[i] ].tuple_parent[j]);
    }
    printf("; ");
    for(j=0;j<TUPLE_SIZE;j++){
      printf("%d ",a1st[ chain[i] ].tuple_child[j]);
    }
    printf("\n");
  }

  //  printf("tuple updated \n\n");
#endif

  return EXIT_SUCCESS;

}


