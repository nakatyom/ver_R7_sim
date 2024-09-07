#include <stdio.h>
#include <time.h>

#include "gyro.h"
#include "ev3api.h"
#include "port.h"
#include "motor.h"


int16_t crnt_angle = 0;
int16_t pre_angle  = 0;
int16_t crnt_rate  = 0;
int16_t pre_rate   = 0;


/* static functions */
void delay_connect_g(int milliseconds){
    clock_t start_time = clock();
    while(clock() < start_time + milliseconds);
}


/* extern functions */
int16_t gyro_sensor_get_angle(sensor_port_t port){
    if(port != gyro_sensor){
        printf("An invalid value entered in gyro_sensor_get_angle().\n");
        return 0;
    }

    /* 前回値の更新 */
    pre_angle = crnt_angle;

    // 現在値の取得
    crnt_angle = ev3_gyro_sensor_get_angle(port);

    // 通信遅れ判定の場合、1ms待って再取得
    if(crnt_angle == pre_angle){ // 前回値と一致
        int mp_l     = motor_get_power(left_motor);
        int mp_l_pre = motor_get_pre_power(left_motor);
        int mp_r     = motor_get_power(right_motor);
        int mp_r_pre = motor_get_pre_power(right_motor);

        if( (mp_l * mp_l_pre * mp_r * mp_r_pre) != 0){ //左右モータのパワーが0でない
            delay_connect_g(1); // 1ms待つ
            crnt_angle = ev3_gyro_sensor_get_angle(port);
        }     
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

int16_t gyro_sensor_get_rate(sensor_port_t port){
    if(port != gyro_sensor){
        printf("An invalid value entered in gyro_sensor_get_rate().\n");
        return 0;
    }

    /* 前回値の更新 */
    pre_rate = crnt_rate;

    // 現在値の取得
    crnt_rate = ev3_gyro_sensor_get_rate(port);

    // 通信遅れ判定の場合、1ms待って再取得
    if(crnt_rate == pre_rate){ // 前回値と一致
        int mp_l     = motor_get_power(left_motor);
        int mp_l_pre = motor_get_pre_power(left_motor);
        int mp_r     = motor_get_power(right_motor);
        int mp_r_pre = motor_get_pre_power(right_motor);

        if( (mp_l * mp_l_pre * mp_r * mp_r_pre) != 0){ //左右モータのパワーが0でない
            delay_connect_g(1); // 1ms待つ
            crnt_rate = ev3_gyro_sensor_get_rate(port);
        }     
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