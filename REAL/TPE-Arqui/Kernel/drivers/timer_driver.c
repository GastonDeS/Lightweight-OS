#ifndef TIMER_DRIVER
#define TIMER_DRIVER

#include <stdint.h>
#include <timer_driver.h>
#include <interrupts.h>

static uint64_t ticks = 0;

void timeHandler() {
  ticks++;
  // por ahora lo vamos a poner aca cada 10 tics
  if ((ticks % 10 ) == 0){
    notSoDummyHandler();
  }
}

uint64_t getTicks() {
  return ticks;
}

#endif
