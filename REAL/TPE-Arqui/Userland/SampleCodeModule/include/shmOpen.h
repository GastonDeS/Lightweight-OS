#ifndef SHMOPEN
#define SHMOPEN

#include <syscallsASM.h>
#include <stddef.h>

void *shmOpen(int fd, int size) ;

#endif