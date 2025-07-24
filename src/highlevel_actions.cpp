#include "highlevel_actions.h"
#include "motor.h"
#include "too_far.h"
#include "idle.h"

void highlevel_actions::turn(direction_t dir) {
    if (too_far::get(dir)) {
        // we're already fully turned in the requested direction
        motor::spinDown();
        return;
    }

    // spin
    const motor::state_t motor_state = motor::state();
    if (motor_state == motor::STATE_STOPPED) {
        motor::spinUp(dir);
    } else if (motor::stateToDirection(motor_state) == oppositeDirection(dir)) {
        motor::spinDown();
        motor::spinUp(dir);
    }
    
    // wait till we're all the way to dir
    while (!too_far::get(dir)) {
        idle();
    }

    // stop the motor
    motor::spinDown();
}