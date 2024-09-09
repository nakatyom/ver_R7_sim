#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "ev3api.h"
#include "body.h"
#include "port.h"
#include "motor.h"
#include "gyro.h"
#include "odometry.h"


static struct coordinate pre_coordinate  = {0.0, 0.0, 0.0}; //前回座標
const int   straight_threshold = 2; //直進時のモータ角度誤差許容範囲
const float delta_theta_thresshold = 10.0;

/* external functions */
void get_crntCoordinate(struct coordinate* crnt_coordinate){
    float delta_L = 0.0; // 移動量

    /* 左右モータの回転量を計算する[rad] */
    float delta_PhL = 3.141592 * (motor_get_counts(left_motor)  - motor_get_pre_counts(left_motor))  / 180.0;
    float delta_PhR = 3.141592 * (motor_get_counts(right_motor) - motor_get_pre_counts(right_motor)) / 180.0;
    
    /* 左右モータの移動量を計算する */
    float delta_LL = (float)(wheel_size / 2) * delta_PhL;
    float delta_LR = (float)(wheel_size / 2) * delta_PhR;

    /* 走行体の旋回角度を計算する */
    float delta_theta = gyro_sensor_get_angle(gyro_sensor) - gyro_sensor_get_pre_angle(gyro_sensor);
    float delta_rad   = 3.141592 * delta_rad /180.0;

    if( (delta_LL > 0.0 && delta_LR < 0.0) || (delta_LL < 0.0 && delta_LR > 0.0) ){ //旋回している
        //ロボットの移動距離
        delta_L = 0.0;

        // 旋回量の計算
        if(abs(delta_LL) != abs(delta_LR) && abs(delta_LL) > abs(delta_LR)){
            // printf("脳筋ブレイクポイント,左:%f,右:%f\n",delta_LL,delta_LR);
            delta_rad = (abs(delta_LL) - abs(delta_LR)) / (float)wheel_dist;
        }else if(abs(delta_LL) != abs(delta_LR) && abs(delta_LL) < abs(delta_LR)){
            delta_rad = (abs(delta_LR) - abs(delta_LL)) / (float)wheel_dist;
        }
        else{
            if(delta_LL > 0){
                delta_rad = delta_LL / (float)wheel_dist;
                // printf("delta_rad:%f\n",delta_rad);
            }
            else if(delta_LR > 0){
                delta_rad = delta_LR / (float)wheel_dist;
            }
        }
        delta_theta = (delta_rad * 360) / 3.141592;

        //printf("delta_LL:%f | ",delta_LL);
        //printf("delta_LR:%f | ",delta_LR);
        //printf("delta_rad:%f | ",delta_rad);


    }

    else { //直進・曲進している
        //ロボットの移動距離
        delta_L = (delta_LL + delta_LR)/(float)2.0;

        if( abs(delta_LL-delta_LR) < ((wheel_size/2)*3.141592*straight_threshold/180.0) ){ //直進している
            //ロボットの旋回量
            // delta_rad = 0.0;
        }
        else { //曲進している
            //ロボットの旋回量
            delta_rad = (delta_LL - delta_LR) / (float)wheel_dist;
            if(abs(delta_rad) > 0.174){ //delta_radが十分大きい
                delta_L = 2 * (delta_L / delta_rad) * sin(delta_rad / 2);
            }
        }
    }


    // 現在座標を計算する
    float pre_rad =+ 3.141592 * pre_coordinate.theta / 180.0;

    //printf("pre_rad:%f | ",pre_rad);
    
    crnt_coordinate->x      = pre_coordinate.x + (float)((double)delta_L * cos(pre_rad + (delta_rad / 2.0)));
    crnt_coordinate->y      = pre_coordinate.y + (float)((double)delta_L * sin(pre_rad + (delta_rad / 2.0)));
    crnt_coordinate->theta  = pre_coordinate.theta + (float)delta_theta;
    
            //前回座標を更新する
    pre_coordinate.x     = crnt_coordinate->x;
    pre_coordinate.y     = crnt_coordinate->y;
    pre_coordinate.theta = crnt_coordinate->theta;

    //printf("前回角度:%f | ",pre_coordinate.theta);

    //printf("\n");
    return;
}

extern void get_preCoordinate(struct coordinate* coordinate_p){
    coordinate_p->x     = pre_coordinate.x;
    coordinate_p->y     = pre_coordinate.y;
    coordinate_p->theta = pre_coordinate.theta;

    return;
}

