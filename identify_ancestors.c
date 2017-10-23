// identify_ancestors.c 
/***************************************************
 alpha--beta--gamma--epsilon で決まる二面角に関して,
 epsilonとその親を引数に与えたときgammaを返す関数
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_IDENTIFY_ANCESTORS 1

extern int iatm;
extern int new_atm;
extern int start;
extern struct atom *a1st;
extern int *relative_parent;
extern int *relation;

int identify_ancestors( int parent, int child ){

  int base;

#ifdef TEST_IDENTIFY_ANCESTORS
   printf("identify_ancestors: parent %d child %d \n",parent,child);
#endif

   if( ( relation[ parent ] == TRIPLE && relation[ child ] == SINGLE ) ||
       ( relation[ parent ] == SINGLE && relation[ child ] == TRIPLE )    ){  // #(parent)-(child)
     relative_parent[ child ] = relative_parent[ parent ];
     child = parent;
     parent = relative_parent[ parent ];

     if( parent != MINUS ){
       base = identify_ancestors( parent,child );
#ifdef TEST_IDENTIFY_ANCESTORS
       printf("parent %d child %d base %d (1)\n",parent,child,base);
#endif
       return base;
     }else{ // parent == MINUS 始点まで一直線
       return MINUS;
     }

   }else if( relation[ child ] == DOUBLE && relation[ parent ] == DOUBLE ){  // =(parent)=(child)
     relative_parent[ child ] = relative_parent[ parent ];
     child = parent;
     parent = relative_parent[ parent ];

     if( parent != MINUS ){
       base = identify_ancestors( parent,child );
#ifdef TEST_IDENTIFY_ANCESTORS
       printf("parent %d child %d base %d (2)\n",parent,child,base);
#endif
       return base;
     }else{ // parent == MINUS 始点まで一直線
       return MINUS;
     }

   }else if( strcmp(a1st[ parent ].code,"T") == 0 ||   // -C#(child)
             strcmp(a1st[ parent ].code,"T1") == 0 ||  // H-C#(child) #C-H(child)
             strcmp(a1st[ parent ].code,"DD") == 0 ||  // =C=(child)
             strcmp(a1st[ parent ].code,"VD") == 0 ){  // =C=O(child) O=C=(child)
     relative_parent[ child ] = relative_parent[ parent ];
     child = parent;
     parent = relative_parent[ parent ];

     if( parent != MINUS ){
       base = identify_ancestors( parent,child );
#ifdef TEST_IDENTIFY_ANCESTORS
       printf("parent %d child %d base %d (3)\n",parent,child,base);
#endif
       return base;
     }else{ // parent == MINUS 始点まで一直線
       return MINUS;
     }

   }else{

     base = parent;
#ifdef TEST_IDENTIFY_ANCESTORS
     printf("parent %d child %d base %d (4)\n",parent,child,base);
#endif
     return base;

   }

}
