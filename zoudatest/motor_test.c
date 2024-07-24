#include "ev3api.h"
#include <stdio.h>
#include "motor_test.h"

int i, set_lpower, set_rpower, left_power, right_power;

/* モーター情報表示 */
void motorLog(motor_port_t lm ,motor_port_t rm){
    
    int left_power = ev3_motor_get_power(lm);
    int right_power = ev3_motor_get_power(rm);
    
    printf("lm's port is %d, and it's power is %d \n",ev3_motor_get_type(lm),left_power);
    printf("rm's port is %d, and it's power is %d \n",ev3_motor_get_type(rm),right_power);
}

//スピード・向きを変えながら進む
void goetrobo(motor_port_t lm ,motor_port_t rm){
    //モーターの出力設定

    switch(i){
        case 0 : set_lpower = 10; set_rpower = 10; i++; break;
        case 1 : set_lpower = 10; set_rpower = 10; i++; break;
        case 2 : set_lpower = 5; set_rpower = 10; i++; break;
        case 3 : set_lpower = 0; set_rpower = 10; i++; break;
        case 4 : set_lpower = -10; set_rpower = 10; i++; break;
        case 5 : set_lpower = 0; set_rpower = 0; i++; break;
        case 6 : set_lpower = 10; set_rpower = 5; i++; break;
        case 7 : set_lpower = 20; set_rpower = 10; i=1; break;
    }

    //モーター出力
    ev3_motor_set_power(lm, set_lpower);
    ev3_motor_set_power(rm, set_rpower);
}