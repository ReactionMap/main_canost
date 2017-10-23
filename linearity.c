// linearity.c 
/***************************************************
 hinge までの結合が直線か否かを判定する関数
 直線ならば TRUE 直線でないならば FALSE
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_LINEARITY 1
// #define TEST_LINEARITY_SECOND 1

extern struct atom *a1st;
extern int *relative_parent;
extern int *relation;

int linearity( int hinge, int parent, int child ){

  int linear_or_broken;

#ifdef TEST_LINEARITY
   printf("linearity: hinge %d parent %d child %d \n", hinge, parent, child);
#endif

#ifdef TEST_LINEARITY_SECOND
   printf("relation of %d is %d \n",hinge,relation[ hinge ]);
   printf("relation of %d is %d \n",parent,relation[ parent ]);
   printf("relation of %d is %d \n",child,relation[ child ]);
#endif

   if( ( relation[ parent ] == TRIPLE && relation[ child ] == SINGLE ) ||
       ( relation[ parent ] == SINGLE && relation[ child ] == TRIPLE )    ){  // #(parent)-(child)

     relative_parent[ child ] = relative_parent[ parent ];
     child = parent;
     parent = relative_parent[ parent ];

     if( parent != hinge ){

       linear_or_broken = linearity( hinge,parent,child );
#ifdef TEST_LINEARITY
       printf("parent %d child %d linear_or_broken %d (1)\n",parent,child,linear_or_broken);
#endif
       return linear_or_broken;

     }else{ // parent == hinge hingeまで一直線
       return TRUE;
     }

   }else if( relation[ child ] == DOUBLE && relation[ parent ] == DOUBLE ){  // =(parent)=(child)

     relative_parent[ child ] = relative_parent[ parent ];
     child = parent;
     parent = relative_parent[ parent ];

     if( parent != hinge ){

       linear_or_broken = linearity( hinge,parent,child );
#ifdef TEST_LINEARITY
       printf("parent %d child %d linear_or_broken %d (2)\n",parent,child,linear_or_broken);
#endif
       return linear_or_broken;

     }else{ // parent == hinge hingeまで一直線
       return TRUE;
     }

   }else if( strcmp(a1st[ parent ].code,"T") == 0 ||   // -C#(child)
             strcmp(a1st[ parent ].code,"T1") == 0 ||  // H-C#(child) #C-H(child)
             strcmp(a1st[ parent ].code,"DD") == 0 ||  // =C=(child)
             strcmp(a1st[ parent ].code,"VD") == 0 ){  // =C=O(child) O=C=(child)

     relative_parent[ child ] = relative_parent[ parent ];
     child = parent;
     parent = relative_parent[ parent ];

     if( parent != hinge ){

       linear_or_broken = linearity( hinge,parent,child );
#ifdef TEST_LINEARITY
       printf("parent %d child %d linear_or_broken %d (3)\n",parent,child,linear_or_broken);
#endif
       return linear_or_broken;

     }else{ // parent == hinge hingeまで一直線
       return TRUE;
     }

   }else{

     linear_or_broken = FALSE;
#ifdef TEST_LINEARITY
       printf("parent %d child %d linear_or_broken %d (4)\n",parent,child,linear_or_broken);
#endif
     return linear_or_broken;

   }

}
