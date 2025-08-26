#ifndef __LOCK_RADIO_H__
#define __LOCK_RADIO_H__

#include <err.h>
#include <radio.h>
#include <cmds.h>

namespace lock_radio {
    /*!
        @brief Type of function that can handle incoming commands.
        @param cmd The command received.
        @param resp A pointer to a response struct that the handler can fill in.
    */
    typedef void (*handler_t)(cmds::command_t cmd, radio::resp_t* resp);

    /*!
        @brief Initializes the radio module and sets up to call the given handler
               function when a command is received.
        @param handler The function to call when a command is received.
        @return An error code indicating success or failure.
    */
    err::t init(handler_t);

    /*!
        @brief Checks for incoming radio messages. If one is found, calls the handler
               passed to init. Otherwise, returns immediately.
        @return An error code indicating success or failure.
    */
    err::t listen();
}

#endif