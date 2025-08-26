#ifndef __HIGHLEVEL_ACTIONS_H__
#define __HIGHLEVEL_ACTIONS_H__

#include "direction.h"
#include <err.h>

namespace highlevel_actions {
    extern const uint8_t motorI2cAddr;

    err::t init();

    /*!
        @brief Turn the lock all the way to the left/right. Block until the turn is done.
    */
    err::t turn(direction_t dir);
}

#endif