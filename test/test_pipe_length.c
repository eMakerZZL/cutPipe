#include "unity.h"
#include "pipe_length.h"
#include <stdlib.h>
#include <assert.h>

typedef struct _pipe_limits_param {
    float reference_abs;
    float lower_bound_abs;
    float upper_bound_abs;
    float working_bound_abs;
} pipe_limits_param;

static pipe_limits_param* pipe_limits = NULL;
static const float max_axis_x_distance = 200.0f;
static const float pipe_length = 1000.0f;
static const float sensorDistance = 10.0f;
static const float residue_length = 20.0f;

static void updata_reference_pos(float offset)
{
    pipe_limits->reference_abs += offset;
}

void setUp(void)
{
    pipe_limits = (pipe_limits_param*)malloc(sizeof(pipe_limits_param));
    assert(pipe_limits);

    pipe_limits->reference_abs     = sensorDistance;
    pipe_limits->lower_bound_abs   = pipe_limits->reference_abs;
    pipe_limits->upper_bound_abs   = pipe_limits->lower_bound_abs + pipe_length - residue_length;
    pipe_limits->working_bound_abs = pipe_limits->lower_bound_abs + max_axis_x_distance;
}

void tearDown(void)
{
    assert(pipe_limits);

    free(pipe_limits);
    pipe_limits = NULL;
}

void test_pipe_length_MeetCutOrderUpdataReferencePos(void)
{
    float cutPos = 30.0f;

    updata_reference_pos(cutPos);

    TEST_ASSERT_EQUAL_FLOAT(40, pipe_limits->reference_abs);
}

void test_pipe_length_MeetCutOrderUpdataReferencePos(void)
{

}
