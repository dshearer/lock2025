#ifndef __REMOTE_RADIO_H__
#define __REMOTE_RADIO_H__

#include <cmds.h>
#include <err.h>
#include <radio.h>

namespace remote_radio {
    err::error_t init();
    err::error_t send(cmds::command_t cmd, radio::resp_t *resp);
}

#endif