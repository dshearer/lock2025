#include <Arduino.h>
#include <led.h>
#include "buttons.h"
#include "remote_radio.h"

#define PIN_RED 6
#define PIN_GREEN 9
#define PIN_BLUE 10

static void sendCommand(cmds::command_t cmd) {
    led::shine(led::YELLOW);
    radio::resp_t resp = { {0} };
    err::t e = remote_radio::send(cmd, &resp);
    ASSERT_OK(e);
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
    ASSERT_OK(e);

    led::blink(led::GREEN, 3);
}

void loop() {
    buttons::update();
}