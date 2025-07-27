#include "lock_radio.h"

static RH_RF69 *gRf69 = nullptr;
static lock_radio::handler_t gHandler = nullptr;

err::error_t lock_radio::init(handler_t handler) {
    gHandler = handler;
    return radio::initRadio(&gRf69);
}

err::error_t lock_radio::listen() {
    if (gRf69 == nullptr) {
        return err::ERR_NOT_INITED;
    }

    if (!gRf69->available()) {
        return err::ERR_OK;
    }

    // read message
    Serial.println("Reading message");
    cmds::command_t cmd = cmds::NOP;
    uint8_t len = sizeof(cmd);
    if (!gRf69->recv((uint8_t *)&cmd, &len)) {
        return err::ERR_RADIO_RECEIVE_FAILED;
    }
    if (len != sizeof(cmd)) {
        return err::ERR_RADIO_INVALID_COMMAND;
    }

    // send to handler
    radio::resp_t resp = {{0}};
    gHandler(cmd, &resp);

    // send response
    Serial.println("Sending response");
    if (!gRf69->send((const uint8_t *)&resp, sizeof(resp))) {
        return err::ERR_RADIO_MSG_ENQUEUE_FAILED;
    }
    if (!gRf69->waitPacketSent(radio::SEND_TIMEOUT_MS)) {
        return err::ERR_RADIO_SEND_FAILED;
    }
    Serial.println(" - done");

    return err::ERR_OK;
}