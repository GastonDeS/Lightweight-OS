#ifndef EXC_DISPATCHER
#define EXC_DISPATCHER

#include <regi.h>
#include <interrupts.h>
#include <exceptions.h>
#include <scheduler.h>
#include <video_driver.h>
#include <timer_driver.h>


void printException();

void exceptionDispatcher(uint64_t exc, registerStruct * registers) {
  exceptionProcess();
  switch(exc) {
    case 0: exc_0h(registers);
    printException("00");
    break;
    case 6: exc_6h(registers);
    printException("06");
    break;
  }
}

void printException(char *exception){
  clearDisplay(0x0000ff);
  writeStr("A fatal exception",19,200,200,0xffffff,0x000000,2,1);
  writeStr(exception,3,200+18*8*2,200,0xffffff,0x000000,2,1);
  writeStr("has ocurred",12,200+18*8*2+3*8*2,200,0xffffff,0x000000,2,1);
  writeStr("wait",5,200,400,0xffffff,0x000000,2,1);
  writeStr("seconds to reboot the system",29,200+8*2*7,400,0xffffff,0x000000,2,1);
  writeStr("5",2,200+8*2*5,400,0xffffff,0x0000ff,2,0);
  delay(20);
  writeStr("4",2,200+8*2*5,400,0xffffff,0x0000ff,2,0);
  delay(20);
  writeStr("3",2,200+8*2*5,400,0xffffff,0x0000ff,2,0);
  delay(20);
  writeStr("2",2,200+8*2*5,400,0xffffff,0x0000ff,2,0);
  delay(20);
  writeStr("1",2,200+8*2*5,400,0xffffff,0x0000ff,2,0);
  delay(20);
}

#endif
