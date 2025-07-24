#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

namespace led {
    void init();
    void shine(uint8_t r, uint8_t g, uint8_t b);
    void off();
}

#endif