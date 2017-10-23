// prune_branch.c 原子が環構造上の原子かどうかを判定する関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_PRUNE_BRANCH 1

#ifdef TEST_PRUNE_BRANCH
extern struct level *top;
extern FILE *dummyfile;
#endif

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern int start;
extern struct queue *head,*tail;

int prune_branch( void ){

  int i,j;

  struct bond *bp;

  int *connector; // 探索先で最もレベルの高い原子が入る配列
  int *linkage; // 環構造の個数
  int *visit_order; // 探索した順番

#ifdef TEST_PRUNE_BRANCH
  int *comps;
  struct level *lp;

  printf(" prune_branch \n");
#endif

  if( (connector = (int *)malloc((iatm+new_atm)*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in prune_branch \n");
    exit( EXIT_SUCCESS );
  }

  if( (linkage = (int *)malloc((iatm+new_atm)*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in prune_branch \n");
    exit( EXIT_SUCCESS );
  }

  if( (visit_order = (int *)malloc((iatm+new_atm)*sizeof(int))) == NULL ){
    printf(" error : Cannot allocate memory in prune_branch \n");
    exit( EXIT_SUCCESS );
  }

  /*** 初期化 ***/
  for( i=1; i < iatm+new_atm ; i++){
    a1st[i].state = UNVISITED;
    a1st[i].couple = UNCOUPLE;
    connector[i] = iatm + new_atm + 1;
    linkage[i] = 0;
    visit_order[i] = MINUS;
  }

  // Queueの初期化
  head = NULL;
  tail = NULL;
  initialize_primary_queue();

  // 頂点の分類開始
  a1st[ start ].state = VISITED;

  if( visit_atom( start, MINUS, connector, linkage, visit_order ) != EXIT_SUCCESS ){
    printf(" error : Cannot execute visit_atom \n");
    exit( EXIT_SUCCESS );
  }

#ifdef TEST_PRUNE_BRANCH
  printf("the linkage of start atom %d \n", linkage[ start ]);
#endif

  while( head != tail ){
    j = remove_primary_queue();
    a1st[j].link = LOOP;
    if( a1st[j].couple == COUPLE ){
      a1st[j].couple = UNCOUPLE;
    }
    for( bp=a1st[j].adj_list ; bp != NULL ;  bp = bp->next ){
      if( a1st[ bp->alias ].layer == a1st[j].layer+1 ){
	if( a1st[ bp->alias ].link == CAVE ){
	  add_primary_queue( bp->alias );
	}else if( a1st[ bp->alias ].link == KNOT ){
	  a1st[ bp->alias ].link = HITCH;
	}
      }
    }
  }

  // 結合数が5以上の原子に関しては全列挙するようにする
  for( i=1; i < iatm+new_atm ; i++ ){
    if( a1st[i].nbnd >= 5 ){
      if( a1st[i].link == BRANCH || a1st[i].link == COLUMN ){
	// a1st[i].link = TRUNK;
	for( bp = a1st[i].adj_list; bp != NULL ; bp=bp->next ){
	  if( a1st[ bp->alias ].layer == a1st[i].layer+1 ){
	    if( a1st[ bp->alias].link == BRANCH || a1st[ bp->alias ].link == COLUMN ){
	      a1st[ bp->alias ].link = TRUNK;
	    }
	  }
	}
      }
    }
  }

#ifdef TEST_PRUNE_BRANCH
  dummyfile=fopen("dummy_prune_branch.dat","w");

  for( lp=top ; lp->next != NULL ; lp=lp->next ){
    fprintf(dummyfile,"%d \n\n",lp->member_Num );
    comps=lp->member;
    for( i=0 ; i < lp->member_Num ; i++ ){
      fprintf(dummyfile," %3d link%3d layer%3d couple%3d linkage %3d connector %3d\n",
              comps[i],a1st[ comps[i] ].link,a1st[ comps[i] ].layer,
              a1st[ comps[i] ].couple, linkage[ comps[i] ], connector[ comps[i] ]);
    }
    fprintf(dummyfile,"\n\n");
  }

  fclose(dummyfile);
#endif

  free(connector);
  free(linkage);
  free(visit_order);

  return EXIT_SUCCESS;

}

int visit_atom( int visitor, int caller, int *connector, int *linkage, int *visit_order ){

  static int vo = 0; // 探索した順序 visiting order

  struct bond *bp;
  struct bond *bp2;

  connector[ visitor ] = vo;
  visit_order[ visitor ] = vo;
  vo++;

  for( bp = a1st[ visitor ].adj_list; bp != NULL ; bp = bp->next ){
    if( bp->alias != caller ){ // 呼び出した原子ではない
      if( a1st[ bp->alias ].state == UNVISITED ){ // 未探索の場合
	if( a1st[ bp->alias ].layer + 1 == a1st[ visitor ].layer ){ // 上向きの結合に関して
	  a1st[ bp->alias ].state = VISITED;
	  if( a1st[ visitor ].link == KNOT ){
	    a1st[ bp->alias ].couple = COUPLE;
	  }
	  if( a1st[ bp->alias ].link != KNOT ){
	    a1st[ bp->alias ].link = CAVE;
	  }

#ifdef TEST_PRUNE_BRANCH
	  printf("call up: visitor %d link %d bp->alias %d\n", visitor, a1st[ bp->alias ].link, bp->alias);
#endif

          // visit_atom の再帰的呼び出し
	  if( visit_atom( bp->alias , visitor, connector, linkage, visit_order ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute visit_atom \n");
	    exit( EXIT_FAILURE );
	  }

	  // 探索終了後
	  if( connector[ bp->alias ] < connector[ visitor ] ){
	    connector[ visitor ] = connector[ bp->alias ];
	    linkage[ visitor ] += linkage[ bp->alias ];
	  }

	}else if( a1st[ bp->alias ].layer == a1st[ visitor ].layer + 1 ){ // 下向きの結合に関して
	  a1st[ bp->alias ].state = VISITED;
	  if( a1st[ bp->alias ].link == KNOT ){
	    if( a1st[ visitor ].link != KNOT ){
	      a1st[ visitor ].link = CAVE;
	    }
	    a1st[ visitor ].couple = COUPLE;
	  }

#ifdef TEST_PRUNE_BRANCH
	  printf("call down: visitor %d link %d bp->alias %d\n", visitor, a1st[ bp->alias ].link, bp->alias);
#endif
          // visit_atom の再帰的呼び出し
	  if( visit_atom( bp->alias , visitor, connector, linkage, visit_order ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute visit_atom \n");
	    exit( EXIT_FAILURE );
	  }

	  // 探索終了後
	  if( connector[ bp->alias ] < visit_order[ visitor ] ){
	    linkage[ visitor ] += linkage[ bp->alias ];
	    if( connector[ bp->alias ] < connector[ visitor ] ){
	      connector[ visitor ] = connector[ bp->alias ];
	    }
	    if( a1st[ visitor ].link != KNOT ){
	      a1st[ visitor ].link = CAVE;
	    }
	  }else{
	    if( linkage[ bp->alias ] > 1 ){
	      linkage[ visitor ] += linkage[ bp->alias ];
	      if( a1st[ visitor ].link != KNOT && a1st[ visitor ].link != CAVE ){
		a1st[ visitor ].link = TRUNK;
#ifdef TEST_PRUNE_BRANCH
		printf("detect TRUNK atom %d linkage %d \n", visitor,linkage[ visitor ]);
#endif
	      }
	    }else{
	      if( connector[ bp->alias ] == visit_order[ visitor ] ){
		for( bp2 = a1st[ visitor ].adj_list; bp2 != NULL ; bp2 = bp2->next ){
		  if( a1st[ bp2->alias ].state == VISITED
		      && visit_order[ bp->alias ] <= visit_order[ bp2->alias ] 
		      && a1st[ bp2->alias ].link == CAVE ){
		    add_primary_queue( bp2->alias );
		  }
		}
	      }
	      if( a1st[ visitor ].link != KNOT && a1st[ visitor ].link != CAVE && a1st[ visitor ].link != TRUNK ){
		a1st[ visitor ].link = COLUMN;
#ifdef TEST_PRUNE_BRANCH
		printf("detect COLUMN atom %d linkage %d \n", visitor,linkage[ visitor ]);
#endif
	      }
	    }
	  }

	}else{
	  continue; // 指定されたレベル外なので探索しない
	}

      }else{ // a1st[ bp->alias ].state == VISITED

	if( a1st[ bp->alias ].layer + 1 == a1st[ visitor ].layer ){ // 上向きの結合に関して

	  linkage[ visitor ]++;
	  if( a1st[ visitor ].link == KNOT ){
	    a1st[ bp->alias ].couple = COUPLE;
	    if( a1st[ bp->alias ].link != KNOT ){
	      a1st[ bp->alias ].link = CAVE;
	    }
	  }

	}

	if( a1st[ bp->alias ].layer == a1st[ visitor ].layer + 1 ){ // 下向きの結合に関して

	  if( connector[ bp->alias ] != visit_order[ visitor ] ){
	    linkage[ visitor ]++;
	  }
	  if( a1st[ bp->alias ].link == KNOT ){
	    a1st[ visitor ].couple = COUPLE;
	    if( a1st[ visitor ].link != KNOT ){
	      a1st[ visitor ].link = CAVE;
	    }
	  }
	  
	}

	if( visit_order[ bp->alias ] < connector[ visitor ] ){
	  connector[ visitor ] = visit_order[ bp->alias ];
	}

      } // a1st[ bp->alias ].state == VISITED
    } // bp->alias != caller

  }

  return EXIT_SUCCESS;

}
