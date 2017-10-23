// scan_children.c レベル 2 の原子の順位を調べる関数 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_SCAN_CHILDREN 1

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern int *direct_parent;
extern int *relative_parent;
extern int *relation;
extern int *confined_bond;

int scan_children( int scanned , int *isolato , int *comrade, int *comNum ){

  int i;

  int children;
  int grade_isolato;
  int iso_type;

  struct bond *bp;

  children = confined_bond[ scanned ] - 1;
  *isolato = MINUS;
  iso_type = MINUS;

  if( children == 0 ){
    // 子を持たない
    iso_type = NOCHILD; 

#ifdef TEST_SCAN_CHILDREN
    printf(" scan_children: scanned %d has no children \n",scanned);
#endif

  }else if( children == 1 ){
    // 子は一つ
    iso_type = FIRST_ONE;
    *comNum = 0;
    for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
      if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	*isolato = bp->alias;
	direct_parent[ bp->alias ] = scanned;
        relative_parent[ bp->alias ] = scanned;
        relation[ bp->alias ] = bp->btype;
      }
    }

#ifdef TEST_SCAN_CHILDREN
    printf(" scan_children: scanned %d has one child \n",scanned);
#endif

  }else if( children == 2 ){

#ifdef TEST_SCAN_CHILDREN
    printf(" scan_children: scanned %d has two children \n",scanned);
#endif

    // 子は二つ
    if( a1st[ scanned ].tuple_child[0] != a1st[ scanned ].tuple_child[1] ){
      // 子には順位が既に付いている
      iso_type = FIRST_TWO;
      *comNum = 0;
      grade_isolato = a1st[ scanned ].tuple_child[0]; // 一番順位の高い子の順位
      for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	  if( a1st[ bp->alias ].grade == grade_isolato ){ // 一番順位の高い子を記憶
	    *isolato = bp->alias;
	  }
	  direct_parent[ bp->alias ] = scanned;
	  relative_parent[ bp->alias ] = scanned;
	  relation[ bp->alias ] = bp->btype;

	}
      }

    }else{
      // 二つの子は同順位
      iso_type = TIE_TWO;
      *comNum = 2;
      i=0;
      for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	  comrade[i] = bp->alias;
	  i++;
	  direct_parent[ bp->alias ] = scanned;
	  relative_parent[ bp->alias ] = scanned;
	  relation[ bp->alias ] = bp->btype;
	}
      }	
    }

  }else if( children == 3 ){

#ifdef TEST_SCAN_CHILDREN
    printf(" scan_children: scanned %d has three children \n",scanned);
#endif

    // 子は三つ
    if( a1st[ scanned ].tuple_child[0] == a1st[ scanned ].tuple_child[1] ){

      if( a1st[ scanned ].tuple_child[1] != a1st[ scanned ].tuple_child[2] ){
	// 0-0-1 の順位の付き方
	iso_type = THIRD_THREE;
	*comNum = 2;
        grade_isolato = a1st[ scanned ].tuple_child[2]; // 一番順位の低い子の順位
	i=0;
	for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){ 
	    if( a1st[ bp->alias ].grade == grade_isolato ){ // 一番順位の低い子を記憶
	      *isolato = bp->alias;
	    }else{
	      comrade[i] = bp->alias; // その他の子は comrade に格納
	      i++;
	    }
	    direct_parent[ bp->alias ] = scanned;
	    relative_parent[ bp->alias ] = scanned;
	    relation[ bp->alias ] = bp->btype;
	  }
	}

      }else{
        // 三つは同順位
	iso_type = TIE_THREE;
	*comNum = 3;
	i=0;
	for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	    comrade[i] = bp->alias; // 全て comrade に格納
	    i++;
	    direct_parent[ bp->alias ] = scanned;
	    relative_parent[ bp->alias ] = scanned;
	    relation[ bp->alias ] = bp->btype;
	  }
	}
      }

    }else{

      if( a1st[ scanned ].tuple_child[1] == a1st[ scanned ].tuple_child[2] ){
	// 0-1-1 の順位の付き方
	iso_type = FIRST_THREE;
	*comNum = 2;
	grade_isolato = a1st[ scanned ].tuple_child[0]; // 一番順位の高い子の順位
	i=0;
	for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	    if( a1st[ bp->alias ].grade == grade_isolato ){ // 一番順位の高い子を記憶
	      *isolato = bp->alias;
	    }else{
	      comrade[i] = bp->alias; // その他の子を comrade に格納
	      i++;
	    }
	    direct_parent[ bp->alias ] = scanned;
	    relative_parent[ bp->alias ] = scanned;
	    relation[ bp->alias ] = bp->btype;
	  }
	}

      }else{
        // 三つは全て異なる順位を持つ
	iso_type = INDIVID_CHILDREN;
	*comNum = 0;
	grade_isolato = a1st[ scanned ].tuple_child[0]; // 一番順位の高い子の順位
	for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	    if( a1st[ bp->alias ].grade == grade_isolato ){ // 一番順位の高い子を記憶
	      *isolato = bp->alias;
	    }
	    direct_parent[ bp->alias ] = scanned;
	    relative_parent[ bp->alias ] = scanned;
	    relation[ bp->alias ] = bp->btype;
	  }
	}

      }

    }

  }else if( children == 4 ){

#ifdef TEST_SCAN_CHILDREN
    printf(" scan_children: scanned %d has four children \n",scanned);
#endif

    if( a1st[ scanned ].tuple_child[0] < a1st[ scanned ].tuple_child[1] ){

      // 四つは全て異なる順位を持つ
      iso_type = INDIVID_CHILDREN;
      *comNum = 0;
      grade_isolato = a1st[ scanned ].tuple_child[0]; // 一番順位の高い子の順位
      for( bp=a1st[ scanned ].adj_list ; bp != NULL ; bp=bp->next ){
	if( a1st[ bp->alias ].layer == a1st[ scanned ].layer+1 ){
	  if( a1st[ bp->alias ].grade == grade_isolato ){ // 一番順位の高い子を記憶
	    *isolato = bp->alias;
	  }
	  direct_parent[ bp->alias ] = scanned;
	  relative_parent[ bp->alias ] = scanned;
	  relation[ bp->alias ] = bp->btype;
	}
      }

    }else{

      printf("error : Cannot deal with the atom %d in scan_children (1) \n", scanned);
      exit( EXIT_FAILURE );

    }

  }else{

    printf("error : Cannot deal with the atom %d in scan_children (2) \n", scanned);
    exit( EXIT_FAILURE );

  }

  return iso_type;

}
