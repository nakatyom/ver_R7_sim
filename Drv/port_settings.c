#include <stdio.h>
#include "app.h"
#include "ev3api.h"
#include "color_sensor.h"
#include "port_settings.h"




/* センサ及びモーターの共通API定義 
   本関数は、固定となる。*/
void commn_settings() {
    /* センサー入力ポートの設定 */
    ev3_sensor_config(touch_sensor ,TOUCH_SENSOR);
    ev3_sensor_config(color_sensor ,COLOR_SENSOR);
    ev3_sensor_config(sonar_sensor ,ULTRASONIC_SENSOR);
    ev3_sensor_config(gyro_sensor  ,GYRO_SENSOR);
    
    /* モーター出力ポートの設定 */
    ev3_motor_config(arm_motor     ,LARGE_MOTOR);
    ev3_motor_config(left_motor    ,MEDIUM_MOTOR);
    ev3_motor_config(right_motor   ,MEDIUM_MOTOR);
    
}

void get_commn_sensor_settings(){
    /*解析したいポートNoを指定すると、接続されているセンサタイプが返ってくる。*/
    printf("port No1:%d\n",ev3_sensor_get_type(1));
    printf("port No2:%d\n",ev3_sensor_get_type(2));
    printf("port No3:%d\n",ev3_sensor_get_type(3));
    printf("port No4:%d\n",ev3_sensor_get_type(4));

}

