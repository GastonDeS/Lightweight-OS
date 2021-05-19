#include <mySemaphore.h>


int sem_open(char* name, int initialValue){
  int result = 0;
  createSemSyscall(name, initialValue, &result);
  return result;
}

//elimina el semforo solo si no tiene procesos eperando
//si se libero devulve 1 sino 0 y si hubo un error -1
int sem_close(int semId){
    int result = 0;
    removeSemSyscall(semId, &result);
    return result;
}

int sem_wait(int semId){
    int result = 0;
    semSleepSyscall(semId, &result);
    return result;
    
}

int sem_post(int semId){
    int result = 0;
    semWakeUpSyscall(semId, &result);
    return result;
}

void printSem(){
    char str[101];
    printSemSyscall(str);
    print("%s", str);
}
