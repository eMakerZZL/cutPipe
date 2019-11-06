#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define PI (3.1415926f)
#define UNIT_RAD (PI / 180)
#define UNIT_ARC_LENGTH (0.1f)

#define MATRIX_DIMENSION (4)
#define VECTOR_DIMENSION (3)

#define GUIDELINE_TYPE_INNER  (0)
#define GUIDELINE_TYPE_OUTTER (1)
#define GUIDELINE_DIR_IN  (0)
#define GUIDELINE_DIR_OUT (1)

typedef enum PipeType{
    circlePipe,
    squarePipe,
}PipeType;

typedef struct LaserCutPipePara {
    PipeType pipe;
    union{
        struct{
            float radius;
            float unit_radian;
            float unit_arc_length;
            int   segment;
        } circle_pipe_param;
        struct{
            float length;
            float width;
        } square_pipe_param;
    };
    float pipe_length;

    float laser_power;
    float laser_pwm;
    float laser_freq;
    float laser_deltaTime;
    float laser_holdTime;

    float laser_pos_x;
    float laser_pos_y;
    float laser_pos_z;
    float laser_angle_x;
    float laser_angle_y;
    float laser_angle_z;
    float laser_height;
    float laser_offset_center_distance;

    float guide_line_angle;
    float guide_line_length;

    float guide_arc_radius;
    float guide_arc_length;
    int   is_default_enalbe_guide_line;
}LaserCutPipePara;

LaserCutPipePara* init_LaserCutPipePara(void);
void destory_LaserCutPipePara(void);
float* CirclePipe_GenerateLaserTrail(float center, float radius_mm);
float* CirclePipe_GenerateCutTrail(void);
float* CirclePipe_ConvertAixsYToAngle(void);
float* CirclePipe_RestoreAxisZCoordVal(void);
#endif /* end of include guard: CONFIG_H */
