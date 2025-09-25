#include "button.h"
#include "pins.h"
#include <Arduino.h>
#include <Bounce2.h>

#define INTERVAL_MS 5
#define DOUBLE_CLICK_MS 500

static Bounce2::Button gTurnButton;
static button::handler_t gHandler = nullptr;
static unsigned long gLastClickMillis = 0;

void button::init(handler_t handler) {
    Serial.println("Initializing button module");

    gHandler = handler;

    gTurnButton.attach(PIN_BUTTON, INPUT_PULLUP);
    gTurnButton.interval(INTERVAL_MS);
    gTurnButton.setPressedState(LOW);
}

void button::update() {
    gTurnButton.update();

    if (gTurnButton.pressed()) {
        const unsigned long now = millis();
        if (gLastClickMillis == 0) {
            // first click
            gLastClickMillis = now;
        } else {
            // second click
            gLastClickMillis = 0;
            gHandler(cmds::TURN_RIGHT);
        }
        return;
    }

    if (gLastClickMillis > 0) {
        // check if the double click timeout has passed
        if (millis() - gLastClickMillis > DOUBLE_CLICK_MS) {
            // timeout, reset last click
            gLastClickMillis = 0;
            gHandler(cmds::TURN_LEFT);
        }
    }
}