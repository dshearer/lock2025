#include "motor.h"
#include "too_far.h"
#include "tb6612fng_driver.h"
#include "pins.h"
#include "stall_detector.h"
#include <num_elems.h>
#include <Arduino.h>

#define MOTOR_SPEED 200
#define MOTOR_SPEED_INCREMENT (MOTOR_SPEED/20)

static unsigned long gMillisTillCenter = 0;

static tb6612fng_driver::mode_t directionToMode(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        return tb6612fng_driver::MODE_DIRECTION_2;
    default:
        return tb6612fng_driver::MODE_DIRECTION_1;
    }
}

static err::t justTurn(direction_t dir, bool* turned = nullptr) {
    // assume that motor is stopped

    err::t e = err::OK;
    StallDetector stallDetector(millis());

    if (too_far::get(dir)) {
        // we're already fully turned in the requested direction
        Serial.print("Already too far ");
        Serial.println(directionToString(dir));
        if (turned != nullptr) {
            *turned = false;
        }
        goto done;
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

        // check for stall
        const err::t stallErr = stallDetector.update(millis(), current::read());
        if (stallErr != err::OK) {
            e = stallErr;
            goto done;
        }
    }

    // remain at full speed
    tb6612fng_driver::setSpeed(MOTOR_SPEED);
    while (!too_far::get(dir)) {
        delay(50);

        // check for stall
        const err::t stallErr = stallDetector.update(millis(), current::read());
        if (stallErr != err::OK) {
            e = stallErr;
            goto done;
        }
    }

    // now, nudge it a little more so that the button doesn't push the gear back
    tb6612fng_driver::setSpeed(MOTOR_SPEED / 2);
    delay(600);

done:
    // stop the motor (but don't release it)
    tb6612fng_driver::setSpeed(0);
    return e;
}

static void goToCenter(direction_t fromDir) {
    const long unsigned timeToStop = millis() + gMillisTillCenter;

    // get up to speed
    tb6612fng_driver::run(directionToMode(oppositeDirection(fromDir)));
    for (int i = 0; i <= MOTOR_SPEED; i += MOTOR_SPEED_INCREMENT) {
        if (millis() >= timeToStop) {
            break;
        }
        tb6612fng_driver::setSpeed(i);
        delay(50); // Allow time for the motor to spin up
    }

    const long unsigned now = millis();
    if (now < timeToStop) {
        delay(timeToStop - now);
    }

    // spin down
    tb6612fng_driver::setSpeed(0);
}

err::t motor::init() {
    tb6612fng_driver::init();
    current::init();

    /*
    Even if a too-far switch is triggered, this doesn't mean that the inner gear is all the way
    to that side. We need to *cause* a too-far switch to be triggered, before we can measure how long
    it takes to go to the center.
    */

    err::t e = err::OK;
    long startMillis = 0;
    long millisTillOtherSide = 0;

    direction_t startDir = DIRECTION_LEFT;
    if (too_far::get(DIRECTION_LEFT)) {
        startDir = DIRECTION_RIGHT;
    }

    // start on one side
    e = justTurn(startDir);
    if (e != err::OK) {
        goto done;
    }

    // measure how long it takes to get to the other side
    startMillis = millis();
    e = justTurn(oppositeDirection(startDir));
    if (e != err::OK) {
        goto done;
    }
    millisTillOtherSide = millis() - startMillis;

    // measure how long it takes to get to the center
    gMillisTillCenter = millisTillOtherSide / 2;

    // go to the center
    goToCenter(oppositeDirection(startDir));

done:
    tb6612fng_driver::run(tb6612fng_driver::MODE_RELEASE);
    return e;
}

err::t motor::turn(direction_t dir) {
    err::t e = err::OK;
    bool turned = false;
    e = justTurn(dir, &turned);
    if (e != err::OK) {
        goto done;
    }
    if (turned) {
        goToCenter(dir);
    }

done:
    tb6612fng_driver::run(tb6612fng_driver::MODE_RELEASE);
    return e;
}
