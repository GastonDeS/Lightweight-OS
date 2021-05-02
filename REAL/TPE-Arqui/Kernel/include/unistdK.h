#ifndef UNISTDK_H
#define UNISTDK

#include <scheduler.h>

void createProcess(void (*function)(),char **argv);
void exceptionProcess();

#endif