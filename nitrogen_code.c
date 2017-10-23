// nitrogen_code.c assigns CANOST codes for nitrogen atoms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_NITROGEN_CODE 1

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;
extern int error_mode;

int nitrogen_code(int nitrogen){

  int icount_single,icount_double,icount_triple,icount_aromatic,icount_five;
  int icount_h1;
  int ierror;

  struct bond *bp;

#ifdef TEST_NITROGEN_CODE
  printf("nitrogen_code \n");
#endif

  icount_single=0;
  icount_double=0;
  icount_triple=0;
  icount_aromatic=0;
  icount_five=0;
  icount_h1=0;

  ierror = 0;

  for( bp=a1st[ nitrogen ].adj_list ; bp != NULL ; bp=bp->next ){

    // count the number of single bonds
    if( bp->btype == 1 ){
      icount_single++;
    }

    // count the number of double bonds
    if( bp->btype == 2 ){
      icount_double++;
    }

    // count the number of triple bonds
    if( bp->btype == 3 ){
      icount_triple++;
    }

    // count the number of aromatic bonds
    if( bp->btype == 4 ){
      icount_aromatic++;
    }

    // count the number of fifth type bonds
    if( bp->btype == 5 ){
      icount_five++;
    }

    // count the number of hydrogen atoms
    if( a1st[ bp->second ].atmNum == 1 ){
      icount_h1++;
    }

  }

  if( icount_aromatic == 2 ){
    if( icount_h1 == 0 ){
      // aromatic nitrogen
      strcpy( a1st[ nitrogen ].code,"NR");
    }else if( icount_h1 == 1 ){
      // aromatic nitrogen with one hydrogen
      strcpy( a1st[ nitrogen ].code,"NR");
    }else{
      ierror = 1;
    }
  }

  if( icount_five == 2 ){
    if( a1st[ nitrogen ].nbnd == 3 ){
      if( icount_h1 == 0 ){
        // nitrogen having one double bond
	strcpy( a1st[ nitrogen ].code,"ND");
      }else if( icount_h1 == 1 ){
        // nitrogen having one double bond with one hydrogen
	strcpy( a1st[ nitrogen ].code,"ND1");
      }else{
	ierror = 2;
      }
    }else{
      ierror = 3;
    }
  }

  if( icount_five == 1 ){
    if( a1st[ nitrogen ].nbnd == 3 ){
      if( icount_h1 == 0 ){
        // nitrogen with two single bonds and a fifth bond
	strcpy( a1st[ nitrogen ].code,"N");
      }else if( icount_h1 == 1 ){
        //  nitrogen with two single bonds and a fifth bond with one hydrogen
	strcpy( a1st[ nitrogen ].code,"N1");
      }else if( icount_h1 == 2 ){
        //  nitrogen with two single bonds and a fifth bond with two hydrogens
	strcpy( a1st[ nitrogen ].code,"N2");
      }else{
	ierror = 4;
      }
    }else{
      ierror = 5;
    }
  }

  if( icount_double == 1 ){
    if( icount_h1 == 0 ){
      // nitrogen having one double bond
      strcpy( a1st[ nitrogen ].code,"ND");
    }else if( icount_h1 == 1 ){
      // nitrogen having one double bond with one hydrogen
      strcpy( a1st[ nitrogen ].code,"ND1");
    }else if( icount_h1 == 2 ){
      // nitrogen having one double bond with two hydrogen
      strcpy( a1st[ nitrogen ].code,"ND2");
    }else{
      ierror = 6;
    }
  }

  if( icount_double == 2 ){
    // nitrogen having two double bonds
    strcpy( a1st[ nitrogen ].code,"NN");
  }

  if( icount_triple == 1 ){
    // nitrogen having one triple bonds
    strcpy( a1st[ nitrogen ].code,"NT");
  }

  if( icount_single == 3 ){
    if( a1st[ nitrogen ].nbnd == 3 ){
      if( icount_h1 == 0 ){
        // nitrogen with three single bonds
	strcpy( a1st[ nitrogen ].code,"N");
      }else if( icount_h1 == 1 ){
        //  nitrogen with three single bonds with one hydrogen
	strcpy( a1st[ nitrogen ].code,"N1");
      }else if( icount_h1 == 2 ){
        //  nitrogen with three single bonds with two hydrogens
	strcpy( a1st[ nitrogen ].code,"N2");
      }else if( icount_h1 == 3 ){
        //  nitrogen with four single bonds with three hydrogens
	strcpy( a1st[ nitrogen ].code,"N3");
      }else{
	ierror = 7;
      }
    }else{
      ierror = 8;
    }
  }

  if( icount_single == 4 ){
    if( a1st[ nitrogen ].nbnd == 4 ){
      if( icount_h1 == 0 ){
        // nitrogen with four single bonds
	strcpy( a1st[ nitrogen ].code,"N");
      }else if( icount_h1 == 1 ){
        //  nitrogen with four single bonds with one hydrogen
	strcpy( a1st[ nitrogen ].code,"N1");
      }else if( icount_h1 == 2 ){
        //  nitrogen with four single bonds with two hydrogens
	strcpy( a1st[ nitrogen ].code,"N2");
      }else if( icount_h1 == 3 ){
        //  nitrogen with four single bonds with three hydrogens
	strcpy( a1st[ nitrogen ].code,"N3");
      }else if( icount_h1 == 4 ){
        //  nitrogen with four single bonds with four hydrogens
	strcpy( a1st[ nitrogen ].code,"N4");
      }else{
	ierror = 9;
      }
    }else{
      ierror = 10;
    }
  }

  if( error_mode == 0 ){
    switch (ierror) {
    case 1:
      printf(" Irregular N type (1) : serial no. %d \n", nitrogen);
      break;
    case 2:
      printf(" Irregular N type (2) : serial no. %d \n", nitrogen);
      break;
    case 3:
      printf(" Irregular N type (3) : serial no. %d \n", nitrogen);
      break;
    case 4:
      printf(" Irregular N type (4) : serial no. %d \n", nitrogen);
      break;
    case 5:
      printf(" Irregular N type (5) : serial no. %d \n", nitrogen);
      break;
    case 6:
      printf(" Irregular N type (6) : serial no. %d \n", nitrogen);
      break;
    case 7:
      printf(" Irregular N type (7) : serial no. %d \n", nitrogen);
      break;
    case 8:
      printf(" Irregular N type (8) : serial no. %d \n", nitrogen);
      break;
    case 9:
      printf(" Irregular N type (9) : serial no. %d \n", nitrogen);
      break;
    case 10:
      printf(" Irregular N type (10) : serial no. %d \n", nitrogen);
      break;
    }

    if( ierror > 0 ){
      printf(" error : Error occured in recognition of substructure \n");
      exit( EXIT_FAILURE );
    }

  }else{

    if( ierror > 0 || ( strcmp( a1st[ nitrogen ].code,"?") == 0 ) ){
      strcpy( a1st[ nitrogen ].code,"irN"); // irregular nitrogen
    }

  }

  return EXIT_SUCCESS;

}
