#include <mySemaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#define TOTAL_PAIR_PROCESSES 100
#define SEM_ID "sem"

void test_sync();
void test_no_sync();