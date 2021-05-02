#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>

void timeHandler();
void delay(int cantTimerTick);
uint64_t getTicks();

#endif
