#ifndef EXC
#define EXC value
#include <regi.h>
#include <stdint.h>
#include <IO_driver.h>
#include <exceptions.h>
#include <video_driver.h>

void exc_0h(registerStruct * registers) {
  saveErrCode(0);
  saveRegisters(registers);
}

void exc_6h(registerStruct * registers) {
  saveErrCode(6);
  saveRegisters(registers);
}

void readError(uint64_t * target) {
  uint64_t error = getErrCode();
  *target = error;
}

#endif
