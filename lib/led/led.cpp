#include "led.h"
#include <Arduino.h>

namespace led {

Rgb::Rgb(uint32_t redPin, uint32_t greenPin, uint32_t bluePin)
: _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
}

void Rgb::shine(color_t color) {
    analogWrite(this->_redPin, color.r);
    analogWrite(this->_greenPin, color.g);
    analogWrite(this->_bluePin, color.b);
}

#define BLINK_DELAY_MS 100

void Rgb::blinkForever(led::color_t color) {
    while (true) {
        this->shine(color);
        delay(BLINK_DELAY_MS);
        this->off();
        delay(BLINK_DELAY_MS);
    }
}

void Rgb::blink(color_t color, int times) {
    if (times < 0) {
        this->blinkForever(color);
        return;
    }

    for (int i = 0; i < times; ++i) {
        this->shine(color);
        delay(BLINK_DELAY_MS);
        this->off();
        if (i < times - 1) { // Avoid delay after the last blink
            delay(BLINK_DELAY_MS);
        }
    }
}

void Rgb::off() {
    this->shine(OFF);
}

}