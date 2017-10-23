// confine_level.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_CONFINE_LEVEL 1

#ifdef TEST_CONFINE_LEVEL
extern FILE *dummyfile;
#endif

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern struct level *top;
extern struct level *down;
extern int lowest;
extern int boundary;
extern int *confined_bond;
extern int atoms_in_confine;

int confine_level( void ){

  int i,j;

  struct bond *bp; // 結合を指すポインタ

  struct level *lp;
  int *comps;

  for( i=1 ; i < iatm+new_atm ; i++ ){
    confined_bond[i] = a1st[i].nbnd;
  }

  atoms_in_confine = 0;
  if( boundary != lowest+1 ){ // CANOSTコード生成のレベルが指定されている
    i = 0;
    for( lp = top; lp->next != NULL ; lp=lp->next ){
      atoms_in_confine += lp->member_Num;
      if( i == boundary ){
	comps = lp->member;
	for( j=0; j < lp->member_Num ; j++ ){
	  if( comps[j] < iatm ){
	    for( bp = a1st[ comps[j] ].adj_list ; bp != NULL ; bp = bp->next ){
	      if( a1st[ bp->alias ].layer >= a1st[ comps[j] ].layer+1 ){
		--confined_bond[ comps[j] ];
		a1st[ bp->alias ].layer =  a1st[ comps[j] ].layer+2;
	      }
	    }
	  }
	}
	down = lp->next; // 指定されたレベルのひとつ下のレベルを down に設定
	break;
      }
      i++;
    }
    atoms_in_confine++;
  }else{
    atoms_in_confine = iatm+new_atm;
  }

#ifdef TEST_CONFINE_LEVEL
  dummyfile=fopen("dummy_confine_level.dat","w");

  fprintf(dummyfile,"atoms_in_confine %d\n",atoms_in_confine);
  for( lp=top ; lp->next != NULL ; lp=lp->next ){
    fprintf(dummyfile,"%d \n\n",lp->member_Num );
    comps=lp->member;
    for( i=0 ; i < lp->member_Num ; i++ ){
      fprintf(dummyfile,"%4d %4d %4d\n",comps[i],a1st[ comps[i] ].codeNum,a1st[ comps[i] ].layer );
    }
    fprintf(dummyfile,"\n\n");
  }

  fclose(dummyfile);
#endif

  return EXIT_SUCCESS;

}
