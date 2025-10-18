#include "current.h"
#include "pins.h"
#include <Arduino.h>

void current::init() {
    pinMode(MOTOR_CURRENT_PIN, INPUT);
}

current::v current::read() {
    // compute voltage coming out of INA169:
    // V_out = (analogRead_value / 1023) × Reference_Voltage

    /*
    Interpreting V_out:

    The INA169's board has a "shunt" resistor of value R_s and a "load" resistor of value R_l.
    The INA169 will output a voltage V_out proportional to the current I_s flowing through the shunt resistor:

        V_out = I_s * R_s * R_l / 1,000

    Rearranging:

        I_s = V_out * 1,000 / (R_s * R_l)

    On our board, R_s = 0.1 ohm and R_l = 10k ohm. So

        I_s = V_out * 1,000 / (0.1 * 1,000)
        I_s = V_out * 10

    Then convert to mA: 1,000 * I_s = 10,000 * V_out (mA)

    VALUE RANGES

    Motor draws between 0 and 150 mA. So 0 <= I_s <= 150 mA. So 0 <= V_out <= 0.15 * 0.1 * 10k / 1,000 = 0.15 V.
    analogRead_value = 1023 * V_out / Reference_Voltage
    So 0 <= analogRead_value <= 47.

    I_s_ma = 1,000 * I_s = 10,000 * V_out = 10,000 * (analogRead_value / 1023) × 3.3
           = 33,000 * analogRead_value / 1023
           = 32.26 * analogRead_value
    */

   return (current::v) analogRead(MOTOR_CURRENT_PIN);
}