#include "too_far.h"
#include <Arduino.h>

#define PIN_TOO_FAR_LEFT 2 
#define PIN_TOO_FAR_RIGHT 3


typedef enum {
    BUTTON_STATE_UNKNOWN = 0,
    BUTTON_STATE_DOWN = 1,
    BUTTON_STATE_UP = 2,
} button_state_t;

void too_far::init() {
    pinMode(PIN_TOO_FAR_LEFT, INPUT_PULLUP);
    pinMode(PIN_TOO_FAR_RIGHT, INPUT_PULLUP);
}

bool too_far::get(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        return digitalRead(PIN_TOO_FAR_LEFT) == LOW;
    case DIRECTION_RIGHT:
        return digitalRead(PIN_TOO_FAR_RIGHT) == LOW;
    default:
        return false;
    }
}

void too_far::printState() {
    Serial.print("Left button state: ");
    Serial.println(too_far::get(DIRECTION_LEFT) ? "DOWN" : "UP");
    Serial.print("Right button state: ");
    Serial.println(too_far::get(DIRECTION_RIGHT)  ? "DOWN" : "UP");
}