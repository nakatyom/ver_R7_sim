#define motor_test_H

#include "ev3api.h"
//#include "etroboc_ext.h"
#include "app.h"

#include <stdio.h>

//関数プロトタイプ宣言
extern void motorLog();
extern void goetrobo();

extern int i;

/* モーター情報表示 */
void motorLog(){
    
    int left_power, right_power;
    
    left_power = ev3_motor_get_power(left_motor);
    right_power = ev3_motor_get_type(right_motor);
    
    printf("left_motor's port is %d, and it's power is %d \n",ev3_motor_get_type(left_motor),left_power);
    printf("right_motor's port is %d, and it's power is %d \n",ev3_motor_get_type(right_motor),right_power);
}

//スピード・向きを変えながら進む
void goetrobo(){
    //モーターの出力設定
    int set_lpower, set_rpower;
    switch(i){
        case 1 : set_lpower = 10; set_rpower = 10; i++; break;
        case 2 : set_lpower = 5; set_rpower = 10; i++; break;
        case 3 : set_lpower = 0; i++; set_rpower = 10; break;
        case 4 : set_lpower = -10; set_rpower = 10; i++; break;
        case 5 : set_lpower = 0; set_rpower = 0; i++; break;
        case 6 : set_lpower = 10; set_rpower = 5; i++; break;
        case 7 : set_lpower = 20; set_rpower = 10; i=1; break;
    }

    //モーター出力
    ev3_motor_set_power(left_motor, set_lpower);
    ev3_motor_set_power(right_motor, set_rpower);
}
