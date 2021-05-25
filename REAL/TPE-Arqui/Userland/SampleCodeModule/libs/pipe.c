// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipe.h>

int pipeOpen(int pipeId){
    int result = 0;
    pipeOpenSyscall(pipeId, &result);
    return result;
}

int pipeCreate(){
    int result = 0;
    pipeCreateSyscall(&result);
    return result;
}

int pipeClose(int pipeId){
    int result = 0;
    pipeCloseSyscall(pipeId, &result);
    return result;
}

int pipeWrite(int pipeId, char * addr, int n){
    int result = 0;
    pipeWriteSyscall(pipeId, addr, n, &result);
    return result;
}

int pipeRead(int pipeId, char * addr, int n){
    int result = 0;
    pipeReadSyscall(pipeId, addr, n, &result);
    return result;
}

void printPipe(){
    int strSize = 1024;
    char str[strSize];
    printPipeSyscall(str, strSize);
    print(STDOUT, "%s", str);
}

int isPipe(int pipeId) {
    return pipeId >= 0;
}

void pipesOpen(int pipeId1, int pipeId2) {
    if(isPipe(pipeId1)) {
        pipeOpen(pipeId1);
    }
    if(isPipe(pipeId2)) {
        pipeOpen(pipeId2);
    }
}

void pipesClose(int pipeId1, int pipeId2) {
    if(isPipe(pipeId1)) {
        pipeClose(pipeId1);
    }
    if(isPipe(pipeId2)) {
        pipeClose(pipeId2);
    }
}