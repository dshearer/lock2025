#ifndef __COMMANDS_H__
#define __COMMANDS_H__

namespace cmds {
    typedef enum {
        NOP = 0,
        TURN_LEFT = 1,
        TURN_RIGHT = 2,
    } command_t;
}

#endif