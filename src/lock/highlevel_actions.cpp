#include "highlevel_actions.h"
#include "motor.h"
#include "too_far.h"
#include "idle.h"
#include "led.h"
#include <Arduino.h>

static unsigned long gMillisTillCenter = 0;

static bool justTurn(direction_t dir) {
    if (too_far::get(dir)) {
        // we're already fully turned in the requested direction
        motor::spinDown();
        return false;
    }

    // shine yellow
    led::shine(255, 255, 0); // yellow

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

    // turn off the LED
    led::off();

    return true;
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

void highlevel_actions::init() {
    // measure how long it takes to get to the center
    Serial.println("Measuring millis till center...");
    justTurn(DIRECTION_LEFT);
    const long startMillis = millis();
    justTurn(DIRECTION_RIGHT);
    Serial.println("Done measuring millis till center");
    gMillisTillCenter = (millis() - startMillis)/2;

    Serial.println("Going to center");
    goToCenter(DIRECTION_RIGHT);
}

void highlevel_actions::turn(direction_t dir) {
    if (justTurn(dir)) {
        goToCenter(dir);
    }
}