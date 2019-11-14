#include "config.h"
#include "matrix.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#define TRAIL_DATA_SIZE (100 * 1024 * 1024)

static LaserCutPipePara* laserCutPipePara = NULL;

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
    laserCutPipePara->unit_arc_length = unit_arc_length_mm >= UNIT_ARC_LENGTH ? unit_arc_length_mm : UNIT_ARC_LENGTH;
    laserCutPipePara->unit_radian = laserCutPipePara->unit_arc_length / laserCutPipePara->circle_pipe_param.radius;

    float count = 2 * PI / laserCutPipePara->unit_radian;
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

    laserCutPipePara->pipe_type = circle_pipe;
    laserCutPipePara->guide_line_type = arc_guide_line;
    laserCutPipePara->pipe_length = 20.0f;
    LaserCutPipe_SetCirclePipeParam(10.0f, 0.1f);
    LaserCutPipe_SetLaserPosition();

    laserCutPipePara->unit_line_length = 0.1;

    laserCutPipePara->base_addr = (float*)calloc(1, sizeof(char) * TRAIL_DATA_SIZE);
    assert(laserCutPipePara->base_addr);

    laserCutPipePara->write_addr = laserCutPipePara->base_addr;

    return laserCutPipePara;
}

void destory_LaserCutPipePara(void)
{
    assert(laserCutPipePara);
    assert(laserCutPipePara->base_addr);

    free(laserCutPipePara->base_addr);
    laserCutPipePara->base_addr = NULL;
    laserCutPipePara->write_addr = NULL;

    free(laserCutPipePara);
    laserCutPipePara = NULL;
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
void CirclePipe_GenerateLaserTrail(float center, float radius_mm)
{
    assert(laserCutPipePara);

    float laser_pos_x = center;
    float laser_pos_y = laserCutPipePara->laser_offset_center_distance;
    float laser_pos_z = laserCutPipePara->circle_pipe_param.radius + laserCutPipePara->laser_height;

    float* laser_trail = laserCutPipePara->write_addr;
    laserCutPipePara->laser_trail_addr = laser_trail;

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
        theta += laserCutPipePara->unit_radian;
    }

    laserCutPipePara->write_addr += (laserCutPipePara->circle_pipe_param.segment * MATRIX_DIMENSION);
}

void CalculateCutTrail(float* trail_buf, const int segments)
{
    for (int i = 0; i < segments; i++)
        *(trail_buf + i * MATRIX_DIMENSION + 2) = sqrt(pow(laserCutPipePara->circle_pipe_param.radius, 2) - pow(*(trail_buf + MATRIX_DIMENSION * i + 1), 2));
}

void GuideLine_GenerateCutTrail(void)
{
    assert(laserCutPipePara);
    assert(laserCutPipePara->guide_line_in_addr);
    assert(laserCutPipePara->guide_line_out_addr);

    if (laserCutPipePara->guide_line_type == arc_guide_line) {
        CalculateCutTrail(laserCutPipePara->guide_line_in_addr, laserCutPipePara->guide_arc_in_segment);
        CalculateCutTrail(laserCutPipePara->guide_line_out_addr, laserCutPipePara->guide_arc_out_segment);
    } else if (laserCutPipePara->guide_line_type == line_guide_line) {
        CalculateCutTrail(laserCutPipePara->guide_line_in_addr, laserCutPipePara->guide_line_segment);
        CalculateCutTrail(laserCutPipePara->guide_line_out_addr, laserCutPipePara->guide_line_segment);
    }
}

void CirclePipe_ConvertAixsYToAngle(float* trail_buf, const int segments)
{
    assert(laserCutPipePara);

    float y, radian, degree;
    float* laser_cut_trail = trail_buf;

    for (int i = 0; i < segments; i++) {
        y = *(laser_cut_trail + i * MATRIX_DIMENSION + 1);
        /**
         * @brief   不可使用atan求值,否则会造成角度突变
         */
        radian = acos(y / laserCutPipePara->circle_pipe_param.radius);
        degree = radian * 180 / PI;
        *(laser_cut_trail + i * MATRIX_DIMENSION + 1) = degree;
    }
}

void CirclePipe_RestoreAxisZCoordVal(float* trail_buf, const int segments)
{
    float* laser_cut_trail = trail_buf;
    for (int i = 0; i < segments; i++) {
        *(laser_cut_trail + i * MATRIX_DIMENSION + 2) = laserCutPipePara->circle_pipe_param.radius + laserCutPipePara->laser_height;
    }
}

static void GuideLine_arc_rotate(float pos_vector[4], const float center_vector[3], const float guide_arc_radius_mm)
{
    rotate_s(pos_vector, center_vector, rotate_y, PI);

    pos_vector[0] += 2 * guide_arc_radius_mm;
}

void GuideLine_GenerateArc(float arc_radius_mm, float arc_length_mm, int guideLine_Type)
{
#define COORDINATE_VALUE_ASSIGNMENT(guide_line_arc)                                                  \
    *(guide_line_arc + i * MATRIX_DIMENSION + 0) = guide_line_center_x + arc_radius_mm * cos(theta); \
    *(guide_line_arc + i * MATRIX_DIMENSION + 1) = guide_line_center_y + arc_radius_mm * sin(theta); \
    *(guide_line_arc + i * MATRIX_DIMENSION + 2) = guide_line_center_z;                              \
    *(guide_line_arc + i * MATRIX_DIMENSION + 3) = 1

    float guide_line_center_x = laserCutPipePara->start_point_x - arc_radius_mm;
    float guide_line_center_y = laserCutPipePara->start_point_y;
    float guide_line_center_z = laserCutPipePara->start_point_z;
    float guide_line_radian = arc_length_mm / arc_radius_mm;
    float guide_line_center_vector[3] = { guide_line_center_x, guide_line_center_y, guide_line_center_z };
    float end_theta = 2 * PI - guide_line_radian;
    float theta;
    int i;

    float *guideLine_arc_in = NULL, *guideLine_arc_out = NULL;
    /**
     * @brief   引入线方向theta --> 0 逆时针
     */
    guideLine_arc_in = laserCutPipePara->write_addr;
    laserCutPipePara->guide_line_in_addr = guideLine_arc_in;

    theta = guide_line_radian;
    i = 0;
    while (theta >= 0) {
        COORDINATE_VALUE_ASSIGNMENT(guideLine_arc_in);
        theta -= laserCutPipePara->unit_radian;
        i++;
    }
    if (theta + laserCutPipePara->unit_radian > 0) {
        theta = 0;
        COORDINATE_VALUE_ASSIGNMENT(guideLine_arc_in);
        i++;
    }
    if (guideLine_Type == GUIDELINE_TYPE_OUTTER) {
        for (int j = 0; j < i; j++)
            GuideLine_arc_rotate(guideLine_arc_in + j * MATRIX_DIMENSION, guide_line_center_vector, arc_radius_mm);
    }
    laserCutPipePara->guide_arc_in_segment = i;

    laserCutPipePara->write_addr += (laserCutPipePara->guide_arc_in_segment * MATRIX_DIMENSION);
    guideLine_arc_in = NULL;
    /**
     * @brief   引出线方向theta --> 0 顺时针
     */
    guideLine_arc_out = laserCutPipePara->write_addr;
    laserCutPipePara->guide_line_out_addr = guideLine_arc_out;

    theta = 2 * PI;
    i = 0;
    while (theta >= end_theta) {
        COORDINATE_VALUE_ASSIGNMENT(guideLine_arc_out);
        theta -= laserCutPipePara->unit_radian;
        i++;
    }
    if (theta + laserCutPipePara->unit_radian > end_theta) {
        theta = end_theta;
        COORDINATE_VALUE_ASSIGNMENT(guideLine_arc_out);
        i++;
    }
    if (guideLine_Type == GUIDELINE_TYPE_OUTTER) {
        for (int j = 0; j < i; j++)
            GuideLine_arc_rotate(guideLine_arc_out + j * MATRIX_DIMENSION, guide_line_center_vector, arc_radius_mm);
    }
    laserCutPipePara->guide_arc_out_segment = i;

    laserCutPipePara->write_addr += (laserCutPipePara->guide_arc_out_segment * MATRIX_DIMENSION);
    guideLine_arc_out = NULL;
#undef COORDINATE_VALUE_ASSIGNMENT
}

void GuideLine_GenerateLine(float line_degree, float line_length_mm)
{
    float guide_line_start_x = laserCutPipePara->start_point_x;
    float guide_line_start_y = laserCutPipePara->start_point_y;
    float guide_line_start_z = laserCutPipePara->start_point_z;
    float guide_line_radian = line_degree * UNIT_RAD;

    /**
     * @brief    guide_line_xs -> guide line x start
     *           guide_line_xe -> guide line x end
     */
    float guide_line_xs = guide_line_start_x + line_length_mm * cos(guide_line_radian - PI / 2);
    float guide_line_ys = guide_line_start_y + line_length_mm * sin(guide_line_radian - PI / 2);
    float guide_line_zs = guide_line_start_z;

    float guide_line_xe = guide_line_start_x;
    float guide_line_ye = guide_line_start_y;
    float guide_line_ze = guide_line_start_z;

    float plan_length = 0.0f;
    int i = 0;

    float delta = 0.0f;
    const float epsinon = 0.001;

    float *guideLine_line_in = NULL, *guideLine_line_out = NULL;

    /**
     * @brief   引入线
     */
    /**1.备份指针*/
    guideLine_line_in = laserCutPipePara->write_addr;
    laserCutPipePara->guide_line_in_addr = guideLine_line_in;
    /**2.计算坐标*/
    while (delta <= epsinon) {
        float t = plan_length / line_length_mm;
        *(guideLine_line_in + i * MATRIX_DIMENSION + 0) = (1 - t) * guide_line_xs + t * guide_line_xe;
        *(guideLine_line_in + i * MATRIX_DIMENSION + 1) = (1 - t) * guide_line_ys + t * guide_line_ye;
        *(guideLine_line_in + i * MATRIX_DIMENSION + 2) = (1 - t) * guide_line_zs + t * guide_line_ze;
        *(guideLine_line_in + i * MATRIX_DIMENSION + 3) = 1;
        plan_length += laserCutPipePara->unit_line_length;
        delta = plan_length - line_length_mm;
        i++;
    }
    /**3.计算残余数据坐标*/
    delta = plan_length - laserCutPipePara->unit_line_length - line_length_mm;
    if (delta < -1 * epsinon) {
        *(guideLine_line_in + i * MATRIX_DIMENSION + 0) = guide_line_xe;
        *(guideLine_line_in + i * MATRIX_DIMENSION + 1) = guide_line_ye;
        *(guideLine_line_in + i * MATRIX_DIMENSION + 2) = guide_line_ze;
        *(guideLine_line_in + i * MATRIX_DIMENSION + 3) = 1;
        i++;
    }
    /**4.更新坐标数据点数量*/
    laserCutPipePara->guide_line_segment = i;

    /**5.更新写指针*/
    laserCutPipePara->write_addr += (laserCutPipePara->guide_line_segment * MATRIX_DIMENSION);

    /**
     * @brief   引出线
     */
    /**1.备份指针*/
    guideLine_line_out = laserCutPipePara->write_addr;
    laserCutPipePara->guide_line_out_addr = guideLine_line_out;

    /**2.计算坐标值*/
    i -= 1;
    for (int j = 0; i >= 0; j++, i--) {
        *(guideLine_line_out + j * MATRIX_DIMENSION + 0) = *(guideLine_line_in + i * MATRIX_DIMENSION + 0);
        *(guideLine_line_out + j * MATRIX_DIMENSION + 1) = *(guideLine_line_in + i * MATRIX_DIMENSION + 1);
        *(guideLine_line_out + j * MATRIX_DIMENSION + 2) = *(guideLine_line_in + i * MATRIX_DIMENSION + 2);
        *(guideLine_line_out + j * MATRIX_DIMENSION + 3) = *(guideLine_line_in + i * MATRIX_DIMENSION + 3);
    }
    /**3.更新写指针*/
    laserCutPipePara->write_addr += (laserCutPipePara->guide_line_segment * MATRIX_DIMENSION);

    guideLine_line_in = NULL;
    guideLine_line_out = NULL;
}
