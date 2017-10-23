// atomic_num.c 原子番号をふる

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_ATOMIC_NUM 1

extern int iatm;
extern int ibnd;
extern struct atom *a1st;
extern struct bond *b1st;

#ifdef TEST_ATOMIC_NUM
extern FILE *dummyfile;
#endif 

int atomic_num( void ){
  
 int i,j;

 int flag;  // 下に登録されていない原子があると0となる

 char *atype[104]={
     "H","HE","LI","BE","B",
     "C","N","O","F","NE",
     "NA","MG","AL","SI","P",
     "S","CL","AR","K","CA",
     "SC","TI","V","CR","MN",
     "FE","CO","NI","CU","ZN",
     "GA","GE","AS","SE","BR",
     "KR","RB","SR","Y","ZR",
     "NB","MO","TC","RU","RH",
     "PD","AG","CD","IN","SN",
     "SB","TE","I","XE","CS",
     "BA","LA","CE","PR","ND",
     "PM","SM","EU","GD","TB",
     "DY","HO","ER","TM","YB",
     "LU","HF","TA","W","RE",
     "OS","IR","PT","AU","HG",
     "TL","PB","BI","PO","AT",
     "RN","FR","RA","AC","TH",
     "PA","U","NP","PU","AM",
     "CM","BK","CF","ES","FM",
     "MD","NO","LR","D"
 };

#ifdef TEST_ATOMIC_NUM
 printf("atomic_num \n");
 dummyfile=fopen("dummy_atomic_num.dat","w");
#endif

 for( i=1 ; i < iatm ; i++){
   flag = 0;
   for(j=0;j<104;j++){
     // 登録されている原子かを判定 
     if( strcmp( a1st[i].atm_type , atype[j] ) == 0 ){ 
       a1st[i].atmNum = j+1;
       flag = 1;
     }
   }
   if( flag == 0 ){  // 未登録の原子を発見
     printf("Irregular type of atom is included : serial no. %d\n" ,i);
   }
#ifdef TEST_ATOMIC_NUM
   fprintf(dummyfile,"%s %d\n",a1st[i].atm_type,a1st[i].atmNum);
#endif
 }

#ifdef TEST_ATOMIC_NUM
 fclose(dummyfile);
#endif

 return EXIT_SUCCESS; 

}
