#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#include <regi.h>

void syscallHandler(registerStruct * registers);

#endif
