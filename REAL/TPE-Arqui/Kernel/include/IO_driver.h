#ifndef IO_DRIVER_H
#define IO_DRIVER_H
#include <stdint.h>
#include <regi.h>

void saveRegisters(registerStruct * registers);
void getRegisters(uint64_t * arr);
void get32bytesFromAddress(uint64_t address, uint64_t * target, uint8_t totalBytes);
void saveErrCode(uint64_t err);
uint64_t getErrCode();
#endif
