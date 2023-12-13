/* 
 * sysclock.h - configuration routines for KL25Z system clock
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 * Reused by Suhas Srinivasa Reddy
 *
 */

#ifndef _SYSCLOCK_H_
#define _SYSCLOCK_H_

#define SYSCLOCK_FREQUENCY (24000000U)

/*
 * Initializes the system clock. You should call this first in your
 * program.
 */
void Init_Sysclock();

#endif  // _SYSCLOCK_H_
