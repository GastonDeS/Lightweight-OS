#ifndef SEMV2_H
#define SEMV2_H

#include <listADT.h>

//crea un semaforo en kernel
//si hubo un error returnValue = -1
void createSem(int semId, int* returnValue){

//elimina el semforo solo si no tiene procesos eperando
//returnValue = 1 si lo libero sin es igual a 0
void removeSem(int semId, int* returnValue);

//si hubo un error returnValue = -1 sino 0
void semSleep(int semId, int* returnValue);

//si habia procesos en la cola returnValue = 1 sino 0
void semWakeUp(int semId, int*returnValue);


#endif