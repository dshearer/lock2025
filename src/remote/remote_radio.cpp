#include "remote_radio.h"
#include <RH_RF69.h>

#define REPLY_TIMEOUT_MS 5000

static RH_RF69 *gRf69 = nullptr;

err::error_t remote_radio::init() {
    return radio::initRadio(&gRf69);
}

err::error_t remote_radio::send(cmds::command_t cmd, radio::resp_t *resp) {
    if (gRf69 == nullptr) {
        return err::ERR_NOT_INITED;
    }
    
    // send command
    if (!gRf69->send((const uint8_t *)&cmd, sizeof(cmd))) {
        return err::ERR_RADIO_MSG_ENQUEUE_FAILED;
    }
    if (!gRf69->waitPacketSent(radio::SEND_TIMEOUT_MS)) {
        return err::ERR_RADIO_SEND_FAILED;
    }

    // get response
    if (!gRf69->waitAvailableTimeout(REPLY_TIMEOUT_MS)) {
        return err::ERR_RADIO_NO_RESPONSE;
    }
    uint8_t len = sizeof(*resp);
    if (!gRf69->recv((uint8_t *)resp, &len)) {
        return err::ERR_RADIO_RECEIVE_FAILED;
    }
    if (len != sizeof(*resp)) {
        return err::ERR_RADIO_INVALID_RESPONSE;
    }

    return err::ERR_OK;
}