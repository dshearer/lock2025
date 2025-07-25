#ifndef __HIGHLEVEL_ACTIONS_H__
#define __HIGHLEVEL_ACTIONS_H__

#include "direction.h"

namespace highlevel_actions {
    void init();

    /*!
        @brief Turn the lock all the way to the left/right. Block until the turn is done.
    */
    void turn(direction_t dir);
}

#endif