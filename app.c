#include <stdio.h>

#include "app.h"
#include "ev3api.h"
#include "port.h"
#include "odometry.h"

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

#include "motor.h"

int test = 1;
struct coordinate crnt = {0.0, 0.0, 0.0};

void boss_task(intptr_t exinf){
    if(test == 0){
    static int32_t le, re;
    le = motor_get_counts(left_motor);
    re = motor_get_counts(right_motor);
    printf("left_enc:%d, right_enc:%d\n", (int)le, (int)re);
    }
    
    else if(test == 1){
        get_crntCoordinate(&crnt);
        //printf("x=%f, y=%f, theta=%f\n",crnt.x, crnt.y,crnt.theta);
    }




}