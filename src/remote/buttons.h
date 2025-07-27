#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <cmds.h>

namespace buttons {
    typedef void(*handler_t)(cmds::command_t);

    void init(handler_t);
    void update();
}

#endif