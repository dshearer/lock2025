#include "motor.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>

#define MOTOR_SPEED 128
#define INCREMENT (MOTOR_SPEED/20)
#define MOTOR_SHIELD_PORT 4

// Create the motor shield object with the default I2C address
// Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
Adafruit_MotorShield AFMS = Adafruit_MotorShield(motor::i2cAddr);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor* myMotor = AFMS.getMotor(MOTOR_SHIELD_PORT);
// You can also make another motor on port M2
// Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

static motor::state_t currState = motor::STATE_STOPPED;
static motor::should_stop_fn_t shouldStopFn = NULL;

direction_t motor::stateToDirection(motor::state_t state) {
    switch (state) {
    case motor::STATE_TURNING_LEFT:
        return DIRECTION_LEFT;
    case motor::STATE_TURNING_RIGHT:
        return DIRECTION_RIGHT;
    default:
        return DIRECTION_NONE;
    }
}

static motor::state_t directionToState(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        return motor::STATE_TURNING_LEFT;
    case DIRECTION_RIGHT:
        return motor::STATE_TURNING_RIGHT;
    default:
        return motor::STATE_STOPPED;
    }
}

void motor::init(motor::should_stop_fn_t f) {
    shouldStopFn = f;

    Serial.println("Calling AFMS begin");
    if (!AFMS.begin()) { // create with the default frequency 1.6KHz
        Serial.println("Could not find Motor Shield. Check wiring.");
        while (true) {}
    }
    Serial.println("Motor Shield found.");

    myMotor->setSpeed(0);
    myMotor->run(RELEASE);
}

void motor::spinDown() {
    if (currState == motor::STATE_STOPPED) {
        // already stopped
        return;
    }

    myMotor->run(stateToDirection(currState));
    for (int i = MOTOR_SPEED; i >= 0; i -= INCREMENT) {
        myMotor->setSpeed(i);
    }
    myMotor->run(RELEASE);
    currState = motor::STATE_STOPPED;
}

const bool gradualSpin = false;

void motor::spinUp(direction_t dir) {
    if (dir == stateToDirection(currState)) {
        // Serial.println("Motor is already spinning in the requested direction.");
        return;
    }

    if (!gradualSpin) {
        currState = directionToState(dir);
        myMotor->run(dir);
        myMotor->setSpeed(MOTOR_SPEED);
        return;
    }

    currState = directionToState(dir);
    myMotor->run(dir);
    for (int i = 0; i <= MOTOR_SPEED; i += INCREMENT) {
        if (shouldStopFn(dir)) {
            Serial.println("Stopping motor due to shouldStopFn");
            motor::spinDown();
            return;
        }
        // Serial.print("Setting speed to: ");
        // Serial.println(i);
        myMotor->setSpeed(i);
        delay(50); // Allow time for the motor to spin up
    }
}

motor::state_t motor::state() {
    return currState;
}