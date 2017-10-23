// permutable.c 置換可能な原子群を見付ける関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_PERM 1

#ifdef TEST_PERM
extern FILE *dummyfile;
#endif

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern struct level *down;
extern struct level *reef;
extern int ore;
extern struct queue *head,*tail;
extern struct queue *front,*back;

int permutable( void ){

  int i,j;
  int x,y;

  int flag_1,flag_2;
  int try_or_skip;

  int *caddy; // 同順位かつCOUPLEな原子集合が入る配列
  int scale; //  同順位かつCOUPLEな原子集合が入る配列の大きさ
  int *pile;
  int peak; //  同順位かつCOUPLEな原子集合の大きさ

  int bond_grade;
  struct bond *bp;

  int current_state;
  int status;

  int captain;

  struct amigo boss;
  struct amigo *amp,*new_amp;
  struct fellow *flp,*flp2;

  int amigo_counter;
  int counter; // 連結成分の大きさ
  int *amigo_order; // 連結成分の代表原子
  int *amigo_sizes;
  int *amigo_preorder;

  struct level *lp;
  int pre_ore;

  lp=reef;
  pre_ore=ore;

  amp=NULL;
  new_amp=NULL;
  flp=NULL;
  flp2=NULL;
 
  try_or_skip=0;

  i=0;
  scale=0;
  flag_1=0;
  flag_2=0;
  peak=1;
  pile=NULL;
  while( reef != down ){ //指定されたレベルの層になるまで
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
      printf("error : Cannot allocate memory in permutable \n");
      exit( EXIT_FAILURE );
    }

    scale=peak; // 同順位かつCOUPLEな原子の集合の大きさ

    while( peak > 0 ){
      caddy[--peak]=pile[i];
      i--;
    }

    amigo_counter=1;

    boss.fellows_Num = scale;
    boss.first = NULL;
    boss.next = NULL;

    if( ( boss.first = (struct fellow *)malloc(sizeof(struct fellow)) ) == NULL ){
      printf("error : Cannot allocate memory in permutable \n");
      exit( EXIT_FAILURE );
    }
    flp = boss.first;
    flp->serial = caddy[0];
    flp->leader = flp;
    flp->next = NULL;

    for(i=1;i<scale;i++){
      if( ( flp->next = (struct fellow *)malloc(sizeof(struct fellow)) ) == NULL ){
	printf("error : Cannot allocate memory in permutable \n");
	exit( EXIT_FAILURE );
      }
      flp->next->leader = flp->leader;
      flp = flp->next;
      flp->serial = caddy[i];
      flp->next = NULL;
    }

#ifdef TEST_PERM
    dummyfile = fopen("dummy_permutable.dat","w");

    fprintf(dummyfile,"initial condition \n");
    for( amp = &boss ; amp != NULL ; amp = amp->next ){
      fprintf(dummyfile,"num. of fellows %d \n",amp->fellows_Num);
      for( flp=amp->first ; flp != NULL ; flp=flp->next ){
	fprintf(dummyfile,"serial no. %d its leader %d \n",flp->serial,flp->leader->serial);
      }
    }
    fprintf(dummyfile,"\n\n");

    for(i=0;i<TUPLE_SIZE;i++){
      printf("%d ",a1st[ caddy[0] ].tuple_child[i]);
    }
    printf("\n");
#endif

    bond_grade = 0;
    current_state = 0;
    status = 0;
    captain = 0;
    initialize_second_queue();
    for(i = 0; i < TUPLE_SIZE; i++){
      // 探索する結合を決定する
      if( a1st[ caddy[0] ].tuple_child[i] == a1st[ caddy[0] ].tuple_child[i+1] ){
	continue;
      }else{
        if( a1st[ caddy[0] ].tuple_child[i] >= reef->next->member_Num ){
	  continue;
	}
	bond_grade = a1st[ caddy[0] ].tuple_child[i];
	for( bp = a1st[ caddy[0] ].adj_list; bp != NULL ; bp = bp->next ){
	  if( a1st[ caddy[0] ].layer + 1 == a1st[ bp->alias ].layer && 
              a1st[ bp->alias ].grade == bond_grade ){
	    break;
	  }
	}
        // 上で決めた結合がBRANCHである原子に接続していたら探索しない
        if( a1st[ bp->alias ].link == BRANCH ){
	  continue;
	}
      }

      // 上で決めた結合のみからなる成分を求める
      for( amp = &boss ; amp != NULL ; amp = amp->next ){
	if( amp->fellows_Num <= 1 ){
	  continue;
	}else{
	  initialize_primary_queue();
	  captain = amp->first->serial;
	  status = a1st[ captain ].state;
	  a1st[ captain ].state = current_state + 1;
	  add_primary_queue( captain );
	  j = 0;
	  counter = 1;
	  while( head != tail ){
	    j = remove_primary_queue();
	    for( bp = a1st[j].adj_list ; bp != NULL ; bp = bp->next ){
	      if( a1st[ bp->alias ].state == status  &&
		  a1st[ bp->alias ].layer == a1st[ captain ].layer &&
		  a1st[ bp->alias ].grade == a1st[ captain ].grade ){
		// 同じ層の原子ならば同じ順位のみ
		add_primary_queue( bp->alias );
		a1st[ bp->alias ].state = current_state + 1;
		counter++;
	      }else if( a1st[ bp->alias ].state <= current_state &&
			a1st[ bp->alias ].layer == a1st[ captain ].layer + 1 &&
			a1st[ bp->alias ].grade == bond_grade ){
		// 子からの探索は上で決めた結合のみ
		add_primary_queue( bp->alias );
		a1st[ bp->alias ].state = current_state + 1;
	      }else{
		continue;
	      }
	    }
	  }
	  if( counter < amp->fellows_Num ){ // 成分として分割する
	    amigo_counter++; // 成分が新たに増加
	    if( (new_amp=(struct amigo *)malloc(sizeof(struct amigo)))==NULL ){
	      printf("error : Cannot allocate memory in permutable \n");
	      exit( EXIT_FAILURE );
	    }
	    new_amp->next = amp->next;
	    amp->next = new_amp;
	    new_amp->fellows_Num = amp->fellows_Num - counter;
	    amp->fellows_Num = counter;
	    new_amp->first = NULL;
	    flp = amp->first;
	    while( flp->next != NULL ){
	      if( a1st[ flp->next->serial ].state != current_state+1 ){
		if( new_amp->first == NULL ){
		  new_amp->first = flp->next;
		  flp2 = new_amp->first;
		}else{
		  flp2->next = flp->next;
		  flp2 = flp->next;
		}
		flp->next = flp->next->next;
		flp2->next = NULL;
		flp2->leader = new_amp->first;
	      }else{
		flp = flp->next;
	      }
	    }
	  }
	  current_state++;
	}
      } // 全ての成分について探索終了
    } // 全ての子について探索終了

#ifdef TEST_PERM
    fprintf(dummyfile,"after dividing \n");
    for( amp=&boss ; amp != NULL ; amp = amp->next ){
      fprintf(dummyfile,"num. of fellows %d \n",amp->fellows_Num);
      for( flp=amp->first ; flp != NULL ; flp=flp->next ){
	fprintf(dummyfile,"serial no. %d its leader %d \n",flp->serial,flp->leader->serial);
      }
    }
    fprintf(dummyfile,"\n");
#endif

    if( (amigo_sizes=(int *)malloc(amigo_counter*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in permutable \n");
      exit( EXIT_FAILURE );
    }

    if( (amigo_order=(int *)malloc(amigo_counter*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in permutable \n");
      exit( EXIT_FAILURE );
    }

    if( (amigo_preorder=(int *)malloc(amigo_counter*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in permutable \n");
      exit( EXIT_FAILURE );
    }

    // 配列 amigo_sizes に各成分の大きさを入れる
    i=0;
    for( amp=&boss ; amp != NULL ; amp=amp->next ){
      amigo_sizes[i]=amp->fellows_Num;
      amigo_preorder[i]=i;
      i++;
      /*
      if( amp->fellows_Num == 1 ){
	a1st[ amp->first->serial ].link = STEM;
      }
      */
    }

    // 成分の大きさで整列
    for( i=1;i<amigo_counter;i++){
      x=amigo_sizes[i];
      y=amigo_preorder[i];
      for( j=i-1 ; j>=0 && amigo_sizes[j] > x ; j-- ){
	amigo_sizes[ j+1 ] = amigo_sizes[j];
	amigo_preorder[ j+1 ] = amigo_preorder[j];
      }
      amigo_sizes[ j+1 ] = x;
      amigo_preorder[ j+1 ] = y;
    }

    // 各成分に順位を付ける
    j=0;
    amigo_order[0]=0;
    for(i=1;i<amigo_counter;i++){
      if( amigo_sizes[i-1] < amigo_sizes[i] ){
	j++;
	amigo_order[i]=j;
      }else{
	amigo_order[i]=j;
      }
    }

    if( j != 0 ){ // 順位が新たについた
      reef=lp;
      ore=pre_ore;
      try_or_skip = 1;
    }

    counter=amigo_sizes[0];
    for(i=1;i<amigo_counter;i++){
      if( amigo_sizes[i] == 1 ){
	counter++;
	continue;
      } 
      if( amigo_sizes[i-1] == amigo_sizes[i] ){
	amigo_order[i]=amigo_order[i-1];
      }else{
	amigo_order[i] += counter-1;
      }
      counter += amigo_sizes[i];
    }

    i=0;
    for( amp=&boss ; amp != NULL ; amp=amp->next ){
      j = amigo_order[ amigo_preorder[i] ];
      for( flp=amp->first; flp != NULL ; flp=flp->next ){
        a1st[ flp->serial ].grade += j;
      }
      i++;
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
    free(amigo_sizes);
    free(amigo_order);
    free(amigo_preorder);

    if( try_or_skip ){
      return TRY;
    }else{
      return SKIP;
    }

  }

}
