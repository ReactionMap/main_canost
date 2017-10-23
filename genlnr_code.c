// genlnr_code.c CANOST線形表記を生成する関数

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_GENLNR_CODE 1

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;
extern int new_atm;
extern int start;
extern struct level *top,*down;
extern struct canost tip;
extern FILE *fo;
extern int option;
extern int boundary;
extern struct queue *head,*tail;
extern int canost_codes;

extern int first_code_head;
extern int *first_code;
extern int length_first_code;
extern char *canonical_code;
extern char *canonical_numbering;
extern int *new_code;
extern int length_new_code;

extern int *leading_code;
extern int length_leading_code;
extern char *leading_canost;
extern char *leading_numbering;
extern int *candidate_code;
extern int length_candidate;

extern int *representative;
extern int *orbit_size;
extern int *unique_numbering;

int genlnr_code( void ){

  int i,j;

  int flag_news;
  int code_length; // CANOSTコードの長さ

  int *temp_first_code;
  char temp_numbering[5];

  int *temp_leading_code;
  char temp_lead_number[5];

  int *new_numbering;

  char amper[5];
  char sand[5];
  int count_amper;

  struct bond *bp,*bp2;

  struct level *lp;
  int  *comps;

  struct canost *s_cano,*t_cano;
  struct canost *newcano;
  struct canost *wkcano;
  struct canost *last;
  struct canost *ins_amp;
  struct canost *sweep_cano; // free用の変数

#ifdef TEST_GENLNR_CODE
  printf("genlnr_code \n");
#endif

  /**** 初期化 *****/
  new_numbering = NULL;

  for(i=1;i<iatm+new_atm;i++){
    a1st[i].state=UNVISITED;
    for( bp = a1st[i].adj_list; bp != NULL ; bp=bp->next){
      bp->ring=0;
    }
  }

  /* ---- 層別ネットワーク全体での順位をふる ------ */
  j=0;
  for( lp=top ; lp != down ; lp=lp->next ){
    comps=lp->member;
    for( i=0 ; i< lp->member_Num ; i++ ){
      a1st[ comps[i] ].grade=j;
      j++;
    }
  }

  if( bonds_sort() != EXIT_SUCCESS ){
    printf("error : Cannot execute bonds_sort \n");
    exit( EXIT_FAILURE );
  }

  t_cano=&tip;
  code_length = 1;

  /**** 線形表記の生成 *****/
  // 新しいcanost構造体を確保
  if( (newcano=(struct canost *)malloc(sizeof(struct canost)))==NULL){
    printf("error : Cannot allocate memory in genlnr_code \n");
    exit( EXIT_FAILURE );
  }
  newcano->serial=*(top->member); // 分子ツリーの根を代入
  strcpy( newcano->cano_code, a1st[ *(top->member) ].code );
  t_cano->next=newcano; // tipの次に接続
  t_cano=newcano;
  s_cano=newcano;
  a1st[ *(top->member) ].state=VISITED;
  // 新しいcanost構造体を確保
  if( (newcano=(struct canost *)malloc(sizeof(struct canost)))==NULL){
    printf("error : Cannot allocate memory in genlnr_code \n");
    exit( EXIT_FAILURE );
  }
  newcano->serial=4*MINUS; // "*" を表す -4 を代入
  strcpy( newcano->cano_code,"*");
  t_cano->next=newcano; // 次に接続
  t_cano=newcano;
  t_cano->next=NULL;

  count_amper=1;
  bp2=NULL;

  while(1){
    last=t_cano;
    flag_news=0;
    while( s_cano != t_cano ){
      if( s_cano->serial > MINUS ){
	for( bp=a1st[ s_cano->serial ].adj_list ;
	     bp != NULL ;
	     bp = bp->next ){
	  if( a1st[ bp->alias ].state == UNVISITED ){ // 未探索の場合
	    if( a1st[ bp->alias ].layer <= boundary ){
	      if( bp->alias < iatm ){
		flag_news++; // CANOSTコードが続く
                code_length++;
		a1st[ bp->alias ].state = VISITED;
                // 新たなcanost構造体を確保
		if( (newcano=(struct canost *)malloc(sizeof(struct canost)))==NULL ){
		  printf("error : Cannot allocate memory in genlnr_code \n");
		  exit( EXIT_FAILURE );
		}
		newcano->serial=bp->alias;
		strcpy( newcano->cano_code, a1st[ bp->alias ].code );
                // 優先順位を調べてこのコードの入る位置を決定
		for( wkcano=last ; wkcano->next != NULL ; wkcano=wkcano->next){
		  if( (wkcano->next)->serial < 0 ){
		    continue;
		  }else{
		    if( a1st[newcano->serial].grade < a1st[(wkcano->next)->serial].grade ){
		      newcano->next=wkcano->next;
		      wkcano->next=newcano;
		      break;
		    }
		  }
		}
		if(wkcano->next == NULL){ // 順位が一番低いので最後尾に加える
		  newcano->next=NULL;
		  wkcano->next=newcano;
		}
	      }else{ // 仮想的な原子の場合
		bp->ring=count_amper;
		count_amper++;
		// ひとつのレベル内で閉じている環
		for( bp2=a1st[bp->alias].adj_list ; bp2 != NULL ; bp2 = bp2->next ){
		  bp2->ring=bp->ring;
		}
		flag_news++; // CANOSTコードが続く
                code_length++;
		a1st[ bp->alias ].state = VISITED;
                // 新たなcanost構造体を確保
		if( (newcano=(struct canost *)malloc(sizeof(struct canost)))==NULL ){
		  printf("error : Cannot allocate memory in genlnr_code \n");
		  exit( EXIT_FAILURE );
		}
		newcano->serial=MINUS;
		strcpy(amper,"&");
		sprintf(sand,"%d",bp->ring);
		strcat(amper,sand);
		strcpy(newcano->cano_code,amper);
                // &numberのnumberによる挿入位置の決定
		for( ins_amp=last ;
		     ins_amp->next != NULL && ins_amp->next->serial == MINUS ;
		     ins_amp=ins_amp->next){
		  if( atoi( (ins_amp->next->cano_code)+1 ) > bp->ring ){
		    break;
		  }
		}
		newcano->next=ins_amp->next;
		ins_amp->next=newcano;
	      }
	    }
	  }else{ // 探策済みの場合
	    if( a1st[ s_cano->serial ].layer+1 == a1st[ bp->alias ].layer ){ // ひとつ下の層の原子の場合
	      if( bp->alias >= iatm ){
		bp->ring = (a1st[bp->alias].adj_list)->ring;
	      }else{
		bp->ring=count_amper; // 第count_amper番目の環
		count_amper++;
                // ひとつ下の層の原子に環の番号を伝える
		for( bp2=a1st[ bp->alias ].adj_list ; bp2 != NULL ; bp2 = bp2->next ){
		  if( bp2->alias == s_cano->serial ){
		    bp2->ring=bp->ring;
		    break;
		  }
		}
	      }
	      flag_news++; // CANOSTコードが続く
	      code_length++;
	      // 新たなcanost構造体の確保
	      if( (newcano=(struct canost *)malloc(sizeof(struct canost)))==NULL ){
		printf("error : Cannot allocate memory in genlnr_code \n");
		exit( EXIT_FAILURE );
	      }
	      newcano->serial=MINUS;
	      strcpy(amper,"&");
	      sprintf(sand,"%d",bp->ring);
	      strcat(amper,sand);
	      strcpy(newcano->cano_code,amper);
              // &numberのnumberによる挿入位置の決定
	      for( ins_amp=last ;
		   ins_amp->next != NULL && ins_amp->next->serial == MINUS ;
		   ins_amp=ins_amp->next){
		if( atoi( (ins_amp->next->cano_code)+1 ) > bp->ring ){
		  break;
		}
	      }
	      newcano->next=ins_amp->next;
	      ins_amp->next=newcano;
	    }else if( a1st[ s_cano->serial ].layer == a1st[ bp->alias ].layer+1 ){ // ひとつ上の層の原子の場合
	      if( a1st[ s_cano->serial ].layer < boundary ){
		if( bp->ring != 0 ){
		  flag_news++; // CANOSTコードが続く
		  code_length++;
	          // 新たなcanost構造体の確保
		  if( (newcano=(struct canost *)malloc(sizeof(struct canost)))==NULL ){
		    printf("error : Cannot allocate memory in genlnr_code \n");
		    exit( EXIT_FAILURE );
		  }
		  newcano->serial=MINUS;
		  strcpy(amper,"&");
		  sprintf(sand,"%d",bp->ring);
		  strcat(amper,sand);
		  strcpy(newcano->cano_code,amper);
		  for( ins_amp=last ;
		       ins_amp->next != NULL && ins_amp->next->serial == MINUS ;
		       ins_amp=ins_amp->next){
		    if( atoi( (ins_amp->next->cano_code)+1 ) > bp->ring ){
		      break;
		    }
		  }
		  newcano->next=ins_amp->next;
		  ins_amp->next=newcano;
		}
	      }
	    }else{
	      continue;
	    }
	  } // 未探索 or 探索済み
	} // 全ての結合に関して探索終了 
      }
      while( last->next != NULL ){
	last=last->next;
      }
      if( s_cano->serial >= MINUS ){
        code_length++;
	if( (newcano=(struct canost *)malloc(sizeof(struct canost)))==NULL ){
	  printf("error : Cannot allocate memory in genlnr_code \n");
	  exit( EXIT_FAILURE );
	}
	newcano->serial=3*MINUS;
	strcpy(newcano->cano_code,",");
	last->next=newcano;
	last=newcano;
	last->next=NULL;
      }
      s_cano=s_cano->next;
    }
    if( flag_news == 0 ){
      t_cano->next->next=NULL;
      break;
    }
    t_cano=last;
    code_length++;
    if( (newcano=(struct canost *)malloc(sizeof(struct canost)))==NULL ){
      printf("error : Cannot allocate memory in genlnr_code \n");
      exit( EXIT_FAILURE );
    }
    newcano->serial=4*MINUS;
    strcpy(newcano->cano_code,"*");
    t_cano->next=newcano;
    t_cano=newcano;
    t_cano->next=NULL;
    s_cano=s_cano->next;
  }

  if( option == CANONICAL ){

#ifdef TEST_GENLNR_CODE
    printf("code_length = %d \n",code_length);
#endif

    if( code_length >= 1 ){
      if( (new_code=(int *)realloc(new_code,2*code_length*sizeof(int)))==NULL ){
	printf(" error : Cannot allocate memory in genlnr_code \n");
	exit( EXIT_FAILURE );
      }
    }
  
    for(i=0; i < 2*code_length ; i++){
      new_code[i]=0;
    }

    length_new_code = 0;
    for(wkcano=tip.next ; wkcano->next != NULL ; wkcano=wkcano->next){
      if( wkcano->serial == -3 && wkcano->next->serial == -4 ){
	continue;
      }
      if( wkcano->serial < 0 ){
        if( wkcano->serial == MINUS ){
          new_code[length_new_code] = (atoi( &(wkcano->cano_code[1]) ) + 10)*MINUS;
        }else{
          new_code[length_new_code] = wkcano->serial;
        }
        length_new_code++;
      }else{
	new_code[length_new_code] = a1st[wkcano->serial].codeNum;
	length_new_code++;
      }
      if( wkcano->serial >= MINUS && wkcano->next->serial >= MINUS ){
	new_code[length_new_code] = 2*MINUS;
	length_new_code++;
      }
    }

#ifdef TEST_GENLNR_CODE
    for(i=0;i < length_new_code; i++){
      printf(" %d ",new_code[i]);
    }
    printf("\n length_new_code = %d \n",length_new_code);
#endif

    temp_first_code = NULL;
    i = comp_int_array(first_code,length_first_code,new_code,length_new_code);
    if( i == -1 ){
#ifdef TEST_GENLNR_CODE
      printf("first_code is first_code \n");
#endif
    }else if( i == 1 ){
#ifdef TEST_GENLNR_CODE
      printf("first_code is new_code starting %d \n",start);
#endif
      first_code_head = start;
      temp_first_code = first_code;
      first_code = new_code;
      new_code = temp_first_code;
      length_first_code = length_new_code;

      if( (canonical_code=(char *)realloc(canonical_code,5*length_first_code*sizeof(char)))==NULL ){
	printf(" error : Cannot allocate memory in genlnr_code \n");
	exit( EXIT_FAILURE );
      }
      canonical_code[0]='\0';

      if( (canonical_numbering=(char *)realloc(canonical_numbering,5*length_first_code*sizeof(char)))==NULL ){
	printf(" error : Cannot allocate memory in genlnr_code \n");
	exit( EXIT_FAILURE );
      }
      canonical_numbering[0]='\0';

      for(wkcano=tip.next ; wkcano->next != NULL ; wkcano=wkcano->next){
	if( wkcano->serial == -3 && wkcano->next->serial == -4 ){
	  continue;
	}
        strcat(canonical_code,wkcano->cano_code);
	if( wkcano->serial < 0 ){
	  strcat(canonical_numbering,wkcano->cano_code);
	}else{
	  sprintf(temp_numbering,"%d",wkcano->serial);
	  strcat(canonical_numbering,temp_numbering);
	}
	if( wkcano->serial >= MINUS && wkcano->next->serial >= MINUS ){
	  strcat(canonical_code,";");
	  strcat(canonical_numbering,";");
	}
      }

    }else if( i == 0 ){
#ifdef TEST_GENLNR_CODE
      printf("first_code is first_code and new_code \n");
#endif
    }else{
      printf("error : Cannot execute comp_int_array \n");
      exit( EXIT_FAILURE );
    }

  }else{

    if( option != UNIQUE ){

    /**** Generate Linear CANOST code  *****/
      fprintf(fo," ");
      for(wkcano=tip.next ; wkcano->next != NULL ; wkcano=wkcano->next){
	if( wkcano->serial == -3 && wkcano->next->serial == -4 ){
	  continue;
	}
	if( option == CODE || option == FILIATION || option == SYMMETRIC || option == CONFORM ){
	  fprintf(fo,"%s",wkcano->cano_code);
	}else if( option == SERIAL ){
	  if( wkcano->serial < 0 ){
	    fprintf(fo,"%s",wkcano->cano_code);
	  }else{
	    fprintf(fo,"%d",wkcano->serial);
	  }
	}else{
	  printf(" Incorrect option \n");
	  exit( EXIT_FAILURE );
	}
	if( wkcano->serial >= MINUS && wkcano->next->serial >= MINUS ){
	  fprintf(fo,";");
	}
      }
      fprintf(fo,"\n");

    }

    if( option == SYMMETRIC || option == UNIQUE ){

#ifdef TEST_GENLNR_CODE
      printf("code_length = %d \n",code_length);
#endif
      if( code_length >= 1 ){
	if( (candidate_code=(int *)realloc(candidate_code,2*code_length*sizeof(int)))==NULL ){
	  printf(" error : Cannot allocate memory in genlnr_code \n");
	  exit( EXIT_FAILURE );
	}

	if( option == SYMMETRIC ){
	  if( (new_numbering=(int *)calloc(iatm,sizeof(int)))==NULL ){
	    printf(" error : Cannot allocate memory in genlnr_code \n");
	    exit( EXIT_FAILURE );
	  }
	}

      }

      for(i=0; i < 2*code_length ; i++){
	candidate_code[i]=0;
      }

      length_candidate = 0;
      i = 0;
      for(wkcano=tip.next ; wkcano->next != NULL ; wkcano=wkcano->next){
	if( wkcano->serial == -3 && wkcano->next->serial == -4 ){
	  continue;
	}
	if( wkcano->serial < 0 ){
	  if( wkcano->serial == MINUS ){
	    candidate_code[length_candidate] = atoi( &(wkcano->cano_code[1]) ) - (canost_codes+iatm+new_atm);
	  }else{
	    candidate_code[length_candidate] = wkcano->serial;
	  }
	  length_candidate++;
	}else{
	  candidate_code[length_candidate] = a1st[wkcano->serial].codeNum;
	  length_candidate++;
	  if( option == SYMMETRIC ){
	    new_numbering[i] = wkcano->serial;
	    i++;
	  }
	}
	if( wkcano->serial >= MINUS && wkcano->next->serial >= MINUS ){
	  candidate_code[length_candidate] = 2*MINUS;
	  length_candidate++;
	}
      }

#ifdef TEST_GENLNR_CODE
      for(i=0;i < length_candidate; i++){
	printf(" %d ",candidate_code[i]);
      }
      printf("\n length_candidate = %d \n",length_candidate);
#endif

      temp_leading_code = NULL;
      i = comp_int_array(leading_code,length_leading_code,candidate_code,length_candidate);
      if( i == -1 ){
#ifdef TEST_GENLNR_CODE
	printf("leading_code is leading_code \n");
#endif
      }else if( i == 1 ){
#ifdef TEST_GENLNR_CODE
	printf("leading_code is candidate_code \n");
#endif
	temp_leading_code = leading_code;
	leading_code = candidate_code;
	candidate_code = temp_leading_code;
	length_leading_code = length_candidate;

	if( (leading_canost=(char *)realloc(leading_canost,5*length_leading_code*sizeof(char)))==NULL ){
	  printf(" error : Cannot allocate memory in genlnr_code \n");
	  exit( EXIT_FAILURE );
	}
	leading_canost[0]='\0';

	if( (leading_numbering=(char *)realloc(leading_numbering,5*length_leading_code*sizeof(char)))==NULL ){
	  printf(" error : Cannot allocate memory in genlnr_code \n");
	  exit( EXIT_FAILURE );
	}
	leading_numbering[0]='\0';

	j = 0;
	for(wkcano=tip.next ; wkcano->next != NULL ; wkcano=wkcano->next){
	  if( wkcano->serial == -3 && wkcano->next->serial == -4 ){
	    continue;
	  }
	  strcat(leading_canost,wkcano->cano_code);
	  if( wkcano->serial < 0 ){
	    strcat(leading_numbering,wkcano->cano_code);
	  }else{
	    sprintf(temp_lead_number,"%d",wkcano->serial);
	    strcat(leading_numbering,temp_lead_number);
	    if( option == SYMMETRIC ){
	      unique_numbering[j] = wkcano->serial;
	      j++;
	    }
	  }
	  if( wkcano->serial >= MINUS && wkcano->next->serial >= MINUS ){
	    strcat(leading_canost,";");
	    strcat(leading_numbering,";");
	  }
	}

	if( option == SYMMETRIC ){
	  if( initgroup(iatm,representative,orbit_size) != EXIT_SUCCESS ){
	    printf("error : Cannot execute initgroup in genlnr_code \n");
	    exit( EXIT_FAILURE );
	  }
	}

      }else if( i == 0 ){
#ifdef TEST_GENLNR_CODE
	printf("leading_code is leading_code and candidate_code \n");
#endif

	if( option == SYMMETRIC ){
	  for( j = 0; j < iatm ; j++){
#ifdef TEST_GENLNR_CODE
	    printf("%d %d %d \n",j,unique_numbering[j],new_numbering[j]);
#endif
	    joingroup(unique_numbering[j],new_numbering[j],representative,orbit_size);
	  }
	}

      }else{
	printf("error: Cannot execute comp_int_array in genlnr_code \n");
	exit( EXIT_FAILURE );
      }

    } // end of if( option == SYMMETRIC || option == UNIQUE )
  
  }

  if( head != tail ){
    if( option == CONFORM ){
      fprintf(fo,"Orders of following atoms depend on their conformation \n");
    }
  }
  while( head != tail ){
    i = remove_primary_queue();
    if( option == CONFORM ){
      if( 1 <= i && i <= iatm ){
	fprintf(fo,"%d;",i);
      }else if( i == MINUS ){
	fprintf(fo,"\n");
      }else{
	continue;
      }
    }
  }

  if( option == FILIATION ){
    if( filiation() != EXIT_SUCCESS ){
      printf("error : Cannot execute filiation \n");
      exit( EXIT_FAILURE );
    }
  }

  sweep_cano=NULL;
  for(wkcano=tip.next ; wkcano != NULL ; wkcano = wkcano->next){
    free(sweep_cano);
    sweep_cano = wkcano;
  }
  free( sweep_cano );

  free( new_numbering );

  return EXIT_SUCCESS;

}
