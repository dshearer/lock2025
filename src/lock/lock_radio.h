#ifndef __LOCK_RADIO_H__
#define __LOCK_RADIO_H__

#include <err.h>
#include <radio.h>
#include <cmds.h>

namespace lock_radio {
    typedef void (*handler_t)(cmds::command_t cmd, radio::resp_t *resp);

    err::error_t init(handler_t);
    err::error_t listen();
}

#endif