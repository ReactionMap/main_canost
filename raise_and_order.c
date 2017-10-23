// raise_and_order.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

extern struct level root;
extern struct level *top;
extern struct level *reef;

int raise_and_order(int higher,int *caddy,int space){

  if( raise_rank( higher, caddy, space ) != EXIT_SUCCESS ){
    printf(" error : Cannot execute raise_rank \n");
    exit( EXIT_FAILURE );
  }

  // top, reef の再初期化
  top = &root;
  reef = top->next;

  if( ordering() != EXIT_SUCCESS ){
    printf(" error : Cannot execute ordering \n");
    exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;

}
