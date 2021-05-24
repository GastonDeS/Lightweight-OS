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

int printPipe(){
    int strSize = 1024;
    char str[strSize];
    printPipeSyscall(str, strSize);
    print("%s", str);
}