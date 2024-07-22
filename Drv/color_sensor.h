#ifndef _COLOR_SENSOR_H
#define _COLOR_SENSOR_H
#include <stdio.h>
#include "app.h"
#include "color_sensor.h"
#include "port_settings.h"

extern void drv_color_sensor_color_sensor();

extern uint8_t ambval;
extern int colval;
extern uint8_t refval;
extern rgb_raw_t rgb_val;


#endif	// _COLOR_SENSOR