#include "led.h"
#include <Arduino.h>

static uint32_t gRedPin = 0;
static uint32_t gGreenPin = 0;
static uint32_t gBluePin = 0;

void led::init(uint32_t redPin, uint32_t greenPin, uint32_t bluePin) {
    gRedPin = redPin;
    gGreenPin = greenPin;
    gBluePin = bluePin;
    pinMode(gRedPin, OUTPUT);
    pinMode(gGreenPin, OUTPUT);
    pinMode(gBluePin, OUTPUT);
}

void led::shine(color_t color) {
    analogWrite(gRedPin, color.r);
    analogWrite(gGreenPin, color.g);
    analogWrite(gBluePin, color.b);
}

#define BLINK_DELAY_MS 100

void led::blink(color_t color, int times) {
    for (int i = 0; i < times; ++i) {
        led::shine(color);
        delay(BLINK_DELAY_MS);
        led::off();
        if (i < times - 1) { // Avoid delay after the last blink
            delay(BLINK_DELAY_MS);
        }
    }
}

void led::off() {
    led::shine(OFF);
}