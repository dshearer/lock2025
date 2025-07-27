#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <cmds.h>

namespace button {
    typedef void(*handler_t)(cmds::command_t);

    void init(handler_t);
    void update();
}

#endif