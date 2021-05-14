#ifndef UNISTD_H
#define UNISTD

#include <syscallsASM.h>

void createProcess(void (*function)(),char **argv);
void myExit();

#endif