#include "commands.h"
#include <Arduino.h>
#include <Bounce2.h>

#define BUTTON_PIN 9
#define INTERVAL_MS 5
#define DOUBLE_CLICK_MS 500

static Bounce2::Button gTurnButton;
static commands::handler_t gHandler = nullptr;
static unsigned long gLastClickMillis = 0;

void commands::init(handler_t handler) {
    Serial.println("Initializing commands module");

    gHandler = handler;

    gTurnButton.attach(BUTTON_PIN, INPUT_PULLUP);
    gTurnButton.interval(INTERVAL_MS);
    gTurnButton.setPressedState(LOW);

    // subscribe to interrupts
    // attachInterrupt(digitalPinToInterrupt(PIN_TURN_LEFT), leftISR, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(PIN_TURN_RIGHT), rightISR, CHANGE);
}

void commands::update() {
    gTurnButton.update();

    if (gTurnButton.pressed()) {
        const unsigned long now = millis();
        if (gLastClickMillis == 0) {
            // first click
            gLastClickMillis = now;
        } else {
            // second click
            gLastClickMillis = 0;
            gHandler(commands::TURN_RIGHT);
        }
        return;
    }

    if (gLastClickMillis > 0) {
        // check if the double click timeout has passed
        if (millis() - gLastClickMillis > DOUBLE_CLICK_MS) {
            // timeout, reset last click
            gLastClickMillis = 0;
            gHandler(commands::TURN_LEFT);
        }
    }
}