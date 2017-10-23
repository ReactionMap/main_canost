// utile_functions.c CANOSTで使う便利な関数ライブラリ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

extern struct queue *head,*tail;
extern struct queue *front,*back;

/* 小文字を大文字に変換する関数 */
char small_to_capital(char c){

  if( ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '\0'){
    if( 'a' <= c && c <= 'z' ){
      c = c - 'a' + 'A';
    }
    return c;
  }else{
    printf("error : There exists illegal atom type \n");
    exit( EXIT_FAILURE );
  }

}

/* Primary Queue */

// Queueの初期化をする関数
int initialize_primary_queue(void){

  head=tail=(struct queue *)malloc(sizeof(struct queue));
  if(head == NULL){
    printf(" error : Cannot allocate memory in initialize_primary_queue  \n");
    exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;

}

// Queueに要素をひとつ加える関数
int add_primary_queue(int x){

  tail->item=x;
  tail->next=(struct queue *)malloc(sizeof(struct queue));
  if(tail->next==NULL){
    printf(" error : Cannot allocate memory in add_primary_queue  \n");
    exit( EXIT_FAILURE );
  }
  tail=tail->next;

  return EXIT_SUCCESS;

}

// Queueから要素をひとつ取り出す関数
int remove_primary_queue(void){

  int x;
  struct queue *p;

  p=head;
  head=p->next;
  x=p->item;
  free(p);

  return x;

}

/* Secondary Queue */

// Queueの初期化をする関数
int initialize_second_queue(void){

  front=back=(struct queue *)malloc(sizeof(struct queue));
  if(front == NULL){
    printf(" error : Cannot allocate memory in initialize_second_queue  \n");
    exit( EXIT_FAILURE );
  }

  return EXIT_SUCCESS;

}

// Queueに要素をひとつ加える関数
int add_second_queue(int x){

  back->item=x;
  back->next=(struct queue *)malloc(sizeof(struct queue));
  if(back->next==NULL){
    printf(" error : Cannot allocate memory in add_second_queue  \n");
    exit( EXIT_FAILURE );
  }
  back=back->next;

  return EXIT_SUCCESS;

}

// Queueから要素をひとつ取り出す関数
int remove_second_queue(void){

  int x;
  struct queue *p;

  p=front;
  front=p->next;
  x=p->item;
  free(p);

  return x;

}

/* Union-Find に使われる関数 */

int initgroup(int n,int *parent,int *treesize){

  int i;

  for(i=0;i<n;i++){
    parent[i]=i;
    treesize[i]=1;
  }

  return EXIT_SUCCESS;

}

int findgroup(int k,int *parent){
     
  if(parent[k] != k){
    parent[k]=findgroup(parent[k],parent);
  }

  return parent[k];

}

int joingroup(int p,int q,int *parent,int *treesize){
   
  int p_parent,q_parent;

  if( p != q ){

    p_parent=findgroup(p,parent);
    q_parent=findgroup(q,parent);

    if( p_parent != q_parent ){

      if( treesize[ p_parent ] >= treesize[ q_parent ] ){
	join(p_parent,q_parent,parent,treesize);
      }else{
	join(q_parent,p_parent,parent,treesize);
      }
    }

  }

  return EXIT_SUCCESS;

}

int join(int p,int q,int *parent,int *treesize){
  
  parent[q] = p;
  treesize[p] += treesize[q];

  return EXIT_SUCCESS;

}

int comp_int_array(int *intarr1,int length1,int *intarr2,int length2){

  int i;
  int short_length;

  int one_or_two;

  one_or_two = 0;
  if( length1 < length2 ){
    one_or_two = -1;
    short_length = length1;
  }else if( length1 > length2 ){
    one_or_two = 1;
    short_length = length2;
  }else{
    short_length = length1;
  }

  for(i=0; i < short_length; i++){
    if( intarr1[i] < intarr2[i] ){
      one_or_two = -1;
      break;
    }else if( intarr1[i] > intarr2[i] ){
      one_or_two = 1;
      break;
    }else{
      continue;
    }
  }

  return one_or_two;

}
