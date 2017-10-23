// assign_cast_code CASTコードをふる関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_ASSIGN_CAST_CODE 1

extern int iatm;
extern struct atom *a1st;
extern int *cast_codes;
extern int *relative_parent;

int assign_cast_code( int epsilon ){

  int alpha,beta,gamma;

#ifdef TEST_ASSIGN_CAST_CODE
  printf("assign_cast_code \n");
  printf("epsilon %d \n",epsilon);
#endif

  alpha=MINUS;
  beta=MINUS;
  gamma=MINUS;

  // 仮想的な原子は, 他方の原子の座標を利用する
  if( epsilon > iatm ){
    if( a1st[ (a1st[epsilon].adj_list)->alias ].grade < a1st[ (a1st[epsilon].last)->alias ].grade ){
      a1st[ epsilon ].xco = a1st[ (a1st[epsilon].last)->alias ].xco;
      a1st[ epsilon ].yco = a1st[ (a1st[epsilon].last)->alias ].yco;
      a1st[ epsilon ].zco = a1st[ (a1st[epsilon].last)->alias ].zco;
    }else{
      a1st[ epsilon ].xco = a1st[ (a1st[epsilon].adj_list)->alias ].xco;
      a1st[ epsilon ].yco = a1st[ (a1st[epsilon].adj_list)->alias ].yco;
      a1st[ epsilon ].zco = a1st[ (a1st[epsilon].adj_list)->alias ].zco;
    }
  }

  if( relative_parent[ epsilon ] != MINUS ){ // relative_parent が同定されているかのチェック
    gamma = identify_ancestors( relative_parent[ epsilon ],epsilon );

    if( gamma != MINUS && a1st[ gamma ].layer >= 2 && relative_parent[ gamma ] != MINUS ){
      beta = identify_ancestors( relative_parent[ gamma ],gamma );
#ifdef TEST_ASSIGN_CAST_CODE
      printf("beta %d gamma %d epsilon %d \n",beta,gamma,epsilon);
      printf("relative_parent of beta %d is %d \n",beta,relative_parent[beta]);
#endif

      if( beta != MINUS && a1st[ beta ].layer >= 1 && relative_parent[ beta ] != MINUS ){
	alpha = identify_ancestors( relative_parent[ beta ],beta );

#ifdef TEST_ASSIGN_CAST_CODE
	printf("alpha %d beta %d gamma %d epsilon %d \n",alpha,beta,gamma,epsilon);
#endif

	if( alpha != MINUS ){
	  cast_codes[ epsilon ] = get_dihedral_angle( alpha, beta, gamma, epsilon );
	}else{
	  cast_codes[ epsilon ] = UK;
	}
      }else{
	cast_codes[ epsilon ] = UK;
      }
    }else{
      cast_codes[ epsilon ] = UK;
    }

  }else{
    cast_codes[ epsilon ] = UK;
  }

#ifdef TEST_ASSIGN_CAST_CODE
  printf("alpha %d beta %d gamma %d epsilon %d \n",alpha,beta,gamma,epsilon);
  printf("atom %d cast %d \n",epsilon,cast_codes[epsilon]);
#endif

  return EXIT_SUCCESS;

}
