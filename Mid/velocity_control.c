#include<stdio.h>

#include "common.h"
#include "port.h"
#include "body.h"
#include "motor.h"
#include "gyro.h"
#include "velocity_control.h"

#define ENC_CORR_R      1.0f       // 右ENC個体差補正係数
#define ENC_CORR_L      1.0f       // 左ENC個体差補正係数
#define SAMPLE_RATE     20.0f      // 計測周波数[ms]


float mid_PID_str_velo(float tag, float maj, int intg_rest, int intg_stop){ 

    const float kp = 0.1f;
    const float ki = 0.01f;
    const float kd = 0.0f;

    static float intg;
    static float err_pre;
    static float err;
     
    err_pre = err;
    err = tag - maj;

    if (intg_stop != 1) intg += err;
    if (intg_rest == 1) intg = 0;

    if (intg > 1000.0f)    intg = 1000.0f;
    if (intg < -1000.0f)   intg = -1000.0f;

    return ((err * kp) + (intg * ki) + ((err - err_pre) * kd));
}


float mid_PID_rot_velo(float tag, float maj, int intg_rest, int intg_stop){ 

    const float kp = 0.4f;
    const float ki = 0.1f;
    const float kd = 0.05f;

    static float intg;
    static float err_pre;
    static float err;
     
    err_pre = err;
    err = tag - maj;
    intg += err;

    if (intg_stop != 1) intg += err;
    if (intg_rest == 1) intg = 0;

    if (intg > 1000.0f)    intg = 1000.0f;
    if (intg < -1000.0f)   intg = -1000.0f;

    return ((err * kp) + (intg * ki) + ((err - err_pre) * kd));
}


float mid_LPF_str_velo(float maj){ 

    const float k = 0.85f;

    static float out;

    out = (out * k) + (maj * (1.0f - k)); 

    return out;
}


void mid_velocity_control(float velo_str_tgt, float velo_rot_tgt){

    static int cnt_r;
    static int cnt_l;
    static int cnt_r_pre;
    static int cnt_l_pre;

    /* 並進速度計算 */
    cnt_r_pre = cnt_r;
    cnt_l_pre = cnt_l;
    cnt_r = motor_get_counts(right_motor); 
    cnt_l = motor_get_counts(left_motor); 

    int velo_r_tmp  = ((cnt_r - cnt_r_pre) * ENC_CORR_R) / SAMPLE_RATE; // cnt ⇒ deg/sec(motor)
    int velo_l_tmp  = ((cnt_l - cnt_l_pre) * ENC_CORR_L) / SAMPLE_RATE; // cnt ⇒ deg/sec(motor)             
    
    float velo_r    = ((float)velo_r_tmp / 360.0f) * (PI_FLOAT * wheel_size);   // deg/sec(motor) ⇒ mm/sec(motor) 
    float velo_l    = ((float)velo_l_tmp / 360.0f) * (PI_FLOAT * wheel_size);   // deg/sec(motor) ⇒ mm/sec(motor)
   
    float velo_str  = mid_LPF_str_velo( ((velo_r + velo_l) * 0.5f)); // mm/sec(robot)

    /* 旋回速度計算 */ 
    float velo_rot = (float)gyro_sensor_get_rate(gyro_sensor);   // deg/sec (yaw rate)

    /* 速度操作量計算（FB項算出）*/
#if (0)
    float velo_str_u = mid_PID_str_velo(velo_str_tgt, velo_str, false, false);    // mm/sec(robot)
    float velo_rot_u = mid_PID_rot_velo(velo_rot_tgt, velo_rot, false, false);    // deg/sec(yaw rate)
#else
    float velo_str_u = 0;   // debug
    float velo_rot_u = 0;   // debug
#endif

    /* 速度操作量計算（FF項とFB項の合成）*/
    float mot_r_u_str = ((velo_str_tgt + velo_str_u) / (PI_FLOAT * wheel_size)) * 360.0f;  // mm/sec(robot) ⇒ deg/sec(motor)
    float mot_l_u_str = ((velo_str_tgt + velo_str_u) / (PI_FLOAT * wheel_size)) * 360.0f;  // mm/sec(robot) ⇒ deg/sec(motor)
    float mot_r_u_rot = (wheel_dist / wheel_size) * (velo_rot_tgt + velo_rot_u);            // deg/sec(yaw rate) ⇒ deg/sec(motor)
    float mot_l_u_rot = (wheel_dist / wheel_size) * (velo_rot_tgt + velo_rot_u);            // deg/sec(yaw rate) ⇒ deg/sec(motor)
    
    /* 速度操作量計算（並進速度と旋回速度の合成）*/
    float mot_r_u_tmp = mot_r_u_str + mot_r_u_rot;  // deg/sec(motor)
    float mot_l_u_tmp = mot_l_u_str - mot_l_u_rot;  // deg/sec(motor)
    
    /* 操作量計算 */
    int mot_r_u = (int)(mot_r_u_tmp * 1.0f);
    int mot_l_u = (int)(mot_l_u_tmp * 1.0f);

    if (mot_r_u >  100)  mot_r_u = 100;
    if (mot_r_u < -100)  mot_r_u = -100;
    if (mot_l_u >  100)  mot_l_u = 100;
    if (mot_l_u < -100)  mot_l_u = -100;

    float batt_v = (float)ev3_battery_voltage_mV();  // Todo:電圧取れない

    ev3_motor_set_power(right_motor, mot_r_u);
    ev3_motor_set_power(left_motor, mot_l_u);

    /* for Debug */
//    printf("velo_str %f | velo_rot %f | velo_str_u %f | velo_rot_u %f | mot_r_u_tmp %f | mot_l_u_tmp %f\n", velo_str, velo_rot, velo_str_u, velo_rot_u, mot_r_u_tmp, mot_l_u_tmp);
//    printf("velo_str %f | velo_rot %f | velo_str_u %f | velo_rot_u %f\n", velo_str, velo_rot, velo_str_u, velo_rot_u);
//    printf("velo_str %f | velo_rot %f\n", velo_str, velo_rot);
//    printf("cnt_r %d | cnt_l %d\n", velo_r_tmp, velo_l_tmp);
//    printf("mot_r_u_str %f | mot_r_u_rot %f\n", mot_r_u_str, mot_r_u_rot);
//    printf("mot_r_u %d | mot_l_u %d\n", mot_r_u, mot_l_u);
//    printf("velo_str %8.4f | velo_rot %8.4f | mot_r_u %3d | mot_l_u %3d | batt_v %8.4f\n", velo_str, velo_rot, mot_r_u, mot_l_u, batt_v);
    printf("velo_str_tgt %8.4f | velo_str_u %8.4f | mot_r_u_str %8.4f | mot_l_u_str %8.4f\n", velo_str_tgt, velo_str_u, mot_r_u_str, mot_l_u_str);
    
//    int mot_r_p = ev3_motor_get_power(right_motor);
//    int mot_l_p = ev3_motor_get_power(left_motor);
//    printf("mot_r_p %3d | mot_l_p %3d\n",mot_r_p, mot_l_p);
}


void mid_velocity_control_reset(void){

    float dummy1 = mid_PID_str_velo(0, 0, 1, 1);
    float dummy2 = mid_PID_rot_velo(0, 0, 1, 1);
}