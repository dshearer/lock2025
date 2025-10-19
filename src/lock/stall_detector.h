#ifndef __STALL_DETECTOR_H__
#define __STALL_DETECTOR_H__

#include <err.h>
#include "current.h"

/*!
    \brief A class for detecting when the motor has stopped working. It can detect problems of various types (which are
    implemented as subclasses of StallDetectorImpl).
*/
class StallDetector {
public:
    StallDetector(unsigned long now);
    ~StallDetector();

    /*!
        \brief Tell the stall detector what the current time is and how much current is going to the motor.
        The method uses this information to decide whether the motor has stopped working. If it hasn't detected
        a problem, it returns err::OK. Otherwise, it returns an appropriate error value.
    */
    err::t update(unsigned long now, current::v current);

private:
    void *_listNode;
};

#endif