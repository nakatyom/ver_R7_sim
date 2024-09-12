#include <stdio.h>
#include <math.h>


#include "motor.h"
#include "ev3api.h"
#include "port.h"
#include "_timer_.h"

// 0:Arm, 1:Right, 2:Left
int32_t crnt_enc[3] = {0, 0, 0};
int32_t pre_enc[3]  = {0, 0, 0};

int crnt_power[3] = {0, 0, 0};
int pre_power[3]  = {0, 0, 0};

// Multiply left motor 0.0~1.0
const float motor_sync = 1.0;


/* static functions */
int is_ValidInput(motor_port_t port, char* func_name){
    if(port != arm_motor && port != right_motor && port != left_motor){
        printf("An invalid value entered in %s.\n",func_name);
        return 1;
    }

    return 0;
}

int checkMissingData(motor_port_t port){
    // 通信遅れ判定の場合、1ms待って再取得
    if(crnt_enc[port] == pre_enc[port]){ // 前回値と一致
        if((pre_power[port] != 0 || crnt_power[port] != 0)){ // モーターパワーが0でない 
            // delay_connect(1); // 1ms待つ
            return 0;
        }
    }
    return 1;
}


/* extern functions */
extern int32_t motor_get_counts(motor_port_t port){
    char func_name[] = "motor_get_counts";
    if(1 == is_ValidInput(port, func_name)) return 0;
    
    // 前回値の更新
    pre_enc[port] = crnt_enc[port];

    // 現在値の取得(通信遅れ判定の場合、1ms待って再取得)
    for(int i=0; i<1; i++){
        crnt_enc[port] = ev3_motor_get_counts(port);
        // i += checkMissingData(port);
    }


    return crnt_enc[port];
}

int32_t motor_get_pre_counts(motor_port_t port){
    char func_name[] = "motor_get_pre_counts";
    if(1 == is_ValidInput(port, func_name)) return 0;

    return pre_enc[port];
}

void motor_reset_counts(motor_port_t port){
    char func_name[] = "motor_reset_counts";
    if(1 == is_ValidInput(port, func_name)) return;

    ev3_motor_reset_counts(port);
    crnt_enc[port] = 0;
    pre_enc[port]  = 0;
    return;
}

/* パワー設定 */
void motor_set_power(motor_port_t port, int power){
    char func_name[] = "motor_set_power";
    if(1 == is_ValidInput(port, func_name)) return;
    
    // パワーを設定
    ev3_motor_set_power(port, power);

    // 前回値,現在値の更新
    pre_power[port]  = crnt_power[port];
    crnt_power[port] = ev3_motor_get_power(port);

    return;
}

void motor_stop(motor_port_t port){
    char func_name[] = "motor_stop";
    if(1 == is_ValidInput(port, func_name)) return;

    // モータを停止
    ev3_motor_stop(port, true);

    // 前回値,現在値の更新
    pre_power[port]  = crnt_power[port];
    crnt_power[port] = 0;
}

int motor_get_power(motor_port_t port){
    char func_name[] = "motor_get_power";
    if(1 == is_ValidInput(port, func_name)) return 0;
    
    /* 前回値, 現在値の更新 */
    pre_power[port]  = crnt_power[port];

    // 現在値の取得(通信遅れ判定の場合、1ms待って再取得)
    for(int i=0; i<1; i++){
        crnt_power[port] = ev3_motor_get_power(port);
        i += checkMissingData(port);
    }

    return crnt_power[port];
}

int motor_get_pre_power(motor_port_t port){
    char func_name[] = "motor_get_pre_power";
    if(1 == is_ValidInput(port, func_name)) return 0;
    
    return pre_power[port];
}


/* using_sensor_task */
void motor_update(motor_port_t port){
    char func_name[] = "motor_update_counts";
    if(1 == is_ValidInput(port, func_name)) return;

    // 前回値の更新
    pre_enc[port] = crnt_enc[port];
    pre_power[port]  = crnt_power[port];

    // 現在値の取得(通信遅れ判定の場合、1ms待って再取得)
    for(int i=0; i<1; i++){
        crnt_enc[port] = ev3_motor_get_counts(port);
        crnt_power[port] = ev3_motor_get_power(port);
        i += checkMissingData(port);
    }
    return;
}