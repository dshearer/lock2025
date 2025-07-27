#include "remote_radio.h"
#include <RH_RF69.h>

#define REPLY_TIMEOUT_MS 5000

static RH_RF69 *gRf69 = nullptr;

err::t remote_radio::init() {
    return radio::initRadio(&gRf69);
}

err::t remote_radio::send(cmds::command_t cmd, radio::resp_t *resp) {
    if (gRf69 == nullptr) {
        return err::NOT_INITED;
    }
    
    // send command
    if (!gRf69->send((const uint8_t *)&cmd, sizeof(cmd))) {
        return err::RADIO_MSG_ENQUEUE_FAILED;
    }
    if (!gRf69->waitPacketSent(radio::SEND_TIMEOUT_MS)) {
        return err::RADIO_SEND_FAILED;
    }

    // get response
    if (!gRf69->waitAvailableTimeout(REPLY_TIMEOUT_MS)) {
        return err::RADIO_NO_RESPONSE;
    }
    uint8_t len = sizeof(*resp);
    if (!gRf69->recv((uint8_t *)resp, &len)) {
        return err::RADIO_RECEIVE_FAILED;
    }
    if (len != sizeof(*resp)) {
        return err::RADIO_INVALID_RESPONSE;
    }

    return err::OK;
}