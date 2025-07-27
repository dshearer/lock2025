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

static bool shouldStopSpinning(direction_t dir) {
  switch (dir) {
  case DIRECTION_LEFT:
    return too_far::get(DIRECTION_LEFT);
  case DIRECTION_RIGHT:
    return too_far::get(DIRECTION_RIGHT);
  default:
    return false;
  }
}

static void handleCommand_button(cmds::command_t cmd) {
  switch (cmd) {
  case cmds::TURN_LEFT:
    Serial.println("Should turn left");
    highlevel_actions::turn(DIRECTION_LEFT);
    break;

  case cmds::TURN_RIGHT:
    Serial.println("Should turn right");
    highlevel_actions::turn(DIRECTION_RIGHT);
    break;

  default:
    return;
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
    Serial.println("ERROR: I2C failed");
    gHadFatalError = true;
    return;
  }
  if (Wire.endTransmission() != 0) {
    Serial.println("endTransmission returned error");
    gHadFatalError = true;
    return;
  }

  too_far::init();
  motor::init(shouldStopSpinning);
  button::init(handleCommand_button);
  highlevel_actions::init();
  err::t e = lock_radio::init(handleCommand_radio);
  ASSERT_OK(e);

  // blink green when ready
  led::blink(led::GREEN, 3);
}

void loop() {
  if (gHadFatalError) {
    led::blink(led::RED, -1); // blink red forever
    return;
  }

  button::update();
  err::t e = lock_radio::listen();
  ASSERT_OK(e);
}