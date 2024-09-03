#include <stdio.h>
#include <stdlib.h>

#include "ev3api.h"
#include "body.h"
#include "port.h"
#include "motor.h"
#include "v_ctrl.h"

#define ENC_CORR_R      1.0f        // 右ENC個体差補正係数
#define ENC_CORR_L      1.0f        // 左ENC個体差補正係数

#define TIRE_SIZE_R     94.0f       // 右タイヤ直径[mm]
#define TIRE_SIZE_L     94.0f       // 左タイヤ直径[mm]
#define TRED_SIZE       125.0f      // トレッド直径[mm]

#define SAMPLE_RATE     100.0f      // 計測周波数[sec]
#define PI_FLOAT        3.141592f   // 円周率

static struct pid_params{
    const float kp;
    const float ki;
    const float kd;
    float pre_err;
    float intg;
};

struct pid_params pid_params_str = {0.1, 0.01, 0.0 , 0.0, 0.0};
struct pid_params pid_params_rot = {0.4, 0.1 , 0.05, 0.0, 0.0};

/* static functions */
float mid_PID_velo(float tgt, float crnt, struct pid_params* pid_params){
    float err;
    
    err = tgt - crnt;
    pid_params->intg += err;

    if (pid_params->intg >  1000.0f) pid_params->intg = 1000.0f;
    if (pid_params->intg < -1000.0f) pid_params->intg = -1000.0f;

    float out = (pid_params->kp)*err + (pid_params->ki)*(pid_params->intg) + (pid_params->kd)*(err - (pid_params->pre_err));

    return out;
}

float mid_LPF_str_velo(float maj){ 
    const float k = 0.85f;
    static float out;

    out = (out * k) + (maj * (1.0f - k)); 

    return out;
}


/* extern functions */
extern void mid_velocity_control(float velo_str_tgt, float velo_rot_tgt){
    /* 並進速度計算 */
    int32_t cnt_r       = motor_get_counts(right_motor); 
    int32_t cnt_l       = motor_get_counts(left_motor); 
    int32_t cnt_r_pre   = motor_get_pre_counts(right_motor);
    int32_t cnt_l_pre   = motor_get_pre_counts(left_motor);
    
    int velo_r_tmp  = ((cnt_r - cnt_r_pre) * ENC_CORR_R) * SAMPLE_RATE; // cnt ⇒ deg/sec(motor)
    int velo_l_tmp  = ((cnt_l - cnt_l_pre) * ENC_CORR_L) * SAMPLE_RATE; // cnt ⇒ deg/sec(motor)             
    
    float velo_r    = ((float)velo_r_tmp / 360.0f) * (PI_FLOAT * TIRE_SIZE_R);   // deg/sec(motor) ⇒ mm/sec(motor) 
    float velo_l    = ((float)velo_l_tmp / 360.0f) * (PI_FLOAT * TIRE_SIZE_L);   // deg/sec(motor) ⇒ mm/sec(motor)

    float velo_str  = mid_LPF_str_velo( ((velo_r + velo_l) * 0.5f)); // mm/sec(robot)

    /* 旋回速度計算 */ 
    float velo_rot = (float)ev3_gyro_sensor_get_rate(gyro_sensor);   // deg/sec (yaw rate)
    
    /* 速度操作量計算（FB項算出）*/
    float velo_str_u = mid_PID_velo(velo_str_tgt, velo_str, &pid_params_str);    // mm/sec(robot)  
    float velo_rot_u = mid_PID_velo(velo_rot_tgt, velo_rot, &pid_params_rot);    // deg/sec(yaw rate)

    /* 速度操作量計算（FF項とFB項の合成）*/
    float mot_r_u_str = ((velo_str_tgt + velo_str_u) / (PI_FLOAT * TIRE_SIZE_R)) * 360.0f;  // mm/sec(robot) ⇒ deg/sec(motor)
    float mot_l_u_str = ((velo_str_tgt + velo_str_u) / (PI_FLOAT * TIRE_SIZE_L)) * 360.0f;  // mm/sec(robot) ⇒ deg/sec(motor)
    float mot_r_u_rot = (velo_rot_tgt + velo_rot_u) * (TRED_SIZE / TIRE_SIZE_R);    // deg/sec(yaw rate) ⇒ deg/sec(motor)
    float mot_l_u_rot = (velo_rot_tgt + velo_rot_u) * (TRED_SIZE / TIRE_SIZE_L);    // deg/sec(yaw rate) ⇒ deg/sec(motor)

    /* 速度操作量計算（並進速度と旋回速度の合成）*/
    float mot_r_u_tmp = mot_r_u_str + mot_r_u_rot;  // deg/sec(motor)
    float mot_l_u_tmp = mot_l_u_str - mot_l_u_rot;  // deg/sec(motor)
    
    /* 操作量計算 */
    int mot_r_u = (int)(mot_r_u_tmp * 1.0f);
    int mot_l_u = (int)(mot_l_u_tmp * 1.0f);

    /* モータ出力 */
    motor_set_power(right_motor, mot_r_u);
    motor_set_power(left_motor , mot_l_u);

    return;
}

extern void mid_velocity_control_str(float velo_str_tgt){
    /* 並進速度計算 */
    int32_t cnt_r       = motor_get_counts(right_motor); 
    int32_t cnt_l       = motor_get_counts(left_motor); 
    int32_t cnt_r_pre   = motor_get_pre_counts(right_motor);
    int32_t cnt_l_pre   = motor_get_pre_counts(left_motor);
    
    int velo_r_tmp  = ((cnt_r - cnt_r_pre) * ENC_CORR_R) * SAMPLE_RATE; // cnt ⇒ deg/sec(motor)
    int velo_l_tmp  = ((cnt_l - cnt_l_pre) * ENC_CORR_L) * SAMPLE_RATE; // cnt ⇒ deg/sec(motor)             
    
    float velo_r    = ((float)velo_r_tmp / 360.0f) * (PI_FLOAT * TIRE_SIZE_R);   // deg/sec(motor) ⇒ mm/sec(motor) 
    float velo_l    = ((float)velo_l_tmp / 360.0f) * (PI_FLOAT * TIRE_SIZE_L);   // deg/sec(motor) ⇒ mm/sec(motor)

    float velo_str  = mid_LPF_str_velo( ((velo_r + velo_l) * 0.5f)); // mm/sec(robot)

    /* 速度操作量計算（FB項算出）*/
    float velo_str_u = mid_PID_velo(velo_str_tgt, velo_str, &pid_params_str);    // mm/sec(robot)
    
    /* 速度操作量計算（FF項とFB項の合成）*/
    float mot_r_u_str = ((velo_str_tgt + velo_str_u) / (PI_FLOAT * TIRE_SIZE_R)) * 360.0f;  // mm/sec(robot) ⇒ deg/sec(motor)
    float mot_l_u_str = ((velo_str_tgt + velo_str_u) / (PI_FLOAT * TIRE_SIZE_L)) * 360.0f;  // mm/sec(robot) ⇒ deg/sec(motor)

    /* 操作量計算 */
    int mot_r_u = (int)(mot_r_u_str * 1.0f);
    int mot_l_u = (int)(mot_l_u_str * 1.0f);
    /* -100~100に整形 */
    if (mot_r_u >  100 || mot_r_u < -100)  mot_r_u = mot_r_u * 100 / abs(mot_r_u);
    if (mot_l_u >  100 || mot_l_u < -100)  mot_r_u = mot_l_u * 100 / abs(mot_l_u);

    /* モータ出力 */
    motor_set_power(right_motor, mot_r_u);
    motor_set_power(left_motor , mot_l_u);

    return;
}

extern void mid_velocity_control_rot(float velo_rot_tgt){
    /* 旋回速度計算 */ 
    float velo_rot = (float)ev3_gyro_sensor_get_rate(gyro_sensor);   // deg/sec (yaw rate)
    
    /* 速度操作量計算（FB項算出）*/
    float velo_rot_u = mid_PID_velo(velo_rot_tgt, velo_rot, &pid_params_rot);    // deg/sec(yaw rate)

    /* 速度操作量計算（FF項とFB項の合成）*/
    float mot_r_u_rot = (TRED_SIZE / TIRE_SIZE_R) * (velo_rot_tgt + velo_rot_u);    // deg/sec(yaw rate) ⇒ deg/sec(motor)
    float mot_l_u_rot = (TRED_SIZE / TIRE_SIZE_L) * (velo_rot_tgt + velo_rot_u);    // deg/sec(yaw rate) ⇒ deg/sec(motor)

    /* 操作量計算 */
    int mot_r_u = (int)(mot_r_u_rot * 1.0f);
    int mot_l_u = (int)(mot_l_u_rot * 1.0f);
    /* -100~100に整形 */
    if (mot_r_u >  100 || mot_r_u < -100)  mot_r_u = mot_r_u * 100 / abs(mot_r_u);
    if (mot_l_u >  100 || mot_l_u < -100)  mot_r_u = mot_l_u * 100 / abs(mot_l_u);
   
    /* モータ出力 */
    motor_set_power(right_motor, mot_r_u);
    motor_set_power(left_motor , mot_l_u);

    return;
}