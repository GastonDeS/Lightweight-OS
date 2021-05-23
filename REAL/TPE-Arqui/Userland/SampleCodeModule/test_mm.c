#include <test_mm.h>

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  print("\n");
  int k;
  for(k=0; k<N; k++){
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = malloc(mm_rqs[rq].size);
      if(mm_rqs[rq].address == NULL)
        print("la direccion: %d es NULL \n", rq);
      total += mm_rqs[rq].size;
      rq++;
    }
    //set
    uint32_t i;
    for (i = 0; i < rq; i++) {
      if (mm_rqs[i].address != NULL){
        uint64_t length = mm_rqs[i].size;
        uint8_t chr = (uint8_t)i;
	      char * dst = (char*)mm_rqs[i].address;
	      while(length--) {
          dst[length] = chr;
        }
      }
    }
  
    //check  
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          print("ERROR!\n"); 
    
    //free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL) {
        free(mm_rqs[i].address);
      }
    print("vuelta : %d de %d \n", k + 1, N); 
  } 
}

