#include "MemMang1.h"

int main(int argc, char const *argv[]){

    //verifico el uso completo de bloques
    char *aux1 = (char*)my_malloc(100);
    char *aux2 = (char*)my_malloc(50);
    char *aux3 = (char*)my_malloc(876);
    printMemoryBLock();
    
    for (int i = 0; i < 100; i++){aux1[i] = 0;}
    for (int i = 0; i < 50; i++){aux2[i] = -1;} 
    for (int i = 0; i < 876; i++){aux3[i] = 0;}
    printAllMemory();

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


    void MM_end();
    
    return 0;
}
