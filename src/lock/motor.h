#ifndef __motor_h__
#define __motor_h__

#include "direction.h"

/*!
    @brief Module for controlling the lock motor.
*/
namespace motor {
    const uint8_t i2cAddr = 0x60;

    typedef enum {
        STATE_STOPPED = 0,
        STATE_TURNING_LEFT = 1,
        STATE_TURNING_RIGHT = 2,
    } state_t;

    direction_t stateToDirection(state_t);

    typedef bool(*should_stop_fn_t)(direction_t);

    /*!
        @brief Initialize the motor module. It takes a function that will be called by spinUp to determine
        if the motor should stop spinning early.
    */
    void init(should_stop_fn_t);
    
    state_t state();

    /*!
        @brief Spin the motor up to full speed in the given direction. This function will gradually increase the speed of the
        motor from 0 to MOTOR_SPEED. If the function passed to init ever returns true, the motor will stop spinning and the
        function will return.

        If the motor is already spinning in the requested direction, this function will do nothing.

        If the motor is spinning in the opposite direction, this function will abruptly reverse the motor's direction.

        @param dir The direction to spin the motor in.
    */
    void spinUp(direction_t dir);

    void spinDown();
}

#endif