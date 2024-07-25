#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>

/* 関数プロトタイプ宣言 */
static int16_t steering_amount_calculation(void);
static void motor_drive_control(int16_t);
static uint16_t getMaxRGBValue(rgb_raw_t);

extern void tracer_task(intptr_t unused);

// グローバル変数宣言
extern int c = 0;

/* 下記の項目は各ロボットに合わせて変えること */
// RGBの最大値255
#define RGB_MAX 255

#define TAGE_BRIGHT 50

/* カラーセンサの輝度設定 */
// #define WHITE_BRIGHTNESS  (180)
// #define BLACK_BRIGHTNESS  (10) 

/* ステアリング操舵量の係数 */
// #define STEERING_COEF     (0.2F) 

/* 走行基準スピード */
#define BASE_SPEED 20

/* ライントレースエッジ切り替え */
#define LEFT_EDGE -1
#define RIGHT_EDGE 1

#endif

/*
#defineは「マクロ」。まけするときに前に書いてある奴が後ろに書いてある奴に置き換わる。
#define A b;
と書いてあったら以降のAはbとして処理される。
マクロ名（今回のA）は慣習として大文字アルファベット。
ヘッダに書いてあってもソースで読み込めばそのまま使える。
*/

/*
staticは同じなまえ.hに定義すれば同じ名前.cからなら参照可能らしい（GPT調べ）
*/

/*
ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_val);
の&rgb_valの部分はrgb_valのアドレスを渡してる。
アドレスを渡すから値を呼ぶこともできるし戻すこともできる。
すなわち戻り値と似たようなもんで、rgb_valに値が入っている。
ちなみに&がアドレス演算子ってやつで、これを使うとその変数のアドレスを表現できる。
*/

/*
数字の後に f や F や l や L を付けるのが接尾語
F を付けるとFloat型を指定できる。
*/