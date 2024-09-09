#include <stdio.h>
#include <math.h>

#include "app.h"
#include "ev3api.h"
#include "port.h"
#include "odometry.h"
#include "smartCarry.h"

/* メインタスク(起動時にのみ関数コールされ�?) */
void main_task(intptr_t unused) {
    /* ポート設定 */
    // sensor   : touch_sensor, color_sensor, sonar_sensor, gyro_sensor
    // actuator : arm_motor, left_motor, right_motor
    set_portCfg();
    
    /* タスク呼び出し */
    sta_cyc(BOSS_CYC);

    /* タスク終了 */
    ext_tsk();
}

bool_t toch_flg = false;
int cnt=0;

void boss_task(intptr_t exinf){
    struct coordinate crnt = {0.0, 0.0, 0.0};
    int angle = 45;
    int now_angle = 0;
    get_crntCoordinate(&crnt);
    toch_flg = ev3_touch_sensor_is_pressed(touch_sensor);
    //printf("x=%f, y=%f, theta=%f\n",crnt.x, crnt.y, crnt.theta);
    printf("x=%f, y=%f, theta=%f\n",crnt.x, crnt.y, crnt.theta);



    if (crnt.x >=300.0f){
        ev3_motor_set_power(left_motor,  -15);
        ev3_motor_set_power(right_motor, 15);
    }
    else if(crnt.x >=200.0f && crnt.x <=300.0f){
        ev3_gyro_sensor_reset(gyro_sensor);
    }
    else{
        ev3_motor_set_power(left_motor,  15);
        ev3_motor_set_power(right_motor, -15);
    }
/*
    
    else if (cnt == 1){
        now_angle = (int)crnt.theta;
        //printf("比較角度(現在( %f° ):目標( %f° )\n",now_angle,angle);
        if (now_angle < angle){
            ev3_motor_set_power(left_motor,  -8);
            ev3_motor_set_power(right_motor, 8);
            
            printf("処理：分岐１");
            printf("比較角度(現在( %d° ):目標( %d° )\n",now_angle,angle);
        }
        else if (now_angle > angle){
            ev3_motor_set_power(left_motor,  8);
            ev3_motor_set_power(right_motor, -8);
            
            printf("処理：分岐２");
            printf("比較角度(現在( %d° ):目標( %d° )\n",now_angle,angle);
        }else{
            printf("角度調整完了");
            cnt = cnt + 1;
            ev3_motor_set_power(left_motor,  0);
            ev3_motor_set_power(right_motor, 0);
        }
    }else{
            ev3_motor_set_power(left_motor,  0);
            ev3_motor_set_power(right_motor, 0);
    }*/
    
    

}