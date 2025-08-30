#include "timer.h"

// NOTE: I think RadioHead uses the TC3 timer

volatile bool gTimer4Fired = false;
volatile bool gTimer5Fired = false;

static Timer *gTimer4 = nullptr;
static Timer *gTimer5 = nullptr;

static bool gInited = false;
static bool gTimer4InUse = false;
static bool gTimer5InUse = false;

void TC4_Handler() {
    gTimer4Fired = true;
    gTimer4->stop();
}

void TC5_Handler() {
    gTimer5Fired = true;
    gTimer4->stop();
}

Timer* Timer::get() {
    if (!gInited) {
        // enable bus clock to TC4 and TC5
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5;
        while (GCLK->STATUS.bit.SYNCBUSY);

        // make timers
        gTimer4 = new Timer(TC4, TC4_IRQn, &gTimer4Fired);
        gTimer5 = new Timer(TC5, TC5_IRQn, &gTimer5Fired);

        gInited = true;
    }

    if (!gTimer4InUse) {
        gTimer4InUse = true;
        return gTimer4;
    }

    if (!gTimer5InUse) {
        gTimer5InUse = true;
        return gTimer5;
    }

    return nullptr; // no timers available
}

void Timer::put(Timer* timer) {
    timer->stop();
    if (timer == gTimer4) {
        gTimer4InUse = false;
    }
    else if (timer == gTimer5) {
        gTimer5InUse = false;
    }
}

Timer::Timer(Tc* tc, IRQn_Type IRQn, volatile bool *timerFired)
    : _tc(tc), _IRQn(IRQn), _timerFired(timerFired) {
}

void Timer::start(int timeoutMs) {
    *(this->_timerFired) = false;

    // reset our timer (good practice)
    this->_tc->COUNT32.CTRLA.reg = TC_CTRLA_SWRST;
    while (this->_tc->COUNT32.STATUS.bit.SYNCBUSY);
    while (this->_tc->COUNT32.CTRLA.bit.SWRST);

    // configure timer
    this->_tc->COUNT32.CTRLA.reg = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_PRESCALER_DIV256;
    while (this->_tc->COUNT32.STATUS.bit.SYNCBUSY);

    // calculate and set compare value
    const uint32_t scaledHz = 48000000 / 256; // corresponds to TC_CTRLA_PRESCALER_DIV256, above
    // Timer ticks scaledHz times per second.
    const uint32_t scaledMsHz = scaledHz / 1000;
    // Timer ticks scaledMsHz times per millisecond.
    const uint32_t timerCount = scaledMsHz * timeoutMs;

    this->_tc->COUNT32.CC[0].reg = timerCount;
    while (this->_tc->COUNT32.STATUS.bit.SYNCBUSY);

    // enable interrupt
    this->_tc->COUNT32.INTENSET.reg = TC_INTENSET_MC0;
    NVIC_EnableIRQ(this->_IRQn);

    // start timer
    this->_tc->COUNT32.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (this->_tc->COUNT32.STATUS.bit.SYNCBUSY);
}

void Timer::stop() {
    // stop the timer
    this->_tc->COUNT32.CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (this->_tc->COUNT32.STATUS.bit.SYNCBUSY);  // Wait for sync

    // clear interrupt flag (required!)
    this->_tc->COUNT32.INTFLAG.reg = TC_INTFLAG_MC0;
}

bool Timer::fired() {
    return *(this->_timerFired);
}