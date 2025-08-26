#ifndef __REMOTE_RADIO_H__
#define __REMOTE_RADIO_H__

#include <cmds.h>
#include <err.h>
#include <radio.h>

namespace remote_radio {
    /*!
        @brief Initialize the radio module for the remote.
        @return An error code indicating success or failure.
    */
    err::t init();
    
    /*!
        @brief Send a command to the lock and wait for a response.
        @param cmd The command to send.
        @param resp A pointer to a response struct to fill in.
        @return An error code indicating success or failure.
    */
    err::t send(cmds::command_t cmd, radio::resp_t *resp);
}

#endif