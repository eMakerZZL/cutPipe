#include "config.h"
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

    cut_trail = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * VECTOR_DIMENSION, sizeof(float));
    assert(cut_trail);

    guideLine_arc_in = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * VECTOR_DIMENSION, sizeof(float));
    assert(guideLine_arc_in);

    guideLine_arc_out = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * VECTOR_DIMENSION, sizeof(float));
    assert(guideLine_arc_out);

    guideLine_line_in = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * VECTOR_DIMENSION, sizeof(float));
    assert(guideLine_line_in);

    guideLine_line_out = (float*)calloc(laserCutPipePara->circle_pipe_param.segment * VECTOR_DIMENSION, sizeof(float));
    assert(guideLine_line_out);

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

float* GuideLine_Arc(float radius_mm, float arc_length_mm, int guideLine_Type)
{
}

float* GuideLine_Line(float angle_deg, float line_length_mm, int guideLine_Type)
{
}
