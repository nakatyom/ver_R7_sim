#include "app.h"
#include <stdio.h>
#include "ev3api.h"
#include "color_sensor.h"
#include "port_settings.h"


/* メインタスク(起動時にのみ関数コールされる) */
void main_task(intptr_t unused) {

    
    printf("Start MainTask!!\n");
    
    /* ライントレースタスクの起動 */
    /* write_log(); */
    commn_settings();    
    
    printf("Start Naka Task!!\n");
    sta_cyc(NAKA_TASK_CYC);

    /* タスク終了 */
    ext_tsk();
}



void naka_task(intptr_t unused){
    
    /* 左右モータ駆動パワーの設定 */
    ev3_motor_set_power(left_motor, 10);
    ev3_motor_set_power(right_motor, 10);

    drv_color_sensor();

}
