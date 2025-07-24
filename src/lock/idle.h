#ifndef __IDLE_H__
#define __IDLE_H__

/*!
 * @brief Put the computer into low-power mode and block. The computer will wake up, and
 * this function return, when an interrupt occurs.
 */
void idle();

#endif