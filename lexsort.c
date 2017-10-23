//lexsort.c radix sortを使って原子をtupleに関して辞書式順序で整列

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_LEXSORT 1

extern struct atom *a1st;

// chain : 整列する層
// length : 整列する層の大きさ
// upper : 整列する層のひとつ上の層の大きさ
// lower : 整列する層のひとつ下の層の大きさ
int lexsort( int *chain, int length, int upper, int lower ){

  int i,j;

  static int *work;
  static int *count_1;
  static int *count_2;
  static int *count_3;
  static int *count_4;

#ifdef TEST_LEXSORT
  printf("lexsort \n");

  printf("length = %d upper = %d lower = %d \n",length,upper,lower);

  for(i=0;i<length;i++){
    printf("%d : ",chain[i]);
    for(j=0;j<TUPLE_SIZE;j++){
      printf("%d ",a1st[ chain[i] ].tuple_parent[j]);
    }
    printf("; ");
    for(j=0;j<TUPLE_SIZE;j++){
      printf("%d ",a1st[ chain[i] ].tuple_child[j]);
    }
    printf("\n");
  }

#endif

  work = NULL;
  count_1 = NULL;
  count_2 = NULL;
  count_3 = NULL;
  count_4 = NULL;

  if( ( work = (int *)calloc(length,sizeof(int)) ) == NULL ){
    printf(" error : Cannot allocate memory in lexsort \n");
    exit( EXIT_FAILURE );
  }

  if( upper >= 1 ){

    if( ( count_1= (int *)calloc(upper+1, sizeof(int)) ) == NULL ){
      printf(" error : Cannot allocate memory in lexsort \n");
      exit( EXIT_FAILURE );
    }

    for(j=TUPLE_SIZE-1;j>=1;j--){
      for(i=0;i<upper+1;i++){
	count_1[i]=0;
      }
      for(i=0;i<length;i++){
	count_1[ a1st[ chain[i] ].tuple_parent[j] ]++;
      }
      for(i=1;i<upper+1;i++){
	count_1[i] += count_1[i-1];
      }
      for(i=length-1;i>=0;i--){
	work[ --count_1[ a1st[ chain[i] ].tuple_parent[j] ] ] = chain[i];
      }
      for(i=0;i<length;i++){
	chain[i] = work[i];
      }
    }
  }

  free(count_1);

  if( lower >= 1 ){

    if( ( count_2=(int *)calloc(lower+1,sizeof(int)) )==NULL ){
      printf("error : Cannot allocate memory in lexsort \n");
      exit( EXIT_FAILURE );
    }

    for(j=TUPLE_SIZE-1;j>=0;j--){
      for(i=0;i<lower+1;i++){
	count_2[i]=0;
      }
      for(i=0;i<length;i++){
	count_2[ a1st[ chain[i] ].tuple_child[j] ]++;
      }
      for(i=1;i<lower+1;i++){
	count_2[i] += count_2[i-1];
      }
      for(i=length-1;i>=0;i--){
	work[ --count_2[ a1st[ chain[i] ].tuple_child[j] ] ]=chain[i];
      }
      for(i=0;i<length;i++){
	chain[i] = work[i];
      }
    }
  }

  free(count_2);

  if( ( count_3=(int *)calloc(length+1,sizeof(int)) )==NULL ){
    printf("error : Cannot allocate memory in lexsort \n");
    exit( EXIT_FAILURE );
  }

  for(i=0;i<length+1;i++){
      count_3[i]=0;
  }
  for(i=0;i<length;i++){
    count_3[ a1st[ chain[i] ].grade ]++;
  }
  for(i=1;i<length+1;i++){
    count_3[i] += count_3[i-1];
  }
  for(i=length-1;i>=0;i--){
    work[ --count_3[ a1st[ chain[i] ].grade ] ]=chain[i];
  }
  for(i=0;i<length;i++){
    chain[i] = work[i];
  }

  free(count_3);

  if( ( count_4=(int *)calloc(upper+1,sizeof(int)) )==NULL ){
    printf("error : Cannot allocate memory in lexsort \n");
    exit( EXIT_FAILURE );
  }

  for(i=0;i<upper+1;i++){
      count_4[i]=0;
  }
  for(i=0;i<length;i++){
    count_4[ a1st[ chain[i] ].tuple_parent[0] ]++;
  }
  for(i=1;i<upper+1;i++){
    count_4[i] += count_4[i-1];
  }
  for(i=length-1;i>=0;i--){
    work[ --count_4[ a1st[ chain[i] ].tuple_parent[0] ] ]=chain[i];
  }
  for(i=0;i<length;i++){
    chain[i] = work[i];
  }

  free(count_4);
  free(work);

#ifdef TEST_LEXSORT
  printf("lexsort finish \n");
#endif

  return EXIT_SUCCESS;

}
