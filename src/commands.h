#ifndef __COMMANDS_H__
#define __COMMANDS_H__

namespace commands {
    typedef enum {
        TURN_LEFT = 1,
        TURN_RIGHT = 2,
    } cmd_t;

    typedef void(*handler_t)(cmd_t);

    void init(handler_t);
    void update();
}

#endif