#include <mySemaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>

#define TOTAL_PAIR_PROCESSES 20
#define SEM_ID "sem"

void test_sync();
void test_no_sync();