#ifndef SEM_H
#define SEM_H

#include <scheduler.h>
#include <MemMang.h>
#include <listADT.h>



//devuleve el id del semaforo
void createSem(int* semId);

void removeSem(int semId);//falta implementar

//si hubo un error flag = -1 
void semSleep(int semId, int* flag);
void semWakeUp(int semId, int*flag);





#endif