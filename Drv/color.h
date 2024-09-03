#ifndef _COLOR_H
#define _COLOR_H

#include "ev3api.h"

/* extern functions */
uint8_t color_sensor_get_reflect(sensor_port_t port);
uint8_t color_sensor_get_rgb_raw(sensor_port_t port);

#endif /* _COLOR_H */