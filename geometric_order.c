// geometric_order.c CASTコードから決まる順位

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_GEOMETRIC_ORDER 1

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern int *cast_codes;
extern int *direct_parent;
extern int *relative_parent;
extern int *relation;

int geometric_order( int *caddy, int scale ){

  int i;

  struct bond *bp;

  int parent;
  int count_children;
  int base_child;
  int count_base;
  int first_or_third;
  int outsider;
  int rotate_angle;

#ifdef TEST_GEOMETRIC_ORDER
  printf("\n geometric_order \n");

  for(i=0;i<scale;i++){
    printf("atom %d parent %d relation %d grade %d \n",
            caddy[i],direct_parent[ caddy[i] ],relation[ caddy[i] ], a1st[ caddy[i] ].grade);
  }
#endif

  // 親を特定
  parent=direct_parent[ caddy[0] ];

#ifdef TEST_GEOMETRIC_ORDER
  if( relative_parent[ parent ] > 0 && relative_parent[ relative_parent [ parent ] ] > 0 ){
    printf("parent %d parent's relation %d gp %d ggp %d \n",parent,relation[ parent ],
	   relative_parent[ parent],relative_parent[ relative_parent[ parent ] ] );
  }else{
    printf("parent %d parent's relation %d gp %d \n",parent,relation[ parent ],relative_parent[ parent ]);
  }
#endif

  count_children=0;
  base_child=iatm+new_atm;
  count_base=0;
  first_or_third=TIE;
  outsider=iatm+new_atm;
  /*
  rotate_angle = 12;
  */

  for( bp=a1st[ parent ].adj_list ; bp != NULL ; bp=bp->next ){
    if( ( a1st[ bp->alias ].layer == a1st[ parent ].layer+1 ) &&
        ( parent == direct_parent[ bp->alias ] ) ){
      // count the number of children
      count_children++;

#ifdef TEST_GEOMETRIC_ORDER
      printf("bp->alias %d grade %d angle %d \n",bp->alias,a1st[ bp->alias ].grade,cast_codes[ bp->alias ] );
#endif

      if( a1st[ bp->alias ].grade < a1st[ caddy[0] ].grade ){
#ifdef TEST_GEOMETRIC_ORDER
	printf("first \n");
#endif
        first_or_third = FIRST;
        outsider=bp->alias;
        count_base++;

	/*
	count_base++;
	first_or_third = FIRST;

        if( outsider == iatm+new_atm ){
	  outsider=bp->alias;
	  rotate_angle = cast_codes[ bp->alias ];

	}else{
	  if( a1st[ bp->alias ].grade < a1st[ outsider ].grade ){
	    outsider=bp->alias;
	    rotate_angle = cast_codes[ bp->alias ];
	  }else{
            if( cast_codes[ bp->alias ] < rotate_angle ){
	      outsider=bp->alias;
	      rotate_angle = cast_codes[ bp->alias ];
	    }
	  }
	}
	*/

      }else if( a1st[ bp->alias ].grade > a1st[ caddy[0] ].grade ){
#ifdef TEST_GEOMETRIC_ORDER
	printf("third \n");
#endif

        first_or_third = THIRD;
        outsider=bp->alias;
        count_base++;

        /*
        count_base++;

	if( first_or_third == TIE ||  first_or_third == THIRD ){

	  if( first_or_third == TIE ){
	    first_or_third = THIRD;
	  }

	  if( outsider == iatm+new_atm ){
	    outsider=bp->alias;
	    rotate_angle = cast_codes[ bp->alias ];
	  }else{
	    if( a1st[ bp->alias ].grade < a1st[ outsider ].grade ){
	      outsider=bp->alias;
	      rotate_angle = cast_codes[ bp->alias ];
	    }else{
	      if( cast_codes[ bp->alias ] < rotate_angle ){
		outsider=bp->alias;
		rotate_angle = cast_codes[ bp->alias ];
	      }
	    }
	  }

	}
	*/

      }else{
	continue;
      }
    }

  }

  if( count_children >= 4 ){

    printf("error : Error occured in geometric_order (1) \n");
    exit( EXIT_FAILURE );

  }else if( count_children == 3 ){
#ifdef TEST_GEOMETRIC_ORDER
    printf("count_children = %d \n",count_children);
#endif

    if( first_or_third == FIRST || first_or_third == THIRD ){

      if( count_base >= 2 ){
	printf("error : Error occured in geometric_order (2) \n");
	exit( EXIT_FAILURE );
      }

      rotate_angle = cast_codes[ outsider ];

      if( cast_codes[ caddy[0] ] < cast_codes[ caddy[1] ] && cast_codes[ caddy[1] ] < rotate_angle ){
	a1st[ caddy[0] ].grade++;
      }
      if( rotate_angle < cast_codes[ caddy[1] ] && cast_codes[ caddy[1] ] < cast_codes[ caddy[0] ] ){
	a1st[ caddy[1] ].grade++;
      }
      if( cast_codes[ caddy[1] ] < rotate_angle && rotate_angle < cast_codes[ caddy[0] ] ){
	a1st[ caddy[0] ].grade++;
      }
      if( cast_codes[ caddy[0] ] < rotate_angle && rotate_angle < cast_codes[ caddy[1] ] ){
	a1st[ caddy[1] ].grade++;
      }
      if( rotate_angle < cast_codes[ caddy[0] ] && cast_codes[ caddy[0] ] < cast_codes[ caddy[1] ] ){
	a1st[ caddy[0] ].grade++;
      }
      if( cast_codes[ caddy[1] ] < cast_codes[ caddy[0] ] && cast_codes[ caddy[0] ] < rotate_angle ){
	a1st[ caddy[1] ].grade++;
      }

    }else{ // first_or_third == TIE
      // conformation 依存の順位を持つ原子
      for(i=0; i < scale ;i++){
	add_primary_queue( caddy[i] );
      }
      add_primary_queue( MINUS );

      if( cast_codes[ caddy[0] ] < cast_codes[ caddy[1] ] && cast_codes[ caddy[1] ] < cast_codes[ caddy[2] ] ){
	a1st[ caddy[0] ].grade++;
	a1st[ caddy[2] ].grade +=2;
      }
      if( cast_codes[ caddy[2] ] < cast_codes[ caddy[1] ] && cast_codes[ caddy[1] ] < cast_codes[ caddy[0] ] ){
	a1st[ caddy[2] ].grade++;
	a1st[ caddy[0] ].grade +=2;
      }
      if( cast_codes[ caddy[1] ] < cast_codes[ caddy[2] ] && cast_codes[ caddy[2] ] < cast_codes[ caddy[0] ] ){
	a1st[ caddy[1] ].grade++;
	a1st[ caddy[0] ].grade +=2;
      }
      if( cast_codes[ caddy[0] ] < cast_codes[ caddy[2] ] && cast_codes[ caddy[2] ] < cast_codes[ caddy[1] ] ){
	a1st[ caddy[0] ].grade++;
	a1st[ caddy[1] ].grade +=2;
      }
      if( cast_codes[ caddy[2] ] < cast_codes[ caddy[0] ] && cast_codes[ caddy[0] ] < cast_codes[ caddy[1] ] ){
	a1st[ caddy[2] ].grade++;
	a1st[ caddy[1] ].grade +=2;
      }
      if( cast_codes[ caddy[1] ] < cast_codes[ caddy[0] ] && cast_codes[ caddy[0] ] < cast_codes[ caddy[2] ] ){
	a1st[ caddy[1] ].grade++;
	a1st[ caddy[2] ].grade +=2;
      }

    }

  }else{ // count_children == 2
#ifdef TEST_GEOMETRIC_ORDER
    printf("count_children = %d \n",count_children);
#endif

    if( ( relation[ parent ] == SINGLE || relation[ parent ] == TRIPLE ) && 
        relation[ caddy[0] ] == SINGLE && relation[ caddy[1] ] == SINGLE ){
      /*
         -- p -- c1     # p -- c1
            |             |
            c2            c2
      */

      // conformation 依存の順位を持つ原子
      for(i=0; i < scale ;i++){
	add_primary_queue( caddy[i] );
      }
      add_primary_queue( MINUS );

      if( cast_codes[ caddy[0] ] < cast_codes[ caddy[1] ] ){
	if( ( 0 <= cast_codes[ caddy[0] ] && cast_codes[ caddy[0] ] <= 3 ) && 
            ( 9 < cast_codes[ caddy[1] ] && cast_codes[ caddy[1] ] <= 11) ){
	  a1st[ caddy[1] ].grade++;
	}else{
	  a1st[ caddy[0] ].grade++;
	}
      }else{
	if( ( 0 <= cast_codes[ caddy[1] ] && cast_codes[ caddy[1] ] <= 3 ) && 
            ( 9 < cast_codes[ caddy[0] ] && cast_codes[ caddy[0] ] <= 11) ){
	  a1st[ caddy[0] ].grade++;
	}else{
	  a1st[ caddy[1] ].grade++;
	}
      }

    }else if( ( relation[ parent ] == DOUBLE || relation[ parent ] == S_OR_D ) && 
              relation[ caddy[0] ] == SINGLE && relation[ caddy[1] ] == SINGLE ){

      /*
         == p -- c1
            |
            c2
      */

      for(i=0; i < scale ;i++){
	if( 3 < cast_codes[ caddy[i] ] && cast_codes[ caddy[i] ] <= 9 ){
	  a1st[ caddy[i] ].grade++;
	}
      }

      if( a1st[ caddy[0] ].grade == a1st[ caddy[1] ].grade ){
	if( cast_codes[ caddy[0] ] < cast_codes[ caddy[1] ] ){
	  a1st[ caddy[1] ].grade++;
	}else{
	  a1st[ caddy[0] ].grade++;
	}
      }

    }else if( relation[ parent ] == S_OR_D  && 
              ( relation[ caddy[0] ] == S_OR_D && relation[ caddy[1] ] == S_OR_D ) ){
      /*
          this structure is considered as 

          == p == c2   == p == c1 
             |            |
             c1           c2
      */

      for(i=0; i < scale ;i++){
	if( 3 < cast_codes[ caddy[i] ] && cast_codes[ caddy[i] ] <= 9 ){
	  a1st[ caddy[i] ].grade++;
	}
      }

    }else if( ( relation[ parent ] == SINGLE || relation[ parent ] == TRIPLE ) && 
              ( ( relation[ caddy[0] ] == SINGLE && relation[ caddy[1] ] == DOUBLE ) || 
	        ( relation[ caddy[0] ] == DOUBLE && relation[ caddy[1] ] == SINGLE ) ||
                ( relation[ caddy[0] ] == S_OR_D && relation[ caddy[1] ] == S_OR_D ) || 
                ( relation[ caddy[0] ] == SINGLE && relation[ caddy[1] ] == S_OR_D ) || 
	        ( relation[ caddy[0] ] == S_OR_D && relation[ caddy[1] ] == SINGLE ) ) ){

      /*
         -- or # p == c2  -- or # p == c1 
                 |                |
                 c1               c2
      */

      // conformation 依存の順位を持つ原子
      for(i=0; i < scale ;i++){
	add_primary_queue( caddy[i] );
      }
      add_primary_queue( MINUS );

      for(i=0; i < scale ;i++){
	if( 3 < cast_codes[ caddy[i] ] && cast_codes[ caddy[i] ] <= 9 ){
	  a1st[ caddy[i] ].grade++;
	}
      }

    }else if( ( relation[ parent ] == SINGLE || relation[ parent ] == TRIPLE ) &&  
              (( relation[ caddy[0] ] == AROMATIC && relation[ caddy[1] ] == AROMATIC ) ||
               ( relation[ caddy[0] ] == DOUBLE && relation[ caddy[1] ] == DOUBLE )) ){

      /*
         -- or # p -=- c1  -- or # p == c1
                 |                 ||
                 ||                c2
                 |
                 c2
      */

      // conformation 依存の順位を持つ原子
      for(i=0; i < scale ;i++){
	add_primary_queue( caddy[i] );
      }
      add_primary_queue( MINUS );

      for(i=0; i < scale ;i++){
	if( 3 < cast_codes[ caddy[i] ] && cast_codes[ caddy[i] ] <= 9 ){
	  a1st[ caddy[i] ].grade++;
	}
      }

      if( a1st[ caddy[0] ].grade == a1st[ caddy[1] ].grade ){
	if( cast_codes[ caddy[0] ] < cast_codes[ caddy[1] ] ){
	  a1st[ caddy[1] ].grade++;
	}else{
	  a1st[ caddy[0] ].grade++;
	}
      }

    }else if( relation[ parent ] == AROMATIC &&  
              ( ( relation[ caddy[0] ] == AROMATIC && relation[ caddy[1] ] == AROMATIC ) ||
                ( relation[ caddy[0] ] == SINGLE   && relation[ caddy[1] ] == AROMATIC ) || 
                ( relation[ caddy[0] ] == AROMATIC && relation[ caddy[1] ] == SINGLE   ) ) ){

      /*
         -=-  p -=- c1  -=- p -=- c1  -=- p -- c1 
              |             |             |           
              ||            c2            ||
              |                           |
              c2                          c2
      */

      for(i=0; i < scale ;i++){
	if( 3 < cast_codes[ caddy[i] ] && cast_codes[ caddy[i] ] <= 9 ){
	  a1st[ caddy[i] ].grade++;
	}
      }

      if( a1st[ caddy[0] ].grade == a1st[ caddy[1] ].grade ){
	if( cast_codes[ caddy[0] ] < cast_codes[ caddy[1] ] ){
	  a1st[ caddy[1] ].grade++;
	}else{
	  a1st[ caddy[0] ].grade++;
	}
      }

    }else{
      printf(" error : Error occured in geometric_order (3) \n");
      exit( EXIT_FAILURE );
    }

  }

#ifdef TEST_GEOMETRIC_ORDER
  for(i=0;i<scale;i++){
    printf("atom %d grade %d \n",caddy[i],a1st[ caddy[i] ].grade);
  }
#endif

  return EXIT_SUCCESS;

}
