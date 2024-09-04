#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "ev3api.h"
#include "odometry.h"
#include "smartCarry.h"


// 旋回角度算出処理
extern float calc_angle(float tgt_pos_x,float tgt_pos_y){
    static const float PI = 3.14159265358979323846264338327950288;   
    float tgt_angle=0;
    struct coordinate crnt = {0.0, 0.0, 0.0};

    get_crntCoordinate(&crnt);
    // arctan2 関数用の下処理。(x座標)
    if (crnt.x <= 0){
        tgt_pos_x = tgt_pos_x + crnt.x;
    }else if(crnt.x > 0){
        tgt_pos_x = tgt_pos_x - crnt.x;
    }
    else{
        printf("旋回角度算出中に異常発生");
    }
    // arctan2 関数用の下処理。(y座標)
    if (crnt.y <= 0){
        tgt_pos_y = tgt_pos_y + crnt.y;
    }else if(crnt.y > 0){
        tgt_pos_y = tgt_pos_y - crnt.y;
    }else{
        printf("旋回角度算出中に異常発生");
    }
    
    tgt_angle = atan2(tgt_pos_y,tgt_pos_x);
    
    tgt_angle = tgt_angle * (180 / PI);

    //printf("(x,y)=(%f,%f) , theta=%f\n目標位置(100,100)への角度：%f°\n",crnt.x, crnt.y, crnt.theta,tgt_angle);
    //printf("(----------------------------------------------------\n");

    
    return tgt_angle;
}

// 走行処理
// 攻略
void hello_scarry(){
    // 定数宣言（目標座標の配列）
    float x_pos_target[6]={133.0,164.0,196.0,168.0,250.0,250.0};

    float y_pos_target[6]={8.0,8.0,100.0,200.0,100.0,34.0};


    // 変数宣言
    struct coordinate crnt = {0.0, 0.0, 0.0};//自己位置座標
    
    //　現在位置取得
    /*
    get_crntCoordinate(&crnt);
    printf("x=%f, y=%f, theta=%f\n",crnt.x, crnt.y, crnt.theta);
    */

}

