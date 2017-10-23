// calculate_dihedral_angle.c 二面角を計算

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main_canost.h"

// #define TEST_CALC_DIHEDRAL_ANGLE 1

extern int iatm;
extern struct atom *a1st;

double calculate_dihedral_angle( int alpha, int beta, int gamma, int epsilon ){

  double angle;
  double angle_degree;
  double direction;
  double rcos;

  double normal_vector1[3],normal_vector2[3];

#ifdef TEST_CALC_DIHEDRAL_ANGLE
  printf("calculate_dihedral_angle \n");
  printf("alpha %d beta %d gamma %d epsilon %d \n", alpha, beta, gamma, epsilon);
#endif

  // Plane 1
  // X coordinate
  normal_vector1[0] = (a1st[beta].yco - a1st[alpha].yco)*(a1st[gamma].zco - a1st[alpha].zco)
    -(a1st[beta].zco - a1st[alpha].zco)*(a1st[gamma].yco - a1st[alpha].yco);
  // Y coordinate
  normal_vector1[1] = (a1st[beta].zco - a1st[alpha].zco)*(a1st[gamma].xco - a1st[alpha].xco)
    -(a1st[beta].xco - a1st[alpha].xco)*(a1st[gamma].zco - a1st[alpha].zco);
  // Z coordinate
  normal_vector1[2] = (a1st[beta].xco - a1st[alpha].xco)*(a1st[gamma].yco - a1st[alpha].yco)
    -(a1st[beta].yco - a1st[alpha].yco)*(a1st[gamma].xco - a1st[alpha].xco);

  // Plane 2
  // X coordinate
  normal_vector2[0] = (a1st[gamma].yco - a1st[beta].yco)*(a1st[epsilon].zco - a1st[beta].zco)
    -(a1st[gamma].zco - a1st[beta].zco)*(a1st[epsilon].yco - a1st[beta].yco);
  // Y coordinate
  normal_vector2[1] = (a1st[gamma].zco - a1st[beta].zco)*(a1st[epsilon].xco - a1st[beta].xco)
    -(a1st[gamma].xco - a1st[beta].xco)*(a1st[epsilon].zco - a1st[beta].zco);
  // Z coordinate
  normal_vector2[2] = (a1st[gamma].xco - a1st[beta].xco)*(a1st[epsilon].yco - a1st[beta].yco)
    -(a1st[gamma].yco - a1st[beta].yco)*(a1st[epsilon].xco - a1st[beta].xco);

  // Calculate Direction of Plane 2 against Plane 1
  direction = normal_vector1[0]*(a1st[epsilon].xco - a1st[gamma].xco)
    +normal_vector1[1]*(a1st[epsilon].yco - a1st[gamma].yco)
    +normal_vector1[2]*(a1st[epsilon].zco - a1st[gamma].zco);

  // Plane 1 - Plane 2
  rcos = (normal_vector1[0]*normal_vector2[0])+(normal_vector1[1]*normal_vector2[1])+(normal_vector1[2]*normal_vector2[2]);

  rcos = rcos/sqrt((normal_vector1[0]*normal_vector1[0]+normal_vector1[1]*normal_vector1[1]+normal_vector1[2]*normal_vector1[2])
               *(normal_vector2[0]*normal_vector2[0]+normal_vector2[1]*normal_vector2[1]+normal_vector2[2]*normal_vector2[2]));

  if( 1.0 <= rcos ){
    angle = acos(0.0);
    // printf("angle = %.20lf \n",angle);
  }else if( -1.0 < rcos && rcos < 1.0 ){
    angle = acos(rcos);
    // printf("angle = %.20lf \n",angle);
  }else if( rcos <= -1.0 ){
    angle = acos(-1.0);
    // printf("angle = %.20lf \n",angle);
  }else{
    printf("error : Cannot calculate dihedral angle \n");
    exit( EXIT_FAILURE );
  }

  angle_degree = (angle*180.0)/acos(-1.0);

  if( direction < 0 ){
    angle_degree = 360 - angle_degree;
  }

#ifdef TEST_CALC_DIHEDRAL_ANGLE
  printf("angle = %lf \n\n",angle_degree);
#endif

  return angle_degree;

}
