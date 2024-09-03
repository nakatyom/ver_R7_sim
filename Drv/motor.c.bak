#include <stdio.h>
#include <math.h>
#include <time.h>

#include "motor.h"
#include "ev3api.h"
#include "port.h"

// 0:Arm, 1:Right, 2:Left
int32_t crnt_enc[3] = {0, 0, 0};
int32_t pre_enc[3]  = {0, 0, 0};

int crnt_power[3] = {0, 0, 0};
int pre_power[3]  = {0, 0, 0};

// Multiply left motor 0.0~1.0
const float motor_sync = 1.0;


/* static functions */
void delay_connect_m(int milliseconds){
    clock_t start_time = clock();
    while(clock() < start_time + milliseconds);
}

/* extern functions */
extern int32_t motor_get_counts(motor_port_t port){
    if(port != arm_motor && port != right_motor && port != left_motor){
        printf("An invalid value entered in motor_get_counts().\n");
        return 0;
    }
    
    // 前回値の更新
    pre_enc[port] = crnt_enc[port];

    // 現在値の取得
    crnt_enc[port] = ev3_motor_get_counts(port);

    // 通信遅れ判定の場合、1ms待って再取得
    if(crnt_enc[port] == pre_enc[port] && (pre_power[port] != 0 || crnt_power[port] != 0)){ // 前回値と一致かつモーターパワーが0でない
            delay_connect_m(1); // 1ms待つ
            crnt_enc[port] = ev3_motor_get_counts(port);
    }

    return crnt_enc[port];
}

extern int32_t motor_get_pre_counts(motor_port_t port){
    if(port != arm_motor && port != right_motor && port != left_motor){
        printf("An invalid value entered in motor_get_pre_counts().\n");
        return 0;
    }

    return pre_enc[port];
}

extern void motor_reset_counts(motor_port_t port){
    ev3_motor_reset_counts(port);
    return;
}

extern void motor_set_power(motor_port_t port, int power){
    if(port != arm_motor && port != right_motor && port != left_motor){
        printf("An invalid value entered in motor_set_power().\n");
        return;
    }
    // 左モータの場合、パワーに補正をかける
    if(port == left_motor){
         power = (int)(motor_sync * power);
    }
    
    // パワーを設定
    ev3_motor_set_power(port, power);

    // 前回値,現在値の更新
    pre_power[port]  = crnt_power[port];
    crnt_power[port] = ev3_motor_get_power(port);

    return;
}

extern void motor_stop(motor_port_t port){
    if(port != arm_motor && port != right_motor && port != left_motor){
        printf("An invalid value entered in motor_stop().\n");
        return;
    }

    // モータを停止
    ev3_motor_stop(port, true);

    // 前回値,現在値の更新
    pre_power[port]  = crnt_power[port];
    crnt_power[port] = ev3_motor_get_power(port);
}

extern int  motor_get_power(motor_port_t port){
    if(port != arm_motor && port != right_motor && port != left_motor){
        printf("An invalid value entered in motor_get_power().\n");
        return;
    }
    
    /* 前回値, 現在値の更新 */
    pre_power[port]  = crnt_power[port];
    crnt_power[port] = ev3_motor_get_power(port);

    return crnt_power[port];
}

extern int  motor_get_pre_power(motor_port_t port){
    if(port != arm_motor && port != right_motor && port != left_motor){
        printf("An invalid value entered in motor_get_pre_power().\n");
        return;
    }
    
    return pre_power[port];
}