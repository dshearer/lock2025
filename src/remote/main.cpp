#include <Arduino.h>
#include <led.h>

#define PIN_RED 6
#define PIN_GREEN 9
#define PIN_BLUE 10

void setup() {
  delay(2000);

  led::init(PIN_RED, PIN_GREEN, PIN_BLUE);
  led::shine(led::PURPLE);

  Serial.println("hi");

  led::blink(led::GREEN, 3);
}

void loop() {}