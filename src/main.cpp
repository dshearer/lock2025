/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "too_far.h"
#include "motor.h"

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

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps
  while (!Serial) {
  };
  Serial.println("\nAdafruit Motorshield v2 - DC Motor test!");

  Serial.println("calling Wire.begin");
  Wire.begin();
  Wire.setTimeout(1000);
  Serial.println("called Wire.begin");

  Serial.println("Begining transmission");
  Wire.beginTransmission(0x60);
  Serial.println("Sending I2C");
  if (Wire.write(0x10) == 0) {
    Serial.println("I2C failed");
  }
  Serial.println("Sent I2C");
  int err = Wire.endTransmission();
  Serial.println("Ending transmission");
  if (err != 0) {
    Serial.println("endTransmission returned error");
  }

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  too_far::init();
  motor::init(shouldStopSpinning);
}

void loop() {
  Serial.println("*****");
  too_far::printState();
  bool tooFarLeft = too_far::get(DIRECTION_LEFT);
  bool tooFarRight = too_far::get(DIRECTION_RIGHT);
  motor::state_t motorState = motor::state();

  switch (motorState) {
  case motor::STATE_STOPPED:
    Serial.println("Motor is stopped");
    if (tooFarLeft) {
      Serial.println("Too far left, spinning right");
      motor::spinUp(DIRECTION_RIGHT);
      return;
    }
    motor::spinUp(DIRECTION_LEFT);
    return;

  case motor::STATE_TURNING_LEFT:
    Serial.println("Motor is turning left");
    if (tooFarLeft) {
      Serial.println("Too far left, stopping motor");
      motor::spinDown(DIRECTION_LEFT);
      Serial.println("Starting to spin right");
      motor::spinUp(DIRECTION_RIGHT);
      return;
    }
    return;

  case motor::STATE_TURNING_RIGHT:
    Serial.println("Motor is turning right");
    if (tooFarRight) {
      Serial.println("Too far right, stopping motor");
      motor::spinDown(DIRECTION_RIGHT);
      Serial.println("Starting to spin left");
      motor::spinUp(DIRECTION_LEFT);
      return;
    }
    return;
  }
}