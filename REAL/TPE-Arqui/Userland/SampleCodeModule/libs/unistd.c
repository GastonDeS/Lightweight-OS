#include <unistd.h>

void createProcess(void (*function)(),char **argv){
    createProcessSyscall(function,argv);
}