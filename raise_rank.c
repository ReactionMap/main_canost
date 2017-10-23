// raise_rank.c ひとつの原子の順位を高くする

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

extern struct atom *a1st;

int raise_rank(int higher,int *caddy,int space){

  int i;
  
  for( i=0;i<space;i++){
    if( higher != caddy[i] ){
	a1st[ caddy[i] ].grade++;
    }
  }

  return EXIT_SUCCESS;

}
