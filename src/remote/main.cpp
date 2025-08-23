#include <Arduino.h>
#include <led.h>
#include "buttons.h"
#include "remote_radio.h"

#define PIN_RED 6
#define PIN_GREEN 9
#define PIN_BLUE 10

static bool gHadFatalError = false;
static led::Mono gLed(A2);

static void fail() {
    gHadFatalError = true;
}

static void sendCommand(cmds::command_t cmd) {
    gLed.on();
    radio::resp_t resp = { {0} };
    err::t e = remote_radio::send(cmd, &resp);
    if (e != err::OK) {
        Serial.print("ERROR: ");
        Serial.println(err::to_string(e));
        gLed.blink(2);
        return;
    }
    Serial.print("Response: ");
    Serial.println(resp.msg);
    gLed.off();
}

void setup() {
    gLed.on();

    buttons::init(sendCommand);
    err::t e = remote_radio::init();
    if (e != err::OK) {
        fail();
        return;
    }

    gLed.off();
}

void loop() {
    if (gHadFatalError) {
        gLed.blink(-1); // blink forever
        return;
    }

    buttons::update();

    // delay(1000);
    // digitalWrite(8, HIGH);
    // delay(1000);
    // digitalWrite(8, LOW);
}