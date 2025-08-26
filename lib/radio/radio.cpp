#include "radio.h"
#include <Arduino.h>

// These pins are for the Adafruit Feather 32u4 RFM69HCW
#define RFM69_CS_LOCK    8
#define RFM69_INT_LOCK   3
#define RFM69_RST_LOCK   4

// These pins are for the remote board
#define RFM69_CS_REMOTE    10  // D10 (a.k.a SS/NSS)
#define RFM69_INT_REMOTE   2   // D2

err::t radio::init(RH_RF69 **rf69, board_t board) {
    uint8_t cs = 0;
    uint8_t intr = 0;
    switch (board) {
        case BOARD_LOCK:
            cs = RFM69_CS_LOCK;
            intr = RFM69_INT_LOCK;
            break;
        case BOARD_REMOTE:
            cs = RFM69_CS_REMOTE;
            intr = RFM69_INT_REMOTE;
            break;
        default:
            return err::UNKNOWN;
    }

    RH_RF69 *radio = new RH_RF69(cs, intr);

    if (board == BOARD_LOCK) {
        // reset radio
        pinMode(RFM69_RST_LOCK, OUTPUT);
        digitalWrite(RFM69_RST_LOCK, LOW);
        digitalWrite(RFM69_RST_LOCK, HIGH);
        delay(10);
        digitalWrite(RFM69_RST_LOCK, LOW);
        delay(10);
    }

    // initialize radio
    if (!radio->init()) {
        return err::RADIO_INIT_FAILED;
    }
    if (!radio->setFrequency(radio::FREQ)) {
        return err::RADIO_SET_FREQ_FAILED;
    }
    // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
    // ishighpowermodule flag set like this:
    radio->setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

    *rf69 = radio;
    return err::OK;
}