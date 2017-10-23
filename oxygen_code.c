// oxygen_code.c assigns CANOST codes for oxygen atoms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

//#define TEST_OXYGEN_CODE 1

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;
extern int error_mode;

int oxygen_code(int oxygen){

  int icount_single,icount_double,icount_five;
  int icount_h;
  int iflag1;
  int ierror;

  struct bond *bp,*bp2;

#ifdef TEST_OXYGEN_CODE
  printf("oxygen_code \n");
#endif

  icount_single=0;
  icount_double=0;
  icount_five=0;
  icount_h=0;

  iflag1=0;

  ierror = 0;

  for( bp=a1st[ oxygen ].adj_list ; bp != NULL ; bp=bp->next){
    if( bp->btype == 1 ){
      icount_single++;
      if( a1st[ bp->second ].atmNum == 1){
	icount_h++;
      }
    }else if( bp->btype == 2 ){
      icount_double++;
    }else if( bp->btype == 5 ){
      icount_five++;
    }else{
      ierror = 1;
    }
  }

  if( icount_single == 2 ){ // sp3 oxygen
    if( icount_h == 1 ){  // -OH oxygen
      strcpy( a1st[ oxygen ].code,"Q1");  // -OH
    }else if( icount_h == 0){
      strcpy( a1st[ oxygen ].code,"Q"); // -O-
    }else{
      ierror = 2;
    }

  }else if( icount_single == 1 ){
    if( a1st[ oxygen ].nbnd == 1 ){
      strcpy( a1st[ oxygen ].code,"Q0"); // -O (-1 charge)
    }else{
      ierror = 3;
    }

  }else if( icount_double == 1 ){ // sp2 oxygen
    if( a1st[ oxygen ].nbnd == 1 ){
      strcpy( a1st[ oxygen ].code,"QD"); // =O
    }else{
      ierror = 4;
    }

  }else if( icount_five == 1 ){

    iflag1 = 0;
    if( a1st[ oxygen ].nbnd == 1 ){
      bp = a1st[ oxygen ].adj_list;
      if( a1st[ bp->second ].atmNum == 7 ){ // A nitrogen is connected
	for( bp2 = a1st[ bp->second ].adj_list ;
             bp2 != NULL ;
             bp2 = bp2->next ){
	  if( bp2->btype == 5 && a1st[ bp2->second ].atmNum == 8 ){
	    // An oxygen is connected via fifth bond
	    iflag1 = 1;             
	  }
	}
	if( iflag1 == 1 ){
	  strcpy( a1st[ oxygen ].code,"Q0"); // - or = O
	}else{
	  ierror = 5;
	}
      }else if(a1st[ bp->second ].atmNum == 15 ){ // A phosphorus is connected
 	strcpy( a1st[ oxygen ].code,"Q"); // - or = O
      }else{
	ierror = 6;
      }
    }else{
      ierror = 7;
    }

  }else{
    ierror = 8;
  }

  if( error_mode == 0 ){

    switch (ierror){
    case 1:
      printf(" Irregular O type (1) : serial no. %d \n", oxygen);
      break;
    case 2:
      printf(" Irregular O type (2) : serial no. %d \n", oxygen);
      break;
    case 3:
      printf(" Irregular O type (3) : serial no. %d \n", oxygen);
      break;
    case 4:
      printf(" Irregular O type (4) : serial no. %d \n", oxygen);
      break;
    case 5:
      printf(" Irregular O type (5) : serial no. %d \n", oxygen);
      break;
    case 6:
      printf(" Irregular O type (6) : serial no. %d \n", oxygen);
      break;
    case 7:
      printf(" Irregular O type (7) : serial no. %d \n", oxygen);
      break;
    case 8:
      printf(" Irregular O type (8) : serial no. %d \n", oxygen);
      break;
    }

    if( ierror > 0 ){
      printf(" error : Error occured in recognition of substructure \n");
      exit( EXIT_FAILURE );
    }

  }else{
    if( ierror > 0 ){
      strcpy( a1st[ oxygen ].code,"irO"); // irregular oxygen
    }
  }

  return EXIT_SUCCESS;

}
