#include "led.h"
#include <Arduino.h>

#define BLINK_SLOW_DELAY_MS 1000
#define BLINK_FAST_DELAY_MS (BLINK_SLOW_DELAY_MS/4)

unsigned long delayForSpeed(led::speed_t speed) {
    switch (speed) {
        case led::FAST:
            return BLINK_FAST_DELAY_MS;
        case led::SLOW:
            return BLINK_SLOW_DELAY_MS;
        default:
            return BLINK_SLOW_DELAY_MS;
    }
}

namespace led {

Mono::Mono(uint32_t pin) : _pin(pin) {
    pinMode(pin, OUTPUT);
}

void Mono::on() {
    digitalWrite(this->_pin, HIGH);
}

void Mono::off() {
    digitalWrite(this->_pin, LOW);
}

void Mono::blinkForever(speed_t speed) {
    const unsigned long ms = delayForSpeed(speed);
    while (true) {
        this->on();
        delay(ms);
        this->off();
        delay(ms);
    }
}

void Mono::blink(int times, speed_t speed) {
    if (times < 0) {
        this->blinkForever(speed);
        return;
    }

    const unsigned long ms = delayForSpeed(speed);
    for (int i = 0; i < times; ++i) {
        this->on();
        delay(ms);
        this->off();
        if (i < times - 1) { // Avoid delay after the last blink
            delay(ms);
        }
    }
}

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

void Rgb::off() {
    this->shine(OFF);
}

void Rgb::blinkForever(led::color_t color, speed_t speed) {
    const unsigned long ms = delayForSpeed(speed);
    while (true) {
        this->shine(color);
        delay(ms);
        this->off();
        delay(ms);
    }
}

void Rgb::blink(color_t color, int times, speed_t speed) {
    if (times < 0) {
        this->blinkForever(color, speed);
        return;
    }

    const unsigned long ms = delayForSpeed(speed);
    for (int i = 0; i < times; ++i) {
        this->shine(color);
        delay(ms);
        this->off();
        if (i < times - 1) { // Avoid delay after the last blink
            delay(ms);
        }
    }
}

}