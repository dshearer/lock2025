#ifndef __DYLAN_ERR_H__
#define __DYLAN_ERR_H__

#include <map>

namespace err {
    typedef int t;

    // error codes
    const t OK = 0;
    const t UNKNOWN = 1;
    const t NOT_INITED = 2;
    const t RADIO_INIT_FAILED = 3;
    const t RADIO_SET_FREQ_FAILED = 4;
    const t RADIO_SEND_FAILED = 5;
    const t RADIO_RECEIVE_FAILED = 6;
    const t RADIO_NO_RESPONSE = 7;
    const t RADIO_INVALID_RESPONSE = 8;
    const t RADIO_INVALID_COMMAND = 9;
    const t RADIO_MSG_ENQUEUE_FAILED = 10;

    const char *to_string(t err);
}

#include <assert.h>

#define ASSERT_OK(e) do { \
    err::t _err = (e); \
    if (_err != err::OK) { \
        Serial.print("ERROR: "); \
        Serial.println(err::to_string(_err)); \
    } \
} while(false)

#endif