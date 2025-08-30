#ifndef __motor_h__
#define __motor_h__

#include "direction.h"

/*!
    @brief Module for controlling the lock motor.
*/
namespace motor {
    const uint8_t i2cAddr = 0x60;

    /*!
        @brief What's the motor currently doing?
    */
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
    
    /*!
        @brief What's the motor currently doing?
    */
    state_t state();

    /*!
        @brief Spin the motor in the given direction. It will continue spinning until shouldStop returns true.
        After starting the motor, this enters a loop where it calls idle and shouldStop. Whatever condition shouldStop
        checks for should cause an interrupt so that the call to idle returns.

        @param dir The direction to spin the motor in.
        @param shouldStopFn A function that will be called periodically while the motor spins. If it returns
        true, then the motor will be stopped immediately.
    */
    void spin(direction_t dir, should_stop_fn_t shouldStop);

    /*!
        @brief Spin the motor up to full speed in the given direction. This function will gradually increase the speed of the
        motor from 0 to the max speed. While doing so, this function calls the should_stop_fn_t passed to init, passing it
        dir. If that function returns true, then this function stops the motor and returns.

        If the motor is already spinning in the requested direction, this function will do nothing.

        If the motor is spinning in the opposite direction, this function will abruptly reverse the motor's direction.

        @param dir The direction to spin the motor in.
    */
    void spinUp(direction_t dir);

    /*!
        @brief Spin the motor down to a stop. This function will gradually decrease the speed of the motor
        from its current speed to 0.

        If the motor is already stopped, this function will do nothing.
    */
    void spinDown();
}

#endif