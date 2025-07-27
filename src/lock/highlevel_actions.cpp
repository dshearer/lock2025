#include "highlevel_actions.h"
#include "motor.h"
#include "too_far.h"
#include "idle.h"
#include "led.h"
#include <Arduino.h>
#include <err.h>

#define TURN_TIMEOUT_MS 5000

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

// NOTE: I think RadioHead uses the TC3 timer

static volatile bool gTimerFired = false;
void TC4_Handler() {
    gTimerFired = true;

    // Stop the timer
    TC4->COUNT32.CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);  // Wait for sync

    // Clear interrupt flag (required!)
    TC4->COUNT32.INTFLAG.reg = TC_INTFLAG_MC0;
}

static void startTimer() {
    // Serial.println("Starting timer...");
    gTimerFired = false;

    // 1. Enable clock to TC4
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5;
    while (GCLK->STATUS.bit.SYNCBUSY);

    // 2. Reset TC4 (good practice)
    TC4->COUNT32.CTRLA.reg = TC_CTRLA_SWRST;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);
    while (TC4->COUNT32.CTRLA.bit.SWRST);

    // 3. Configure timer
    TC4->COUNT32.CTRLA.reg = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_PRESCALER_DIV1024;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);

    // 4. Calculate and set compare value
    const uint32_t scaledHz = 48000000 / 1024;
    const uint32_t turnTimeoutSecs = TURN_TIMEOUT_MS / 1000;
    const uint32_t timerCount = scaledHz * turnTimeoutSecs;

    TC4->COUNT32.CC[0].reg = timerCount;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);

    // 5. Enable interrupt
    TC4->COUNT32.INTENSET.reg = TC_INTENSET_MC0;
    NVIC_EnableIRQ(TC4_IRQn);

    // 6. Start timer
    TC4->COUNT32.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);
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

    if (turned != nullptr) {
        *turned = true;
    }

    // shine yellow
    led::shine(led::YELLOW);

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
    startTimer();
    while (!too_far::get(dir) && !gTimerFired) {
        idle();
    }

    // stop the motor
    motor::spinDown();

    // turn off the LED
    led::off();

    // check status
    if (!too_far::get(dir)) {
        // we timed out before we completed the turn
        return err::HARDWARE_FAILURE;
    }

    return err::OK;
}

static void goToCenter(direction_t fromDir) {
    const unsigned long startMillis = millis();
    motor::spinUp(oppositeDirection(fromDir));
    const unsigned long spinUpDuration = millis() - startMillis;
    unsigned long toDelay = 0;
    if (spinUpDuration < gMillisTillCenter) {
        toDelay = gMillisTillCenter - spinUpDuration;
    }
    delay(toDelay);
    motor::spinDown();
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
    goToCenter(DIRECTION_RIGHT);
    return err::OK;
}

err::t highlevel_actions::turn(direction_t dir) {
    bool turned = false;
    const err::t e = justTurn(dir, &turned);
    if (e != err::OK) {
        return e;
    }
    if (turned) {
        goToCenter(dir);
    }
    return err::OK;
}