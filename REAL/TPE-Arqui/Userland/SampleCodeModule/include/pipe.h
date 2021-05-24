#ifndef PIPE_H
#define PIPE_H

#include <syscallsASM.h>
#include <stdio.h>
#include <stddef.h>

int pipeOpen(int pipeId);

int pipeCreate();

int pipeClose(int pipeId);

int pipeWrite(int pipeId, char * addr, int n);

int pipeRead(int pipeId, char * addr, int n);

void printPipe();

int isPipe(int pipeId);

void pipesOpen(int pipeId1, int pipeId2);

void pipesClose(int pipeId1, int pipeId2);

int isPipe(int pipeId);

void pipesOpen(int pipeId1, int pipeId2);

void pipesClose(int pipeId1, int pipeId2);

#endif