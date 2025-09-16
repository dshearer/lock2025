#include "motor.h"
#include "too_far.h"
#include "timer.h"
#include "idle.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>

#define TURN_TIMEOUT_MS 5000

#define MOTOR_SPEED 200
#define MOTOR_SPEED_INCREMENT (MOTOR_SPEED/20)
#define MOTOR_SHIELD_PORT 4

// Create the motor shield object with the default I2C address
// Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor* myMotor = AFMS.getMotor(MOTOR_SHIELD_PORT);
// You can also make another motor on port M2
// Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

static unsigned long millisTillCenter = 0;

static err::t justTurn(direction_t dir, bool* turned = nullptr) {
    // assume that motor is stopped

    if (too_far::get(dir)) {
        // we're already fully turned in the requested direction
        if (turned != nullptr) {
            *turned = false;
        }
        return err::OK;
    }

    if (turned != nullptr) {
        *turned = true;
    }

    // get up to speed
    myMotor->run(dir);
    for (int i = 0; i <= MOTOR_SPEED; i += MOTOR_SPEED_INCREMENT) {
        if (too_far::get(dir)) {
            break;
        }
        myMotor->setSpeed(i);
        delay(50); // Allow time for the motor to spin up
    }

    // remain at full speed
    const unsigned long spinStart = millis();
    while (!too_far::get(dir)) {
        delay(50);

        if (millis() - spinStart > TURN_TIMEOUT_MS) {
            // we timed out
            myMotor->setSpeed(0);
            myMotor->run(RELEASE);
            return err::HARDWARE_FAILURE;
        }
    }

    // now, nudge it a little more so that the button doesn't push the gear back
    delay(300);

    // stop the motor
    myMotor->setSpeed(0);
    myMotor->run(RELEASE);
    return err::OK;
}

static void goToCenter(direction_t fromDir) {
    timer::start(millisTillCenter);

    // get up to speed
    myMotor->run(oppositeDirection(fromDir));
    for (int i = 0; i <= MOTOR_SPEED; i += MOTOR_SPEED_INCREMENT) {
        if (timer::fired()) {
            break;
        }
        myMotor->setSpeed(i);
        delay(50); // Allow time for the motor to spin up
    }

    while (!timer::fired()) {
        idle();
    }

    // spin down
    myMotor->setSpeed(0);
    myMotor->run(RELEASE);
}

err::t motor::init() {
    Serial.println("Calling AFMS begin");
    if (!AFMS.begin()) { // create with the default frequency 1.6KHz
        Serial.println("Could not find Motor Shield. Check wiring.");
        return err::HARDWARE_FAILURE;
    }
    Serial.println("Motor Shield found.");

    myMotor->setSpeed(0);
    myMotor->run(RELEASE);

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
    millisTillCenter = (millis() - startMillis) / 2;

    // go to the center
    goToCenter(DIRECTION_RIGHT);
    
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