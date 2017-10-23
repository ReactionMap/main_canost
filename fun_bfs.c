// fun_bfs.c 幅優先探索をする関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_BFS 1

#ifdef TEST_BFS
extern struct atom *va;
extern FILE *dummyfile;
#endif

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;
extern int new_atm;
extern int start;
extern struct level *top,*down;
extern struct queue *head,*tail;
extern int lowest;

int fun_bfs( void ){

  int i,j;

  struct bond *bp,*bp2; // 結合を指すポインタ

  int counter; // 同じ層に属す原子の個数を数える
  int *comps; // 原子を格納する配列を指すポインタ

  int floor; // 層のレベルを表す

#ifdef TEST_BFS
  struct level *lp;

  printf("fun_bfs \n");

  lp=NULL;
#endif

  /**** 初期化 *****/
  for(i=1;i<iatm;i++){
    a1st[i].state=UNVISITED;
    a1st[i].grade=0;
    a1st[i].layer=0;
    a1st[i].link=BRANCH;
  }

  for(i=0;i<(2*ibnd+1);i++){
    b1st[i].alias=MINUS;
  }

  bp=NULL;
  bp2=NULL;
  comps=NULL;

  // Queueの初期化
  head=NULL;
  tail=NULL;
  initialize_primary_queue();

  /***** 幅優先探策の開始 *****/  
  add_primary_queue(start); // Queueに始点の原子を入れる
  a1st[start].state=VISITED; // 始点の原子は探策済み
  a1st[start].layer=0; // 始点の原子はレベル0の層の原子
  add_primary_queue(MINUS); // 層の区切りを表すMINUSをQueueに入れる

  // comps を最上位の層にする
  comps=top->member;

  // 次の層を表す構造体を確保
  if( (top->next=(struct level *)malloc(sizeof(struct level)))==NULL){
    printf(" error : Cannot allocate memory in fun_bfs \n");
    exit( EXIT_FAILURE );
  }
  down=top->next;
  down->prev=top; // ひとつ上の層を記憶

  j=0;
  counter=0;
  floor=0;
  while( head != tail ){ // Queueが空でない
    j=remove_primary_queue(); // Queueから原子をひとつ取り出す
    if( j != MINUS ){ // それが層の区切りを表すMINUSではない
      *comps=j; // 原子jを配列に格納
      comps++;
      if( j < iatm ){
        // 原子jの隣接リストを辿る
	for( bp=a1st[j].adj_list ; bp != NULL ; bp=bp->next ){
      	  // 探策済みか未探索かを判定
	  if( a1st[ bp->second ].state == UNVISITED ){
	    a1st[ bp->second ].layer = floor+1;  //属す層の決定
	    bp->alias=bp->second; // 結合先の原子は実在の原子
	    counter++; // レベル(floor+1)の頂点の個数を数える
	    add_primary_queue( bp->second ); // 結合先の原子をQueueの入れる
	    a1st[ bp->second ].state = VISITED; // 探策済みとする
	  }else{
	    if( a1st[ bp->second ].layer < a1st[j].layer ){
              // 原子jより上のレベルの層に属す原子
	      bp->alias=bp->second;
	    }else if( a1st[ bp->second ].layer == a1st[j].layer ){
	      // 原子jと同じレベルの層に属す原子
	      if( bp->alias == MINUS ){ // まだ結合先の原子が指定されていない
		bp->alias=iatm+new_atm; // 結合先に仮想的な原子を指定
                // 元々の結合先の原子にもその情報を伝える
		bp2=a1st[ bp->second ].adj_list;
		while( bp2->second != j ){
		  bp2=bp2->next;
		}
		bp2->alias=iatm+new_atm; // 結合先に仮想的な原子を指定
		genAtom(j,bp->second,floor+1); // 仮想的な原子の生成
		add_primary_queue( iatm+new_atm ); 
		new_atm++; // 仮想的な原子の個数を数える
		counter++; // レベル(floor+1)の頂点の個数を数える
	      }
	    }else{  // 原子jより下のレベルの層の原子で既に探策されQueueに入っている
	      bp->alias=bp->second;
              if( a1st[ bp->second ].state == VISITED ){
		a1st[ bp->second ].link = KNOT; // 環構造の結び目
	      }
              a1st[ bp->second ].state = REVISITED;
	    }
	  }
	}
      } // end of if( j < iatm )
    }else{
      if( counter != 0 ){
	floor++; // レベルをひとつ下げる
	down->member_Num=counter; // レベルfloorの原子数はcounter
	if((down->member=(int *)malloc(counter*sizeof(int)))==NULL){
	  printf(" error : Cannot allocate memory in fun_bfs \n");
	  exit( EXIT_FAILURE );
	}
	comps=down->member;
	if( (down->next=(struct level *)malloc(sizeof(struct level)))==NULL){
	  printf(" error : Cannot allocate memory in fun_bfs \n");
	  exit( EXIT_FAILURE );
	}
	(down->next)->prev=down; // ひとつ上の層を記憶
	down=down->next; // ひとつ下の層に移動
	add_primary_queue( MINUS ); // 層の区切りを表すMINUSをQueueに入れる
	counter=0; // counterを再初期化
      }
    }
  }

  lowest=floor+1; // 最下位の層のレベルを記憶

#ifdef TEST_BFS
  printf("lowest = %d \n",lowest);
#endif

  down->member_Num=0;
  down->member=NULL;
  down->next=NULL;

#ifdef TEST_BFS
  dummyfile=fopen("dummy_fun_bfs.dat","w");

  for( lp=top ; lp->next != NULL ; lp=lp->next ){
    fprintf(dummyfile,"%d \n\n",lp->member_Num );
    comps=lp->member;
    for( i=0 ; i < lp->member_Num ; i++ ){
      if( comps[i] < iatm ){
	fprintf(dummyfile,"atom %4d condeNum %4d layer %4d link %4d \n",
                           comps[i],a1st[ comps[i] ].codeNum,
                           a1st[ comps[i] ].layer,a1st[ comps[i] ].link );
      }else{
	fprintf(dummyfile,"atom %4d codeNum %4d layer %4d link %4d \n",
                           comps[i],va[ comps[i]-iatm ].codeNum,
                           va[ comps[i]-iatm ].layer,va[ comps[i]-iatm ].link );
      }
    }
    fprintf(dummyfile,"\n\n");
  }

  fclose(dummyfile);
#endif

  return EXIT_SUCCESS;

}
