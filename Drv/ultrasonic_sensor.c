#include "ev3api.h"
#include "color_sensor.h"
#include "port_settings.h"
#include <stdio.h>
#include "app.h"

/* ev3_ultrasonic_sensor_listen() 超音波検出判定はRaspikeでは利用不可 */
void drv_get_ultrasonic(){

    int16_t sens_val;
    sens_val = ev3_ultrasonic_sensor_get_distance(sonar_sensor);

    printf("distance Value:%d\n",sens_val);
}

