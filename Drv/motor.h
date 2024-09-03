#ifndef _MOTOR_H
#define _MOTOR_H

#include "ev3api.h"

/* extern functions */
extern int32_t motor_get_counts(motor_port_t port);
extern int32_t motor_get_pre_counts(motor_port_t port);
extern void motor_reset_counts(motor_port_t port);
extern void motor_set_power(motor_port_t port, int power);
extern int  motor_get_power(motor_port_t port);
extern int  motor_get_pre_power(motor_port_t port);
extern void motor_stop(motor_port_t port);

#endif /* _MOTOR_H */