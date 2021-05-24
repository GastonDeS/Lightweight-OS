#ifndef SEM_H
#define SEM_H

#include <listADT.h>
#include <scheduler.h>
#include <stddef.h>
#include <semAsm.h>
#include <stdlib.h>

#define SEM_MAX 20 //cantidad de semaforos permitdos

//si hubo un error returnValue = -1, sino retorna el semId
void createSem(char *semName, int initialValue, int* returnValue);

//elimina el semforo solo si no tiene procesos eperando
//returnValue = 1 si lo libero sino es igual a 0
void removeSem(int semId, int* returnValue);

//si hubo un error returnValue = -1 sino 0
void semWait(int semId, int* returnValue);

//desbloque solo al proceso con ese pid, si esta
int semPostPid(int semId, int pid);

void printSem(char *result, int strSize);


//para el kernel
void semPost(int semId, int*returnValue);
int semPidBlock(int semId);

#endif