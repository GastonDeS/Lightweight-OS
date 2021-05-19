#ifndef SEM_H
#define SEM_H

#include <listADT.h>
#include <scheduler.h>
#include <stddef.h>
#include <semAsm.h>
#include <stdlib.h>

#define BLOCK 1000 //cantidad de semaforos permitdos

//crea un semaforo en kernel
//si hubo un error returnValue = -1
void createSem(char *semName, int initialValue, int* returnValue);

//elimina el semforo solo si no tiene procesos eperando
//returnValue = 1 si lo libero sino es igual a 0
void removeSem(int semId, int* returnValue);

//si hubo un error returnValue = -1 sino 0
void semSleep(int semId, int* returnValue);

//si habia procesos en la cola returnValue = 1 sino 0
void semWakeUp(int semId, int*returnValue);

void printSem(char *result);

#endif