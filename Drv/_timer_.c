#include <time.h>
#include "_timer_.h"

/* extern functions */
void delay_connect(int milliseconds){
    clock_t start_time = clock();
    while(clock() < start_time + milliseconds);
}
