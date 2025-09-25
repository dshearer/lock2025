#include "timer.h"
#include <Arduino.h>

// NOTE: I think RadioHead uses the TC3 timer

static volatile bool gTimerFired = false;

static void stop() {
    // Stop the timer
    TC4->COUNT32.CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);  // Wait for sync

    // Clear interrupt flag (required!)
    TC4->COUNT32.INTFLAG.reg = TC_INTFLAG_MC0;
}

void TC4_Handler() {
    noInterrupts();
    gTimerFired = true;
    stop();
    interrupts();
}

void timer::start(int timeoutMs) {
    // 4. Calculate and set compare value
    const uint32_t scaledHz = 48000000 / 256; // corresponds to TC_CTRLA_PRESCALER_DIV256, above
    // Timer ticks scaledHz times per second.
    const uint32_t scaledMsHz = scaledHz / 1000;
    // Timer ticks scaledMsHz times per millisecond.
    const uint32_t timerCount = scaledMsHz * timeoutMs;

    noInterrupts();

    gTimerFired = false;

    // 1. Enable clock to TC4
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5;
    while (GCLK->STATUS.bit.SYNCBUSY);

    // 2. Reset TC4 (good practice)
    TC4->COUNT32.CTRLA.reg = TC_CTRLA_SWRST;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);
    while (TC4->COUNT32.CTRLA.bit.SWRST);

    // 3. Configure timer
    TC4->COUNT32.CTRLA.reg = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_PRESCALER_DIV256;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);
    TC4->COUNT32.CC[0].reg = timerCount;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);

    // 5. Enable interrupt
    TC4->COUNT32.INTENSET.reg = TC_INTENSET_MC0;
    NVIC_EnableIRQ(TC4_IRQn);

    // 6. Start timer
    TC4->COUNT32.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);

    interrupts();
}

void timer::stop() {
    noInterrupts();
    stop();
    interrupts();
}

bool timer::fired() {
    return gTimerFired;
}