#include <Arduino.h>
#include <led.h>
#include "buttons.h"
#include "remote_radio.h"

#define PIN_RED 6
#define PIN_GREEN 9
#define PIN_BLUE 10

static bool gHadFatalError = false;

static void fail(const char* msg) {
    Serial.print("FATAL ERROR: ");
    Serial.println(msg);
    gHadFatalError = true;
}

static void sendCommand(cmds::command_t cmd) {
    led::shine(led::YELLOW);
    radio::resp_t resp = { {0} };
    err::t e = remote_radio::send(cmd, &resp);
    if (e != err::OK) {
        Serial.print("ERROR: ");
        Serial.println(err::to_string(e));
        led::blink(led::RED, 2);
        return;
    }
    Serial.print("Response: ");
    Serial.println(resp.msg);
    led::off();
}

void setup() {
    led::init(PIN_RED, PIN_GREEN, PIN_BLUE);
    led::shine(led::PURPLE);

    Serial.println("hi");
    buttons::init(sendCommand);
    err::t e = remote_radio::init();
    if (e != err::OK) {
        fail(err::to_string(e));
        return;
    }

    led::blink(led::GREEN, 3);
}

void loop() {
    if (gHadFatalError) {
        led::blink(led::RED, -1); // blink red forever
        return;
    }

    buttons::update();
}