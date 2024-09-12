#include <stdio.h>

#include "ev3api.h"
#include "port.h"
#include "color.h"

 uint8_t crnt_ref = 0;
 /* uint16_t r, g, b */
 rgb_raw_t crnt_rgb = {0, 0, 0};

/* static functions */


/* extern functions */
uint8_t color_sensor_get_reflect(sensor_port_t port){
    if(port != color_sensor){
        printf("An invalid value entered in color_sensor_get_reflect().\n");
        return 0;
    }

    /* 現在値の取得 */
    crnt_ref = ev3_color_sensor_get_reflect(port);

    return crnt_ref;
}

void color_sensor_get_rgb_raw(sensor_port_t port, rgb_raw_t* rgb_val){
    if(port != color_sensor){
        printf("An invalid value entered in color_sensor_get_rgb_raw().\n");
        return;
    }

    /* 現在値の取得 */
    ev3_color_sensor_get_rgb_raw(port, &crnt_rgb);
    rgb_val->r = crnt_rgb.r;
    rgb_val->g = crnt_rgb.g;
    rgb_val->b = crnt_rgb.b;

    return;
}


/* using_sensor_task */
void color_sensor_update(sensor_port_t port){
    if(port != color_sensor){
        printf("An invalid value entered in color_sensor_update().\n");
        return;
    }

    /* 現在値の取得 */
    crnt_ref = ev3_color_sensor_get_reflect(port);
    ev3_color_sensor_get_rgb_raw(port, &crnt_rgb);

    return;
}