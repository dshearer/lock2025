#ifndef __too_far_h__
#define __too_far_h__

#include "direction.h"

namespace too_far {
    void init();

    /*!
        @brief Get whether the lock is too far in the given direction.
        @param dir The direction to check. Must be DIRECTION_LEFT or DIRECTION_RIGHT.
        @return true if the lock is too far in the given direction, false otherwise.
    */
    bool get(direction_t);
};

#endif