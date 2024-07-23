#include "ev3api.h"
#include "port_settings.h"
#include <stdio.h>
#include "app.h"


void drv_gyro_sensor(){
    int16_t angle_val;
    int16_t rate_val;
    angle_val = ev3_gyro_sensor_get_angle(gyro_sensor);
    rate_val = ev3_gyro_sensor_get_rate(gyro_sensor);

    printf("Angle：%d , Rate:%d\n",angle_val,rate_val);
    
}

void drv_gyro_reset(){
    ev3_gyro_sensor_reset(gyro_sensor);
    printf("Reset Gyro Sensor\n");
}

