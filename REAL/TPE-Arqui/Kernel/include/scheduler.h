#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <interrupts.h>
#include <stdlib.h>

void yield();
uint64_t * scheduler(uint64_t *currentProces);
void addProcess(uint64_t *currentProces, char *name,uint64_t *pid,uint64_t *ep, uint64_t argc, char** argv);
void endProcessWrapper(uint64_t pid, int *result);
void getPid(uint64_t *pid);
void listAllProcess(char **ProcessList);
void blockProcess(uint64_t pid, int *result);
void unlockProcess(uint64_t pid, int *result);
void exceptionProcess();
void ps(char *array);
void nice(uint64_t pid, uint64_t priority, int *result);

#endif