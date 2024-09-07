#ifndef _GYRO_H
#define _GYRO_H

#include "ev3api.h"

/* extern functions */
extern int16_t gyro_sensor_get_angle(sensor_port_t port);
extern int16_t gyro_sensor_get_pre_angle(sensor_port_t port);
extern int16_t gyro_sensor_get_rate(sensor_port_t port);
extern int16_t gyro_sensor_get_pre_rate(sensor_port_t port);

#endif /* _GYRO_H */