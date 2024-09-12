#include <stdio.h>

#include "gyro.h"
#include "ev3api.h"
#include "port.h"
#include "motor.h"
#include "_timer_.h"


int16_t crnt_angle = 0;
int16_t pre_angle  = 0;
int16_t crnt_rate  = 0;
int16_t pre_rate   = 0;


/* static functions */
int checkMissingData_g(motor_port_t port){
    // 通信遅れ判定の場合、1ms待って再取得
    if(crnt_angle == pre_angle){ // 前回値と一致
        int mp_l     = motor_get_power(left_motor);
        int mp_l_pre = motor_get_pre_power(left_motor);
        int mp_r     = motor_get_power(right_motor);
        int mp_r_pre = motor_get_pre_power(right_motor);

        if( (mp_l * mp_l_pre * mp_r * mp_r_pre) != 0){ //左右モータのパワーが0でない
            // delay_connect(1); // 1ms待つ
            crnt_angle = ev3_gyro_sensor_get_angle(port);
            return 0;
        }     
    }

    return 1;
}

/* extern functions */
int16_t gyro_sensor_get_angle(sensor_port_t port){
    if(port != gyro_sensor){
        printf("An invalid value entered in gyro_sensor_get_angle().\n");
        return 0;
    }

    /* 前回値の更新 */
    pre_angle = crnt_angle;  

    // 現在値の取得(通信遅れ判定の場合、1ms待って再取得)
    for(int i=0; i<1; i++){
        crnt_angle = ev3_gyro_sensor_get_angle(port);
        i += checkMissingData_g(port);
    }
    
    return crnt_angle;
}

int16_t gyro_sensor_get_pre_angle(sensor_port_t port){
    if(port != gyro_sensor){
        printf("An invalid value entered in gyro_sensor_get_pre_angle().\n");
        return 0;
    }

    return pre_angle;
}

void gyro_sensor_reset(sensor_port_t port){
    if(port != gyro_sensor){
        printf("An invalid value entered in gyro_sensor_reset().\n");
        return;
    }

    ev3_gyro_sensor_reset(port);
    crnt_angle = 0;
    pre_angle  = 0;

    return;
}

int16_t gyro_sensor_get_rate(sensor_port_t port){
    if(port != gyro_sensor){
        printf("An invalid value entered in gyro_sensor_get_rate().\n");
        return 0;
    }

    /* 前回値の更新 */
    pre_rate = crnt_rate;
    
    // 現在値の取得(通信遅れ判定の場合、1ms待って再取得)
    for(int i=0; i<1; i++){
        crnt_rate = ev3_gyro_sensor_get_rate(port);
        i += checkMissingData_g(port);
    }
    
    return crnt_rate;
}

int16_t gyro_sensor_get_pre_rate(sensor_port_t port){
    if(port != gyro_sensor){
        printf("An invalid value entered in gyro_sensor_get_pre_rate().\n");
        return 0;
    }

    return pre_rate;
}


/* using_sensor_task */
void gyro_sensor_update(sensor_port_t port){
    if(port != gyro_sensor){
        printf("An invalid value entered in gyro_sensor_update().\n");
        return;
    }

    /* 前回値の更新 */
    pre_angle = crnt_angle;
    pre_rate = crnt_rate; 

    // 現在値の取得(通信遅れ判定の場合、1ms待って再取得)
    for(int i=0; i<1; i++){
        crnt_angle = ev3_gyro_sensor_get_angle(port);
        crnt_rate = ev3_gyro_sensor_get_rate(port);
        i += checkMissingData_g(port);
    }

    return;
}