#ifndef __direction_h__
#define __direction_h__

#include <Adafruit_MotorShield.h>

typedef enum
{
    DIRECTION_NONE = 0,
    DIRECTION_LEFT = BACKWARD,
    DIRECTION_RIGHT = FORWARD,
} direction_t;

static inline const char* directionToString(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        return "left";
    case DIRECTION_RIGHT:
        return "right";
    default:
        return "unknown";
    }
}

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

const direction_t ALL_DIRECTIONS[] = {
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
};

#endif