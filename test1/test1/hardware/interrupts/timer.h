/*
 * timer.h
 *
 *  Created on: 23.02.2014
 *      Author: john
 */
#include "../../global.h"

#ifndef TIMER_H_
#define TIMER_H_

extern uint8_t red_state;

extern void SetUpTimers();

extern void Timer0IntHandlerA();
extern void Timer0IntHandlerB();
extern void Timer1IntHandlerA();
extern void Timer1IntHandlerB();
extern void Timer2IntHandlerA();
extern void Timer2IntHandlerB();
extern void Timer3IntHandlerA();
extern void Timer3IntHandlerB();
extern void Timer4IntHandlerA();
extern void Timer4IntHandlerB();
extern void Timer5IntHandlerA();
extern void Timer5IntHandlerB();

#endif /* TIMER_H_ */
