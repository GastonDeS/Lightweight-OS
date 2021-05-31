#include <sharedMemory.h>

#define N 100000

void slowInc2(uint64_t *p, uint64_t inc){
    int k = 0;
    uint64_t aux = *p;
  	aux += inc;
  	for (k = 0; k < 1000; k++);
  	*p = aux;
}


void inc2(){
	int i;
	uint64_t * ptr = shmOpen(1,  sizeof(uint64_t));

	if (ptr == 0)
		print(-1,"Error shm_open\n");

  	for (i = 0; i < N; i++)
		slowInc2(ptr, 1);

	print(-1,"end inc2: %d", *ptr);
	myExit();
}



void dec2(){
	int i;
	uint64_t * ptr = shmOpen(1, sizeof(uint64_t));

	if (ptr == 0)
		print(-1,"Error shm_open\n");

	for (i = 0; i < N; i++)
		slowInc2(ptr, -1);

	print(-1,"end dec2 : %d", *ptr);
	myExit();
}


void test_shm() {

    char *argv[2];
	argv[0] = "inc";
    argv[1] = NULL;
    createProcess(inc2, 0,argv);

	argv[0] = "dec";
    argv[1] = NULL;
    createProcess(dec2, 0,argv);
}
