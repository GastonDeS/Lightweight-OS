#include <inforeg.h>
#include <stdint.h>
#include <syscallsASM.h>

void getRegisters(uint64_t * registers){
    getRegistersSyscall(registers);
}
