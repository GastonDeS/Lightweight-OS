#ifndef TIMER_DRIVER
#define TIMER_DRIVER

#include <stdint.h>
#include <timer_driver.h>
#include <interrupts.h>

static int counter = 0;
static int flag = 0;
static uint64_t ticks = 0;

void timeHandler() {
  ticks++;// por ahora lo vamos a poner aca cada N tics
  if(flag){
    counter++;
  }else
    if((ticks % 90) == 0)
      notSoDummyHandler(); 
}

void delay(int cantTimerTick){ //se le pasa la cantidad de ciclos de 55ms
  counter = 0;
  flag = 1;
  while (counter <= cantTimerTick){
    _hlt();
  }
  flag = 0;
}

uint64_t getTicks() {
  return ticks;
}

#endif
