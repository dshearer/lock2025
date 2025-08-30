#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>

class Timer {
public:
    /*!
        @brief Get a timer. If there are no timers available, returns nullptr.
        Timers must be returned with put() when no longer needed.
    */
    static Timer *get();

    /*!
        @brief Return a timer to the pool of available timers.
        After calling this function, the timer pointer is no longer valid and must not be used.
        @param timer The timer to return.
    */
    static void put(Timer *timer);

    /*!
        @brief Start the timer with a timeout in milliseconds. If the timer is already running, it will be restarted
        with the given timeout.
    */
    void start(int timeoutMs);

    /*!
        @brief Stop the timer if it is running. If the timer is not running, this function does nothing.
    */
    void stop();

    /*!
        @brief Check if the timer has fired (i.e., if the timeout has elapsed).
        After the timer fires, this function will return true until the timer is started again.
        @return true if the timer has fired, false otherwise.
    */
    bool fired();

private:
    Timer(Tc *tc, IRQn_Type IRQn, volatile bool *timerFired);
    
    Tc *_tc;
    IRQn_Type _IRQn;
    volatile bool *_timerFired;
};

#endif