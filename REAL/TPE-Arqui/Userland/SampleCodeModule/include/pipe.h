#ifndef PIPE_H
#define PIPE_H

#include <syscallsASM.h>
#include <stdio.h>

int pipeOpen(int pipeId);

int pipeCreate();

int pipeClose(int pipeId);

int pipeWrite(int pipeId, char * addr, int n);

int pipeRead(int pipeId, char * addr, int n);

int printPipe();

#endif