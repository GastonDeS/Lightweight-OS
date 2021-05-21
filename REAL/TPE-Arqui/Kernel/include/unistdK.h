#ifndef UNISTDK_H
#define UNISTDK

#include <scheduler.h>

void createProcess(void (*function)(),char **argv, uint64_t *pid);
void exceptionProcess();

#endif