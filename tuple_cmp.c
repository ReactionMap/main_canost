// tuple_cmp.c tupleが等しいかを判定

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

extern int iatm;
extern int new_atm;
extern struct atom *a1st;

int tuple_cmp( int *tuple_right, int *tuple_left ){

  int i;

  int flag;

  flag=0;
  for(i=0;i<TUPLE_SIZE;i++){
    if( tuple_right[i] >= iatm+new_atm ){
      flag=1;
    }
    if( tuple_left[i] >= iatm+new_atm ){
      flag=1;
    }
    if( (tuple_right[i] >= iatm+new_atm) && (tuple_left[i] >= iatm+new_atm) ){
      flag=0;
      break;
    }
    if( flag == 1 ){
      break;
    }
    if( (a1st[ tuple_right[i] ].codeNum != a1st[ tuple_left[i] ].codeNum ) ||
	(a1st[ tuple_right[i] ].grade != a1st[ tuple_left[i] ].grade ) ){
      flag=1;
      break;
    }
  }

  return flag;

}
