// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <inforeg.h>
#include <stdint.h>
#include <syscallsASM.h>

void getRegisters(uint64_t * registers){
    getRegistersSyscall(registers);
}
