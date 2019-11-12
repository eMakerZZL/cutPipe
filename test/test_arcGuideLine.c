#include "arcGuideLine.h"
#include "assert.h"
#include "circlePipe.h"
#include "config.h"
#include "data.h"
#include "matrix.h"
#include "unity.h"
#include <stdio.h>

#define MAXMUL_DELTA (0.001)

static LaserCutPipePara* laserParam = NULL;
static float* guideLine_arc_in = NULL;
static float* guideLine_arc_out = NULL;
static float* guideLine_line_in = NULL;
static float* guideLine_line_out = NULL;
static float* guideLine_arc_in_cut_trail = NULL;
static float* guideLine_arc_out_cut_trail = NULL;
static float* guideLine_line_in_cut_trail = NULL;
static float* guideLine_line_out_cut_trail = NULL;

void setUp(void)
{
    laserParam = init_LaserCutPipePara();
    assert(laserParam);

    CirclePipe_GenerateLaserTrail(laserParam->pipe_length / 2, laserParam->circle_pipe_param.radius / 2);
    guideLine_arc_in = get_GuideLineArcIn_addr();
    guideLine_arc_out = get_GuideLineArcOut_addr();

    guideLine_arc_in_cut_trail = get_GuideLineArcIn_CutTrail_addr();
    guideLine_arc_out_cut_trail = get_GuideLineArcOut_CutTrail_addr();
}

void tearDown(void)
{
    destory_LaserCutPipePara();
    laserParam = NULL;
    guideLine_arc_in = NULL;
    guideLine_arc_out = NULL;
    guideLine_line_in = NULL;
    guideLine_line_out = NULL;
    guideLine_arc_in_cut_trail = NULL;
    guideLine_arc_out_cut_trail = NULL;
    guideLine_line_in_cut_trail = NULL;
    guideLine_line_out_cut_trail = NULL;
}

void test_arcGuideLine_checkGuideLineArcInOk(void)
{
    GuideLine_GenerateArc(laserParam->circle_pipe_param.radius / 4, 5, GUIDELINE_TYPE_INNER);

    for (int i = 0; i < 201; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_in_buf[i * VECTOR_DIMENSION + 0], *(guideLine_arc_in + i * MATRIX_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_in_buf[i * VECTOR_DIMENSION + 1], *(guideLine_arc_in + i * MATRIX_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_in_buf[i * VECTOR_DIMENSION + 2], *(guideLine_arc_in + i * MATRIX_DIMENSION + 2));
    }
}

void test_arcGuideLine_checkGuideLineArcOutOk(void)
{
    GuideLine_GenerateArc(laserParam->circle_pipe_param.radius / 4, 5, GUIDELINE_TYPE_INNER);

    for (int i = 0; i < 201; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_out_buf[i * VECTOR_DIMENSION + 0], *(guideLine_arc_out + i * MATRIX_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_out_buf[i * VECTOR_DIMENSION + 1], *(guideLine_arc_out + i * MATRIX_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_out_buf[i * VECTOR_DIMENSION + 2], *(guideLine_arc_out + i * MATRIX_DIMENSION + 2));
    }

}

void test_arcGuideLine_checkOutterGuideLineArcInOk(void)
{
    GuideLine_GenerateArc(laserParam->circle_pipe_param.radius / 4, 5, GUIDELINE_TYPE_OUTTER);

    for (int i = 0; i < 201; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_in_buf_outter[i * VECTOR_DIMENSION + 0], *(guideLine_arc_in + i * MATRIX_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_in_buf_outter[i * VECTOR_DIMENSION + 1], *(guideLine_arc_in + i * MATRIX_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_in_buf_outter[i * VECTOR_DIMENSION + 2], *(guideLine_arc_in + i * MATRIX_DIMENSION + 2));
    }
}

void test_arcGuideLine_checkOutterGuideLineArcOutOk(void)
{
    GuideLine_GenerateArc(laserParam->circle_pipe_param.radius / 4, 5, GUIDELINE_TYPE_OUTTER);

    for (int i = 0; i < 201; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_out_buf_outter[i * VECTOR_DIMENSION + 0], *(guideLine_arc_out + i * MATRIX_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_out_buf_outter[i * VECTOR_DIMENSION + 1], *(guideLine_arc_out + i * MATRIX_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_out_buf_outter[i * VECTOR_DIMENSION + 2], *(guideLine_arc_out + i * MATRIX_DIMENSION + 2));
    }
}

void test_arcGuideLine_checkGuideLineArcInOutterCutTrailOk(void)
{
    GuideLine_GenerateArc(laserParam->circle_pipe_param.radius / 4, 5, GUIDELINE_TYPE_OUTTER);
    GuideLine_GenerateCutTrail();

    for (int i = 0; i < 201; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_in_cut_buf_outter[i * VECTOR_DIMENSION + 0], *(guideLine_arc_in_cut_trail + i * VECTOR_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_in_cut_buf_outter[i * VECTOR_DIMENSION + 1], *(guideLine_arc_in_cut_trail + i * VECTOR_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_in_cut_buf_outter[i * VECTOR_DIMENSION + 2], *(guideLine_arc_in_cut_trail + i * VECTOR_DIMENSION + 2));
    }
}

void test_arcGuideLine_checkGuideLineArcOutOutterCutTrailOk(void)
{
    GuideLine_GenerateArc(laserParam->circle_pipe_param.radius / 4, 5, GUIDELINE_TYPE_OUTTER);
    GuideLine_GenerateCutTrail();

    for (int i = 0; i < 201; i++) {
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_out_cut_buf_outter[i * VECTOR_DIMENSION + 0], *(guideLine_arc_out_cut_trail + i * VECTOR_DIMENSION + 0));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_out_cut_buf_outter[i * VECTOR_DIMENSION + 1], *(guideLine_arc_out_cut_trail + i * VECTOR_DIMENSION + 1));
        TEST_ASSERT_FLOAT_WITHIN(MAXMUL_DELTA, guilde_line_arc_out_cut_buf_outter[i * VECTOR_DIMENSION + 2], *(guideLine_arc_out_cut_trail + i * VECTOR_DIMENSION + 2));
    }
}
