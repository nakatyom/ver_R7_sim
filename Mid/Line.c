#include "app.h"
#include "Line.h"
#include <stdio.h>

/* ライントレースタスク(100msec周期で関数コールされる) */
void tracer_task(intptr_t unused) {

    int16_t steering_amount; /* ステアリング操舵量の計算 */
    
    /* ステアリング操舵量の計算 */
    steering_amount = steering_amount_calculation();

    /* 走行モータ制御 */
    motor_drive_control(steering_amount);

    /* タスク終了 */
    ext_tsk();  //これなん
}

// RGBの最大値を返す関数
static uint16_t getMaxRGBValue(rgb_raw_t color) {
    uint16_t max = color.r; // 初期値として赤を設定
    if (color.g > max) {
        max = color.g;
    }
    if (color.b > max) {
        max = color.b;
    }
    return max;
}

/* ステアリング操舵量の計算 */
static int16_t steering_amount_calculation(void){

//    uint16_t  target_brightness; /* 目標輝度値 */
//    float32_t diff_brightness;   /* 目標輝度との差分値 */
//    int16_t   steering_amount;   /* ステアリング操舵量 */
    rgb_raw_t rgb_val;           /* カラーセンサ取得値 */
    rgb_val.r = 0;
    rgb_val.g = 0;
    rgb_val.b = 0;

    /* カラーセンサ値の取得 */
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_val);

    int bright = getMaxRGBValue(rgb_val) / RGB_MAX;    //明度の割合
    bright = bright * 100;  //明度の百分率

    // 差分を計算
    int diff = TAGE_BRIGHT - bright;

    // 加速分を計算
    int a = diff * 2; // 目標50%に対しての誤差なので2倍するといいんじゃね（適当）

    // 押さえつけ力を計算
    int b = 2 / a;

    // 誤差を蓄積
    c = c + diff;

    // 物は試し蓄積誤差を足すだけ
    int steering_amount = 0;
    steering_amount = a - b;
    steering_amount = steering_amount - c;

    if(steering_amount >= 60){
        c = -10;
        steering_amount = -10;
    }
    else if(steering_amount <= -60){
        c = 10;
        steering_amount = 10;
    }
    printf("a = %d \n b = %d \n c = %d \n steerring_amount = %d \n", a, b, c, steering_amount);
    /*__________________________今日はここまで_____________________________________*/

    
//    /* 目標輝度値の計算 */
//    //180 + 10
//    target_brightness = (WHITE_BRIGHTNESS + BLACK_BRIGHTNESS) / 2;
//
//    /* 目標輝度値とカラーセンサ値の差分を計算 */
//    //(float32_t)で型変換しながら目標輝度からRGBの緑の値を引く。
//    //でもなんでこれで輝度差分がわかるんだ
//    diff_brightness = (float32_t)(target_brightness - rgb_val.g);
//
//    /* ステアリング操舵量を計算 */
//    //輝度差分 * 0.2F
//    //Fを付けるとFloat型にした状態で記述することができる
//    //てかなんでこんなに型変換してんだ
//    steering_amount = (int16_t)(diff_brightness * STEERING_COEF);

    return steering_amount;


    /*
    1. 目標値を設定
       ⇒ どの色空間を使うかによる
       ⇒ 明度の割合にする。 50%

    2. 現在地を計算
       ⇒ RGB の計算から求める
       ⇒ 最大値の割合からだからすぐ 

    3. 差分を計算（誤差を計算） gosa
       ⇒ 1.2. が決まれば決まる

    4. 加速度を計算 a
       a = gosa * 2
    
    5. 押さえつけ力を計算 b
       b = gosa / saidai * 100
       b = 2 / gosa

    6. 誤差を蓄積
       gosa = gosa / 10
       c = c + gosa
       c = c / 10
       
    7. 押さえつけ力と蓄積誤差をもとに最終加速度を追加 kasoku
       d = a - b
       kasoku = d - c

    8. 加速度から制御量を計算
       

    */

}

/* 走行モータ制御 */
static void motor_drive_control(int16_t steering_amount){

    int left_motor_power, right_motor_power; /*左右モータ設定パワー*/

    /* 左右モータ駆動パワーの計算(走行エッジを右にする場合はRIGHT_EDGEに書き換えること) */
    left_motor_power  = (int)(BASE_SPEED + (steering_amount * LEFT_EDGE));
    right_motor_power = (int)(BASE_SPEED - (steering_amount * LEFT_EDGE));

    /* 左右モータ駆動パワーの設定 */
    ev3_motor_set_power(left_motor, left_motor_power);
    ev3_motor_set_power(right_motor, right_motor_power);

    return;
}