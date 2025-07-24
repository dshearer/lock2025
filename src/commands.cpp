#include "commands.h"
#include <Arduino.h>
#include <Bounce2.h>

#define PIN_TURN_LEFT 12
#define PIN_TURN_RIGHT 13
#define INTERVAL_MS 5

static Bounce2::Button gTurnLeftButton, gTurnRightButton;
static commands::handler_t gHandler = nullptr;

void commands::init(handler_t handler) {
    Serial.println("Initializing commands module");

    gHandler = handler;

    gTurnLeftButton.attach(PIN_TURN_LEFT, INPUT_PULLUP);
    gTurnRightButton.attach(PIN_TURN_RIGHT, INPUT_PULLUP);
    gTurnLeftButton.interval(INTERVAL_MS);
    gTurnRightButton.interval(INTERVAL_MS);
    gTurnLeftButton.setPressedState(LOW);
    gTurnRightButton.setPressedState(LOW);

    // subscribe to interrupts
    // attachInterrupt(digitalPinToInterrupt(PIN_TURN_LEFT), leftISR, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(PIN_TURN_RIGHT), rightISR, CHANGE);
}

void commands::update() {
    gTurnLeftButton.update();
    gTurnRightButton.update();

    if (gTurnLeftButton.pressed()) {
        gHandler(commands::TURN_LEFT);
    }

    if (gTurnRightButton.pressed()) {
        gHandler(commands::TURN_RIGHT);
    }
}