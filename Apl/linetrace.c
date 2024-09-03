#include "ev3api.h"
#include "port.h"

const int lr_sw = -1; // L_COURSE:1, R_COURSE:-1
const float KP = 0.7;
const float KD = 0.3;
int BASE = 40;        // base speed
const u_int8_t target = 60;

int pre_err = 0;
float motor_compensater = 0.92;

void linetrace(){

    /* 反射強度の取得 */
    u_int8_t crnt = ev3_color_sensor_get_reflect(color_sensor);

    /* err計算 */
    int err = target - crnt;

    int out;
    if(err >= 5 || err <= -5){
        /* P or PD計算 */
        //int out = (int)( KP * err );
        out = (int)( KP * err + KD * (err-pre_err) );
    }
    else{
        /*補正ゼロ(直進する)*/
        out = 0;
    }
    pre_err = err;

    /* パワー計算 */
    int left_power  = BASE + lr_sw * out;
    int right_power = BASE - lr_sw * out;

    /* モータ出力 */
    left_power = (int)(motor_compensater * left_power);
    ev3_motor_set_power(left_motor, left_power);
    ev3_motor_set_power(right_motor, right_power);

    printf("%d, %d : ", crnt, err);
    printf("%d, %d\n", left_power, right_power);
}