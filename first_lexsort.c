//first_lexsort.c radix sortを使って原子をtupleに関して辞書式順序で整列

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_FIRST_LEXSORT 1

extern int iatm;
extern int new_atm;
extern struct atom *a1st;
extern int canost_codes;

// chain : 整列する層
// length : 整列する層の大きさ
// lower : 整列する層のひとつ下の層の大きさ
int first_lexsort( int *chain, int length, int lower ){

  int i,j;

  int *work;
  int *count;
  int *count_1;
  int *count_2;

  work=NULL;
  count=NULL;
  count_1=NULL;
  count_2=NULL;

  if( (work=(int *)malloc(length*sizeof(int)))==NULL ){
    printf("error : Cannot allocate memory in lexsort \n");
    exit( EXIT_FAILURE );
  }

  // link のタイプで順位付け
  if( (count=(int *)malloc( 8*sizeof(int)))==NULL ){
    printf("error : Cannot allocate memory in lexsort \n");
    exit( EXIT_FAILURE );
  }

  for(i=0 ; i < 8 ; i++){
      count[i]=0;
  }
  for(i=0 ; i< length ; i++){
    count[ a1st[ chain[i] ].link ]++;
  }
  for(i=1 ; i < 8 ; i++){
    count[i] += count[i-1];
  }
  for(i=length-1 ; i >= 0 ; i--){
    work[ --count[ a1st[ chain[i] ].link ] ]=chain[i];
  }
  for(i=0;i<length;i++){
    chain[i] = work[i];
  }

  free(count);

#ifdef TEST_FIRST_LEXSORT
  for(i=0 ; i< length ; i++){
    printf("%4d ", chain[i]);
  }
  printf("\n");

  for(i=0 ; i< length ; i++){
    printf("%4d ", a1st[ chain[i] ].link);
  }
  printf("\n\n");
#endif

  // 子の順位で比較
  if( lower >= 1 ){

    if( (count_1=(int *)malloc((lower+1)*sizeof(int)))==NULL ){
      printf("error : Cannot allocate memory in lexsort \n");
      exit( EXIT_FAILURE );
    }
  
    for(j=TUPLE_SIZE-1;j>=0;j--){
      for(i=0;i<lower+1;i++){
        count_1[i]=0;
      }
      for(i=0;i<length;i++){
	if( a1st[ chain[i] ].tuple_child[j] > iatm+new_atm ){
	  count_1[ lower ]++;
	}else{
	  count_1[ a1st[ a1st[ chain[i] ].tuple_child[j] ].grade ]++;
	}
      }
      for(i=1;i<lower+1;i++){
        count_1[i] += count_1[i-1];
      }
      for(i=length-1;i>=0;i--){
	if( a1st[ chain[i] ].tuple_child[j] > iatm+new_atm ){
	  work[ --count_1[ lower ] ]=chain[i];
	}else{
	  work[ --count_1[ a1st[ a1st[ chain[i] ].tuple_child[j] ].grade ] ]=chain[i];
	}
      }
      for(i=0;i<length;i++){
        chain[i] = work[i];
      }
    }
  
  }

  free(count_1);

  if( (count_2=(int *)malloc((canost_codes+2)*sizeof(int)))==NULL ){
    printf("error : Cannot allocate memory in lexsort \n");
    exit( EXIT_FAILURE );
  }

  // 子のCANOSTコードで比較
  if( lower >= 1 ){

    for(j=TUPLE_SIZE-1;j>=0;j--){
      for(i=0 ; i < canost_codes+2 ; i++){
        count_2[i]=0;
      }
      for(i=0;i<length;i++){
	if( a1st[ chain[i] ].tuple_child[j] > iatm+new_atm ){
	  count_2[ canost_codes+1 ]++;
	}else{
	  count_2[ a1st[ a1st[ chain[i] ].tuple_child[j] ].codeNum ]++;
	}
      }
      for(i=1;i<canost_codes+2;i++){
        count_2[i] += count_2[i-1];
      }
      for(i=length-1;i>=0;i--){
	if( a1st[ chain[i] ].tuple_child[j] > iatm+new_atm ){
	  work[ --count_2[ canost_codes+1 ] ]=chain[i];
	}else{
	  work[ --count_2[ a1st[ a1st[ chain[i] ].tuple_child[j] ].codeNum ] ]=chain[i];
	}
      }
      for(i=0;i<length;i++){
        chain[i] = work[i];
      }
    }
  
  }

  for(i=0 ; i< canost_codes+1 ; i++){
      count_2[i]=0;
  }
  for(i=0 ; i< length ; i++){
    count_2[ a1st[ chain[i] ].codeNum ]++;
  }
  for(i=1 ; i < canost_codes+1 ; i++){
    count_2[i] += count_2[i-1];
  }
  for(i=length-1 ; i >= 0 ; i--){
    work[ --count_2[ a1st[ chain[i] ].codeNum ] ]=chain[i];
  }
  for(i=0;i<length;i++){
    chain[i] = work[i];
  }

  free(count_2);
  free(work);

#ifdef TEST_FIRST_LEXSORT
  for(i=0 ; i< length ; i++){
    printf("%4d ", chain[i]);
  }
  printf("\n");

  for(i=0 ; i< length ; i++){
    printf("%4d ", a1st[ chain[i] ].link);
  }
  printf("\n\n");
#endif

  return EXIT_SUCCESS;

}
