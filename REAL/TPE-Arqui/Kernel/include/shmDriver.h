#ifndef SHMDRIVER_H
#define SHMDRIVER_H

#include <stddef.h>
#include <stdlib.h>

#define SHM_SIZE 10

void shmOpen(int id, int size, void *returnValue);

#endif
