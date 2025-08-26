#include "buttons.h"
#include <Arduino.h>
#include <Bounce2.h>

#define BUTTON_LEFT_PIN 5
#define BUTTON_RIGHT_PIN 6
#define INTERVAL_MS 5

static Bounce2::Button gTurnLeftButton, gTurnRightButton;
static buttons::handler_t gHandler = nullptr;

void buttons::init(handler_t handler) {
    gHandler = handler;

    gTurnLeftButton.attach(BUTTON_LEFT_PIN, INPUT_PULLUP);
    gTurnRightButton.attach(BUTTON_RIGHT_PIN, INPUT_PULLUP);
    gTurnLeftButton.interval(INTERVAL_MS);
    gTurnRightButton.interval(INTERVAL_MS);
    gTurnLeftButton.setPressedState(LOW);
    gTurnRightButton.setPressedState(LOW);
}

void buttons::update() {
    gTurnLeftButton.update();
    gTurnRightButton.update();

    if (gTurnLeftButton.pressed()) {
        gHandler(cmds::TURN_LEFT);
        return;
    }

    if (gTurnRightButton.pressed()) {
        gHandler(cmds::TURN_RIGHT);
        return;
    }
}