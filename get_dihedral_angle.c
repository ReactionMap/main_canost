// get_dihedral_angle.c 二面角に基づくコードを計算

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_canost.h"

int get_dihedral_angle( int alpha, int beta, int gamma, int epsilon ){

  int angle_code; // 二面角に基づくコード

  double angle;

  angle = calculate_dihedral_angle(alpha,beta,gamma,epsilon);

  if( ( angle >= 0 && angle <= 20 ) || ( angle >= 340 && angle <= 360 ) ){
    angle_code = 0 ;
  }else if( angle > 20 && angle <= 40 ){
    angle_code = 1;
  }else if( angle > 40 && angle <= 80 ){
    angle_code = 2;
  }else if( angle > 80 && angle <= 100 ){
    angle_code = 3;
  }else if( angle > 100 && angle <= 140 ){
    angle_code = 4;
  }else if( angle > 140 && angle <= 160 ){
    angle_code = 5;
  }else if( angle > 160 && angle <= 200 ){
    angle_code = 6;
  }else if( angle > 200 && angle <= 220 ){
    angle_code = 7;
  }else if( angle > 220 && angle <= 260 ){
    angle_code = 8;
  }else if( angle > 260 && angle <= 280 ){
    angle_code = 9;
  }else if( angle > 280 && angle <= 320 ){
    angle_code = 10;
  }else if( angle > 320 && angle <= 340 ){
    angle_code = 11 ;
  }else{
    printf("Error : Cannot assign dihedral code \n");
    exit( EXIT_FAILURE );
  }

  return angle_code;

}
