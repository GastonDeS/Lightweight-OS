#ifndef SEM_H
#define SEM_H

#include <scheduler.h>
#include <MemMang.h>
#include <listADT.h>



//devuleve el id del semaforo
void createSem(int* semId);
//elimina el semforo solo si no tiene procesos eperando
void removeSem(int semId, int* returnValue);

//si hubo un error flag = -1 
void semSleep(int semId, int* returnValue);
void semWakeUp(int semId, int*returnValue);





#endif