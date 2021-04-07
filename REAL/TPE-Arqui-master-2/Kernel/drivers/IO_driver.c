#ifndef IO_DRIVER
#define IO_DRIVER
#include <regi.h>
#include <stdint.h>

static registerStruct registerSnapshot;

void saveRegisters(registerStruct * registers) {
  registerSnapshot.r15 = registers->r15;
  registerSnapshot.r14 = registers->r14;
  registerSnapshot.r13 = registers->r13;
  registerSnapshot.r12 = registers->r12;
  registerSnapshot.r11 = registers->r11;
  registerSnapshot.r10 = registers->r10;
  registerSnapshot.r9 = registers->r9;
  registerSnapshot.r8 = registers->r8;
  registerSnapshot.rsi = registers->rsi;
  registerSnapshot.rdi = registers->rdi;
  registerSnapshot.rbp = registers->rbp;
  registerSnapshot.rdx = registers->rdx;
  registerSnapshot.rcx = registers->rcx;
  registerSnapshot.rbx = registers->rbx;
  registerSnapshot.rax = registers->rax;
  registerSnapshot.rsp = registers->rsp;
  registerSnapshot.rip = registers->rip;
}

registerStruct * getRegisters() {
  return &registerSnapshot;
}

void get32bytesFromAddress(uint64_t address, uint8_t * target, uint8_t totalBytes) {
  uint64_t * pos = (uint64_t *) address;
  for (uint8_t i = 0; i < totalBytes; i++) {
    target[i] = pos[i];
  }
}

#endif
