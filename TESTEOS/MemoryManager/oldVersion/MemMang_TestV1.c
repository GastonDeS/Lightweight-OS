#include "MemMang.h"

int main(int argc, char const *argv[]){

    //verifico el uso completo de bloques
    char *aux1 = (char*)my_malloc(100);
    char *aux2 = (char*)my_malloc(100);
    char *aux3 = (char*)my_malloc(100);
    char *aux4 = (char*)my_malloc(100);
    char *aux5 = (char*)my_malloc(100);

    printMemoryBLock();
    checkMemory();
  /*  
    my_free(aux1);
    my_free(aux2);
    my_free(aux3);
    my_free(aux4);
    my_free(aux5);
    printMemoryBLock();
    checkMemory();
/*
    
    aux1 = (char*)my_malloc(100);
    aux2 = (char*)my_malloc(50);
    aux3 = (char*)my_malloc(876);
    for (int i = 0; i < 100; i++){aux1[i] = i;}
    for (int i = 0; i < 50; i++){aux2[i] = i;} 
    for (int i = 0; i < 876; i++){aux3[i] = i;}
    printAllMemory();
    checkMemory();

    my_free(aux2);
    printMemoryBLock();
    checkMemory();
    

    char *auxT = (char*)my_malloc(38);
    auxT = (char*)my_malloc(529);
    auxT = (char*)my_malloc(43);
    auxT = (char*)my_malloc(456);
    auxT = (char*)my_malloc(352);
    auxT = (char*)my_malloc(5263);
    auxT = (char*)my_malloc(50);
    auxT = (char*)my_malloc(69);
    auxT = (char*)my_malloc(89);
    printMemoryBLock();
    checkMemory();
    */
    
    void MM_end();
    return 0;
}
