#include "pipe_length.h"
#include "unity.h"
#include <assert.h>
#include <stdlib.h>

typedef struct _pipe_limits_param {
    float reference_abs;
    float global_lower_bound_abs;
    float global_upper_bound_abs;
    float local_lower_bound_abs;
    float local_upper_bound_abs;
} pipe_limits_param;

static pipe_limits_param* pipe_limits = NULL;
static const float max_axis_x_distance = 200.0f;
static const float pipe_length = 1000.0f;
static const float sensorDistance = 10.0f;
static const float residue_length = 20.0f;
static const float cut_offset = 5.0f;

static void init_pipe_limits(void)
{
    pipe_limits = (pipe_limits_param*)malloc(sizeof(pipe_limits_param));
    assert(pipe_limits);

    pipe_limits->reference_abs = sensorDistance;

    pipe_limits->global_lower_bound_abs = pipe_limits->reference_abs;
    pipe_limits->global_upper_bound_abs = pipe_limits->global_lower_bound_abs + pipe_length - residue_length;

    pipe_limits->local_lower_bound_abs = pipe_limits->global_lower_bound_abs;
    pipe_limits->local_upper_bound_abs = pipe_limits->local_lower_bound_abs + max_axis_x_distance;
}

static void updata_reference_pos(float offset)
{
    pipe_limits->reference_abs += offset;

    pipe_limits->local_lower_bound_abs = pipe_limits->reference_abs;
    pipe_limits->local_upper_bound_abs = pipe_limits->local_lower_bound_abs + max_axis_x_distance;
}

static unsigned int has_enough_length(float work_pos)
{
    return work_pos > pipe_limits->local_upper_bound_abs ? 1 : 0;
}

void setUp(void)
{
    init_pipe_limits();
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

void test_pipe_length_MeetCutOrderUpdataLocalBound(void)
{
    float cutPos = 30.0f;

    updata_reference_pos(cutPos);

    TEST_ASSERT_EQUAL_FLOAT(40, pipe_limits->local_lower_bound_abs);
    TEST_ASSERT_EQUAL_FLOAT(40 + max_axis_x_distance, pipe_limits->local_upper_bound_abs);
}

void test_pipe_length_CutPosGreatXPosFinishWork(void)
{
    float cutPos = 300;

    TEST_ASSERT_TRUE(has_enough_length(cutPos));
}
