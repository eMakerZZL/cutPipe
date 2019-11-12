#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define PI (3.141592654f)
#define UNIT_RAD (PI / 180)
#define UNIT_ARC_LENGTH (0.1f)

#define MATRIX_DIMENSION (4)
#define VECTOR_DIMENSION (3)

#define GUIDELINE_TYPE_INNER  (0)
#define GUIDELINE_TYPE_OUTTER (1)

typedef enum PipeType{
    circlePipe,
    squarePipe,
}PipeType;

typedef struct LaserCutPipePara {
    PipeType pipe;
    union{
        struct{
            float radius;
            float center_x;
            float center_y;
            float center_z;
            int   segment;
        } circle_pipe_param;
        struct{
            float length;
            float width;
        } square_pipe_param;
    };
    float pipe_length;
    float unit_line_length;
    float unit_arc_length;
    float unit_radian;

    float laser_power;
    float laser_pwm;
    float laser_freq;
    float laser_deltaTime;
    float laser_holdTime;

    float laser_angle_x;
    float laser_angle_y;
    float laser_angle_z;
    float laser_height;
    float laser_offset_center_distance;

    float guide_line_angle;
    float guide_line_length;
    int   guide_line_segment;

    float guide_arc_radius;
    float guide_arc_length;
    int   guide_arc_in_segment;
    int   guide_arc_out_segment;

    float start_point_x;
    float start_point_y;
    float start_point_z;
}LaserCutPipePara;

LaserCutPipePara* init_LaserCutPipePara(void);
void destory_LaserCutPipePara(void);

float* get_GuideLineArcIn_addr(void);
float* get_GuideLineArcOut_addr(void);
float* get_GuideLineArcIn_CutTrail_addr(void);
float* get_GuideLineArcOut_CutTrail_addr(void);

float* get_GuideLineIn_addr(void);
float* get_GuideLineOut_addr(void);
float* get_GuideLineIn_CutTrail_addr(void);
float* get_GuideLineOut_CutTrail_addr(void);

float* CirclePipe_GenerateLaserTrail(float center, float radius_mm);
float* CirclePipe_GenerateCutTrail(void);
float* CirclePipe_ConvertAixsYToAngle(void);
float* CirclePipe_RestoreAxisZCoordVal(void);
void GuideLine_GenerateArc(float arc_radius_mm, float arc_length_mm, int guideLine_Type);
void GuideLine_GenerateCutTrail(void);
void GuideLine_GenerateLine(float line_degree, float line_length_mm);
#endif /* end of include guard: CONFIG_H */
