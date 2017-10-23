// split_component.c 2層間の結合のみからなる連結成分に分解する関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_SPLIT_COMPONENT 1

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern struct level *down;
extern struct level *reef;
extern int ore;
extern struct queue *head,*tail;
extern struct queue *front,*back;

int split_component(void){

  int h,i,j,k;
  int x,y,z;

  int flag_1,flag_2;
  int try_or_skip;

  int *caddy; // 同順位かつCOUPLEな原子集合が入る配列
  int scale; //  同順位かつCOUPLEな原子集合が入る配列の大きさ
  int *pile;
  int peak; //  同順位かつCOUPLEな原子集合の大きさ

  int bond_grade;
  struct bond *bp;

  int child_type;

  int *connection; // 連結成分の代表原子
  int *compo_sizes; // 各連結成分の大きさ
  int *split_sizes;
  int *split_order;
  int *split_preorder;
  int *split_index;

  struct level *lp;
  int pre_ore;

#ifdef TEST_SPLIT_COMPONENT
  printf(" split_component \n");
#endif

  lp=reef;
  pre_ore=ore;
  try_or_skip=0;

  i=0;
  scale=0;
  flag_1=0;
  flag_2=0;
  peak=1;
  pile=NULL;
  while( reef != down ){ //最下位のレベルの層になるまで
    flag_1=0;
    flag_2=0;
    pile=reef->member; // 層の原子
    while( ore+1 < reef->member_Num ){ // 層の範囲
      if( !(flag_1) && a1st[ pile[ore] ].couple == UNCOUPLE ){
	ore++;
	continue;
      }
      // 同順位かつCOUPLEな原子の集合か否かの判定
      if( a1st[ pile[ore] ].grade == a1st[ pile[ore+1] ].grade &&
          a1st[ pile[ore] ].couple == COUPLE &&
          a1st[ pile[ore+1] ].couple == COUPLE ){
        peak++;  // 同順位かつCOUPLEな原子の個数
	flag_1=1; // 同順位かつCOUPLEな原子が見付かった
	ore++;
      }else{ 
	ore++;
	if( flag_1 ){
	  i=ore-1;
	  flag_2=1; // 同順位かつCOUPLEな原子の集合の最後
	}
      }
      if( flag_1 && flag_2 ){  // 最も順位の高い同順位かつCOUPLEな集合が見付けられた
	break;
      }
    }
    if( flag_1 && flag_2 ){ // 最も順位の高い同順位かつCOUPLEな集合が見付けられた
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
    // 同順位かつCOUPLEな原子の集合は存在しない
    return TRY;

  }else{

    if( (caddy=(int *)malloc(peak*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in split_component \n");
      exit( EXIT_FAILURE );
    }

    scale=peak; // 同順位かつCOUPLEな原子の集合の大きさ

    while( peak > 0 ){
      caddy[--peak]=pile[i];
      i--;
    }

    if( (connection=(int *)malloc(scale*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in split_component \n");
      exit( EXIT_FAILURE );
    }

    if( (compo_sizes=(int *)malloc(scale*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in split_component \n");
      exit( EXIT_FAILURE );
    }

    if( initgroup(scale,connection,compo_sizes) != EXIT_SUCCESS ){
      printf("error : Cannot execute initgroup in split_component \n");
      exit( EXIT_FAILURE );
    }

    if( (split_sizes=(int *)malloc(scale*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in split_component \n");
      exit( EXIT_FAILURE );
    }

    if( (split_order=(int *)malloc(scale*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in split_component \n");
      exit( EXIT_FAILURE );
    }

    if( (split_preorder=(int *)malloc(scale*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in split_component \n");
      exit( EXIT_FAILURE );
    }

    if( (split_index=(int *)malloc(scale*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in split_component \n");
      exit( EXIT_FAILURE );
    }

    for(i=0;i<scale;i++){
      split_sizes[i]=1;
      split_order[i]=0;
      split_preorder[i]=0;
      split_index[i]=i;
    }

    bond_grade=0;
    child_type=0;
    initialize_second_queue();
    for(h=0;h<TUPLE_SIZE;h++){
      // 探索する結合を決定する
      if( a1st[ caddy[0] ].tuple_child[h] == a1st[ caddy[0] ].tuple_child[h+1] ){
	continue;
      }else{
        if( a1st[ caddy[0] ].tuple_child[h] >= reef->next->member_Num ){
	  continue;
	}
	bond_grade = a1st[ caddy[0] ].tuple_child[h];
	for( bp=a1st[ caddy[0] ].adj_list; bp != NULL ; bp=bp->next ){
	  if( a1st[ caddy[0] ].layer+1 == a1st[ bp->alias ].layer && 
              a1st[ bp->alias ].grade == bond_grade ){
	    break;
	  }
	}
        // 上で決めた探索する結合がBRANCHである原子に接続する結合ならば探索しない
        if( a1st[ bp->alias ].link == BRANCH ){
	  continue;
	}
      }

      for(i=0;i<scale;i++){
	if( a1st[ caddy[i] ].state <= child_type ){
	  initialize_primary_queue(); // ひとつの連結成分の原子 
	  a1st[ caddy[i] ].state = child_type+1;
	  add_primary_queue( caddy[i] );
	  j=0;
	  while( head != tail ){
	    j=remove_primary_queue();
	    for( bp=a1st[j].adj_list; bp != NULL ; bp=bp->next ){
	      if( a1st[ bp->alias ].state <= child_type && a1st[ bp->alias ].link != BRANCH ){
                if( a1st[ bp->alias ].layer == a1st[ caddy[0] ].layer &&
		    a1st[ bp->alias ].grade == a1st[ caddy[0] ].grade ){
                  // 同じ層の原子ならば同じ順位のみ
		  add_primary_queue( bp->alias );
		  for(k=0;k<scale;k++){
		    if( caddy[k] == bp->alias ){
		      break;
		    }
		  }
		  if( joingroup(i,k,connection,compo_sizes) != EXIT_SUCCESS ){
		    printf("error : Cannot execute joingroup in split_component \n");
		    exit( EXIT_FAILURE );
		  }
		  a1st[ bp->alias ].state = child_type+1;
		}else if( a1st[ bp->alias ].layer == a1st[ caddy[0] ].layer+1 &&
			  a1st[ bp->alias ].grade == bond_grade ){
                    // 子への探索は上で決めた結合のみ
		    add_primary_queue( bp->alias );
		    add_second_queue( bp->alias );
		    a1st[ bp->alias ].state = child_type+1;
		}else{
		  continue;
		}
	      }
	    }
	  }
	}
	child_type++;
      }

      // 各原子の属す連結成分の大きさ
      for(j=0;j<scale;j++){
	k=findgroup(j,connection);
	split_sizes[j]=compo_sizes[k];
      }

      // 連結成分の大きさで整列
      for(j=1; j < scale ;j++){
	x=split_sizes[j];
	y=split_index[j];
	z=split_preorder[j];
	for( k=j-1 ; k>=0 && split_sizes[k] > x ; k-- ){
	  split_sizes[ k+1 ] = split_sizes[k];
	  split_index[ k+1 ] = split_index[k];
	  split_preorder[ k+1 ] = split_preorder[k];
	}
	split_sizes[ k+1 ] = x;
	split_index[ k+1 ] = y;
	split_preorder[ k+1 ] = z;
      }

      // それまでに決まっていた順位で整列
      for(j=1; j < scale ;j++){
	x=split_preorder[j];
	y=split_index[j];
	z=split_sizes[j];
	for( k=j-1 ; k>=0 && split_preorder[k] > x ; k-- ){
	  split_preorder[ k+1 ] = split_preorder[k];
	  split_index[ k+1 ] = split_index[k];
	  split_sizes[ k+1 ] = split_sizes[k];
	}
	split_preorder[ k+1 ] = x;
	split_index[ k+1 ] = y;
	split_sizes[ k+1 ] = z;
      }

      // 新たな順位を付ける
      k=0;
      for(j=1; j < scale ;j++){
        if( split_sizes[j-1] == split_sizes[j] && split_preorder[j-1] == split_preorder[j] ){
	  split_order[j]=k;
	}else{
	  k=j;
	  split_order[j]=k;
	}
      }

      // 新たな順位が付いた
      if( k != 0 ){
	try_or_skip = 1;
      }

      // 順位を記憶
      for(j=0;j<scale;j++){
	split_preorder[j]=split_order[j];
      }

    } // 全ての子について探索終了

    if( try_or_skip ){ // 新たな順位が付いた
      reef=lp;
      ore=pre_ore;
    }

    // 各連結成分の各原子にその連結成分の順位を付ける
    for(i=0;i<scale;i++){
      a1st[ caddy[ split_index[i] ] ].grade += split_preorder[i];
      if( split_sizes[i] == 1 ){
	a1st[ caddy[ split_index[i] ] ].couple = UNCOUPLE;
      }
    }

    // 全原子を未探索に戻す
    for(i=0;i<scale;i++){
      a1st[ caddy[i] ].state = UNVISITED;
    }

    while( front != back ){
      i=remove_second_queue();
      a1st[i].state = UNVISITED;
    }

    free(caddy);
    free(connection);
    free(compo_sizes);
    free(split_sizes);
    free(split_order);
    free(split_preorder);
    free(split_index);

    if( try_or_skip ){
      return TRY;
    }else{
      return SKIP;
    }

  }

}
