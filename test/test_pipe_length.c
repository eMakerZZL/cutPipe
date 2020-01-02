#include "pipe_length.h"
#include "unity.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum {
    VaildPos,
    InvaildPos,
    NeedFeedPos,
} pipe_state_type;

typedef struct _pipe_limits_param {
    unsigned int auto_feed_flg;
    float original_abs;
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

/**
 * @brief   init_pipe_limits 初始化管坐标参数
 */
static void init_pipe_limits(void)
{
    pipe_limits = (pipe_limits_param*)malloc(sizeof(pipe_limits_param));
    assert(pipe_limits);

    pipe_limits->reference_abs = sensorDistance;
    pipe_limits->original_abs  = pipe_limits->reference_abs - sensorDistance;

    pipe_limits->global_lower_bound_abs = pipe_limits->reference_abs;
    pipe_limits->global_upper_bound_abs = pipe_limits->global_lower_bound_abs + pipe_length - residue_length;

    pipe_limits->local_lower_bound_abs = pipe_limits->global_lower_bound_abs;
    pipe_limits->local_upper_bound_abs = pipe_limits->local_lower_bound_abs + max_axis_x_distance;
}

/**
 * @brief   updata_reference_pos 更新端面的绝对坐标值,此函数只能在切割指令中调用
 */
static void updata_reference_pos(float offset_rel)
{
    pipe_limits->reference_abs += offset_rel + cut_offset;

    pipe_limits->local_lower_bound_abs = pipe_limits->reference_abs;
}

static void updata_local_pos(void)
{
    pipe_limits->local_upper_bound_abs = pipe_limits->local_lower_bound_abs + max_axis_x_distance;
    if (pipe_limits->local_upper_bound_abs > pipe_limits->global_upper_bound_abs)
        pipe_limits->local_upper_bound_abs = pipe_limits->global_upper_bound_abs;

    pipe_limits->original_abs = pipe_limits->local_lower_bound_abs - sensorDistance;
}

static float pipe_feed_length(void)
{
    return max_axis_x_distance - (pipe_limits->local_upper_bound_abs - pipe_limits->local_lower_bound_abs);
}

static float convert_rel2abs(float offset_rel)
{
    return pipe_limits->original_abs + offset_rel;
}

static float convert_abs2rel(float pos_abs)
{
    return pos_abs - pipe_limits->original_abs;
}

static pipe_state_type has_enough_length(float work_pos_abs)
{
    if (work_pos_abs > pipe_limits->global_upper_bound_abs || work_pos_abs < pipe_limits->global_lower_bound_abs)
        return InvaildPos;

    if (pipe_limits->auto_feed_flg) {
        if (work_pos_abs > pipe_limits->local_upper_bound_abs)
            return NeedFeedPos;
    } else {
        if (work_pos_abs > pipe_limits->local_upper_bound_abs)
            return InvaildPos;
    }

    if (work_pos_abs < pipe_limits->local_lower_bound_abs)
        return InvaildPos;

    return VaildPos;
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

    TEST_ASSERT_EQUAL_FLOAT(45, pipe_limits->reference_abs);
}

void test_pipe_length_MeetCutOrderUpdataLocalBound(void)
{
    float cutPos = 30.0f;

    updata_reference_pos(cutPos);

    TEST_ASSERT_EQUAL_FLOAT(45, pipe_limits->local_lower_bound_abs);
    TEST_ASSERT_EQUAL_FLOAT(210, pipe_limits->local_upper_bound_abs);
}

void test_pipe_length_WorkPosGreatMaxBoundWasError(void)
{
    float workPos = pipe_limits->global_upper_bound_abs + 10.0f;

    TEST_ASSERT_EQUAL_UINT32(InvaildPos, has_enough_length(workPos));
}

void test_pipe_length_HasAutoFeedWorkPosGreatMaxAxisX(void)
{
    float workPos = pipe_limits->local_upper_bound_abs + 10.0f;

    pipe_limits->auto_feed_flg = true;

    TEST_ASSERT_EQUAL_UINT32(NeedFeedPos, has_enough_length(workPos));
}

void test_pipe_length_NotHasAutoFeedWorkPosGreatMaxAxisX(void)
{
    float workPos = pipe_limits->local_upper_bound_abs + 10.0f;

    pipe_limits->auto_feed_flg = false;

    TEST_ASSERT_EQUAL_UINT32(InvaildPos, has_enough_length(workPos));
}

void test_pipe_length_NotHasAutoFeedWorkPosInAxisX(void)
{
    float workPos = pipe_limits->local_lower_bound_abs + 10.0f;

    pipe_limits->auto_feed_flg = false;

    TEST_ASSERT_EQUAL_UINT32(VaildPos, has_enough_length(workPos));
}

void test_pipe_length_HasAutoFeedWorkPosInAxisX(void)
{
    float workPos = pipe_limits->local_lower_bound_abs + 10.0f;

    pipe_limits->auto_feed_flg = true;

    TEST_ASSERT_EQUAL_UINT32(VaildPos, has_enough_length(workPos));
}

void test_pipe_length_CutOrderNeedReferencePosAddCutOffset(void)
{
    float cutPos = 20.0f;
    float RealPos = 20.0f + pipe_limits->reference_abs + cut_offset;

    updata_reference_pos(cutPos);

    TEST_ASSERT_EQUAL_FLOAT(RealPos, pipe_limits->reference_abs);
}

void test_pipe_length_AfterAutoFeedLocalUpperBoundHasChange(void)
{
    float cutPos = 20.0f;
    float realPos = 235;

    updata_reference_pos(cutPos);
    TEST_ASSERT_EQUAL_FLOAT(25, pipe_feed_length());
    updata_local_pos();

    TEST_ASSERT_EQUAL_FLOAT(realPos, pipe_limits->local_upper_bound_abs);
}

void test_pipe_length_IfAufoFeedLengthLessThanMaxAixsLengthOnlyMoveVailLength(void)
{
    float cutPos = 900.0f;
    float realPos = 990.0f;

    updata_reference_pos(cutPos);
    updata_local_pos();

    TEST_ASSERT_EQUAL_FLOAT(realPos, pipe_limits->local_upper_bound_abs);
}

void test_pipe_length_AfterAutoFeedOriginalPosHasChange(void)
{
    float cutPos = 900.0f;
    float realPos = 915.0f - sensorDistance;

    updata_reference_pos(cutPos);
    updata_local_pos();

    TEST_ASSERT_EQUAL_FLOAT(realPos, pipe_limits->original_abs);
}

void test_pipe_length_covertrel2abs(void)
{
    float cutPos = 500.0f;
    float workPos = 180.0f;

    updata_reference_pos(cutPos);
    updata_local_pos();

    TEST_ASSERT_EQUAL_FLOAT(685, convert_rel2abs(workPos));
}

void test_pipe_length_covertabs2rel(void)
{
    float cutPos = 500.0f;

    updata_reference_pos(cutPos);
    updata_local_pos();

    TEST_ASSERT_EQUAL_FLOAT(180, convert_abs2rel(685));
}



