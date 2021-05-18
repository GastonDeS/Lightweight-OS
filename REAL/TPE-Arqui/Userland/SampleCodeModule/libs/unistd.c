#include <unistd.h>

void yield(){
    yieldSyscall();
}

void createProcess(void (*function)(),char **argv){
    createProcessSyscall(function,argv);
}

void myExit(){
    uint64_t pid;
    getPidSyscall(&pid);
    endProcessSyscall(pid);
    while (1);
}