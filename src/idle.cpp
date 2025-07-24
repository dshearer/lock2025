#include "idle.h"
#include <LowPower.h>

void idle() {
    return;

    // Serial.println("Sleeping");
    #if defined (__AVR__)
    LowPower.idle(SLEEP_8S, ADC_ON, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    #elif defined (__SAMD21G18A__)
    LowPower.idle(IDLE_2);
    #endif
    // Serial.println("Woke up from sleep");
}