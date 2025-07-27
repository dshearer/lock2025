/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <led.h>
#include "too_far.h"
#include "motor.h"
#include "button.h"
#include "idle.h"
#include "highlevel_actions.h"
#include "led.h"
#include "lock_radio.h"

#define PIN_RED 11
#define PIN_GREEN 12
#define PIN_BLUE 13

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
    e = highlevel_actions::turn(DIRECTION_LEFT);
    break;

  case cmds::TURN_RIGHT:
    Serial.println("Should turn right");
    e = highlevel_actions::turn(DIRECTION_RIGHT);
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

void setup() {
  delay(2000);

  led::init(PIN_RED, PIN_GREEN, PIN_BLUE);
  led::shine(led::PURPLE);

  // set up serial
  Serial.begin(9600); // set up Serial library at 9600 bps
  const unsigned long serialTimeoutMillis = millis() + 2000;
  while (!Serial && millis() < serialTimeoutMillis) {
    delay(10);
  }

  // set up Wire
  Wire.begin();
  Wire.setTimeout(1000);
  Wire.beginTransmission(0x60);
  if (Wire.write(0x10) == 0) {
    fail("I2C failed");
    return;
  }
  if (Wire.endTransmission() != 0) {
    fail("endTransmission returned error");
    return;
  }

  too_far::init();
  button::init(handleCommand_button);
  err::t e = highlevel_actions::init();
  if (e != err::OK) {
    fail(err::to_string(e));
    return;
  }
  e = lock_radio::init(handleCommand_radio);
  if (e != err::OK) {
    fail(err::to_string(e));
    return;
  }

  // blink green when ready
  led::blink(led::GREEN, 3);
}

void loop() {
  if (gHadFatalError) {
    led::blink(led::RED, -1); // blink red forever
    return;
  }

  button::update();
  const err::t e = lock_radio::listen();
  if (e != err::OK) {
    Serial.print("ERROR: ");
    Serial.println(err::to_string(e));
  }
}