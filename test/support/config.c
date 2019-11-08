#include "config.h"
#include "matrix.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

static LaserCutPipePara* laserCutPipePara = NULL;
static float* laser_trail = NULL;
static float* cut_trail = NULL;
static float* guideLine_arc_in = NULL;
static float* guideLine_arc_out = NULL;
static float* guideLine_line_in = NULL;
static float* guideLine_line_out = NULL;

/**
 * @brief   LaserCutPipe_SetCirclePipeParam 设置母管参数
 *
 * @param   pipe_radius_mm 母管半径
 * @param   unit_arc_length_mm 单位弧长,用于设置数据间隔
 */
static void LaserCutPipe_SetCirclePipeParam(float pipe_radius_mm, float unit_arc_length_mm)
{
    assert(laserCutPipePara);

    laserCutPipePara->circle_pipe_param.radius = pipe_radius_mm;
    laserCutPipePara->circle_pipe_param.unit_arc_length = unit_arc_length_mm >= UNIT_ARC_LENGTH ? unit_arc_length_mm : UNIT_ARC_LENGTH;
    laserCutPipePara->circle_pipe_param.unit_radian = laserCutPipePara->circle_pipe_param.unit_arc_length / laserCutPipePara->circle_pipe_param.radius;


    /* TODO:  可修改成linsapce形式,同时计算控制卡能够识别的直线最短距离<08-11-19, yourname> */
    float count = 2 * PI / laserCutPipePara->circle_pipe_param.unit_radian;
    int icount = (int)(count + 0.5);
    laserCutPipePara->circle_pipe_param.segment = icount;
}

/**
 * @brief   LaserCutPipe_SetLaserPosition 设置激光器参数
 */
static void LaserCutPipe_SetLaserPosition()
{
    assert(laserCutPipePara);

    laserCutPipePara->laser_angle_x = 0;
    laserCutPipePara->laser_angle_y = 0;
    laserCutPipePara->laser_angle_z = 0;
    laserCutPipePara->laser_height = 5.0f;
    laserCutPipePara->laser_offset_center_distance = 0;
}

LaserCutPipePara* init_LaserCutPipePara(void)
{
    laserCutPipePara = (LaserCutPipePara*)calloc(1, sizeof(LaserCutPipePara));
    assert(laserCutPipePara);

    laserCutPipePara->pipe = circlePipe;
    laserCutPipePara->pipe_length = 20.0f;
    LaserCutPipe_SetCirclePipeParam(10.0f, 0.1f);
    LaserCutPipe_SetLaserPosition();

    laser_trail = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * MATRIX_DIMENSION, sizeof(float));
    assert(laser_trail);

    guideLine_arc_in = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * MATRIX_DIMENSION, sizeof(float));
    assert(guideLine_arc_in);

    guideLine_arc_out = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * MATRIX_DIMENSION, sizeof(float));
    assert(guideLine_arc_out);

    guideLine_line_in = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * MATRIX_DIMENSION, sizeof(float));
    assert(guideLine_line_in);

    guideLine_line_out = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * MATRIX_DIMENSION, sizeof(float));
    assert(guideLine_line_out);

    cut_trail = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * VECTOR_DIMENSION, sizeof(float));
    assert(cut_trail);

    return laserCutPipePara;
}

void destory_LaserCutPipePara(void)
{
    assert(laserCutPipePara);
    free(laserCutPipePara);
    laserCutPipePara = NULL;

    assert(laser_trail);
    free(laser_trail);
    laser_trail = NULL;

    assert(cut_trail);
    free(cut_trail);
    cut_trail = NULL;

    assert(guideLine_arc_in);
    free(guideLine_arc_in);
    guideLine_arc_in = NULL;

    assert(guideLine_arc_out);
    free(guideLine_arc_out);
    guideLine_arc_out = NULL;

    assert(guideLine_line_in);
    free(guideLine_line_in);
    guideLine_line_in = NULL;

    assert(guideLine_line_out);
    free(guideLine_line_out);
    guideLine_line_out = NULL;
}

float* get_GuideLineArcIn_addr(void)
{
    assert(guideLine_arc_in);
    return guideLine_arc_in;
}

float* get_GuideLineArcOut_addr(void)
{
    assert(guideLine_arc_out);
    return guideLine_arc_out;
}

/**
 * @brief   CirclePipe_GenerateLaserTrail 生成激光器运行路径
 *
 * @param   center 圆心位置
 * @param   radius_mm 半径
 *
 * @return  
 * @author  ZhangZL
 * @data    2019-11-05
 */
float* CirclePipe_GenerateLaserTrail(float center, float radius_mm)
{
    assert(laserCutPipePara);
    assert(laser_trail);
    assert(cut_trail);

    if (radius_mm > laserCutPipePara->circle_pipe_param.radius)
        return NULL;

    if (radius_mm <= 0)
        return NULL;

    float laser_pos_x = center;
    float laser_pos_y = laserCutPipePara->laser_offset_center_distance;
    float laser_pos_z = laserCutPipePara->circle_pipe_param.radius + laserCutPipePara->laser_height;

    laserCutPipePara->circle_pipe_param.center_x = laser_pos_x;
    laserCutPipePara->circle_pipe_param.center_y = laser_pos_y;
    laserCutPipePara->circle_pipe_param.center_z = laser_pos_z;

    laserCutPipePara->start_point_x = laser_pos_x + radius_mm;
    laserCutPipePara->start_point_y = laser_pos_y;
    laserCutPipePara->start_point_z = laser_pos_z;

    float theta = 0;

    for (int i = 0; i < laserCutPipePara->circle_pipe_param.segment; i++) {
        *(laser_trail + i * MATRIX_DIMENSION + 0) = laser_pos_x + radius_mm * cos(theta);
        *(laser_trail + i * MATRIX_DIMENSION + 1) = laser_pos_y + radius_mm * sin(theta);
        *(laser_trail + i * MATRIX_DIMENSION + 2) = laser_pos_z;
        *(laser_trail + i * MATRIX_DIMENSION + 3) = 1;
        theta += laserCutPipePara->circle_pipe_param.unit_radian;
    }

    return laser_trail;
}

float* CirclePipe_GenerateCutTrail(void)
{
    assert(laserCutPipePara);
    assert(laser_trail);
    assert(cut_trail);

    for (int i = 0; i < laserCutPipePara->circle_pipe_param.segment; i++) {
        *(cut_trail + i * VECTOR_DIMENSION + 0) = *(laser_trail + i * MATRIX_DIMENSION + 0);
        *(cut_trail + i * VECTOR_DIMENSION + 1) = *(laser_trail + i * MATRIX_DIMENSION + 1);
        *(cut_trail + i * VECTOR_DIMENSION + 2) = sqrt(pow(laserCutPipePara->circle_pipe_param.radius, 2) - pow(*(cut_trail + 3 * i + 1), 2));
    }

    return cut_trail;
}

float* CirclePipe_ConvertAixsYToAngle(void)
{
    assert(laserCutPipePara);
    assert(laser_trail);
    assert(cut_trail);

    float y, radian, degree;
    for (int i = 0; i < laserCutPipePara->circle_pipe_param.segment; i++) {
        y = *(cut_trail + i * VECTOR_DIMENSION + 1);
        /**
         * @brief   不可使用atan求值,否则会造成角度突变
         */
        radian = acos(y / laserCutPipePara->circle_pipe_param.radius);
        degree = radian * 180 / PI;
        *(cut_trail + i * VECTOR_DIMENSION + 1) = degree;
    }

    return cut_trail;
}

float* CirclePipe_RestoreAxisZCoordVal(void)
{
    for (int i = 0; i < laserCutPipePara->circle_pipe_param.segment; i++) {
        *(cut_trail + i * VECTOR_DIMENSION + 2) = laserCutPipePara->circle_pipe_param.radius + laserCutPipePara->laser_height;
    }

    return cut_trail;
}

static void GuideLine_arc_rotate(float pos_vector[4], const float center_vector[3], const float guide_arc_radius_mm)
{
    rotate_s(pos_vector, center_vector, rotate_y, PI);

    pos_vector[0] += 2 * guide_arc_radius_mm;
}

float* GuideLine_GenerateArc(float arc_radius_mm, float arc_length_mm, int guideLine_Type)
{
#define COORDINATE_VALUE_ASSIGNMENT(guide_line_arc) \
        *(guide_line_arc + i * MATRIX_DIMENSION + 0) = guide_line_center_x + arc_radius_mm * cos(theta);\
        *(guide_line_arc + i * MATRIX_DIMENSION + 1) = guide_line_center_y + arc_radius_mm * sin(theta);\
        *(guide_line_arc + i * MATRIX_DIMENSION + 2) = guide_line_center_z;\
        *(guide_line_arc + i * MATRIX_DIMENSION + 3) = 1

    float guide_line_center_x = laserCutPipePara->start_point_x - arc_radius_mm;
    float guide_line_center_y = laserCutPipePara->start_point_y;
    float guide_line_center_z = laserCutPipePara->start_point_z;
    float guide_line_radian = arc_length_mm / arc_radius_mm;
    float guide_line_center_vector[3] = { guide_line_center_x, guide_line_center_y, guide_line_center_z };
    float end_theta = 2 * PI - guide_line_radian;
    float theta;
    int i;

    /**
     * @brief   引入线方向theta --> 0 逆时针
     */
    theta = guide_line_radian;
    i = 0;
    while (theta >= 0) {
        COORDINATE_VALUE_ASSIGNMENT(guideLine_arc_in);
        theta -= laserCutPipePara->circle_pipe_param.unit_radian;
        i++;
    }
    if (theta + laserCutPipePara->circle_pipe_param.unit_radian > 0)
    {
        theta = 0;
        COORDINATE_VALUE_ASSIGNMENT(guideLine_arc_in);
        i++;
    }
    if (guideLine_Type == GUIDELINE_TYPE_OUTTER) {
        for (int j = 0; j < i; j++)
            GuideLine_arc_rotate(guideLine_arc_in + j * MATRIX_DIMENSION, guide_line_center_vector, arc_radius_mm);
    }

    /**
     * @brief   引出线方向theta --> 0 顺时针
     */
    theta = 2 * PI;
    i = 0;
    while (theta >= end_theta) {
        COORDINATE_VALUE_ASSIGNMENT(guideLine_arc_out);
        theta -= laserCutPipePara->circle_pipe_param.unit_radian;
        i++;
    }
    if (theta + laserCutPipePara->circle_pipe_param.unit_radian > end_theta)
    {
        theta = end_theta;
        COORDINATE_VALUE_ASSIGNMENT(guideLine_arc_out);
        i++;
    }
    if (guideLine_Type == GUIDELINE_TYPE_OUTTER) {
        for (int j = 0; j < i; j++)
            GuideLine_arc_rotate(guideLine_arc_out + j * MATRIX_DIMENSION, guide_line_center_vector, arc_radius_mm);
    }
#undef COORDINATE_VALUE_ASSIGNMENT
}

float* GuideLine_GenerateLine(float angle_deg, float line_length_mm, int guideLine_Type)
{
    float guide_line_center_x = laserCutPipePara->start_point_x;
    float guide_line_center_y = laserCutPipePara->start_point_y;
    float guide_line_center_z = laserCutPipePara->start_point_z;
}
