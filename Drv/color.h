#ifndef _COLOR_H
#define _COLOR_H

#include "ev3api.h"

/* extern functions */
extern uint8_t color_sensor_get_reflect(sensor_port_t port);
extern void    color_sensor_get_rgb_raw(sensor_port_t port, rgb_raw_t* rgb_val);

/* using_sensor_task */
extern void color_sensor_update(sensor_port_t port);

#endif /* _COLOR_H */