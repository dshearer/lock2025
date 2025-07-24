#include "led.h"
#include <Arduino.h>

#define PIN_RED 11
#define PIN_GREEN 12
#define PIN_BLUE 13

void led::init() {
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
}

void led::shine(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(PIN_RED, r);
    analogWrite(PIN_GREEN, g);
    analogWrite(PIN_BLUE, b);
}

void led::off() {
    led::shine(0, 0, 0);
}