#ifndef _PORT_SETTINGS_H
#define _PORT_SETTINGS_H
#include <stdio.h>
#include "ev3api.h"
#include "app.h"

extern void commn_settings();

/* センサーポートの定義 */
static const sensor_port_t
    touch_sensor    = EV3_PORT_1,
    color_sensor    = EV3_PORT_2,
    sonar_sensor    = EV3_PORT_3,
    gyro_sensor     = EV3_PORT_4;

/* モーターポートの定義 */
static const motor_port_t
    arm_motor       = EV3_PORT_A,
    left_motor      = EV3_PORT_C,
    right_motor     = EV3_PORT_B;

    
#endif	// _PORT_SETTINGS_H