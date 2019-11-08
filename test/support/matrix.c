#include "matrix.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**  
 * 关于以下旋转角度以顺时针为正角度
 * */

static void matrix_multiply(const float mat[4][4], float column_vector[4])
{
#define MARTIX_MULTIPLY(row) \
    mat[row][0] * column_vector[0] + \
    mat[row][1] * column_vector[1] + \
    mat[row][2] * column_vector[2] + \
    mat[row][3] * column_vector[3]

    float temp_vector[4] = {0};
    temp_vector[0] = MARTIX_MULTIPLY(0);
    temp_vector[1] = MARTIX_MULTIPLY(1);
    temp_vector[2] = MARTIX_MULTIPLY(2);
    temp_vector[3] = MARTIX_MULTIPLY(3);

    memcpy(column_vector, temp_vector, sizeof(float) * 4);
#undef MARTIX_MULTIPLY
}

void rotate_x(float pos_vector[4],float radian)
{
    /**
        1 , 0            , 0           , 0;
        0 , cos(radian)  , sin(radian) , 0;
        0 , -sin(radian) , cos(radian) , 0;
        0 , 0            , 0           , 1;
    */
    float mat[4][4] = {0};
    mat[0][0] = 1 ; mat[0][1] = 0            ; mat[0][2] = 0           ; mat[0][3] = 0 ;
    mat[1][0] = 0 ; mat[1][1] = cos(radian)  ; mat[1][2] = sin(radian) ; mat[1][3] = 0 ;
    mat[2][0] = 0 ; mat[2][1] = -sin(radian) ; mat[2][2] = cos(radian) ; mat[2][3] = 0 ;
    mat[3][0] = 0 ; mat[3][1] = 0            ; mat[3][2] = 0           ; mat[3][3] = 1 ;

    matrix_multiply(mat, pos_vector);
}

void rotate_y(float pos_vector[4],float radian)
{
    /** 
        cos(radian)  , 0 , sin(radian) , 0;
        0            , 1 , 0           , 0;
        -sin(radian) , 0 , cos(radian) , 0;
        0            , 0 , 0           , 1;
    */
    float mat[4][4] = {0};
    mat[0][0] = cos(radian)  ; mat[0][1] = 0 ; mat[0][2] = sin(radian) ; mat[0][3] = 0 ;
    mat[1][0] = 0            ; mat[1][1] = 1 ; mat[1][2] = 0           ; mat[1][3] = 0 ;
    mat[2][0] = -sin(radian) ; mat[2][1] = 0 ; mat[2][2] = cos(radian) ; mat[2][3] = 0 ;
    mat[3][0] = 0            ; mat[3][1] = 0 ; mat[3][2] = 0           ; mat[3][3] = 1 ;

    matrix_multiply(mat, pos_vector);
}
void rotate_z(float pos_vector[4],float radian)
{
    /** 
        cos(radian)  , sin(radian) , 0 , 0;
        -sin(radian) , cos(radian) , 0 , 0;
        0            , 0           , 1 , 0;
        0            , 0           , 0 , 1;
    */
    float mat[4][4] = {0};
    mat[0][0] = cos(radian)  ; mat[0][1] = sin(radian) ; mat[0][2] = 0 ; mat[0][3] = 0 ;
    mat[1][0] = -sin(radian) ; mat[1][1] = cos(radian) ; mat[1][2] = 0 ; mat[1][3] = 0 ;
    mat[2][0] = 0            ; mat[2][1] = 0           ; mat[2][2] = 1 ; mat[2][3] = 0 ;
    mat[3][0] = 0            ; mat[3][1] = 0           ; mat[3][2] = 0 ; mat[3][3] = 1 ;

    matrix_multiply(mat, pos_vector);
}

void rotate_s(float pos_vector[4], const float center_vector[3], void(*rotate)(float pos_vector[4], float radian), float radian)
{
    parallel(pos_vector, -center_vector[0], -center_vector[1], -center_vector[2]);

    rotate(pos_vector, radian);

    parallel(pos_vector,  center_vector[0],  center_vector[1],  center_vector[2]);
}

void parallel(float pos_vector[4], float delta_x, float delta_y, float delta_z)
{
    /** 
        1 , 0 , 0 , delta_x;
        0 , 1 , 0 , delta_y;
        0 , 0 , 1 , delta_z;
        0 , 0 , 0 , 1;
    */
    float mat[4][4] = {0};
    mat[0][0] = 1 ; mat[0][1] = 0 ; mat[0][2] = 0 ; mat[0][3] = delta_x ;
    mat[1][0] = 0 ; mat[1][1] = 1 ; mat[1][2] = 0 ; mat[1][3] = delta_y ;
    mat[2][0] = 0 ; mat[2][1] = 0 ; mat[2][2] = 1 ; mat[2][3] = delta_z ;
    mat[3][0] = 0 ; mat[3][1] = 0 ; mat[3][2] = 0 ; mat[3][3] = 1 ;

    matrix_multiply(mat, pos_vector);
}

