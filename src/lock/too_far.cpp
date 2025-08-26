#include "too_far.h"
#include "pins.h"
#include <Arduino.h>



static volatile bool leftButtonDown = false;
static volatile bool rightButtonDown = false;
static volatile bool shouldPrintState = false;

static void leftISR() {
    leftButtonDown = digitalRead(PIN_TOO_FAR_LEFT) == LOW;
    shouldPrintState = true;
}

static void rightISR() {
    rightButtonDown = digitalRead(PIN_TOO_FAR_RIGHT) == LOW;
    shouldPrintState = true;
}

void too_far::init() {
    pinMode(PIN_TOO_FAR_LEFT, INPUT_PULLUP);
    pinMode(PIN_TOO_FAR_RIGHT, INPUT_PULLUP);

    // get current button states
    leftButtonDown = digitalRead(PIN_TOO_FAR_LEFT) == LOW;
    rightButtonDown = digitalRead(PIN_TOO_FAR_RIGHT) == LOW;

    // subscribe to interrupts
    attachInterrupt(digitalPinToInterrupt(PIN_TOO_FAR_LEFT), leftISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_TOO_FAR_RIGHT), rightISR, CHANGE);
}

bool too_far::get(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        return leftButtonDown;
    case DIRECTION_RIGHT:
        return rightButtonDown;
    default:
        return false;
    }
}

void too_far::printState() {
    Serial.print("Left button state: ");
    Serial.println(too_far::get(DIRECTION_LEFT) ? "DOWN" : "UP");
    Serial.print("Right button state: ");
    Serial.println(too_far::get(DIRECTION_RIGHT) ? "DOWN" : "UP");
}

void too_far::printStateOnInterruptLoop() {
    while (true) {
        if (shouldPrintState) {
            printState();
            shouldPrintState = false;
        }
        delay(10);
    }
}