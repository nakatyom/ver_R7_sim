#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "ev3api.h"
#include "port.h"

/* log.txt pass */
#define FILE_PATH_LOG  "log.txt"

bool is_head_line = true;

const int target_dist = 100; //mm
const int wheel_r = 47; //mm

float crnt_dist = 0.0;
int left_power = 0;
int right_power = 0;
float motor_comp = 0.92;

/* private functions */
void printTimeStamp(FILE *fp, bool is_head){  // Assuming that the file is opened correctly. 
    struct timeval tvToday;
    struct tm *gm_time;

    /* get current time */
    gettimeofday(&tvToday, NULL); // Today
    gm_time = localtime(&tvToday.tv_sec);

    /* add Date Stamp */
    if(is_head == true){
        fprintf(fp, "Logging Date is...\n");
        fprintf(fp, "%d/%d/%d\n",gm_time->tm_year+1900,gm_time->tm_mon+1,gm_time->tm_mday);
        fprintf(fp, "%d:%d:%d | ",gm_time->tm_hour,gm_time->tm_min,gm_time->tm_sec);
        is_head = false;
    }

    /* add Time Stamp */
    else if(is_head == false){
        fprintf(fp, "%d:%d:%d | ",gm_time->tm_hour,gm_time->tm_min,gm_time->tm_sec);
    }
}

void printLog(int dist, uint8_t ref, rgb_raw_t* rgb_val){
    // open Target file.
    FILE *fp = fopen(FILE_PATH_LOG, "a");
    if(fp == NULL){
        printf("fail open file on get_line.c");
        return;
    }
    //write Log : Date(once)
    //            Time,data,data,...
    printTimeStamp(fp, is_head_line);
    fprintf(fp, "%d,", dist);
    fprintf(fp, "%d,", ref);
    fprintf(fp, "%d,%d,%d\n", rgb_val->r, rgb_val->g, rgb_val->b);

    // close Target file.
    fclose(fp);
}

/* external functions */
extern void get_line(){
    if(is_head_line == true){
        ev3_motor_reset_counts(right_motor);
    }

    /* 現在距離計算 */
    int32_t r_digree = ev3_motor_get_counts(right_motor);
    crnt_dist = wheel_r * (r_digree * 3.1415 / 180);

    /* センサ値を取得 */
    u_int8_t ref;
    rgb_raw_t rgb_val;
    ref = ev3_color_sensor_get_reflect(color_sensor);
    ev3_color_sensor_get_rgb_raw(color_sensor,&rgb_val);

    /* ロギング */
    printLog((int)crnt_dist, ref, &rgb_val);

    /* モータ駆動 */
    if((int)crnt_dist <= target_dist/2){
        if(left_power <= 40){
            left_power  += 2;
        }
        if(right_power <= 40){
            right_power += 2;
        }
    }
    else if((int)crnt_dist > target_dist/2 && (int)crnt_dist <= target_dist){
        if(left_power >= 0){
            left_power  += 2;
        }
        if(right_power >= 0){
            right_power += 2;
        }
    }
    else {
    ev3_motor_stop(left_motor, (bool_t)1);
    ev3_motor_stop(right_motor, (bool_t)1);
    ext_tsk(); //task終了
    }

    int left_power_t = (int)(motor_comp * left_power);
    ev3_motor_set_power(left_motor, left_power_t);
    ev3_motor_set_power(right_motor, right_power);
    printf("%.1f,%d,%d\n", crnt_dist, left_power_t, right_power);
}