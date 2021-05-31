#include <shmOpen.h>
#include <unistd.h>
#include <stdint.h>

void test_shm();
void slowInc2(uint64_t *p, uint64_t inc);
void dec2();
void inc2();