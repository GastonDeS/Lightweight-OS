#ifndef SEM_H
#define SEM_H

#include <scheduler.h>
#include <MemMang.h>
#include <listADT.h>

int maxId = 0;

typedef struct{
    int semId;
    listADT processQueue;
}semData;

int equals(void *e1, void *e2){
    semData element1 = *((semData *)e1);
    semData element2 = *((semData *)e2);
    return element2.semId == element1.semId;
};

listADT semList = NULL;

//devuleve el id del semaforo
int createSem();

//si hubo un error flag = -1 
void semSleep(int semId, int* flag);
void semWakeUp(int semId, int*flag);





#endif