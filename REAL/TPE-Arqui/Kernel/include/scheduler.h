#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <interrupts.h>

uint64_t * scheduler(uint64_t *);
void addProcess(uint64_t *);
void endProcess(uint64_t pid);

#endif