#ifdef __cplusplus
extern "C" {
#endif
#include "ev3api.h"
#include "color_sensor.h"
#include "port_settings.h"
#include "ultrasonic_sensor.h"
#include "gyro_sensor.h"
#include <stdio.h>

/* タスク優先度 */
#define MAIN_PRIORITY    (TMIN_APP_TPRI + 1) /* メインタスク */
#define TRACER_PRIORITY  (TMIN_APP_TPRI + 2) /* ライントレースタスク */
#define NAKANO_PRIORITY   (TMIN_APP_TPRI + 3)

/* タスク周期の定義 */
#define LINE_TRACER_PERIOD  (100 * 1000) /* ライントレースタスク:100msec周期 */

#ifndef STACK_SIZE
#define STACK_SIZE      (4096)
#endif /* STACK_SIZE */

#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void naka_task(intptr_t exinf);


#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
