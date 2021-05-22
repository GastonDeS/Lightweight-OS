#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../BuddyMemMang.h"

#define MAX_BLOCKS 512
#define MAX_MEMORY 16384 //Should be around 80% of memory managed by the MM

int randNum(int min, int max)
{
    return (min + rand() % (max + 1 - min));
}

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void test_mm(){
  MM_end();

  int errores=0;

  //testeo si forma y reforma la lista bien

  void * posta = my_malloc(8000);
  checkMemory();
  my_free(posta);
  checkMemory();
  printf("HASTA ACAA\n\n\n");
  
  //testeo que no me de memoria masyor al heapsize
  void * start = my_malloc(16384);
  if (start != NULL) {
    printf("me debería dar null\n");
    errores+=1;
  }
  my_free(start);

  
  //testeo si se forma bien el array a listas en el buddy
  void * test2 = my_malloc(200);
  if (checkMemory()==0){
    printf("error de checkMem test 2");
    errores +=1;
  }
 if (checkMemory()==0){
    printf("error de checkMem test 22");
 }
  my_free(test2);
  if (checkMemory()==0){
    printf("error de checkMem test 23");
  }


  //testeo si funciona bien el free y si se junta con el buddy la memoria
  void * test3 =my_malloc(8000);
  printf("_____  ACAA _____ \n");
  my_free(test3);
  void * newTest3 = my_malloc(1600);
  if (newTest3 == NULL){
    printf("error test3 \n");
    errores +=1;
  }
 if (checkMemory()==0){
    printf("error de checkMem test 3");
 }
  my_free(newTest3);
  if (checkMemory()==0){
    printf("error de checkMem tes 3 post free");
  }

  //testeo si ocupa bien toda la memoria con dos mallocs de mitad de memoria
  void * test4 =my_malloc(8000);
  if (checkMemory()==0){
    printf("error de checkMem malloc test 4");
  }
  void * newTest4 = my_malloc(8000);
  if (checkMemory()==0){
    printf("error de checkMem segundo malloc test 4");
  }
  if (newTest4 == NULL){
    printf("error test4 \n");
    errores +=1;
  }
  my_free(test4);
  my_free(newTest4);


  //testeo si con un solo bloque de 32 gigas ya no puedo hacer mas de toda la memoria (no deberia);
  void * test5 =my_malloc(1);
  void * newTest5 = my_malloc(16360);
  if (newTest5 != NULL){
    printf("error test5 \n");
    errores +=1;
  }
  my_free(test5);
  my_free(newTest5);

  //Testeo de hacer mas de un free a ver si no me da problemas
  void * test6=my_malloc(200);
  if (checkMemory()==0){
    printf("error de checkMem malloc test 6");
  }
  my_free(test6);
  my_free(test6);
  my_free(test6);
  if (checkMemory()==0){
    printf("error de checkMem post test 6");
  }
  //Testeo de hacer un bloque chico y uno de mas de la mitad, no se deberia poder
  void * test7=my_malloc(5);
  void * newTest7=my_malloc(10000);
  if (newTest7 != NULL){
    printf("error test7 \n");
    errores +=1;
  }
  my_free(test7);
  my_free(newTest7);

  printf("Total de errores: %d\n", errores);

  printf("////////////////////////////////////////////////\n");

  mm_rq mm_rqs[MAX_BLOCKS];
  uint64_t rq;
  uint32_t total;
  uint8_t cont;


  while (1){
    rq = 0;
    total = 0;
    cont = 1;
    uint64_t i = 0;
    
    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY && cont){
      mm_rqs[rq].size = randNum(1024, 4000);
      printf("creo %d \n", mm_rqs[rq].size);
      mm_rqs[rq].address = my_malloc(mm_rqs[rq].size); // TODO: Port this call as required
      if(!checkMemory()) {
        printf("error");
      }
      if(mm_rqs[rq].address == NULL) {
        cont = 0;
        printf("entre");
      }
      total += mm_rqs[rq].size;
      rq++;
    }

  

    if(!checkMemory()){
      printf("error");
    }

    // Free
    for (i = 0; i < rq; i++) {
      if (mm_rqs[i].address != NULL) {
        if(!checkMemory()) {
        printf("error");
      }
        my_free(mm_rqs[i].address);
        printf("free");
      }
    }

    printf("\n\n\n");
  }
  }


int main(){
  test_mm();
  return 0;
}