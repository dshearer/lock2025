/*
Driver for TB6612FNG (https://www.pololu.com/product/713).
*/

#include "tb6612fng_driver.h"
#include "pins.h"
#include <Arduino.h>

/*
STBY pin must be driven high to take the motor *out of* standby.
The motor board drives it (and all other control pins) low by default.
*/

#define MAX_SPEED 255

void tb6612fng_driver::init() {
    pinMode(MOTOR_PIN_AIN1, OUTPUT);
    pinMode(MOTOR_PIN_AIN2, OUTPUT);
    pinMode(MOTOR_PIN_PWMA, OUTPUT);
    pinMode(MOTOR_PIN_STBY, OUTPUT);
    run(MODE_RELEASE);
}

void tb6612fng_driver::run(mode_t mode) {
    switch (mode) {
        case MODE_DIRECTION_1:
            digitalWrite(MOTOR_PIN_AIN1, HIGH);
            digitalWrite(MOTOR_PIN_AIN2, LOW);
            digitalWrite(MOTOR_PIN_STBY, HIGH);
            break;

        case MODE_DIRECTION_2:
            digitalWrite(MOTOR_PIN_AIN1, LOW);
            digitalWrite(MOTOR_PIN_AIN2, HIGH);
            digitalWrite(MOTOR_PIN_STBY, HIGH);
            break;

        case MODE_RELEASE:
            digitalWrite(MOTOR_PIN_STBY, LOW);
            digitalWrite(MOTOR_PIN_AIN1, LOW);
            digitalWrite(MOTOR_PIN_AIN2, LOW);
            analogWrite(MOTOR_PIN_PWMA, 0);
            break;
    }
}

void tb6612fng_driver::setSpeed(uint32_t speed) {
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }
    analogWrite(MOTOR_PIN_PWMA, speed);
}