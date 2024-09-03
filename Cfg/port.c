#include "ev3api.h"
#include "port.h"

/* センサーの定義 */
extern const sensor_port_t touch_sensor = EV3_PORT_1;
extern const sensor_port_t color_sensor = EV3_PORT_2;
extern const sensor_port_t sonar_sensor = EV3_PORT_3;
extern const sensor_port_t gyro_sensor  = EV3_PORT_4;

/* モータの定義 */
extern const motor_port_t arm_motor     = EV3_PORT_A;
extern const motor_port_t left_motor    = EV3_PORT_C;
extern const motor_port_t right_motor   = EV3_PORT_B;

void set_portCfg(){
    /* センサー入力ポート設定 */
    ev3_sensor_config(touch_sensor ,TOUCH_SENSOR);
    ev3_sensor_config(color_sensor ,COLOR_SENSOR);
    ev3_sensor_config(sonar_sensor ,ULTRASONIC_SENSOR);
    ev3_sensor_config(gyro_sensor  ,GYRO_SENSOR);
    /* モーター出力ポート設定 */
    ev3_motor_config(arm_motor     ,LARGE_MOTOR);
    ev3_motor_config(left_motor    ,MEDIUM_MOTOR);
    ev3_motor_config(right_motor   ,MEDIUM_MOTOR);
}