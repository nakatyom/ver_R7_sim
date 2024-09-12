#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "ev3api.h"
#include "odometry.h"
#include "smartCarry.h"
#include "port.h"

bool_t turn_flag = false;
bool_t drive_flag = false;
bool_t init_flag = false; 
int tgt_angl = 0;
int now_angle = 0;
int tgt_dist = 0;
int now_dist = 0;
float now_x=0.0f;
float now_y=0.0f;
int roop_cnt=0;
struct coordinate crnt = {0.0, 0.0, 0.0};//自己位置座標

// 走行距離計算処理
extern int dist(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt((dx * dx) + (dy * dy));
}

// 旋回角度算出処理
extern int calc_angle(float tgt_pos_x, float tgt_pos_y){
    static const float PI = 3.14159265358979323846264338327950288;   
    float tgt_angle=0;
    struct coordinate crnt = {0.0, 0.0, 0.0};
    get_crntCoordinate(&crnt);

    // 処理1 原点移動    
    // (x座標)
    //tgt_pos_x = tgt_pos_x - crnt.x;
    //tgt_pos_y = tgt_pos_y - crnt.y;
    
    
    if (crnt.x <= 0.0f){
        tgt_pos_x = tgt_pos_x + abs(crnt.x);
    }else if(crnt.x > 0.0f){
        tgt_pos_x = tgt_pos_x - abs(crnt.x);
    }
    else{
        printf("旋回角度算出中に異常発生");
    }
    // (y座標)
    if (crnt.y <= 0.0f){
        tgt_pos_y = tgt_pos_y + abs(crnt.y);
    }else if(crnt.y > 0.0f){
        tgt_pos_y = tgt_pos_y - abs(crnt.y);
    }else{
        printf("旋回角度算出中に異常発生");
    }

    // 処理2角度計算
    // ロボからみた目的地が第1象限の時。(x,y) (+,+)
    printf("\n平行移動後の座標(%d,%d)",(int)tgt_pos_x,(int)tgt_pos_y);
    if (tgt_pos_x > 0 && tgt_pos_y >= 0){
        printf("| 第1象限\n");
        if (tgt_pos_y == 0){
            tgt_angle=0;
        }else{
            tgt_angle=atan2(tgt_pos_x,tgt_pos_y);
            tgt_angle = tgt_angle * (180 / PI);
            tgt_angle = tgt_angle - 270; 
        }

    }else if (tgt_pos_x <= 0 && tgt_pos_y > 0){
    // ロボからみた目的地が第2象限の時。(x,y) (-,+)
        printf("| 第2象限\n");
        if (tgt_pos_x == 0){
            tgt_angle=-270;
        }else{
            tgt_angle=atan2(tgt_pos_y,tgt_pos_x);
            tgt_angle = tgt_angle * (180 / PI);
            tgt_angle = tgt_angle; 
        }
    }else if (tgt_pos_x < 0 && tgt_pos_y <= 0){
    // ロボからみた目的地が第3象限の時。(x,y) (-,-)
        printf("| 第3象限\n");
        if (tgt_pos_y == 0){
            tgt_angle=-180;
        }else{
            tgt_angle=atan2(tgt_pos_x,tgt_pos_y);
            tgt_angle = tgt_angle * (180 / PI);
            tgt_angle = tgt_angle; 
        }
    }else if (tgt_pos_x >= 0 && tgt_pos_y < 0){
    // ロボからみた目的地が第4象限の時。(x,y) (+,-)
        printf("| 第4象限\n");
        if (tgt_pos_x == 0){
            tgt_angle=-90;
        }else{
            tgt_angle= atan2(tgt_pos_y,tgt_pos_x);
            tgt_angle = tgt_angle * (180 / PI);
            tgt_angle = tgt_angle;
        }
    }

    
    //tgt_angle = atan((tgt_pos_y-crnt.y)/(tgt_pos_x-crnt.x));
    printf("(x,y)=(%f,%f) | ",tgt_pos_x,tgt_pos_y);
    

    //printf("(x,y)=(%f,%f) , theta=%f\n 目標位置(100,100)への角度：%f°\n",crnt.x, crnt.y,crnt.theta,tgt_angle);
    //printf("(----------------------------------------------------\n");

    return (int)tgt_angle;
}

// 旋回処理[暫定]
extern bool_t proc_turn(int now_angle, int tgt_angle){
    bool_t flag = false;
    //get_crntCoordinate(&crnt);
    if (now_angle < tgt_angle){
        ev3_motor_set_power(left_motor,  -5);
        ev3_motor_set_power(right_motor, 5);
        //printf("処理：分岐１");
        printf("比較角度(現在( %d° ):目標( %d° )\n",now_angle,tgt_angle);
        flag =false;
    }
    else if (now_angle > tgt_angle){
        ev3_motor_set_power(left_motor,  10);
        ev3_motor_set_power(right_motor, -10);
        //printf("処理：分岐２");
        printf("比較角度(現在( %d° ):目標( %d° )\n",now_angle,tgt_angle);
        flag =false;
    }else{
        printf("旋回完了\n");

        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        flag =true;
    }
    return flag;
}

// 走行処理[暫定]
extern bool_t proc_run(float now_dist,float tgt_dist){
    printf("距離情報(現在( %d mm):目標( %d mm )\n",(int)now_dist,(int)tgt_dist);
    bool_t flag = false;
    if (now_dist < tgt_dist){
        ev3_motor_set_power(left_motor,  20);
        ev3_motor_set_power(right_motor, 20);
        flag = false;
    }
    else if (now_dist > tgt_dist){
        ev3_motor_set_power(left_motor,  -2);
        ev3_motor_set_power(right_motor, -2);
        flag = false;
    }else{
        printf("走行完了");
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        flag = true;
    }
    return flag;
}

// 攻略
void hello_neo(){
    // 定数宣言（目標座標の配列
//    float x_pos_target[6]={420.0,580.0,860.0,-1040.0,-720.0,-1360.0,-1300.0,-1000.0,0};
//    float y_pos_target[6]={-140.0,-520.0,-240.0,-260.0,-780.0,-1040.0,-560.0,-260.0,-260.0};
//    float x_pos_target[6]={420.0,580.0,240.0,-484.0,-720.0,-1360.0,-560.0,-260.0,0};
//    float y_pos_target[6]={-140.0,-520.0,-860.0,-836.0,-780.0,-1040.0,-1300.0,-1000.0,-260.0};
    float x_pos_target[6]={420.0,520.0,240.0,-220.0,-780.0,-1040.0,-560.0,-260.0,-260.0};
    float y_pos_target[6]={-140.0,-580.0,-860.0,-760.0,-720.0,-1360.0,-1300.0,-720.0,0.0};
    get_crntCoordinate(&crnt);

    // 変数宣言
    now_angle = (int)crnt.theta;    
    printf("x=%f, y=%f, theta=%f | ",crnt.x, crnt.y, crnt.theta);
    // 旋回処理
    if (init_flag == false){ 
        tgt_angl = calc_angle(x_pos_target[roop_cnt],y_pos_target[roop_cnt]);
        init_flag = true;
    }else if (init_flag == true && turn_flag==false && drive_flag==false){
        turn_flag=proc_turn(now_angle,tgt_angl);
        if(turn_flag==true){
            tgt_dist = dist(crnt.x,crnt.y,x_pos_target[roop_cnt],y_pos_target[roop_cnt]);
            now_x = crnt.x;
            now_y = crnt.y;
        }
    }
    // 走行処理
    if (turn_flag==true && drive_flag==false){
        now_dist = dist(crnt.x,crnt.y,now_x,now_y);
        //tgt_dist = (int)dist(crnt.x,crnt.y,x_pos_target[roop_cnt],y_pos_target[roop_cnt]);
        drive_flag = proc_run(now_dist,tgt_dist);

    }
    if (turn_flag==true && drive_flag==true){
        roop_cnt = roop_cnt + 1;
        turn_flag = false;
        drive_flag = false;
        init_flag = false;
        printf("移動完了(ループ%d回)\n",roop_cnt);
        printf("現在座標（%f,%f）\n",crnt.x,crnt.y);
        printf("目標座標（%f,%f）\n",x_pos_target[roop_cnt],y_pos_target[roop_cnt]);
    }

}

