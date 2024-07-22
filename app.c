#include "app.h"
#include "list_api.h"
#include <stdio.h>
#include "ev3api.h"

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
    

    uint8_t ambval=0;
    int colval=0;
    uint8_t refval=0;
    rgb_raw_t rgb_val;


    /* 左右モータ駆動パワーの設定 */
    ev3_motor_set_power(left_motor, 10);
    ev3_motor_set_power(right_motor, 10);

    ambval = ev3_color_sensor_get_ambient(color_sensor);
    colval = ev3_color_sensor_get_color(color_sensor);
    refval = ev3_color_sensor_get_reflect(color_sensor);
    
    ev3_color_sensor_get_rgb_raw(color_sensor,&rgb_val);

    printf("アンビエント値：%d",ambval);
    printf("\n");
    printf("色変数：%d",colval);
    printf("\n");
    printf("反射光：%d",refval);
    printf("\n");
    printf("R：%d G:%d B:%d\n",rgb_val.r,rgb_val.g,rgb_val.b);


    
    

}
