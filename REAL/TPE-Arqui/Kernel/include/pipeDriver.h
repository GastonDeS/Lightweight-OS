#ifndef PIPEDRIVER_H
#define PIPEDRIVER_H

#include <stddef.h>
#include <stdlib.h>
#include <sem.h>
#include <scheduler.h>
#include <listADT.h>

#define BLOCK 100 //cantidad de semaforos permitdos
#define PIPE_SIZE 256 //cantidad de semaforos permitdos


void pipe(int *returnValue);

//si borro la el pipe devuelve 1 sino 0
//no barra el pipe si hay proceso bloqueados
void pipeClose(int pipeId, int *returnValue);

void pipeWrite(int pipeId, char * addr, int n, int *returnValue);

void pipeRead(int pipeId, char * addr, int n, int *returnValue);

#endif