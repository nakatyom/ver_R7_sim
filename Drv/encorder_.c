#include "ev3api.h"
#include "encorder_.h"
#include <stdio.h>

//モーターの角位置を取得してprintf
void counts_api(motor_port_t lm, motor_port_t rm){
    
    lm_counts = ev3_motor_get_counts(lm);
    rm_counts = ev3_motor_get_counts(rm);

    printf("Motor %d's counts %d, \n Motor %d's counts %d."lm, lm_counts, rm, rm_counts);
    
}