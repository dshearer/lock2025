#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

namespace led {

    class Mono {
    private:
        uint32_t _pin;

        void blinkForever();

    public:
        Mono(uint32_t pin);
        void on();
        void off();
        /*!
              \brief If times is negative, blinks forever.
        */
        void blink(int times);
    };

    typedef struct {
        uint8_t r, g, b;
    } color_t;

    const color_t OFF = { 0, 0, 0 };
    const color_t RED = { 255, 0, 0 };
    const color_t GREEN = { 0, 255, 0 };
    const color_t BLUE = { 0, 0, 255 };
    const color_t PURPLE = { 128, 0, 128 };
    const color_t YELLOW = { 255, 255, 0 };

    class Rgb {
    private:
        uint32_t _redPin;
        uint32_t _greenPin;
        uint32_t _bluePin;

        void blinkForever(led::color_t color);

    public:
        Rgb(uint32_t redPin, uint32_t greenPin, uint32_t bluePin);
        void shine(color_t);
        void off();
        /*!
              \brief If times is negative, blinks forever.
        */
        void blink(color_t color, int times);
    };
}

#endif