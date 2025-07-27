#include "err.h"

static const char *error_strings[] = {
    [err::ERR_OK] = "No error",
    [err::ERR_UNKNOWN] = "Unknown error",
    [err::ERR_NOT_INITED] = "Module not initialized",
    [err::ERR_RADIO_INIT_FAILED] = "Radio initialization failed",
    [err::ERR_RADIO_SET_FREQ_FAILED] = "Radio frequency setting failed",
    [err::ERR_RADIO_SEND_FAILED] = "Radio send failed",
    [err::ERR_RADIO_RECEIVE_FAILED] = "Radio receive failed",
    [err::ERR_RADIO_NO_RESPONSE] = "No response from remote",
    [err::ERR_RADIO_INVALID_RESPONSE] = "Invalid response from remote",
    [err::ERR_RADIO_INVALID_COMMAND] = "Invalid command sent to remote",
    [err::ERR_RADIO_MSG_ENQUEUE_FAILED] = "Failed to enqueue message for sending",
};

const char *err::to_string(error_t err) {
    if (err < 0 || ((unsigned) err) >= sizeof(error_strings) / sizeof(error_strings[0]) || error_strings[err] == nullptr) {
        return "Invalid error code";
    }
    return error_strings[err];
}