#ifndef PIPEDRIVER_H
#define PIPEDRIVER_H

#include <stddef.h>
#include <stdlib.h>
#include <sem.h>


#define PIPE_MAX 20   //cantidad de semaforos permitdos
#define BUFF_SIZE 256  //cantidad de semaforos permitdos

//devuelve el pipeId y si hubo un error devuelve -1
void pipeCreate(int *returnValue);

void pipeOpen(int pipeId, int *returnValue);

//si borro la el pipe devuelve 1 sino 0
//solo eliminar el pipe si pudo eliminar el sem
void pipeClose(int pipeId, int *returnValue);

void pipeWrite(int pipeId, char * addr, int n, int *returnValue);

void pipeRead(int pipeId, char * addr, int n, int *returnValue);

void printPipe(char *str, int strSize);

#endif