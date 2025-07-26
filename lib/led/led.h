#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

namespace led {
    typedef struct {
        uint8_t r, g, b;
    } color_t;

    const color_t OFF = {0, 0, 0};
    const color_t RED = {255, 0, 0};
    const color_t GREEN = {0, 255, 0};
    const color_t BLUE = {0, 0, 255};
    const color_t PURPLE = {128, 0, 128};
    const color_t YELLOW = {255, 255, 0};

    void init(uint32_t redPin, uint32_t greenPin, uint32_t bluePin);
    void shine(color_t);
    void blink(color_t color, int times);
    void off();
}

#endif