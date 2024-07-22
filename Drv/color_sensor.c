#include "ev3api.h"
#include "color_sensor.h"
#include "port_settings.h"
#include <stdio.h>
#include "app.h"


void drv_color_sensor(intptr_t unused){

    uint8_t ambval=0;
    int colval=0;
    uint8_t refval=0;
    rgb_raw_t rgb_val;

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