#include "radio.h"
#include <Arduino.h>

#define RFM69_CS    8
#define RFM69_INT   3
#define RFM69_RST   4

err::t radio::initRadio(RH_RF69 **rf69) {
    RH_RF69 *radio = new RH_RF69(RFM69_CS, RFM69_INT);

    // reset radio
    pinMode(RFM69_RST, OUTPUT);
    digitalWrite(RFM69_RST, LOW);
    digitalWrite(RFM69_RST, HIGH);
    delay(10);
    digitalWrite(RFM69_RST, LOW);
    delay(10);

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