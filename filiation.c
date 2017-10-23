// filiation.c 各原子の親を出力

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

//#define TEST_FILIATION 1

extern int iatm;
extern struct atom *a1st;
extern FILE *fo;
extern struct level *top,*down;

int filiation(void){

  int i;
  
  int *comps;
  struct bond *bp;
  struct level *lp; 

#ifdef TEST_FILIATION
  printf("filiation \n");
#endif

  fprintf(fo,"%d-root\n",*(top->member)+1 );
 
  for( lp=top ; lp->next->next != down->next ; lp=lp->next ){
    comps=lp->member;
    for( i=0 ; i< lp->member_Num ; i++ ){
      if( comps[i] < iatm ){
	for( bp=a1st[ comps[i] ].adj_list ; bp != NULL ; bp=bp->next){
	  if( a1st[ bp->alias ].layer > a1st[ comps[i] ].layer ){
	    if( bp->ring == 0 ){
	      fprintf(fo,"%d-%d;",bp->second+1,comps[i]+1);
	    }
	  }
	  if( bp->ring != 0 ){
	    fprintf(fo,"&%d-%d;",bp->ring,comps[i]+1);
	  }
	}
      }
    }
    fprintf(fo,"\n");
  }
  fprintf(fo,"\n");  

  return EXIT_SUCCESS;

}

