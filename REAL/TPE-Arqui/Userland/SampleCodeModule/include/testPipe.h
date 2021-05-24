#ifndef PIPETEST_H
#define PIPETEST_H

#include <syscallsASM.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <commands.h>
#include <mySemaphore.h>

void testPipe();

#define SEM_NAME "testP"

#endif