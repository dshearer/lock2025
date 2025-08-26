#ifndef __RADIO_H__
#define __RADIO_H__

#include <err.h>
#include <RH_RF69.h>

namespace radio {
    const float FREQ = 915.0;
    const unsigned short SEND_TIMEOUT_MS = 500;

    typedef struct {
        char msg[16];
    } resp_t;

    typedef enum {
        BOARD_REMOTE,
        BOARD_LOCK
    } board_t;

    err::t initRadio(RH_RF69 **rf69, board_t board);
}

#endif