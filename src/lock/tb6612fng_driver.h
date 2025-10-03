#ifndef __TB6612FNG_DRIVER_H__
#define __TB6612FNG_DRIVER_H__

#include <stdint.h>

namespace tb6612fng_driver {
    typedef enum {
        MODE_DIRECTION_1,
        MODE_DIRECTION_2,
        MODE_RELEASE
    } mode_t;

    void init();
    void run(mode_t mode);
    void setSpeed(uint32_t speed); // 0 to 255
}

#endif