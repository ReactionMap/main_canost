// recognition
/* recongize a partial structure around each atom, and
   assign a CANOST code to the atom */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_RECOG 1

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;

#ifdef TEST_RECOG
extern FILE *dummyfile;
#endif

int recognition( void ){

  int i;

  int icount_single,icount_double,icount_triple,icount_aromatic,icount_five;
  int icount_d_o;
  int icount_h;
  int ierror;

  struct bond *bp;

  char acount_h[2];

#ifdef TEST_RECOG
  printf("recognition \n");
#endif

  /*** Recognize substructure ***/
  icount_single=0;
  icount_double=0;
  icount_triple=0;
  icount_aromatic=0;
  icount_five=0;
  icount_d_o=0;
  icount_h=0;

  ierror=0;
  bp=NULL;

  for(i=1; i < iatm ;i++){

#ifdef TEST_RECOG
    printf("recognition atom %d \n",i);
#endif

    if( a1st[i].atmNum == 6 ){  // Carbon

      if( carbon_code(i) != EXIT_SUCCESS ){
	printf("error: Cannot assign CANOST codes for carbon atoms \n");
	exit( EXIT_FAILURE );
      }

    }else if( a1st[i].atmNum == 8 ){  // Oxygen

      if( oxygen_code(i) != EXIT_SUCCESS ){
	printf("error: Cannot assign CANOST codes for oxygen atoms \n");
	exit( EXIT_FAILURE );
      }

    }else if( a1st[i].atmNum == 9 ||
              a1st[i].atmNum == 17 ||
              a1st[i].atmNum == 35 ||
	      a1st[i].atmNum == 53 ){ // Halogen atoms

      if( halogen_code(i) != EXIT_SUCCESS ){
	printf("error: Cannot assign CANOST codes for halogen atoms \n");
	exit( EXIT_FAILURE );
      }

    }else if( a1st[i].atmNum == 1 ){  // Hydrogen

      if( a1st[i].nbnd == 1 ){
        strcpy( a1st[i].code,"H");
      }else{
        ierror=1;
	printf(" Illegal H type : serial no. %d \n ",i);
      }

    }else if( a1st[i].atmNum == 104 ){  // Diplogen

      if( a1st[i].nbnd == 1 ){
        strcpy( a1st[i].code,"D");
      }else{
        ierror=1;
	printf(" Illegal Diplogen type : serial no. %d \n ",i);
      }

    }else if( a1st[i].atmNum == 7 ){  // Nitrogen

      if( nitrogen_code(i) != EXIT_SUCCESS ){
	printf("error: Cannot assign CANOST codes for nitorogen atoms \n");
	exit( EXIT_FAILURE );
      }

    }else if( a1st[i].atmNum == 15 ){  // Phosphorus

      icount_double=0;
      icount_aromatic=0;
      icount_five=0;

      for( bp=a1st[i].adj_list ; bp != NULL ; bp=bp->next ){
	if( bp->btype == 2 ){
	  icount_double++; // count the number of double bonds 
	}

	if( bp->btype == 4 ){
	  icount_aromatic++; // count the number of double bonds 
	}

	if( bp->btype == 5 ){
	  icount_five++; // count the number of double bonds 
	}

      }

      if( icount_double >= 1 ){
        strcpy( a1st[i].code,"PD");

      }else if( icount_aromatic >= 1 ){
        strcpy( a1st[i].code,"PD");

      }else if( icount_five >= 2 ){
        strcpy( a1st[i].code,"PD");

      }else{
        strcpy( a1st[i].code,"P");
      }

    }else if( a1st[i].atmNum == 16 ){  // Sulfur

      icount_double=0;
      icount_aromatic=0;
      icount_d_o=0;

      for( bp=a1st[i].adj_list ; bp != NULL ; bp=bp->next ){
	if( bp->btype == 2 ){
	  icount_double++; // count the number of double bonds 
	  if( a1st[ bp->second ].atmNum == 8 ){
            icount_d_o++;  // count number of =O
	  }
	}

	if( bp->btype == 4 ){
	  icount_aromatic++;
	}

      }

      if( icount_double >= 1 ){
	if( icount_d_o == 2 ){
	  strcpy( a1st[i].code,"SW");
	}else{
	  strcpy( a1st[i].code,"SD");
	}

      }else if( icount_aromatic >= 1 ){
	strcpy( a1st[i].code,"SD");

      }else{
        strcpy( a1st[i].code,"S");
      }

    }else{  // X (other atom)

      icount_single=0;
      icount_double=0;
      icount_triple=0;
      icount_aromatic=0;
      icount_d_o=0;
      icount_h=0;

      for( bp=a1st[i].adj_list ; bp != NULL ; bp=bp->next ){

        if( bp->btype == 1){
          icount_single++; // count the number of single bonds
	  if( a1st[ bp->second ].atmNum == 1 ){
	    icount_h++; // count the number of hydrogens
	  }

	}else if( bp->btype == 2 ){
	  icount_double++; // count the number of double bonds 
	  if( a1st[ bp->second ].atmNum == 8 ){
            icount_d_o++;  // count number of =O
	  }

	}else if( bp->btype == 3 ){
          icount_triple++; // count number of triple bonds

	}else if( bp->btype == 4 ){
	  icount_aromatic++;  // count number of Hydrogen
	}

      }

      if( icount_aromatic == 2 || icount_aromatic == 1 ){
	strcpy( a1st[i].code,a1st[i].atm_type);
        strcat( a1st[i].code,"R");  // X in aromatic ring
      }else if( icount_double == 2 ){ 
        if( icount_d_o == 2 ){
	  strcpy( a1st[i].code,a1st[i].atm_type);
          strcat( a1st[i].code,"W");  // O=X=O
	}else{
          strcpy( a1st[i].code, a1st[i].atm_type);
          strcat( a1st[i].code,"N");  // =X=
	}
      }else if( icount_double == 1 ){
	strcpy( a1st[i].code, a1st[i].atm_type);
        strcat( a1st[i].code,"D");  // =X
      }else if( icount_triple == 1 ){
	strcpy( a1st[i].code, a1st[i].atm_type);
        strcat( a1st[i].code,"T");  // #X
      }else if( icount_h >= 1 ){
        sprintf(acount_h,"%d",icount_h);
    	strcpy( a1st[i].code, a1st[i].atm_type);
        strcat( a1st[i].code, acount_h);  // Xnp
      }else{ 
	strcpy( a1st[i].code, a1st[i].atm_type); // other atom
      }

    } // end of (other atoms)

  }

#ifdef TEST_RECOG
  dummyfile=fopen("dummy_recognition.dat","w");

  for(i=1;i<iatm;i++){
    fprintf(dummyfile,"%d %s \n",i,a1st[i].code );
  }

  fclose(dummyfile);
#endif

  if( ierror == 1 ){
    printf(" error : Error occured in recognition of substructure \n");
    exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;

}
