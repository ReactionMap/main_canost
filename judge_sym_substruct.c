// judge_sym_substruct.c 規範化のメインループ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

// #define TEST_JUDGE_SYM 1

extern int iatm;
extern struct atom *a1st;
extern struct queue *head,*tail;
extern struct queue *front,*back;
extern FILE *fo;
extern int *representative;
extern int *orbit_size;

int judge_sym_substruct( void ){

  int i,j;

  int orbit_number;

#ifdef TEST_JUDGE_SYM
  printf("judge_sym_substruct \n");
#endif

  orbit_number = MINUS;

  if( initialize_primary_queue() != EXIT_SUCCESS ){
    printf(" error : Cannot initialize primary queue \n");
    exit( EXIT_FAILURE );
  }

  if( front != back ){
    i=1;
    fprintf(fo,"\n");
    fprintf(fo,"*** symmetric substructures *** \n");
    while( front != back || head != tail ){
      j=remove_second_queue();
      if( j == 3*MINUS ){
	orbit_number = MINUS;
	if( head != tail ){
	  add_primary_queue(j);
	}
      }
      if( orbit_number == MINUS ){
	while( head != tail ){
	  j=remove_primary_queue();
	  if( j == 3*MINUS ){
	    orbit_number = MINUS;
	    if( head != tail ){
	      add_primary_queue(j);
	      j=remove_primary_queue();
	    }else{
	      continue;
	    }
	  }
	  if( orbit_number == MINUS ){
	    fprintf(fo,"--------  %d\n",i);
	    i++;
	    orbit_number = findgroup(j,representative);
	    fprintf(fo,"%d,",j);
	    while(1){
	      j=remove_primary_queue();
	      if( j >= iatm ){
		fprintf(fo,"&nr,");
	      }else if( 0 <= j && j < iatm ){
		fprintf(fo,"%d,",j);
	      }else if( j == MINUS ){
		fprintf(fo,"\n");
	      }else if( j == 2*MINUS ){
		fprintf(fo,"----- \n");
		break;
	      }
	    }
	  }else{
	    if( orbit_number == findgroup(j,representative) ){
	      fprintf(fo,"%d,",j);
	      while(1){
		j=remove_primary_queue();
		if( j >= iatm ){
		  fprintf(fo,"&nr,");
		}else if( 0 <= j && j < iatm ){
		  fprintf(fo,"%d,",j);
		}else if( j == MINUS ){
		  fprintf(fo,"\n");
		}else if( j == 2*MINUS ){
		  fprintf(fo,"----- \n");
		  break;
		}
	      }
	    }else{
	      add_primary_queue(j);
	      while(1){
		j=remove_primary_queue();
		add_primary_queue(j);
		if( j == 2*MINUS ){
		  break;
		}
	      }
	    }
	  } // end of if( orbit_number == MINUS )
	} // end of while( head != tail )
      } // end of if ( orbit_number == MINUS )
      if( front != back ){
	if( orbit_number == MINUS ){
	  fprintf(fo,"-------- %d\n",i);
	  i++;
	  if( j == 3*MINUS ){
	    j = remove_second_queue();
	  }
	  orbit_number = findgroup(j,representative);
	  fprintf(fo,"%d,",j);
	  while(1){
	    j=remove_second_queue();
	    if( j >= iatm ){
	      fprintf(fo,"&nr,");
	    }else if( 0 <= j && j < iatm ){
	      fprintf(fo,"%d,",j);
	    }else if( j == MINUS ){
	      fprintf(fo,"\n");
	    }else if( j == 2*MINUS ){
	      fprintf(fo,"----- \n");
	      break;
	    }
	  }
	}else{
	  if( orbit_number == findgroup(j,representative) ||
             ( a1st[j].link == BRANCH || a1st[j].link == COLUMN || 
               a1st[j].link == LOOP || a1st[j].link == HITCH ) ){
	    fprintf(fo,"%d,",j);
	    while(1){
	      j=remove_second_queue();
	      if( j >= iatm ){
		fprintf(fo,"&nr,");
	      }else if( 0 <= j && j < iatm ){
		fprintf(fo,"%d,",j);
	      }else if( j == MINUS ){
		fprintf(fo,"\n");
	      }else if( j == 2*MINUS ){
		fprintf(fo,"----- \n");
		break;
	      }
	    }
	  }else{
	    add_primary_queue(j);
	    while(1){
	      j=remove_second_queue();
	      add_primary_queue(j);
	      if( j == 2*MINUS ){
		break;
	      }
	    }
	  }
	} // end of if( orbit_number == MINUS )
      } // end of if ( front != back )
    } // end of while( front != back || head != tail )
    fprintf(fo,"******************************* \n\n");
  }

  return EXIT_SUCCESS;

}
