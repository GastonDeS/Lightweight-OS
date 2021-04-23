#include <test_util.h>
#include <MemMang.h>
#include <stdio.h>
#include <stdGraphics.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 5000 //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void * memsetsss(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  while (1){
    rq = 0;
    total = 0;
    drawRect(0,0,10,10,0x130923);
    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = malloc(mm_rqs[rq].size); 
      if(mm_rqs[rq].address == NULL)
        drawRect(11,0,10,10,0x00ff00);
        // printf("la direccion: %d\n es NULL", rq);
      total += mm_rqs[rq].size;
      rq++;
    }

    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL) {
        drawRect(31,0,10,10,0x0000ff);
        memsetsss(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required
      }
        
    
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          drawRect(21,0,10,10,0xff0000);
          // printf("ERROR!\n"); // TODO: Port this call as required
    

    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        free(mm_rqs[i].address); 
  } 
}

