#include <unistd.h>
#include <syscallsASM.h>

void createProcess(void (*function),char **argv){
    createProcessSyscall(function,argv);
}