#include "err.h"

static const char *error_strings[] = {
    [err::OK] = "No error",
    [err::UNKNOWN] = "Unknown error",
    [err::NOT_INITED] = "Module not initialized",
    [err::RADIO_INIT_FAILED] = "Radio initialization failed",
    [err::RADIO_SET_FREQ_FAILED] = "Radio frequency setting failed",
    [err::RADIO_SEND_FAILED] = "Radio send failed",
    [err::RADIO_RECEIVE_FAILED] = "Radio receive failed",
    [err::RADIO_NO_RESPONSE] = "No response from remote",
    [err::RADIO_INVALID_RESPONSE] = "Invalid response from remote",
    [err::RADIO_INVALID_COMMAND] = "Invalid command sent to remote",
    [err::RADIO_MSG_ENQUEUE_FAILED] = "Failed to enqueue message for sending",
};

const char *err::to_string(t err) {
    if (err < 0 || ((unsigned) err) >= sizeof(error_strings) / sizeof(error_strings[0]) || error_strings[err] == nullptr) {
        return "Invalid error code";
    }
    return error_strings[err];
}