#include "motor.h"
#include "too_far.h"
#include "timer.h"
#include "idle.h"
#include "tb6612fng_driver.h"
#include <Arduino.h>

#define TURN_TIMEOUT_MS 5000

#define MOTOR_SPEED 200
#define MOTOR_SPEED_INCREMENT (MOTOR_SPEED/20)

static unsigned long millisTillCenter = 0;

static tb6612fng_driver::mode_t directionToMode(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        return tb6612fng_driver::MODE_DIRECTION_1;
    default:
        return tb6612fng_driver::MODE_DIRECTION_2;
    }
}

static err::t justTurn(direction_t dir, bool* turned = nullptr) {
    // assume that motor is stopped

    if (too_far::get(dir)) {
        // we're already fully turned in the requested direction
        Serial.print("Already too far ");
        Serial.println(directionToString(dir));
        if (turned != nullptr) {
            *turned = false;
        }
        return err::OK;
    }

    if (turned != nullptr) {
        *turned = true;
    }

    Serial.print("Turning ");
    Serial.println(directionToString(dir));

    // get up to speed
    tb6612fng_driver::run(directionToMode(dir));
    for (int i = 0; i <= MOTOR_SPEED; i += MOTOR_SPEED_INCREMENT) {
        if (too_far::get(dir)) {
            break;
        }
        tb6612fng_driver::setSpeed(i);
        delay(50); // Allow time for the motor to spin up
    }

    // remain at full speed
    const unsigned long spinStart = millis();
    while (!too_far::get(dir)) {
        delay(50);

        if (millis() - spinStart > TURN_TIMEOUT_MS) {
            // we timed out
            tb6612fng_driver::setSpeed(0);
            tb6612fng_driver::run(tb6612fng_driver::MODE_RELEASE);
            Serial.println("Turn timed out");
            return err::HARDWARE_FAILURE;
        }
    }

    // now, nudge it a little more so that the button doesn't push the gear back
    delay(300);

    // stop the motor
    tb6612fng_driver::setSpeed(0);
    tb6612fng_driver::run(tb6612fng_driver::MODE_RELEASE);
    return err::OK;
}

static void goToCenter(direction_t fromDir) {
    timer::start(millisTillCenter);

    // get up to speed
    tb6612fng_driver::run(directionToMode(oppositeDirection(fromDir)));
    for (int i = 0; i <= MOTOR_SPEED; i += MOTOR_SPEED_INCREMENT) {
        if (timer::fired()) {
            break;
        }
        tb6612fng_driver::setSpeed(i);
        delay(50); // Allow time for the motor to spin up
    }

    while (!timer::fired()) {
        idle();
    }

    // spin down
    tb6612fng_driver::setSpeed(0);
    tb6612fng_driver::run(tb6612fng_driver::MODE_RELEASE);
}

err::t motor::init() {
    tb6612fng_driver::init();

    /*
    Even if a too-far switch is triggered, this doesn't mean that the inner gear is all the way
    to that side. We need to *cause* a too-far switch to be triggered, before we can measure how long
    it takes to go to the center.
    */

    long millisTillOtherSide = 0;

    direction_t startDir = DIRECTION_LEFT;
    if (too_far::get(DIRECTION_LEFT)) {
        startDir = DIRECTION_RIGHT;
    }

    // start on one side
    err::t e = justTurn(startDir);
    if (e != err::OK) {
        return e;
    }

    // measure how long it takes to get to the other side
    const long startMillis = millis();
    e = justTurn(oppositeDirection(startDir));
    if (e != err::OK) {
        return e;
    }
    millisTillOtherSide = millis() - startMillis;

    // measure how long it takes to get to the center
    millisTillCenter = millisTillOtherSide / 2;

    // go to the center
    goToCenter(oppositeDirection(startDir));
    
    return err::OK;
}

err::t motor::turn(direction_t dir) {
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