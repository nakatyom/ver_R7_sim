#ifndef _STATE_H
#define _STATE_H

#include "ev3api.h"


extern enum state{
    PRE_START       = 0,
    LAP_RUN         = 1,
    DUBBLE_LOOP     = 2,
    DEBRI_REMOVE    = 3,
    SMART_CARRY     = 4
};

#endif /* _PORT_H */