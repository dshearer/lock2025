#ifndef __CURRENT_H__
#define __CURRENT_H__

#include <stdint.h>

namespace current {
    typedef uint8_t v;

    void init();

    /*!
    \breif Get the current motor current. The returned value v is such that the current motor current (in mA)
    is approximately 32.26 * v.
    */
    v read();
}

#endif