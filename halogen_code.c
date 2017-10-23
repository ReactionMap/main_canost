// halogen_code.c aasings CANOST condes for halogens

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_HALOGEN_CODE 1

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;

int halogen_code(int halogen){

  int ierror;

#ifdef TEST_HALOGEN_CODE
  printf("halogen_code \n");
#endif

  ierror = 0;

  if( a1st[ halogen ].atmNum == 9 ){  // Flourine
    if( a1st[ halogen ].nbnd == 1 ){
      strcpy( a1st[ halogen ].code,"LF");
    }else{
      ierror=1;
      printf(" Illegal F type : serial no. %d \n ", halogen);
    }

  }else if( a1st[ halogen ].atmNum == 17 ){  // Chlorine
    if( a1st[ halogen ].nbnd == 1 ){
      strcpy( a1st[ halogen ].code,"LC");
    }else{
      ierror=1;
      printf(" Illegal Cl type : serial no. %d \n ", halogen);
    }

  }else if( a1st[ halogen ].atmNum == 35 ){  // Bromine
    if( a1st[ halogen ].nbnd == 1 ){
      strcpy( a1st[ halogen ].code,"LB");
    }else{
      ierror=1;
      printf(" Illegal Br type : serial no. %d \n ", halogen);
    }

  }else if( a1st[ halogen ].atmNum == 53 ){  // Iodine
    if( a1st[ halogen ].nbnd == 1 ){
      strcpy( a1st[ halogen ].code,"LJ");
    }else{
      ierror=1;
      printf(" Illegal I type : serial no. %d \n ",halogen);
    }
  }

  if( ierror == 1 ){
    printf(" error : Error occured in recognition of substructure \n");
    exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;

}
