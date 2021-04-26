#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "test_util.h"
#include "MemMang.h" 

#define MAX_BLOCKS 100000
#define MAX_MEMORY 10000 //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  while (1){
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = my_malloc(mm_rqs[rq].size); // TODO: Port this call as required
      if(mm_rqs[rq].address == NULL)
        //printf("la direccion: %d es NULL \n", rq);
      total += mm_rqs[rq].size;
      rq++;
    }
 
    if(checkMemory(0)<0){
      checkMemory(1);
      printMemoryBLock(0);
      return;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
          printf("ERROR!\n"); // TODO: Port this call as required
          checkMemory(1);
          return;
        }

    if(checkMemory(1)<0){
      checkMemory(1);
      printMemoryBLock(0);
      return;
    }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        my_free(mm_rqs[i].address);  // TODO: Port this call as required

    if(checkMemory(0)<0){
      checkMemory(1);
      printMemoryBLock(0);
      return;
    }
    printf("\n\n\n");
    sleep(1);
  } 
}

int main(){
  test_mm();
  return 0;
}
