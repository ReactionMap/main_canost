// set_upper_level.c レベル 1 2  の原子の順位を固定する関数 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_SET_UPPER_LEVEL 1

extern int iatm;
extern struct atom *a1st;
extern int new_atm;
extern int start;
extern struct level root;
extern struct level *top;
extern struct level *reef;
extern int *cast_codes;
extern int *relative_parent;

int set_upper_level( void ){

  int i,j;

  int *caddy; // 同順位の原子集合が入る配列
  int scale; //  同順位の原子集合が入る配列の大きさ
  int higher; // 作為的に順位を高くする原子の番号
  int *class;  //順位を記憶しておくための配列

  int *pile_one;
  int *pile_two;

  struct level *lv_one;
  struct level *lv_two;

  int *competitor;
  int compeNum;

  int pivNum;
  int *pivots;

  int *ranker;
  int rank;

#ifdef TEST_SET_UPPER_LEVEL
  printf(" set_upper_level \n");
#endif

  pile_one = NULL;
  pile_two = NULL;

  // lv_one を reef に設定
  lv_one = reef;
  pile_one = lv_one->member;

  for( i=0; i < lv_one->member_Num ; i++ ){
    if( identify_parent( pile_one[i] ) != EXIT_SUCCESS ){
      printf(" error : Cannot execute identify_parent \n");
      exit( EXIT_FAILURE );
    }
#ifdef TEST_SET_UPPER_LEVEL
    printf("pile_one %d is %d \n",i,pile_one[i]);
#endif
  }

  // lv_two を reef->next に設定
  lv_two = reef->next;
  if( lv_two != NULL ){ // reef->next に相当するレベルの層があれば以下を実行
    pile_two = lv_two->member;

    for( i=0; i < lv_two->member_Num ; i++ ){
      if( identify_parent( pile_two[i] ) != EXIT_SUCCESS ){
	printf(" error : Cannot execute identify_parent \n");
	exit( EXIT_FAILURE );
      }
#ifdef TEST_SET_UPPER_LEVEL
      printf("pile_two %d is %d \n",i,pile_two[i]);
#endif
    }
  }

  /******* lv_one が ひとつの原子からなる場合 *********/
  if( lv_one->member_Num == 1 ){

    if( single_child( pile_one[0] ) != EXIT_SUCCESS ){
      printf(" error : Cannot execute single_child \n");
      exit( EXIT_SUCCESS );
    }

  /******* lv_one が ふたつの原子からなる場合 *********/
  }else if( lv_one->member_Num == 2 ){

#ifdef TEST_SET_UPPER_LEVEL
    printf("set_upper_level: level one layer contains two atoms. \n");
#endif

    if( a1st[ pile_one[0] ].grade == a1st[ pile_one[1] ].grade ){

      if( (class=(int *)malloc((iatm+new_atm)*sizeof(int)))==NULL){
	printf(" error : Cannot allocate memory in set_upper_level \n");
	exit( EXIT_FAILURE );
      }

      for(i=1;i<iatm+new_atm;i++){
	class[i]=a1st[i].grade;
      }

      scale = 2; // pile_one は 2 個の原子からなる

      if( (caddy=(int *)malloc(scale*sizeof(int)))==NULL){
	printf(" error : Cannot allocate memory in set_upper_level \n");
	exit( EXIT_FAILURE );
      }

      for(i=0; i < scale ;i++){
	caddy[i] = pile_one[i];
      }

      for(i=0;i<scale;i++){
	higher = caddy[i];

	if( raise_and_order( higher, caddy, scale ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute raise_and_order \n");
	  exit( EXIT_FAILURE );
	}

        // pile_one[0] -- start -- pile_one[1] が成す角度を調べる
	relative_parent[ start ] = pile_one[0];
	relative_parent[ pile_one[0] ] = MINUS;

	if( linearity( pile_one[0], start , pile_one[1] ) == TRUE ){  // 一直線の場合

	  relative_parent[ start ] = MINUS;
	  relative_parent[ pile_one[0] ] = start;
	  relative_parent[ pile_one[1] ] = start;

#ifdef TEST_SET_UPPER_LEVEL
	  printf("set_upper_level calls linear_pair. \n");
#endif

	  if( linear_pair( pile_one[0] ,pile_one[1] ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute linear_pair \n");
	    exit( EXIT_FAILURE );
	  }

	}else{

	  relative_parent[ start ] = MINUS;
	  relative_parent[ pile_one[0] ] = start;
	  relative_parent[ pile_one[1] ] = start;

#ifdef TEST_SET_UPPER_LEVEL
	  printf("set_upper_level calls broken_line_pair. \n");
#endif

	  if( broken_line_pair( pile_one[0], pile_one[1] ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute broken_line_pair \n");
	    exit( EXIT_FAILURE );
	  }

	}

	for( j=1 ; j < iatm+new_atm ; j++){
	  a1st[j].grade = class[j];
	  cast_codes[j] = MINUS;
	}

      }

      free(class);
      free(caddy);

    }else{ // pile_one[0] != pile_one[1]

      relative_parent[ start ] = pile_one[0];
      relative_parent[ pile_one[0] ] = MINUS;

      if( linearity( pile_one[0], start , pile_one[1] ) == TRUE ){ // 一直線の場合

#ifdef TEST_SET_UPPER_LEVEL
	printf("set_upper_level: the two atoms form a line \n");
#endif

	relative_parent[ start ] = MINUS;
	relative_parent[ pile_one[0] ] = start;
	relative_parent[ pile_one[1] ] = start;

#ifdef TEST_SET_UPPER_LEVEL
	printf("set_upper_level calls linear_pair \n");
#endif

	if( linear_pair( pile_one[0] ,pile_one[1] ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute linear_pair \n");
	  exit( EXIT_FAILURE );
	}

      }else{

	relative_parent[ start ] = MINUS;
	relative_parent[ pile_one[0] ] = start;
	relative_parent[ pile_one[1] ] = start;

#ifdef TEST_SET_UPPER_LEVEL
	printf("set_upper_level calls broken_line_pair \n");
#endif

	if( broken_line_pair( pile_one[0], pile_one[1] ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute broken_line_pair \n");
	  exit( EXIT_FAILURE );
	}

      }

    }

  /******* lv_one が 三つの原子からなる場合 *********/
  }else if( lv_one->member_Num == 3 ){

    if( a1st[ pile_one[0] ].grade == a1st[ pile_one[1] ].grade ){

      compeNum = 2;

      if( (competitor=(int *)malloc(compeNum*sizeof(int))) == NULL ){
	printf(" error : Cannot allocate memory in set_upper_level \n");
	exit( EXIT_FAILURE );
      }

      if( a1st[ pile_one[1] ].grade == a1st[ pile_one[2] ].grade ){
	// 0-0-0

	if( (class=(int *)malloc((iatm+new_atm)*sizeof(int)))==NULL){
	  printf(" error : Cannot allocate memory in set_upper_level \n");
	  exit( EXIT_FAILURE );
	}

	for(i=1;i<iatm+new_atm;i++){
	  class[i]=a1st[i].grade;
	}

	scale = 3; // pile_one は 3 個の原子からなる

	if( (caddy=(int *)malloc(scale*sizeof(int)))==NULL){
	  printf(" error : Cannot allocate memory in set_upper_level \n");
	  exit( EXIT_FAILURE );
	}

	for(i=0; i < scale ;i++){
	  caddy[i] = pile_one[i];
	}

	for(i=0;i<scale;i++){
	  higher = caddy[i];

	  if( raise_and_order( higher, caddy, scale ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute raise_and_order \n");
	    exit( EXIT_FAILURE );
	  }

	  // 0-1-1
          if( i == 0 ){
	    competitor[0] = caddy[1];
	    competitor[1] = caddy[2];
	  }else if( i == 1 ){
	    competitor[0] = caddy[0];
	    competitor[1] = caddy[2];
	  }else{
	    competitor[0] = caddy[0];
	    competitor[1] = caddy[1];
	  }

#ifdef TEST_SET_UPPER_LEVEL
	  printf("set_upper_level calls distinguish_competitive_atoms. \n");
#endif

	  if( distinguish_competitive_atoms( higher, competitor, compeNum ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute distinguish_competitive_atoms \n");
	    exit( EXIT_FAILURE );
	  }

	  for( j=1 ; j < iatm+new_atm ; j++){
	    a1st[j].grade = class[j];
	    cast_codes[j] = MINUS;
	  }

	}

	free(class);
	free(caddy);

      }else{
	// 0-0-1
        competitor[0] = pile_one[0];
	competitor[1] = pile_one[1];

#ifdef TEST_SET_UPPER_LEVEL
	printf("set_upper_level calls distinguish_competitive_atoms. \n");
#endif

        if( distinguish_competitive_atoms( pile_one[2], competitor, compeNum ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute distinguish_competitive_atoms \n");
	  exit( EXIT_FAILURE );
	}
        
      }

      free(competitor);

    }else{

      if( a1st[ pile_one[1] ].grade == a1st[ pile_one[2] ].grade ){
	// 0-1-1
	compeNum = 2;

	if( (competitor=(int *)malloc(compeNum*sizeof(int))) == NULL ){
	  printf(" error : Cannot allocate memory in set_upper_level \n");
	  exit( EXIT_FAILURE );
	}

        competitor[0] = pile_one[1];
	competitor[1] = pile_one[2];

#ifdef TEST_SET_UPPER_LEVEL
	printf("set_upper_level calls distinguish_competitive_atoms. \n");
#endif

        if( distinguish_competitive_atoms( pile_one[0], competitor, compeNum ) != EXIT_SUCCESS ){
	  printf(" error : Cannot execute distinguish_competitive_atomss \n");
	  exit( EXIT_FAILURE );
	}

	free(competitor);

      }else{
	// 0-1-2
        rank = 3;
	if( (ranker=(int *)malloc(rank*sizeof(int)))==NULL){
	  printf(" error : Cannot allocate memory in set_upper_level \n");
	  exit( EXIT_FAILURE );
	}

	for( i=0; i < rank ; i++){
	  ranker[i] = pile_one[i];
	}

#ifdef TEST_SET_UPPER_LEVEL
        printf("set_upper_level calls arrange_ordered_atom (1) \n");
#endif

	if( arrange_ordered_atoms( ranker, rank ) != EXIT_SUCCESS ){
	  printf(" error :  Cannot execute arrange_ordered_atoms \n");
	  exit( EXIT_FAILURE );
	}

	free(ranker);

      }
    }

  /******* lv_one が 四つの原子からなる場合 *********/
  }else if( lv_one->member_Num == 4 ){

    if( a1st[ pile_one[0] ].grade == a1st[ pile_one[1] ].grade ){

      if( a1st[ pile_one[1] ].grade == a1st[ pile_one[2] ].grade ){

	/*
	compeNum = 3;

	if( (competitor=(int *)malloc(compeNum*sizeof(int))) == NULL ){
	  printf(" error : Cannot allocate memory in set_upper_level \n");
	  exit( EXIT_FAILURE );
	}
	*/

	if( a1st[ pile_one[2] ].grade == a1st[ pile_one[3] ].grade ){
	  // 0-0-0-0
	  if( (class=(int *)malloc((iatm+new_atm)*sizeof(int)))==NULL){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  for(i=1;i<iatm+new_atm;i++){
	    class[i]=a1st[i].grade;
	  }

	  scale = 4; // pile_one は 4 個の原子からなる

	  if( (caddy=(int *)malloc(scale*sizeof(int)))==NULL){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  for(i=0; i < scale ;i++){
	    caddy[i] = pile_one[i];
	  }

	  for(i=0; i < scale ;i++){
	    higher = caddy[i];

	    if( raise_and_order( higher, caddy, scale ) != EXIT_SUCCESS ){
	      printf(" error : Cannot execute raise_and_order \n");
	      exit( EXIT_FAILURE );
	    }

	    if( set_upper_level( ) != EXIT_SUCCESS ){
	      printf(" error : Cannot execute set_upper_level \n");
	      exit( EXIT_FAILURE );
	    }

            // 0-1-1-1
	    /*
            if( i == 0 ){
	      competitor[0] = caddy[1];
	      competitor[1] = caddy[2];
	      competitor[2] = caddy[3];
	    }else if( i == 1 ){
	      competitor[0] = caddy[0];
	      competitor[1] = caddy[2];
	      competitor[2] = caddy[3];
	    }else if( i == 2 ){
	      competitor[0] = caddy[0];
	      competitor[1] = caddy[1];
	      competitor[2] = caddy[3];
	    }else{
	      competitor[0] = caddy[0];
	      competitor[1] = caddy[1];
	      competitor[2] = caddy[2];
	    }

	    if( distinguish_competitive_atoms( higher, competitor, compeNum ) != EXIT_SUCCESS ){
	      printf(" error : Cannot execute distinguish_competitive_atoms \n");
	      exit( EXIT_FAILURE );
	    }
	    */

	    for( j=1 ; j < iatm+new_atm ; j++){
	      a1st[j].grade = class[j];
	      cast_codes[j] = MINUS;
	    }

	  }

	  free(class);
	  free(caddy);

	}else{ // a1st[ pile_one[2] ].grade != a1st[ pile_one[3] ].grade

	  // 0-0-0-1
	  compeNum = 3;

	  if( (competitor=(int *)malloc(compeNum*sizeof(int))) == NULL ){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  competitor[0] = pile_one[0];
	  competitor[1] = pile_one[1];
	  competitor[2] = pile_one[2];

#ifdef TEST_SET_UPPER_LEVEL
	  printf("set_upper_level calls distinguish_competitive_atoms. \n");
#endif

	  if( distinguish_competitive_atoms( pile_one[3], competitor, compeNum ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute distinguish_competitive_atoms \n");
	    exit( EXIT_FAILURE );
	  }

	  free(competitor);

	}

      }else{ // a1st[ pile_one[1] ].grade != a1st[ pile_one[2] ].grade

	/*
	pivNum = 2;

	if( (pivots=(int *)malloc(pivNum*sizeof(int)))==NULL){
	  printf(" error : Cannot allocate memory in set_upper_level \n");
	  exit( EXIT_FAILURE );
	}

	compeNum = 2;

	if( (competitor=(int *)malloc(compeNum*sizeof(int))) == NULL ){
	  printf(" error : Cannot allocate memory in set_upper_level \n");
	  exit( EXIT_FAILURE );
	}
	*/

	if( a1st[ pile_one[2] ].grade == a1st[ pile_one[3] ].grade ){
	  // 0-0-1-1
	  if( (class=(int *)malloc((iatm+new_atm)*sizeof(int)))==NULL){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  for(i=1;i<iatm+new_atm;i++){
	    class[i]=a1st[i].grade;
	  }

	  scale = 2;

	  if( (caddy=(int *)malloc(scale*sizeof(int)))==NULL){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  caddy[0] = pile_one[0];
	  caddy[1] = pile_one[1];

	  /*
	  competitor[0] = pile_one[2];
	  competitor[1] = pile_one[3];
	  */

	  for(i=0; i < scale ;i++){
	    higher = caddy[i];

	    if( raise_and_order( higher, caddy, scale ) != EXIT_SUCCESS ){
	      printf(" error : Cannot execute raise_and_order \n");
	      exit( EXIT_FAILURE );
	    }

	    if( set_upper_level( ) != EXIT_SUCCESS ){
	      printf(" error : Cannot execute set_upper_level \n");
	      exit( EXIT_FAILURE );
	    }

            // 0-1-2-2
	    /*
	    if( i == 0 ){
	      pivots[0] = caddy[0];
	      pivots[1] = caddy[1];
	    }else{
	      pivots[0] = caddy[1];
	      pivots[1] = caddy[0];
	    }

	    if( distinguish_competitive_atoms2( pivots, competitor, FIRST_THREE ) != EXIT_SUCCESS ){
	      printf(" error : Cannot execute distinguish_competitive_atoms2 \n");
	      exit( EXIT_FAILURE );
	    }
	    */

	    for( j=1 ; j < iatm+new_atm ; j++){
	      a1st[j].grade = class[j];
	      cast_codes[j] = MINUS;
	    }

	  }

	  free(class);
	  free(caddy);

	}else{ // a1st[ pile_one[2] ].grade != a1st[ pile_one[3] ].grade
	  // 0-0-1-2
	  pivNum = 2;

	  if( (pivots=(int *)malloc(pivNum*sizeof(int)))==NULL){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  compeNum = 2;

	  if( (competitor=(int *)malloc(compeNum*sizeof(int))) == NULL ){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  pivots[0] = pile_one[2];
	  pivots[1] = pile_one[3];
         
          competitor[0] = pile_one[0];
	  competitor[1] = pile_one[1];

#ifdef TEST_SET_UPPER_LEVEL
	  printf("set_upper_level calls distinguish_competitive_atoms2. \n");
#endif

	  if( distinguish_competitive_atoms2( pivots, competitor, THIRD_THREE ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute distinguish_competitive_atoms2 \n");
	    exit( EXIT_FAILURE );
	  }

	  free(pivots);
	  free(competitor);

	}

      }

    }else{ // a1st[ pile_one[0] ].grade != a1st[ pile_one[1] ].grade
      if( a1st[ pile_one[1] ].grade == a1st[ pile_one[2] ].grade ){

	if( a1st[ pile_one[2] ].grade == a1st[ pile_one[3] ].grade ){
	  // 0-1-1-1
	  compeNum = 3;

	  if( (competitor=(int *)malloc(compeNum*sizeof(int))) == NULL ){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  competitor[0] = pile_one[1];
	  competitor[1] = pile_one[2];
	  competitor[2] = pile_one[3];

#ifdef TEST_SET_UPPER_LEVEL
	  printf("set_upper_level calls distinguish_competitive_atoms. \n");
#endif

	  if( distinguish_competitive_atoms( pile_one[0], competitor, compeNum ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute distinguish_competitive_atomss \n");
	    exit( EXIT_FAILURE );
	  }

	  free(competitor);

	}else{  // a1st[ pile_one[2] ].grade != a1st[ pile_one[3] ].grade
	  // 0-1-1-2
          pivNum = 2;

	  if( (pivots=(int *)malloc(pivNum*sizeof(int))) == NULL ){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  compeNum = 2;

	  if( (competitor=(int *)malloc(compeNum*sizeof(int))) == NULL ){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  pivots[0] = pile_one[0];
	  pivots[1] = pile_one[3];
         
          competitor[0] = pile_one[1];
	  competitor[1] = pile_one[2];

#ifdef TEST_SET_UPPER_LEVEL
	  printf("set_upper_level calls distinguish_competitive_atoms2. \n");
#endif

	  if( distinguish_competitive_atoms2( pivots, competitor, THIRD_THREE ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute distinguish_competitive_atoms2 \n");
	    exit( EXIT_FAILURE );
	  }

	  free(pivots);
	  free(competitor);

	}

      }else{ // a1st[ pile_one[1] ].grade != a1st[ pile_one[2] ].grade

	if( a1st[ pile_one[2] ].grade == a1st[ pile_one[3] ].grade ){
	  // 0-1-2-2
          pivNum = 2;

	  if( (pivots=(int *)malloc(pivNum*sizeof(int))) == NULL ){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  compeNum = 2;

	  if( (competitor=(int *)malloc(compeNum*sizeof(int))) == NULL ){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  pivots[0] = pile_one[0];
	  pivots[1] = pile_one[1];
         
          competitor[0] = pile_one[2];
	  competitor[1] = pile_one[3];

#ifdef TEST_SET_UPPER_LEVEL
	  printf("set_upper_level calls distinguish_competitive_atoms2. \n");
#endif

	  if( distinguish_competitive_atoms2( pivots, competitor, FIRST_THREE ) != EXIT_SUCCESS ){
	    printf(" error : Cannot execute distinguish_competitive_atoms2 \n");
	    exit( EXIT_FAILURE );
	  }

	  free(pivots);
	  free(competitor);

	}else{
	  // 0-1-2-3
	  rank = 4;
	  if( (ranker=(int *)malloc(rank*sizeof(int)))==NULL){
	    printf(" error : Cannot allocate memory in set_upper_level \n");
	    exit( EXIT_FAILURE );
	  }

	  for( i=0; i < rank ; i++){
	    ranker[i] = pile_one[i];
	  }

#ifdef TEST_SET_UPPER_LEVEL
	  printf("set_upper_level calls arrange_ordered_atom (2) \n");
#endif

	  if( arrange_ordered_atoms( ranker, rank ) != EXIT_SUCCESS ){
	    printf(" error :  Cannot execute arrange_ordered_atoms \n");
	    exit( EXIT_FAILURE );
	  }

	  free(ranker);

	}
      }
    }

  }else if(  lv_one->member_Num == 5 ){

    rank = 5;
    if( (ranker=(int *)malloc(rank*sizeof(int)))==NULL){
      printf(" error : Cannot allocate memory in set_upper_level \n");
      exit( EXIT_FAILURE );
    }

    for( i=0; i < rank ; i++){
      ranker[i] = pile_one[i];
    }

#ifdef TEST_SET_UPPER_LEVEL
    printf("set_upper_level calls arrange_ordered_atom (3) \n");
#endif

    if( arrange_ordered_atoms( ranker, rank ) != EXIT_SUCCESS ){
      printf(" error :  Cannot execute arrange_ordered_atoms \n");
      exit( EXIT_FAILURE );
    }

    free(ranker);

  }else{

    printf(" error : Cannot deal with the atom %d \n", start);
    exit( EXIT_FAILURE );

  }

  return EXIT_SUCCESS;

}
