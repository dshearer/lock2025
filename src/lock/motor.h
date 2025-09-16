#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "direction.h"
#include <err.h>

namespace motor {
    const uint8_t motorI2cAddr = 0x60;;

    /*!
        @brief Connect to the motor controller over I2C. Must be called before any other functions in this namespace.
        Prepare for the motor to be used.
        @return An error code indicating success or failure.
    */
    err::t init();

    /*!
        @brief Turn the lock all the way to the left/right. Block until the turn is done.
        @param dir The direction to turn. Must be DIRECTION_LEFT or DIRECTION_RIGHT.
        @return An error code indicating success or failure.
    */
    err::t turn(direction_t dir);
}

#endif