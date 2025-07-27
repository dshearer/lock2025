#ifndef __LOCK_RADIO_H__
#define __LOCK_RADIO_H__

#include <err.h>
#include <radio.h>
#include <cmds.h>

namespace lock_radio {
    typedef void (*handler_t)(cmds::command_t cmd, radio::resp_t *resp);

    err::t init(handler_t);
    err::t listen();
}

#endif