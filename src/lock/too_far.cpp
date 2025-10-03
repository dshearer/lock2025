#include "too_far.h"
#include "pins.h"
#include <num_elems.h>
#include <Arduino.h>

static uint32_t directionToPin(direction_t dir) {
    switch (dir) {
    case DIRECTION_LEFT:
        return PIN_TOO_FAR_LEFT;
    case DIRECTION_RIGHT:
        return PIN_TOO_FAR_RIGHT;
    default:
        return 0;
    }
}

void too_far::init() {
    pinMode(PIN_TOO_FAR_LEFT, INPUT_PULLUP);
    pinMode(PIN_TOO_FAR_RIGHT, INPUT_PULLUP);

    for (size_t i = 0; i < NUM_ELEMS(ALL_DIRECTIONS); i++) {
        const direction_t dir = ALL_DIRECTIONS[i];
        Serial.print("too far ");
        Serial.print(directionToString(dir));
        Serial.print(": ");
        Serial.println(get(dir) ? "YES" : "NO");
    }
}

bool too_far::get(direction_t dir) {
    const uint32_t pin = directionToPin(dir);
    if (pin == 0) {
        return false;
    }
    return digitalRead(pin) == LOW;
}
