#ifndef __TIMER_H__
#define __TIMER_H__

namespace timer {
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
}

#endif