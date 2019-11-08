#include "circlePipe.h"
#include "config.h"
#include "matrix.h"
#include "data.h"
#include "unity.h"
#include <stdlib.h>

#define MAXMUL_DELTA (0.001)
#define MAXMUL_DEGREE_DELTA (0.01)

static LaserCutPipePara* laserParam = NULL;

void setUp(void)
{
    laserParam = init_LaserCutPipePara();
}

void tearDown(void)
{
    destory_LaserCutPipePara();
    laserParam = NULL;
}

void test_circlePipe_laserUnitRadWasOk(void)
{
    float count = 2 * PI / laserParam->circle_pipe_param.unit_radian;

    TEST_ASSERT_EQUAL_FLOAT(0.01, laserParam->circle_pipe_param.unit_radian);
    TEST_ASSERT_EQUAL_FLOAT(628, (int)(count + 0.5));
}

void test_circlePipe_laserTrailLessThanPipeRadius(void)
{
    float* laser_trail = CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius / 2);
    TEST_ASSERT_NOT_NULL(laser_trail);

    for (int i = 0; i < laserParam->circle_pipe_param.segment; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, laser_trail_buf_less[i * VECTOR_DIMENSION + 0], *(laser_trail + i * MATRIX_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, laser_trail_buf_less[i * VECTOR_DIMENSION + 1], *(laser_trail + i * MATRIX_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, laser_trail_buf_less[i * VECTOR_DIMENSION + 2], *(laser_trail + i * MATRIX_DIMENSION + 2));
    }
}

void test_circlePipe_laserTrailEqualToPipeRadius(void)
{
    float* laser_trail = CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius);
    TEST_ASSERT_NOT_NULL(laser_trail);

    for (int i = 0; i < laserParam->circle_pipe_param.segment; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, laser_trail_buf_eq[i * VECTOR_DIMENSION + 0], *(laser_trail + i * MATRIX_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, laser_trail_buf_eq[i * VECTOR_DIMENSION + 1], *(laser_trail + i * MATRIX_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, laser_trail_buf_eq[i * VECTOR_DIMENSION + 2], *(laser_trail + i * MATRIX_DIMENSION + 2));
    }
}

void test_circlePipe_laserTrailGreatThanPipeRadiusWasNotAllow(void)
{
    float* laser_trail = CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius * 2);

    TEST_ASSERT_NULL(laser_trail);
}

/** void test_circlePipe_laserTrailCircleCenterOverWorkingPositionNotAllow(void) */
/** { */
/** float* laser_trail = CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius / 2); */
/**  */
/** TEST_ASSERT_NULL(laser_trail); */
/** } */

void test_circlePipe_laserTrailCircleRadiusLessThanOrEqualZeroNotAllow(void)
{
    float* laser_trail = CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, 0);

    TEST_ASSERT_NULL(laser_trail);
}

void test_circlePipe_PipeTrailCircleLessThanPipeRadius(void)
{
    float* laser_trail = CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius / 2);
    float* pipe_trail = CirclePipe_GenerateCutTrail();

    for (int i = 0; i < laserParam->circle_pipe_param.segment; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, pipe_trail_buf_less[i * VECTOR_DIMENSION + 0], *(pipe_trail + i * VECTOR_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, pipe_trail_buf_less[i * VECTOR_DIMENSION + 1], *(pipe_trail + i * VECTOR_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, pipe_trail_buf_less[i * VECTOR_DIMENSION + 2], *(pipe_trail + i * VECTOR_DIMENSION + 2));
    }
}

void test_circlePipe_PipeTrailCircleEqualToPipeRadius(void)
{
    float* laser_trail = CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius);
    float* pipe_trail = CirclePipe_GenerateCutTrail();

    for (int i = 0; i < laserParam->circle_pipe_param.segment; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, pipe_trail_buf_eq[i * VECTOR_DIMENSION + 0], *(pipe_trail + i * VECTOR_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, pipe_trail_buf_eq[i * VECTOR_DIMENSION + 1], *(pipe_trail + i * VECTOR_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, pipe_trail_buf_eq[i * VECTOR_DIMENSION + 2], *(pipe_trail + i * VECTOR_DIMENSION + 2));
    }
}

void test_circlePipe_LaserRadiusLessThanPipeRadius_ConverAngleOk(void)
{
    CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius / 2);
    CirclePipe_GenerateCutTrail();
    float* pipe_trail = CirclePipe_ConvertAixsYToAngle();

    for (int i = 0; i < laserParam->circle_pipe_param.segment; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DEGREE_DELTA, pipe_trail_angle_buf_less[i * VECTOR_DIMENSION + 0], *(pipe_trail + i * VECTOR_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DEGREE_DELTA, pipe_trail_angle_buf_less[i * VECTOR_DIMENSION + 1], *(pipe_trail + i * VECTOR_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DEGREE_DELTA, pipe_trail_angle_buf_less[i * VECTOR_DIMENSION + 2], *(pipe_trail + i * VECTOR_DIMENSION + 2));
    }
}

void test_circlePipe_LaserRadiusEqualToPipeRadius_ConverAngleOk(void)
{
    CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius);
    CirclePipe_GenerateCutTrail();
    float* pipe_trail = CirclePipe_ConvertAixsYToAngle();

    for (int i = 0; i < laserParam->circle_pipe_param.segment; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DEGREE_DELTA, pipe_trail_angle_buf_great[i * VECTOR_DIMENSION + 0], *(pipe_trail + i * VECTOR_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DEGREE_DELTA, pipe_trail_angle_buf_great[i * VECTOR_DIMENSION + 1], *(pipe_trail + i * VECTOR_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DEGREE_DELTA, pipe_trail_angle_buf_great[i * VECTOR_DIMENSION + 2], *(pipe_trail + i * VECTOR_DIMENSION + 2));
    }
}

void test_circlePipe_RestroeAxisZValueOk(void)
{
    CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius);
    CirclePipe_GenerateCutTrail();
    CirclePipe_ConvertAixsYToAngle();
    float* pipe_trail = CirclePipe_RestoreAxisZCoordVal();

    for (int i = 0; i < laserParam->circle_pipe_param.segment; i++) {
        TEST_ASSERT_EQUAL_FLOAT(laserParam->circle_pipe_param.radius + laserParam->laser_height, *(pipe_trail + i * VECTOR_DIMENSION + 2));
    }
}
