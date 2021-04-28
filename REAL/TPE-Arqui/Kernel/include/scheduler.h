#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <interrupts.h>

uint64_t * scheduler(uint64_t *);
void addProcess(uint64_t *);
void endProcess(uint64_t pid);
void getPid(uint64_t *pid);
void listAllProcess(char **ProcessList);
void blockProcess(uint64_t pid);
void unlockProcess(uint64_t pid);

#endif