// main_canost.c ***** main *****

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_MAIN 1 
// #define TEST_MAIN_SECOND 1
// #define TEST_MAIN_THIRD 1

#ifdef TEST_MAIN
 FILE *dummyfile; // a file for debugging
#endif

/************* external variables *****************/

/*** input, output files, output format ***/
FILE *fin; // input mol file
FILE *fo;  // output file
int option; // option indicating output format
int opt_level; // input argument for designating the level of line-notation
int boundary; // the designated level
int atoms_in_confine; // the number of atoms within the designated level

/*** molecular graph ***/
int iatm;  // the number of atoms
int ibnd;  // the number of bonds
struct atom *a1st; // head pointer of structure atom
struct bond *b1st; // head pointer of structure bond
int new_atm; // the number of pseudoatoms
struct atom *va; // head pointer of strcture atom for psuedoatoms
int *confined_bond; // array for storing the number of bonds connecting atoms within
// the designated level for each atom

/*** layered network ***/
int start; // the starting point of a layered network
struct level root; // the highest layer of a layered network
struct level *top,*down; // pointers for structure level
int lowest; // points the lowest layer

/*** pointers for the lexicographically first line-notation ***/
struct canost tip; // head pointer of the array for storing CANOST codes
int first_code_head; // the head atom of the lexicograohically first line-notation
int *first_code; // the pointer of the lexicograohically first line-notation
int length_first_code; // the length of the lexicograohically first line-notation
char *canonical_code;
char *canonical_numbering;
int *new_code;
int length_new_code;

/*** pointers for the lexicographically first line-notation for each atom ***/
int *leading_code;
int length_leading_code;
char *leading_canost;
char *leading_numbering;
int *candidate_code;
int length_candidate; 

/*** symmetric moieties  ***/
int *interim_order;
struct level *caput,*pes;
int *representative;
int *orbit_size;
int *unique_numbering;

/*** canonicalization ***/
int equive,pre_equive; // the number of tie-classes
int destination; // is 1 if top-down sorting, is 0 if bottom-up sorting

/*** intestional ordering ***/
struct level *reef; // pointer for a layer containing a tie-class
int ore; // points the atom investigated in layer reef

/*** CANOST codes ***/
int canost_codes; // the number of CANOST codes

/*** Queue ***/
struct queue *head,*tail; // Queue
struct queue *front,*back; // Queue

/*** CAST codes ***/
int *cast_codes; // head pointer for the array stroring CAST codes
int *direct_parent; // directly connected parent
int *relative_parent; // the parent for determining the dihedral angle
int *relation; // the bond relation between relative_parent and child

/*** Error mode ***/
int error_mode; // 0: reject irregular atom 1: accept irregular atom

/*****************************************/

// main function
int main( int argc, char *argv[] ){

  int i,j;

  int option_check;
  int level_check;
  
  char in_line[MAX_LINE]; // the read line
  char firsts[4],seconds[4]; // the end-points of the bond

  char atms[4],bnds[4]; // the numbers of atoms and bonds

  int first_atm; // an end-point of a bond
  int second_atm; // the other end-point of the bond
  int bnd_type; // the type of the bond
  int bnd_stereo;

  struct level *lp,*lp2;

#ifdef TEST_MAIN
  struct bond *bp;
#endif

  /******** Open the input molfile *********/ 
  // if the number of the input arguments is less than 5, show the usage
  if(argc < 5){
    printf(" Usage : %% ./main_canost <output-format -c | -n | -p | -s | -d | -u | -e> "
           "<coding_level f | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10> "
           "input-filename output-filename \n");
    exit( EXIT_FAILURE );
  }

  option_check = 0;
  level_check = 0;

  // the output format
  if( strcmp(argv[1],"-c") == 0 || strcmp(argv[1],"-ci") == 0  ){
    option = CODE; // output CANOST codes
    option_check = 1;
  }else if( strcmp(argv[1],"-n") == 0 || strcmp(argv[1],"-ni") == 0 ){
    option = SERIAL; // output serial numbers
    option_check = 1;
  }else if( strcmp(argv[1],"-p") == 0 || strcmp(argv[1],"-pi") == 0 ){
    option = FILIATION; // output CANOST codes and filiation
    option_check = 1;
  }else if( strcmp(argv[1],"-s") == 0 || strcmp(argv[1],"-si") == 0 ){
    option = SYMMETRIC; // output CANOST codes and symmetric moieties
    option_check = 1;
  }else if( strcmp(argv[1],"-d") == 0 || strcmp(argv[1],"-di") == 0 ){
    option = CONFORM; // output CANOST codes and atoms whose ranks depends on conformatin
    option_check = 1;
  }else if( strcmp(argv[1],"-u") == 0 || strcmp(argv[1],"-ui") == 0 ){
    option = CANONICAL; // output the canonical line-notation
    option_check = 1;
  }else if( strcmp(argv[1],"-e") == 0 || strcmp(argv[1],"-ei") == 0 ){
    option = UNIQUE; //  output the canonical line-notations for each atom
    option_check = 1;
  }else{
    option_check = 0;
  }

  if( argv[1][2] == 'i' ){
    // printf("%c \n",argv[1][2]);
    error_mode = 1;
  }

  // the designated level is greater than 2 and less than 11
  if( strcmp(argv[2],"f")==0 ){
    opt_level = MINUS ;
    level_check = 1;
  }else{
    opt_level = atoi(argv[2]);
    if( 2 < opt_level && opt_level <= 10 ){
      level_check = 1;
    }
  }

  if( option_check == 0 || level_check == 0 ){
    // if the input arguments are wrong, show the usage
    printf(" Usage : %% ./main_canost <output-format -c | -n | -p | -s | -d | -u | -e> "
           "<coding_level f | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10> "
           "input-filename output-filename \n"
           "\n"
           " Output format : -c output canost code \n"
           "                 -n output serial number \n"
           "                 -p output canost code and filiation \n"
           "                 -s output canost code and symmetric substructures \n"
           "                 -d output canost code and atoms depending on their conformations \n"
           "                 -u output canonical canost code of input molecule \n"
           "                 -e output canonical canost code starting at each atom \n"
           " Output format : -ci -c + irregular \n"
           "                 -ni -n + irregular \n"
           "                 -pi -p + irregular \n"
           "                 -si -s + irregular \n"
           "                 -di -d + irregular \n"
           "                 -ui -u + irregular \n"
           "                 -ei -e + irregular \n"
           "\n"
           " Coding Level  : f entire canost code \n"
           "                 3 canost code in level 3 \n"
           "                 4 canost code in level 4 \n"
           "                 - - - - - - - - - - - -  \n"
           "                 10 canost code in level 10 \n");
    exit( EXIT_FAILURE );
  }

#ifdef TEST_MAIN_SECOND
  printf("input-filename = %s\n",argv[3]);
#endif

  // open the input mol file
  if((fin=fopen(argv[3],"r")) == NULL){
     printf(" error : Cannot open molfile \n");
     exit( EXIT_FAILURE );
  }

  // open the output file
  if((fo=fopen(argv[4],"w")) == NULL){
     printf(" error : Cannot open output-file \n");
     exit( EXIT_FAILURE );
  }

  /*********** read the input mol file and construcut a molecular graph *******/

  /* skip the three lines from the head */
  for(i=0;i<3;i++){
    if( fgets(in_line,MAX_LINE,fin) == NULL ){
      printf(" error : Cannot read input file \n");
      exit( EXIT_FAILURE );
    }
  }

  /* read the number of atoms and bonds */
  if( fgets(in_line,MAX_LINE,fin) == NULL ){
    printf(" error : Cannot read input file \n");
    exit( EXIT_FAILURE );
  }

  for(i=0;i<3;i++){
    atms[i]=in_line[i];
  }
  atms[3] = '\0';
  iatm = atoi(atms);
  iatm = iatm + 1; // for a1st[ ZERO ]

  for(i=0;i<3;i++){
    bnds[i]=in_line[i+3];
  }
  bnds[3] = '\0';
  ibnd = atoi(bnds);

  /* allocate an array storing the atoms */
  if( (a1st=(struct atom *)calloc(iatm,sizeof(struct atom)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
  }

  /* allocate an array storing the bonds */
  if( (b1st=(struct bond *)calloc(2*ibnd+1,sizeof(struct bond)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
  }

  /* allocate an arrray storing the number of bonds within the designated level */
  if( (confined_bond=(int *)calloc(iatm,sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
  }

  /**** construct a molecular graph ****/

  /* atom ZERO is an atom at origin  */
  a1st[0].xco = 0.0;
  a1st[0].yco = 0.0;
  a1st[0].zco = 0.0;
  a1st[0].nbnd = 0;
  a1st[0].atmNum = 0;
  strcpy(a1st[0].code,"?");
  a1st[0].codeNum = 0;
  strcpy(a1st[0].atm_type,"?");
  a1st[0].adj_list = NULL;
  a1st[0].last = NULL;
  a1st[0].state = UNVISITED;
  a1st[0].grade = MINUS;
  a1st[0].layer = MINUS;
  a1st[0].link = MINUS;
  for(i=0;i < TUPLE_SIZE ;i++){
    a1st[0].tuple_parent[i] = MINUS;
    a1st[0].tuple_child[i] = MINUS;
  }
  confined_bond[0] = 0;

  /* read information on the atoms */
  for(i=1;i<iatm;i++){
    if( fgets(in_line,MAX_LINE,fin) == NULL ){
      printf(" error : Cannot read input file \n");
      exit( EXIT_FAILURE );
    }
    if( sscanf(in_line,"%lf %lf %lf %s",&(a1st[i].xco),&(a1st[i].yco),
	       &(a1st[i].zco),a1st[i].atm_type) == EOF ){
      printf(" error : Cannot read input file \n");
      exit( EXIT_FAILURE );
    }
    a1st[i].nbnd = 0;
    a1st[i].atmNum = 0;
    strcpy(a1st[i].code,"?");
    a1st[i].codeNum = 0;
    for(j=0;j<3;j++){
      a1st[i].atm_type[j] = small_to_capital( a1st[i].atm_type[j] );
    }
  }

  /* read information on the bonds */
  for(i=0;i<ibnd;i++){
    if( fgets(in_line,MAX_LINE,fin) == NULL ){
      printf(" error : Cannot read input file \n");
      exit( EXIT_FAILURE );
    }

    for(j=0;j<3;j++){
      firsts[j] = in_line[j];
    }
    firsts[3] = '\0';
    first_atm = atoi(firsts);

    for(j=0;j<3;j++){
      seconds[j] = in_line[j+3];
    }
    seconds[3] = '\0';
    second_atm = atoi(seconds);

    if( sscanf(in_line+6,"%d %d",&bnd_type,&bnd_stereo) == EOF ){
      printf(" error : Cannot read input file \n");
      exit( EXIT_FAILURE );
    }
  
    b1st[2*i].second = second_atm;
    b1st[2*i].alias = MINUS;
    b1st[2*i].btype = bnd_type;
    b1st[2*i].bstereo = bnd_stereo;

    b1st[2*i+1].second = first_atm;
    b1st[2*i+1].alias = MINUS;
    b1st[2*i+1].btype = bnd_type;
    b1st[2*i+1].bstereo = bnd_stereo;

    /* make adjacent lists */
    a1st[ first_atm ].nbnd++; // count the number of bonds
    if( a1st[ first_atm ].last == NULL ){
      a1st[ first_atm ].adj_list = &b1st[2*i];
      a1st[ first_atm ].last = &b1st[2*i];
    }else{
      (a1st[ first_atm ].last)->next = &b1st[2*i];
      a1st[ first_atm ].last = &b1st[2*i];
    }

    a1st[ second_atm ].nbnd++; // count the number of bonds
    if( a1st[ second_atm ].last == NULL){
      a1st[ second_atm ].adj_list = &b1st[2*i+1];
      a1st[ second_atm ].last = &b1st[2*i+1];
    }else{
      (a1st[ second_atm ].last)->next = &b1st[2*i+1];
      a1st[ second_atm ].last = &b1st[2*i+1];
    }
  }

  fclose(fin);  // close the input mol file

#ifdef TEST_MAIN
  dummyfile=fopen("dummy_main.dat","w");

  for(i=0;i<iatm;i++){
    fprintf(dummyfile,"%f %f %f %s %d \n",a1st[i].xco,a1st[i].yco,
            a1st[i].zco,a1st[i].atm_type,a1st[i].nbnd);
    for( bp=a1st[i].adj_list ; bp != NULL ; bp=bp->next){
      fprintf(dummyfile,"%d %d %d\n",bp->second,bp->btype,bp->bstereo);
    }
  }
 
  fclose(dummyfile);
#endif

  /********* existence of an atom having too high valence  *************/
  for( i=1 ; i < iatm ; i++ ){
    if( a1st[i].nbnd > TUPLE_SIZE ){
      printf(" Warning : The atom %d has valence of %d \n", i, a1st[i].nbnd);
      exit( EXIT_FAILURE );
    }
  }

  /********* recongnize atomic numbers ***************/
  if( atomic_num() != EXIT_SUCCESS ){
     printf(" error : Cannot recognize atomic numbers \n");
     exit( EXIT_FAILURE );
  }

  /********* recognize partial structures and assign CANOST codes ************/
  if( recognition() != EXIT_SUCCESS ){
     printf(" error : Cannot recognize substructures \n");
     exit( EXIT_FAILURE );
  }

  /********* assign the priority number of CANOST condes **********/
  if( assign_codeNum() != EXIT_SUCCESS ){
     printf(" error : Cannot assign canost code number \n");
     exit( EXIT_FAILURE );
  }

  /**** innitialize pointers  *******/
  va = NULL;
  direct_parent = NULL;
  relative_parent = NULL;
  relation = NULL;
  cast_codes = NULL;
  confined_bond = NULL;

  first_code = NULL;
  canonical_code = NULL;
  canonical_numbering = NULL;
  new_code = NULL;

  leading_code = NULL;
  leading_canost = NULL;
  leading_numbering = NULL;
  candidate_code = NULL;

  interim_order = NULL;
  caput = NULL;
  pes = NULL;
  representative = NULL;
  orbit_size = NULL; 
  unique_numbering = NULL;

  /**** the highest layer of the layered network *****/
  top = &root;
  top->prev = NULL;

  if((top->member=(int *)malloc(sizeof(int)))==NULL){
    printf(" error : Cannot allocate memory in main_cansot \n");
    exit( EXIT_FAILURE );
  }
  top->member_Num = 1; // the number of the atoms in the layer

  /**** initialize an array storing the lexicographically first line-notation *****/
  if( option == CANONICAL ){

    first_code_head = 0;

    length_first_code = 1;
    if( (first_code=(int *)realloc(first_code,length_first_code*sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }
    first_code[0] = canost_codes+1;

    if( (canonical_code=(char *)realloc(canonical_code,length_first_code*sizeof(char)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }
    canonical_code[0]='\0';

    if( (canonical_numbering=(char *)realloc(canonical_numbering,length_first_code*sizeof(char)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }
    canonical_numbering[0]='\0';

    length_new_code = 1;
    if( (new_code=(int *)realloc(new_code,length_new_code*sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }
    new_code[0] = canost_codes+1;

  }

  /********* symmetric moieties **********/
  if( option == SYMMETRIC ){
    if( (representative=(int *)calloc(iatm,sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }

    if( (orbit_size=(int *)calloc(iatm,sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }

    if( (unique_numbering=(int *)calloc(iatm,sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }

    if( initgroup(iatm,representative,orbit_size) != EXIT_SUCCESS ){
      printf("error : Cannot execute initgroup in main_canost \n");
      exit( EXIT_FAILURE );
    }

  }

  /********** generating CANOST notations *************/
  for(i = 1; i < iatm; i++){
    // for( i = 2 ; i < 3 ; i++ ){
    start = i;

#ifdef TEST_MAIN_THIRD
    printf("\n ---- start atom %d ----- \n",start);
#endif

    /* ------- construct a layered network with the aid of breadth-first search -------- */
    top = &root;
    down = &root;
    new_atm = 0;

    if( fun_bfs() != EXIT_SUCCESS ){
      printf(" error : Cannot execute breadth first search \n");
      exit( EXIT_FAILURE );
    }

    /* ------- reallocate an array storing atoms and pseudoatoms ------- */
    if( (a1st=(struct atom *)realloc(a1st,(iatm+new_atm)*sizeof(struct atom)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }

    if( (confined_bond=(int *)realloc(confined_bond,(iatm+new_atm)*sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }

    for(j=iatm;j<iatm+new_atm;j++){
      a1st[j]=va[ j-iatm ];
    }

    if( opt_level == MINUS || lowest < opt_level ){
      boundary = lowest+1;
    }else{
      boundary = opt_level-1;
    }

    /* ------- cofine the level fo generated CANOST notation -------- */
    if( confine_level() != EXIT_SUCCESS ){
      printf(" error : Cannot execute confine_level \n");
      exit( EXIT_FAILURE );
    }

    /* ------- detect pendant layered subnetwork ----------*/
    if( prune_branch() != EXIT_SUCCESS ){
      printf(" error : Cannot execute prune_branch \n");
      exit( EXIT_FAILURE );
    }

    /* ------- allocate arrays for CAST codes --------*/
    /**** allocate direct_parent, relative_parent, relation, cast_codes *****/
    if( (direct_parent=(int *)realloc(direct_parent,(iatm+new_atm)*sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }

    if( (relative_parent=(int *)realloc(relative_parent,(iatm+new_atm)*sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }

    if( (relation=(int *)realloc(relation,(iatm+new_atm)*sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }

    if( (cast_codes=(int *)realloc(cast_codes,(iatm+new_atm)*sizeof(int)))==NULL ){
      printf(" error : Cannot allocate memory in main_canost \n");
      exit( EXIT_FAILURE );
    }

    /* -------- the lexicographically first line-notation for the star atom ----------- */
    if( option == SYMMETRIC || option == UNIQUE ){ 

      length_leading_code = 1;
      if( (leading_code=(int *)realloc(leading_code,length_leading_code*sizeof(int)))==NULL ){
	printf(" error : Cannot allocate memory in main_canost \n");
	exit( EXIT_FAILURE );
      }
      leading_code[0] = canost_codes+1;

      if( (leading_canost=(char *)realloc(leading_canost,length_leading_code*sizeof(char)))==NULL ){
	printf(" error : Cannot allocate memory in main_canost \n");
	exit( EXIT_FAILURE );
      }
      leading_canost[0]='\0';

      if( (leading_numbering=(char *)realloc(leading_numbering,length_leading_code*sizeof(char)))==NULL ){
	printf(" error : Cannot allocate memory in main_canost \n");
	exit( EXIT_FAILURE );
      }
      leading_numbering[0]='\0';

      length_candidate = 1;
      if( (candidate_code=(int *)realloc(candidate_code,length_candidate*sizeof(int)))==NULL ){
	printf(" error : Cannot allocate memory in main_canost \n");
	exit( EXIT_FAILURE );
      }
      candidate_code[0] = canost_codes+1;

    }

    if( option == SYMMETRIC ){

      if( (interim_order=(int *)realloc(interim_order,(iatm+new_atm)*sizeof(int)))==NULL ){
	printf(" error : Cannot allocate memory in main_canost \n");
	exit( EXIT_FAILURE );
      }

    }

    /* ------- canonicalization ------- */
    if( canonicalization() != EXIT_SUCCESS ){
      printf(" error : Cannot canonicalize the input chemical graph \n");
      exit( EXIT_FAILURE );
    }

    /* -------- output the lexixographically first line-notation for the start atom ----------- */
    if( option == UNIQUE ){
      fprintf(fo,"--- Canonical CANOST code starting with serial no. %d atom --- \n ",start);
      fprintf(fo,"%s \n",leading_canost);
      fprintf(fo,"--- (one of) the canonical numbering(s) starting with serial no. %d atom --- \n ",
	      start);
      fprintf(fo,"%s \n\n",leading_numbering);
    }

    for(j=0; j < new_atm ;j++){
      free(va[j].last);
      free(va[j].adj_list);
    }

    /*
    for(j=0;j < iatm ; j++ ){
      printf("%d %d %d \n",j,representative[j],orbit_size[j]);
    }
    */

    if( option == SYMMETRIC ){
      lp2=NULL;
      for( lp = caput ; lp != NULL ; lp = lp->next ){
	free(lp->member);
	free(lp2);
	lp2 = lp;
      }
      free(lp2);

    }

    top=&root;
    lp2=NULL;
    for( lp = top->next ; lp != NULL ; lp = lp->next ){
      free(lp->member);
      free(lp2);
      lp2 = lp;
    }
    free(lp2);

  }

  if( option == CANONICAL ){
    fprintf(fo,"\n--- Canonical CANOST code --- \n ");
    fprintf(fo,"%s \n",canonical_code);
    fprintf(fo,"--- (one of) the canonical numbering(s) starting with serial no. %d atom --- \n ",
            first_code_head);
    fprintf(fo,"%s \n",canonical_numbering);
  }

  fclose(fo);

  if( option == UNIQUE ){

    if( equive_search(argv[4]) != EXIT_SUCCESS ){
      printf(" error : Cannot execute equive_search \n");
      exit( EXIT_FAILURE );
    }

  }

  free(top->member);

  free(a1st);
  free(b1st);
  free(va);
  free(confined_bond);

  free(direct_parent);
  free(relative_parent);
  free(relation);
  free(cast_codes);

  free(first_code);
  free(canonical_code);
  free(canonical_numbering);
  free(new_code);

  free(leading_code);
  free(leading_canost);
  free(leading_numbering);
  free(candidate_code);

  free(representative);
  free(orbit_size);
  free(unique_numbering);

  if( option == SYMMETRIC ){
    free(interim_order);
  }

  return EXIT_SUCCESS;

}
