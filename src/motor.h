#ifndef __motor_h__
#define __motor_h__

#include "direction.h"

namespace motor {
    typedef enum {
        STATE_STOPPED = 0,
        STATE_TURNING_LEFT = 1,
        STATE_TURNING_RIGHT = 2,
    } state_t;

    typedef bool(*should_stop_fn_t)(direction_t);

    void init(should_stop_fn_t);
    state_t state();
    void spinUp(direction_t);
    void spinDown();
}

#endif