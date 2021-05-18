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

void test_mm(){

  int errores=0;


  //testeo que no me de memoria masyor al heapsize
  void * start = my_malloc(16384);
  if (start != NULL) {
    printf("me debería dar null\n");
    errores+=1;
  }

  
  //testeo si se forma bien el array a listas en el buddy
  void * test2 = my_malloc(200);
  if (checkMemory()==0){
    printf("error de checkMem");
    errores +=1;
  }
  checkMemory();
  my_free(test2);
  checkMemory();


  //testeo si funciona bien el free y si se junta con el buddy la memoria
  void * test3 =my_malloc(8000);
  my_free(test3);
  void * newTest3 = my_malloc(1600);
  if (newTest3 == NULL){
    printf("error test3 \n");
    errores +=1;
  }
  checkMemory();
  my_free(newTest3);
  checkMemory();

  //testeo si ocupa bien toda la memoria con dos mallocs de mitad de memoria
  void * test4 =my_malloc(8000);
  checkMemory();
  void * newTest4 = my_malloc(8000);
   checkMemory();
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
  checkMemory();
  my_free(test6);
  my_free(test6);
  my_free(test6);
  checkMemory();


  printf("Total de errores: %d\n", errores);
  MM_end();

}

int main(){
  test_mm();
  return 0;
}