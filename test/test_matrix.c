#include "config.h"
#include "matrix.h"
#include "unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_matrix_parallel_ori2des(void)
{
    float pos_vector[4] = { 0, 0, 0, 1 };
    float delta_x = 10;
    float delta_y = 20;
    float delta_z = 30;
    float des_vector[4] = { 10, 20, 30, 1 };
    parallel(pos_vector, delta_x, delta_y, delta_z);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);
}

void test_matrix_parallel_pos2des(void)
{
    float pos_vector[4] = { 100, 200, 300, 1 };
    float delta_x = 10;
    float delta_y = 20;
    float delta_z = 30;
    float des_vector[4] = { 110, 220, 330, 1 };
    parallel(pos_vector, delta_x, delta_y, delta_z);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);
}

void test_matrix_rotatex_ori2des(void)
{
    float pos_vector[4] = { 0, 0, 0, 1 };
    float des_vector[4] = { 0, 0, 0, 1 };

    rotate_x(pos_vector, 0);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_x(pos_vector, PI / 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_x(pos_vector, PI);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_x(pos_vector, 1.5 * PI);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_x(pos_vector, 2 * PI);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);
}

void test_matrix_rotatey_ori2des(void)
{
    float pos_vector[4] = { 0, 0, 0, 1 };
    float des_vector[4] = { 0, 0, 0, 1 };

    rotate_y(pos_vector, 0);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_y(pos_vector, PI / 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_y(pos_vector, PI);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_y(pos_vector, 1.5 * PI);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_y(pos_vector, 2 * PI);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);
}

void test_matrix_rotatez_ori2des(void)
{
    float pos_vector[4] = { 0, 0, 0, 1 };
    float des_vector[4] = { 0, 0, 0, 1 };

    rotate_z(pos_vector, 0);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_z(pos_vector, PI / 2);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_z(pos_vector, PI);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_z(pos_vector, 1.5 * PI);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);

    rotate_z(pos_vector, 2 * PI);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);
}

void test_matrix_rotatex_pos2des(void)
{
    float pos_vector[4] = { 10, 10, 10, 1 };
    float des_vector[4] = { 10, 13.6602540378444, 3.66025403784439, 1 };

    rotate_x(pos_vector, PI / 6);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);
}

void test_matrix_rotatey_pos2des(void) 
{
    float pos_vector[4] = { 10, 10, 10, 1 };
    float des_vector[4] = { 13.6602540378444, 10, 3.66025403784439, 1 };

    rotate_y(pos_vector, PI / 6);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);
}

void test_matrix_rotatez_pos2des(void) 
{
    float pos_vector[4] = { 10, 10, 10, 1 };
    float des_vector[4] = { 13.6602540378444, 3.66025403784439, 10, 1 };

    rotate_z(pos_vector, PI / 6);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);
}

void test_matrix_rotates_pos2des(void)
{
    float pos_vector[4] = { 10, 10, 10, 1 };
    float des_vector[4] = { 10, 10, 10, 1 };
    float center_vector[3] = {10, 10, 10};

    rotate_s(pos_vector, center_vector, PI / 6, rotate_x);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(des_vector, pos_vector, 4);
}
