#include "stall_detector.h"
#include "current.h"
#include <num_elems.h>
#include <cstddef>
#include <cassert>
#include <Arduino.h>

class StallDetectorImpl {
public:
    virtual ~StallDetectorImpl() {}
    virtual err::t update(unsigned long now, current::v current) = 0;
};

typedef bool(*isProblematicCurrentFunc_t)(current::v);

class CurrentCountingDetector : public StallDetectorImpl {
private:
    // params
    isProblematicCurrentFunc_t _isProblematicCurrent;
    size_t _nbrToError;

    // state
    current::v _currents[20];
    size_t _currentCount;
    size_t _oldestIdx;
    size_t _problematicCurrentCount;

public:
    CurrentCountingDetector(isProblematicCurrentFunc_t isProblematicCurrent, size_t nbrToError) 
    : _isProblematicCurrent(isProblematicCurrent), _nbrToError(nbrToError), _currentCount(0), _oldestIdx(0), _problematicCurrentCount(0) {}

    virtual err::t update(unsigned long now, current::v current) {
        if (this->_currentCount >= NUM_ELEMS(this->_currents)) {
            // replace oldest current
            const current::v prevOldestCurrent = this->_currents[this->_oldestIdx];
            this->_currents[this->_oldestIdx] = current;
            ++this->_oldestIdx;
            if (this->_oldestIdx >= NUM_ELEMS(this->_currents)) {
                this->_oldestIdx = 0;
            }

            // update problematic-current count
            if (this->_isProblematicCurrent(current)) {
                if (!this->_isProblematicCurrent(prevOldestCurrent)) {
                    ++this->_problematicCurrentCount;
                }
            } else if (this->_isProblematicCurrent(prevOldestCurrent)) {
                --this->_problematicCurrentCount;
            }
        } else {
            // append to array
            this->_currents[this->_currentCount] = current;
            ++this->_currentCount;

            // update problematic-current count
            if (this->_isProblematicCurrent(current)) {
                ++this->_problematicCurrentCount;
            }
        }

        Serial.print("Problematic-current count: ");
        Serial.println(this->_problematicCurrentCount);

        const bool isStalled = this->_problematicCurrentCount >= this->_nbrToError;
        if (isStalled) {
            Serial.println("Stalled due to current");
        }
        return isStalled ? err::MOTOR_STALLED : err::OK;
    }
};

#define TURN_TIMEOUT_MS 5000

class TimeStallDetector : public StallDetectorImpl {
private:
    unsigned long _startTime;

public:
    TimeStallDetector(unsigned long now) : _startTime(now) {}

    virtual err::t update(unsigned long now, current::v current) {
        if (now < this->_startTime) {
            return false;
        }
        const bool isStalled = now - this->_startTime >= TURN_TIMEOUT_MS;
        if (isStalled) {
            Serial.println("Stalled due to timeout");
        }
        return isStalled ? err::GEAR_NEVER_REACHED_END : err::OK;
    }
};

#define HIGH_CURRENT 200
#define NEEDED_HIGH_CURRENT_COUNT 3

static bool currentIsHigh(current::v current) {
    return current >= HIGH_CURRENT;
}

static bool currentIsZero(current::v current) {
    return current == 0;
}

class ListNode {
public:
    StallDetectorImpl *detector;
    ListNode *next;

    ListNode(StallDetectorImpl *d) : detector(d), next(nullptr) {}
};

StallDetector::StallDetector(unsigned long now) : _listNode(nullptr) {
    const bool active = true;

    if (!active) {
        return;
    }

    ListNode *node1 = new ListNode(new CurrentCountingDetector(currentIsHigh, NEEDED_HIGH_CURRENT_COUNT));
    ListNode *node2 = new ListNode(new CurrentCountingDetector(currentIsZero, 15));
    ListNode *node3 = new ListNode(new TimeStallDetector(now));
    node1->next = node2;
    node2->next = node3;
    this->_listNode = node2;
}

StallDetector::~StallDetector() {
    ListNode *node = (ListNode *) this->_listNode;
    while (node != nullptr) {
        ListNode *next = node->next;
        delete node->detector;
        delete node;
        node = next;
    }
}

err::t StallDetector::update(unsigned long now, current::v current) {
    err::t e = err::OK;
    ListNode *node = (ListNode *) this->_listNode;
    while (node != nullptr) {
        const err::t currErr = node->detector->update(now, current);
        if (e == err::OK) {
            e = currErr;
        }
        node = node->next;
    }
    return e;
}