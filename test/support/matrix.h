#ifndef MATRIX_H
#define MATRIX_H

void rotate_x(float pos_vector[4], float radian);
void rotate_y(float pos_vector[4], float radian);
void rotate_z(float pos_vector[4], float radian);
void rotate_s(float pos_vector[4], const float center_vector[3], void(*rotate)(float pos_vector[4], float radian), float radian);
void parallel(float pos_vector[4], float delta_x, float delta_y, float delta_z);

#endif /* end of include guard: MATRIX_H */

