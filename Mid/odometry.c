#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "ev3api.h"
#include "body.h"
#include "port.h"
#include "motor.h"
#include "odometry.h"

struct coordinate pre_coordinate  = {0.0, 0.0, 0.0};
int   straight_threshold = 2; //直進時のモータ角度誤差許容範囲
float delta_theta_thresshold = 10.0;


/* external functions */
void get_crntCoordinate(struct coordinate* crnt_coordinate){
    float delta_PhL     = 0.0; // 左タイヤの回転角度(radian)
    float delta_PhR     = 0.0; // 右タイヤの回転角度(radian)
    float delta_LL      = 0.0; // 左タイヤの移動量(mm)
    float delta_LR      = 0.0; // 右タイヤの移動量(mm)
    float delta_L       = 0.0; // ロボットの移動量(mm)
    float delta_theta   = 0.0; // ロボットの旋回量(degree)
    double delta_rad    = 0.0; // ロボットの旋回量(radian)
    double pre_rad      = 0.0; // ロボットの旋回量(radian)

    //左右モータの回転量を計算する[rad]
    delta_PhL = 3.141592 * (motor_get_counts(left_motor)  - motor_get_pre_counts(left_motor))  / 180.0;
    delta_PhR = 3.141592 * (motor_get_counts(right_motor) - motor_get_pre_counts(right_motor)) / 180.0;
    // printf("%f, %f\n", delta_PhL, delta_PhR);
    
    /* 左右モータの移動量を計算する */
    delta_LL = (float)(wheel_size / 2) * delta_PhL;
    delta_LR = (float)(wheel_size / 2) * delta_PhR;

    // printf("delta_LL:delta_LR:(%d,%d)\n",delta_LL,delta_LR);

    if( (delta_LL > 0.0 && delta_LR < 0.0) || (delta_LL < 0.0 && delta_LR > 0.0) ){ //旋回
        printf("旋回処理\n");
        // ロボットの移動距離
        delta_L = 0.0;
        // 旋回量の計算
        // delta_theta = (delta_LR - (float)wheel_dist) / abs(delta_LL - delta_LR);
        // delta_theta = (180 * delta_LR) / (( ( delta_LR * (float)wheel_dist) / ( delta_LR + delta_LL ) ) * 3.141592);

        if(delta_LL > delta_LR){
            delta_theta *= -1;
        }
    }
    
    else { //直進・曲進している
        printf("処理に入らない想定のほう。\n");
        /*
        delta_L = (delta_LL + delta_LR)/(float)2.0;

        if( abs(delta_LL-delta_LR) < ((wheel_size/2)*3.141592*straight_threshold/180.0) ){ //直進している
            //ロボットの旋回量
            delta_rad = 0.0;
        }
        else { //曲進している
            //ロボットの旋回量
            //delta_rad = (delta_LL - delta_LR) / (float)wheel_dist;
            delta_theta = (180 * delta_LR) / (( ( delta_LR * (float)wheel_dist) / ( delta_LR + delta_LL ) ) * 3.141592);
            if(abs(delta_rad) > delta_theta_thresshold){ //delta_thetaが大きい
                delta_L = 2 * (delta_L / delta_rad) * sin(delta_rad / 2);
                printf("BBB, ");
            }
        }    
        */
    }


    // 現在座標を計算する
    pre_rad = 3.141592 * pre_coordinate.theta / 180.0;
    delta_rad = 3.141592 * delta_theta;
    //delta_theta = 180.0 * delta_rad / 3.141592;
    
    crnt_coordinate->x      = pre_coordinate.x + (float)((double)delta_L * cos(pre_rad + (delta_rad / 2.0)));
    crnt_coordinate->y      = pre_coordinate.y + (float)((double)delta_L * sin(pre_rad + (delta_rad / 2.0)));
    crnt_coordinate->theta  = pre_coordinate.theta + (float)delta_theta;
    
    // 小数点下第１位で切り捨て
    crnt_coordinate->x      = floor((crnt_coordinate->x * 10.0) / 10.0);
    crnt_coordinate->y      = floor((crnt_coordinate->y * 10.0) / 10.0);
    crnt_coordinate->theta  = floor((crnt_coordinate->theta * 10.0) / 10.0);
    
    //前回座標を更新する
    pre_coordinate.x     = crnt_coordinate->x;
    pre_coordinate.y     = crnt_coordinate->y;
    pre_coordinate.theta = crnt_coordinate->theta;

    return;
}

extern void get_preCoordinate(struct coordinate* coordinate_p){
    coordinate_p->x     = pre_coordinate.x;
    coordinate_p->y     = pre_coordinate.y;
    coordinate_p->theta = pre_coordinate.theta;

    return;
}

