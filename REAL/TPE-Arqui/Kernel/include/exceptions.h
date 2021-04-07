#ifndef EXC_H
#define EXC_H

#include <regi.h>
#include <stdint.h>

void exc_0h(registerStruct * registers);
void exc_6h(registerStruct * registers);
void readError(uint64_t * target);

#endif
