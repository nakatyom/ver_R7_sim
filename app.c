#include "app.h"
#include <stdio.h>
#include "ev3api.h"
#include "port_settings.h"
#include "linetrace.h"

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
    //printf("10ms Task");
    mid_linetrace_pid();


}
