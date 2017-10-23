// carbon_code.c assigns CANOST codes for carbon atoms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_CARBON_CODE 1

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;
extern int error_mode;

int carbon_code(int carbon){

  int icount_single,icount_double,icount_triple,icount_aromatic,icount_five;
  int icount_s_o,icount_d_o;
  int icount_h1,icount_h2;
  int iflag1,iflag2;
  int ierror;

  struct bond *bp,*bp2,*bp3;

#ifdef TEST_CARBON_CODE
  printf("carbon_code \n");
#endif

  icount_single=0;
  icount_double=0;
  icount_triple=0;
  icount_aromatic=0;
  icount_five=0;
  icount_s_o=0;
  icount_d_o=0;
  icount_h1=0;
  icount_h2=0;

  ierror = 0;

  for( bp=a1st[ carbon ].adj_list ; bp != NULL ; bp=bp->next ){

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

    // count the number of C-O
    if( bp->btype == 1 && a1st[ bp->second ].atmNum == 8 ){
      icount_s_o++;
    }

    // count the number of C=O
    if( bp->btype == 2 && a1st[ bp->second ].atmNum == 8 ){
      icount_d_o++;
    }

    // count the number of hydrogen atoms
    if( a1st[ bp->second ].atmNum == 1 ){
      icount_h1++;
    }

  }

  if( a1st[ carbon ].nbnd == 4 ){ // sp3 carbon

    if( icount_single == 4 ){  // four single bonds 
      if( icount_h1 == 0 ){
	strcpy( a1st[ carbon ].code,"C");   // code "C"
      }else if(icount_h1 == 1){
	strcpy( a1st[ carbon ].code,"C1");  // code "C1"
      }else if(icount_h1 == 2){
	strcpy( a1st[ carbon ].code,"C2");  // code "C2"
      }else if(icount_h1 == 3){
	strcpy( a1st[ carbon ].code,"C3");  // code "C3"
      }else if(icount_h1 == 4){
	strcpy( a1st[ carbon ].code,"C4");  // code "C4"
      }else{
	ierror = 1;
      }
    }else{
      ierror = 2;
    }

  }else if( a1st[ carbon ].nbnd == 3 ){  // sp2 carbon

    if( icount_aromatic == 3 ){ // the number of aromatic bonds is three

      if( icount_h1 == 0 ){
	strcpy( a1st[ carbon ].code,"Y");  // aromatic C=C without hydrogen 
      }else if( icount_h1 == 1){
	strcpy( a1st[ carbon ].code,"Y1");  // aromatic C=C-H with hydrogen
      }else{
	ierror = 3;
      }
 
    }else if( icount_aromatic == 2 ){ // the number of aromatic bonds is two

      iflag1=0;
      iflag2=0;

      if( icount_s_o == 1 ){ // An oxygen is connected via single bond
	for( bp=a1st[ carbon ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( bp->btype == 1 && a1st[ bp->second ].atmNum == 8 ){ // the oxygen
	    icount_h2 = 0;
            // check the oxygen
	    for( bp2 = a1st[ bp->second ].adj_list ;
		 bp2 != NULL;
		 bp2 = bp2->next ){
	      if( a1st[ bp2->second ].atmNum == 1 ){ // Hydrogen is connected
		icount_h2++;
	      }
	    }
	    if( icount_h2 == 1){ // -OH
	      iflag1=1;
	    }
	  }
	  if( bp->btype == 4 && a1st[ bp->second ].atmNum == 6 ){
	    // A carbon is connected via aromatic bond
	    for( bp2 = a1st[ bp->second ].adj_list ;
		 bp2 != NULL;
		 bp2 = bp2->next ){
	      if( bp2->btype == 2 && a1st[ bp2->second ].atmNum == 8 ){
	        // An oxygen is connected via double bond
		iflag2=1;
	      }
	    }
	  }
	}
	if( iflag1 == 1 && iflag2 == 1 ){
	  strcpy( a1st[ carbon ].code,"Y");  // C in tropolone
	  /* 
	  strcpy( a1st[ carbon ].code,"YTT");  // C in tropolone
	  */
	}
      } // end of if( icount_s_o == 1 )

      if( icount_d_o == 1 ){ // An oxygen is connected via double bond
	iflag1=1;
	for( bp=a1st[ carbon ].adj_list ; bp != NULL ; bp=bp->next ){
	  if( bp->btype == 4 && a1st[ bp->second ].atmNum == 6 ){
	    // A carbon is connected via aromatic bond
	    for( bp2 = a1st[ bp->second ].adj_list ;
		 bp2 != NULL;
		 bp2 = bp2->next ){
	      if( bp2->btype == 1 && a1st[ bp2->second ].atmNum == 8 ){
		// An oxygen is connected via single bond
		icount_h2=0;
                // count the number of hydrogen atoms
		for( bp3 = a1st[ bp2->second ].adj_list ;
		     bp3 != NULL;
		     bp3 = bp3->next ){
		  if( a1st[ bp3->second ].atmNum == 1 ){
		    icount_h2++;
		  }
		}
		if( icount_h2 == 1 ){ // -OH
		  iflag2=1;
		}
	      }
	    }
	  }
	}
	if( iflag1 == 1 && iflag2 == 1 ){
	  strcpy( a1st[ carbon ].code,"V");  // C in tropolone
	  /* 
	  strcpy( a1st[ carbon ].code,"YTT");  // C in tropolone
	  */
	}
      } // end of if( icount_d_o == 1 )

      if( !(iflag1 ==1 && iflag2 == 1) ){
	if( icount_h1 == 0 ){
	  strcpy( a1st[ carbon ].code,"Y");  // aromatic C=C
	}else if( icount_h1 == 1){
	  strcpy( a1st[ carbon ].code,"Y1");  // aromatic C=C-H
	}else{
	  ierror = 4;
	}
      }

    }else if( icount_aromatic == 0){  // non aromatic

      if( icount_d_o == 1 ){ // An oxygen atom is connected via double bond

	if( icount_h1 == 0 ){
	  strcpy( a1st[ carbon ].code,"V");   //   |
	                                      //  -C=O
	}else if(icount_h1 == 1){
	  strcpy( a1st[ carbon ].code,"V1");  //   |
	                                      // H-C=O

	}else if(icount_h1 == 2){             //   H
	  strcpy( a1st[ carbon ].code,"V2");  //   |
	                                      // H-C=O
	}else{
	  ierror = 5;
	}

      }else{

	if( icount_double == 1 && icount_single == 2 ){ //  |
	                                                // -C=
	  if( icount_h1 == 0){
	    strcpy( a1st[ carbon ].code,"DS");   //   |
	                                         //  -C=
	  }else if(icount_h1 == 1){
	    strcpy( a1st[ carbon ].code,"D1");   //   |
	                                         // H-C=
	  }else if(icount_h1 == 2){
	    strcpy( a1st[ carbon ].code,"D2");   //   H
	                                         //   |
	                                         // H-C=
	  }else{
	    ierror = 6;
	  }

	}else if( (icount_five == 2 && icount_single == 1) ||
                  icount_five == 3 ){

	  if( icount_h1 == 0 ){
	    strcpy( a1st[ carbon ].code,"DS");
	  }else if( icount_h1 == 1 ){
	    strcpy( a1st[ carbon ].code,"D1");
	  }else{
	    ierror = 7;
	  }

	}else{
	  ierror = 8;
	}
      
      } // end of if( icount_d_o == 1 )

    }else{
      ierror = 9;
    }

  }else if( a1st[ carbon ].nbnd == 2 ){  // sp carbon

    if( icount_double == 2 && icount_d_o == 2 ){
      strcpy( a1st[ carbon ].code,"CW");           // O=C=O
    }else if( icount_double == 2 && icount_d_o == 1 ){
      strcpy( a1st[ carbon ].code,"VD");           // =C=O
    }else if( icount_double == 2 && icount_d_o == 0 ){
      strcpy( a1st[ carbon ].code,"DD");           // =C=
    }else if( icount_double == 0 ){
      if( icount_triple == 1 && icount_single == 1 ){
	if(icount_h1 == 0){
	  strcpy( a1st[ carbon ].code,"T");  // -C#
	}else if(icount_h1 == 1){
	  strcpy( a1st[ carbon ].code,"T1");  // H-C#
	}else{
	  ierror = 10;
	}
      }else{
	ierror = 11;
      }
    }

  }else if( a1st[ carbon ].nbnd == 1 ){

    bp=a1st[ carbon ].adj_list;
    if( bp->btype == 3 && a1st[ bp->second ].atmNum == 7 ){  // sp carbon of -N#C
      strcpy( a1st[ carbon ].code,"iN");
      /* isonitryl carbon */
    }else{
      ierror = 12;
    }

  }else{
    ierror = 13;
  }

  if( error_mode == 0 ){
    switch (ierror){
    case 1:
      printf(" Irregular C type (sp3) (1) : serial no. %d \n", carbon);
      break;
    case 2:
      printf(" Irregular C type (sp3) (2) : serial no. %d \n", carbon);
      break;
    case 3:
      printf(" Irregular C type (sp2) aromatic (1) : serial no. %d \n", carbon);
      break;
    case 4:
      printf(" Irregular C type (sp2) aromatic (2) : serial no. %d \n", carbon);
      break;
    case 5:
      printf(" Irregular C type (carbonyl) (1) : serial no. %d \n", carbon);
      break;
    case 6:
      printf(" Irregular C type (sp2) (1) : serial no. %d\n",carbon);
      break;
    case 7:
      printf(" Irregular C type (sp2) (2) : serial no. %d\n",carbon);
      break;
    case 8:
      printf(" Irregular C type (sp2) (3) : serial no. %d\n",carbon);
      break;
    case 9:
      printf(" Irregular C type (sp2) (4) : serial no. %d\n",carbon);
      break;
    case 10:
      printf(" Irregular C type (sp) (1) : serial no. %d\n", carbon);
      break;
    case 11:
      printf(" Irregular C type (sp) (2) : serial no. %d\n",carbon);
      break;
    case 12:
      printf(" Irregular C type (else) (1) : serial no. %d \n",carbon);
      break;
    case 13:
      printf(" Irregular C type (else) (2) : serial no. %d \n",carbon);
      break;
      // defalut:
      // printf(" Unspecified error : serial no. %d \n",carbon);
      // break;
    }

    if( ierror > 0 ){
      printf(" error : Error occured in recognition of substructure \n");
      exit( EXIT_FAILURE );
    }

  }else{

    if( ierror > 0 ){
      strcpy( a1st[ carbon ].code,"irC");  // irregular carbon
    }

  }

  return EXIT_SUCCESS;

}
