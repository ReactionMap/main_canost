// sym_substructure.c ひとつの原子の順位を高くする

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

extern int iatm;
extern int new_atm;
extern struct atom *a1st;
extern struct queue *head,*tail;
extern struct level *pes;
extern struct level *reef;
extern int ore;
extern int *confined_bond;
extern int *representative;
extern int *orbit_size;

int sym_substructure(void){

  int i,j,k;
  
  int flag_1,flag_2;

  int *inter_root;
  int scale;
  int *pile;
  int peak;

  struct bond *bp,*bp2;
  int counter;
  int wall; // 結合レベルを表す変数

  i=0;
  scale=0;
  flag_1=0;
  flag_2=0;
  peak=1;
  pile=NULL;
  while( reef != pes ){ //最下位のレベルの層になるまで
    flag_1=0;
    flag_2=0;
    pile=reef->member; // 層の原子
    while( ore+1 < reef->member_Num ){ // 層の範囲
      // 同順位の原子の集合か否かの判定
      if( !(flag_1) && confined_bond[ pile[ore] ] == 1 ){
	ore++;
	continue;
      }
      if( a1st[ pile[ore] ].grade == a1st[ pile[ore+1] ].grade && 
          a1st[ pile[ore] ].state == UNVISITED &&
          a1st[ pile[ore+1] ].state == UNVISITED ){
        peak++;  // 同順位な原子の個数
        flag_1=1; // 同順位な原子が見付かった
        ore++;
      }else{
        ore++;
        if( flag_1 ){
          i=ore-1;
          flag_2=1; // 同順位な原子の集合の最後
        }
      }
      if( flag_1 && flag_2 ){  // 最も順位の高い同順位の集合が見付けられた
        break;
      }
    }
    if( flag_1 && flag_2 ){ // 最も順位の高い同順位の集合が見付けられた
      break;
    }
    if( flag_1 && (ore+1 == reef->member_Num) ){
      i=ore;
      break;
    }
    reef=reef->next; // 次の層に移る
    ore=0; // 層の先頭から
  }

  if( peak <= 1 ){
    // 同順位の原子の集合は存在しない
    return EXIT_SUCCESS;

  }else{

    if( (inter_root=(int *)calloc(peak,sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in sym_substructure \n");
      exit( EXIT_FAILURE );
    }

    scale=peak; // 同順位の原子の集合の大きさ
    j = peak;

    k = 0;
    while( j > 0 ){
      if( a1st[ pile[i] ].link == BRANCH || a1st[ pile[i] ].link == COLUMN ||
          a1st[ pile[i] ].link == LOOP ){
	inter_root[--peak]=pile[i];
      }else{
	k = findgroup( pile[i],representative );
	if( orbit_size[k] > 1 ){
	  inter_root[--peak]=pile[i];
	}
      }
      i--;
      j--;
    }

    if( (scale - peak) > 1 ){
      bp=NULL;
      for( i = peak ; i < scale ; i++ ){
	counter=0;
	wall=0;
	initialize_primary_queue();
	add_primary_queue( inter_root[i] );
	add_primary_queue( MINUS );
	add_second_queue( inter_root[i] );
	add_second_queue( MINUS );
	a1st[ inter_root[i] ].state = VISITED;
	while( head != tail ){
	  j=remove_primary_queue();
	  if( ( j > MINUS && a1st[j].link != KNOT && a1st[j].link != HITCH ) || j == inter_root[i] ){
	    for( bp=a1st[j].adj_list; bp != NULL ; bp= bp->next ){
	      if( a1st[ bp->alias ].layer == a1st[j].layer+1 ){
		if( a1st[ bp->alias ].link == KNOT || a1st[ bp->alias ].link == HITCH ){
		  if( a1st[ bp->alias ].state <= i+iatm+REVISITED ){
		    add_primary_queue( bp->alias );
		    add_second_queue( bp->alias );
		    a1st[ bp->alias ].state = i+iatm+REVISITED+1;
		    counter++;
		  }
		}else{
		  if( a1st[ bp->alias ].state == UNVISITED ){
		    add_primary_queue( bp->alias );
		    add_second_queue( bp->alias );
		    a1st[ bp->alias ].state = VISITED;
		    counter++;
		  }else if( a1st[ bp->alias ].state != VISITED && a1st[ bp->alias ].state <= i+iatm+REVISITED ){
		    add_primary_queue( bp->alias );
		    add_second_queue( bp->alias );
		    a1st[ bp->alias ].state = i+iatm+REVISITED+1;
		    counter++;
		    for( bp2=a1st[ bp->alias ].adj_list ; bp2 != NULL ; bp2=bp2->next ){
		      if( a1st[ bp2->alias ].layer == a1st[ bp->alias ].layer+1 ){
			a1st[ bp2->alias ].state = i+iatm+REVISITED;
		      }
		    }
		    /*
        	    if( wall < SYM_SEARCH ){
		      add_primary_queue( bp->alias );
		      add_second_queue( bp->alias );
		      a1st[ bp->alias ].state = i+iatm+REVISITED+1;
		      counter++;
		      for( bp2=a1st[ bp->alias ].adj_list ; bp2 != NULL ; bp2=bp2->next ){
		        if( a1st[ bp2->alias ].layer == a1st[ bp->alias ].layer+1 ){
			  a1st[ bp2->alias ].state = i+iatm+REVISITED;
		        }
		      }
		    }
		    */
		  }else{
		    continue;
		  }
		}
	      } // end of if( a1st[ bp->alias ].layer == a1st[j].layer+1 )
	    }
	  }else if( j <= MINUS ){
	    if( counter != 0 ){
	      counter=0;
	      wall++;
	      add_primary_queue( MINUS );
	      add_second_queue( MINUS );
	    }
	  }else if( a1st[j].link == KNOT || a1st[j].link == HITCH ){
	    if( j < iatm ){
	      k = findgroup( j,representative );
	      if( orbit_size[k] > 1 ){
		for( bp=a1st[j].adj_list ; bp != NULL ; bp=bp->next ){
		  if( a1st[ bp->alias ].layer == a1st[j].layer+1 ){
		    if( a1st[ bp->alias ].state != VISITED && a1st[ bp->alias ].state <= i+iatm+REVISITED ){
		      add_primary_queue( bp->alias );
		      add_second_queue( bp->alias );
		      a1st[bp->alias ].state = i+iatm+REVISITED+1;
		      counter++;
		    }
		    for( bp2=a1st[ bp->alias ].adj_list ; bp2 != NULL ; bp2=bp2->next ){
		      if( a1st[ bp2->alias ].layer == a1st[ bp->alias ].layer+1 ){
			a1st[ bp2->alias ].state = i+iatm+REVISITED;
		      }
		    }
		  }
		}
	      } // end of if( orbit_size[k] > 1 )
	    } // end of if( j <= iatm )
	  }else{
	    continue;
	  }
	} // end of while
	add_second_queue( 2*MINUS );
      } // end of for(i+0;i<scale;i++)
      add_second_queue( 3*MINUS );

      for( i=0 ; i < iatm+new_atm ; i++){
	if( a1st[i].state >= iatm ){
	  a1st[i].state = REVISITED;
	}
      }

    }

    free(inter_root);

    if( sym_substructure() != EXIT_SUCCESS ){
      printf("error : Cannot execute sym_substructure \n");
      exit( EXIT_FAILURE );
    }

    return EXIT_SUCCESS;
  
  }

}
