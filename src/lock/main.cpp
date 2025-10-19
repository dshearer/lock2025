#include <Wire.h>
#include <led.h>
#include "too_far.h"
#include "button.h"
#include "idle.h"
#include "motor.h"
#include "led.h"
#include "lock_radio.h"
#include "pins.h"
#include "tb6612fng_driver.h"

static bool gHadFatalError = false;

static void fail(const char *msg) {
  Serial.print("FATAL ERROR: ");
  Serial.println(msg);
  gHadFatalError = true;
}

static void handleCommand_button(cmds::command_t cmd) {
  err::t e = err::OK;
  switch (cmd) {
  case cmds::TURN_LEFT:
    Serial.println("Should turn left");
    e = motor::turn(DIRECTION_LEFT);
    break;

  case cmds::TURN_RIGHT:
    Serial.println("Should turn right");
    e = motor::turn(DIRECTION_RIGHT);
    break;

  default:
    return;
  }

  if (e != err::OK) {
    // oh no!
    fail(err::to_string(e));
  }
}

static void handleCommand_radio(cmds::command_t cmd, radio::resp_t *resp) {
  handleCommand_button(cmd);
  strncpy(resp->msg, "Done", sizeof(resp->msg) - 1);
}

static led::Mono gLed(13); // on-board LEd

void setup() {
  delay(2000);

  // set up serial
  Serial.begin(9600); // set up Serial library at 9600 bps
  const unsigned long serialTimeoutMillis = millis() + 2000;
  while (!Serial && millis() < serialTimeoutMillis) {
    delay(10);
  }

  Serial.println("Starting up...");

  too_far::init();
  Serial.println("too_far initialized");

  button::init(handleCommand_button);
  Serial.println("button initialized");

  err::t e = motor::init();
  if (e != err::OK) {
    fail(err::to_string(e));
    return;
  }
  Serial.println("motor initialized");

  e = lock_radio::init(handleCommand_radio);
  if (e != err::OK) {
    fail(err::to_string(e));
    return;
  }
  Serial.println("lock_radio initialized");

  // blink green when ready
  gLed.blink(3, led::FAST);
  Serial.println("Ready");
}

void loop() {
  if (gHadFatalError) {
    gLed.blink(-1, led::SLOW); // blink red forever
    return;
  }

  button::update();
  const err::t e = lock_radio::listen();
  if (e != err::OK) {
    Serial.print("ERROR: ");
    Serial.println(err::to_string(e));
  }
}