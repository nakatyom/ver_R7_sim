#include "app.h"
#include <stdio.h>
#include "ev3api.h"
#include "color_sensor.h"
#include "port_settings.h"
#include "ultrasonic_sensor.h"
#include "gyro_sensor.h"


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


//uint8_t cnt;
void naka_task(intptr_t unused){
    /*    
    cnt = cnt + 1; 
    if (cnt >= 250) {
        cnt = 0;
        drv_gyro_reset();
    }
    printf("Main counter:%d\n",cnt);*/

    /* 左右モータ駆動パワーの設定 */
    //ev3_motor_set_power(left_motor, -90);
    //ev3_motor_set_power(right_motor, 90);

    // カラーセンサのテスト関数
    //drv_color_sensor();
    
    // 共通センサのget関数
    //get_commn_sensor_settings();
    
    // 超音波センサのテスト関数
    //drv_get_ultrasonic();

    // ジャイロセンサのテスト関数
    printf("call gyro___");
    drv_gyro_sensor();

}
