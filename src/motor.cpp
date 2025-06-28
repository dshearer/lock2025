#include "motor.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>

#define DIRECTION_LEFT BACKWARD
#define DIRECTION_RIGHT FORWARD

#define MOTOR_SPEED 128

// Create the motor shield object with the default I2C address
// Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor* myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
// Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

static motor::state_t currState = motor::STATE_STOPPED;
static motor::should_stop_fn_t shouldStopFn = NULL;

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

void setSpinningState(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        currState = motor::STATE_TURNING_LEFT;
        break;
    case DIRECTION_RIGHT:
        currState = motor::STATE_TURNING_RIGHT;
        break;
    }
}

void motor::spinDown(direction_t dir) {
    myMotor->run(dir);
    for (int i = MOTOR_SPEED; i >= 0; i -= 5) {
        myMotor->setSpeed(i);
    }
    myMotor->run(RELEASE);
    currState = motor::STATE_STOPPED;
}

void motor::spinUp(direction_t dir) {
    setSpinningState(dir);
    myMotor->run(dir);
    for (int i = 0; i <= MOTOR_SPEED; i += 5) {
        if (shouldStopFn(dir)) {
            Serial.println("Stopping motor due to shouldStopFn");
            motor::spinDown(dir);
            return;
        }
        Serial.print("Setting speed to: ");
        Serial.println(i);
        delay(50); // Allow time for the motor to spin up
        myMotor->setSpeed(i);
    }
}

motor::state_t motor::state() {
    return currState;
}