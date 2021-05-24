#include <unistd.h>

void yield(){
    yieldSyscall();
}

uint64_t createProcess(void (*function)(), int foreground,char **argv){
    uint64_t pid;
    createProcessSyscall(function,foreground,argv, &pid);
    return pid;
}

void myExit(){
    uint64_t pid;
    getPidSyscall(&pid);
    int ans;
    endProcessSyscall(pid,&ans);
    while (1);
}

int nice(uint64_t pid, uint64_t prio){
    int ans;
    niceSyscall(pid, prio,&ans);
    return ans;
}

int block(uint64_t pid) {
    int ans;
    blockPidSyscall(pid, &ans);
    return ans;
}

int unblock(uint64_t pid) {
    int ans;
    unblockPidSyscall(pid, &ans);
    return ans;
}

int kill(uint64_t pid){
    int ans;
    endProcessSyscall(pid,  &ans);
    return ans;
}