#ifndef __direction_h__
#define __direction_h__

#include <Adafruit_MotorShield.h>

typedef enum
{
    DIRECTION_NONE = 0,
    DIRECTION_LEFT = BACKWARD,
    DIRECTION_RIGHT = FORWARD,
} direction_t;

static inline direction_t oppositeDirection(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        return DIRECTION_RIGHT;
    case DIRECTION_RIGHT:
        return DIRECTION_LEFT;
    default:
        return DIRECTION_NONE;
    }
}

#endif