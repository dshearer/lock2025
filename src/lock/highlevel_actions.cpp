#include "highlevel_actions.h"
#include "motor.h"
#include "too_far.h"
#include "idle.h"
#include "led.h"
#include "timer.h"
#include <err.h>

#define TURN_TIMEOUT_MS 5000

const uint8_t highlevel_actions::motorI2cAddr = motor::i2cAddr;

static unsigned long gMillisTillCenter = 0;

static bool shouldStopSpinning(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        return too_far::get(DIRECTION_LEFT);
    case DIRECTION_RIGHT:
        return too_far::get(DIRECTION_RIGHT);
    default:
        return false;
    }
}

static err::t justTurn(direction_t dir, bool* turned = nullptr) {
    if (too_far::get(dir)) {
        // we're already fully turned in the requested direction
        motor::spinDown();
        if (turned != nullptr) {
            *turned = false;
        }
        return err::OK;
    }

    // get timer
    Timer *timer = Timer::get();
    if (timer == nullptr) {
        motor::spinDown();
        return err::NO_TIMERS;
    }

    if (turned != nullptr) {
        *turned = true;
    }

    err::t retval = err::OK;

    // spin
    const motor::state_t motor_state = motor::state();
    if (motor_state == motor::STATE_STOPPED) {
        motor::spinUp(dir);
    }
    else if (motor::stateToDirection(motor_state) == oppositeDirection(dir)) {
        motor::spinDown();
        motor::spinUp(dir);
    }

    // wait till we're all the way to dir
    timer->start(TURN_TIMEOUT_MS);
    while (!too_far::get(dir) && !timer->fired()) {
        idle();
    }
    timer->stop();

    if (timer->fired()) {
        // we timed out
        motor::spinDown();
        retval = err::HARDWARE_FAILURE;
        goto done;
    }

    // now, nudge it a little more so that the button doesn't push the gear back
    timer->start(500);
    while (!timer->fired()) {
        idle();
    }
    timer->stop();

    // stop the motor
    motor::spinDown();

done:
    Timer::put(timer);
    return retval;
}

static err::t goToCenter(direction_t fromDir) {
    err::t retval = err::OK;

    // get timer
    Timer *timer = Timer::get();
    if (timer == nullptr) {
        motor::spinDown();
        retval = err::NO_TIMERS;
        goto done;
    }

    timer->start(gMillisTillCenter);
    motor::spinUp(oppositeDirection(fromDir));
    while (!timer->fired()) {
        idle();
    }
    motor::spinDown();

done:
    Timer::put(timer);
    return retval;
}

err::t highlevel_actions::init() {
    motor::init(shouldStopSpinning);

    // measure how long it takes to get to the center
    err::t e = justTurn(DIRECTION_LEFT);
    if (e != err::OK) {
        return e;
    }
    const long startMillis = millis();
    e = justTurn(DIRECTION_RIGHT);
    if (e != err::OK) {
        return e;
    }
    gMillisTillCenter = (millis() - startMillis) / 2;

    // go to the center
    return goToCenter(DIRECTION_RIGHT);
}

err::t highlevel_actions::turn(direction_t dir) {
    bool turned = false;
    err::t e = justTurn(dir, &turned);
    if (e != err::OK) {
        return e;
    }
    if (turned) {
        e = goToCenter(dir);
        if (e != err::OK) {
            return e;
        }
    }
    return err::OK;
}