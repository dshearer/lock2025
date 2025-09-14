#include <Arduino.h>
#include <led.h>
#include "buttons.h"
#include "remote_radio.h"

#define PIN_RED 6
#define PIN_GREEN 9
#define PIN_BLUE 10

static bool gHadFatalError = false;
static led::Rgb gLed(9, 10, 11);

static void fail() {
    gHadFatalError = true;
}

static void sendCommand(cmds::command_t cmd) {
    Serial.print("Sending command ");
    Serial.println(cmd);

    gLed.shine(led::YELLOW);
    radio::resp_t resp = { {0} };
    err::t e = remote_radio::send(cmd, &resp);
    if (e != err::OK) {
        gLed.blink(led::RED, 3, led::FAST);
        return;
    }
    gLed.off();
}

void setup() {
    delay(2000);

    gLed.shine(led::PURPLE);

    // set up serial
    Serial.begin(9600); // set up Serial library at 9600 bps
    const unsigned long serialTimeoutMillis = millis() + 2000;
    while (!Serial && millis() < serialTimeoutMillis) {
        delay(10);
    }

    Serial.println("Starting up...");

    buttons::init(sendCommand);
    err::t e = remote_radio::init();
    if (e != err::OK) {
        fail();
        return;
    }

    gLed.blink(led::GREEN, 3, led::FAST);
    Serial.println("Ready.");
}

void loop() {
    if (gHadFatalError) {
        gLed.blink(led::RED, -1, led::SLOW); // blink forever
        return;
    }

    buttons::update();
}