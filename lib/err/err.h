#ifndef __DYLAN_ERR_H__
#define __DYLAN_ERR_H__

#include <map>

namespace err {
    typedef int error_t;

    // error codes
    const error_t ERR_OK = 0;
    const error_t ERR_UNKNOWN = 1;
    const error_t ERR_NOT_INITED = 2;
    const error_t ERR_RADIO_INIT_FAILED = 3;
    const error_t ERR_RADIO_SET_FREQ_FAILED = 4;
    const error_t ERR_RADIO_SEND_FAILED = 5;
    const error_t ERR_RADIO_RECEIVE_FAILED = 6;
    const error_t ERR_RADIO_NO_RESPONSE = 7;
    const error_t ERR_RADIO_INVALID_RESPONSE = 8;
    const error_t ERR_RADIO_INVALID_COMMAND = 9;
    const error_t ERR_RADIO_MSG_ENQUEUE_FAILED = 10;

    const char *to_string(error_t err);
}

#include <assert.h>

#define ASSERT_OK(e) do { \
    err::error_t _err = (e); \
    if (_err != err::ERR_OK) { \
        Serial.print("ERROR: "); \
        Serial.println(err::to_string(_err)); \
    } \
} while(false)

#endif