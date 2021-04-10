#include "MemMang.h"
#include<time.h>

#define MAX_RAND 200
#define MIN_RAND 20

int randNum(int min, int max);

int main(int argc, char const *argv[]){

    //uniones del free
    /*
    char *aux1 = (char*)my_malloc(100);
    char *aux2 = (char*)my_malloc(100);
    printMemoryBLock();
    my_free(aux1);
    printMemoryBLock();
    my_free(aux2);
    printMemoryBLock();
    char *aux3 = (char*)my_malloc(100);
    char *aux4 = (char*)my_malloc(100);
    char *aux5 = (char*)my_malloc(100);
    char *aux6 = (char*)my_malloc(100);
    printMemoryBLock();
    my_free(aux6);
    printMemoryBLock();
    my_free(aux4);
    printMemoryBLock();
    my_free(aux5);
    printMemoryBLock();
    my_free(aux3);
    printMemoryBLock();
    */

    //busqueda del bloque libre adecuado
    /*
    char *aux1 = (char*)my_malloc(100);
    char *aux2 = (char*)my_malloc(75);
    char *aux3 = (char*)my_malloc(100);
    char *aux4 = (char*)my_malloc(50);
    char *aux5 = (char*)my_malloc(100);
    char *aux6 = (char*)my_malloc(25);
    printMemoryBLock();
    my_free(aux2);
    my_free(aux4);
    my_free(aux6);
    printMemoryBLock();

    aux2 = (char*)my_malloc(20);
    printMemoryBLock();
    aux4 = (char*)my_malloc(70);
    printMemoryBLock();
    aux6 = (char*)my_malloc(100);//lo mete a final
    printMemoryBLock();
    char* aux7 = (char*)my_malloc(45);
    printMemoryBLock();
    printAllMemory();
    */

    //limites del heaps
    /*
    char *aux = (char*)my_malloc(200);
    while(aux != NULL)
       aux = my_malloc(200);
    printMemoryBLock(); //se esperan 35 bloques
    */

    //fragmentacion 
    srand(time(NULL)); 
    char *aux[1000];
    int flag = 1, dim;
    for (int i = 0; i < 1000 && flag; i++){
        aux[i] = (char*)my_malloc(randNum(MIN_RAND, MAX_RAND));
        if(aux[i] == NULL){
            flag = 0;
            dim = i;
        }
    }
    srand(time(NULL)*time(NULL)); 
    printMemoryBLock();
    for (int i = 0; i < dim; i++){
        my_free(aux[randNum(0, dim-1)]);
    }
    printMemoryBLock();

    void MM_end();
    return 0;
}

int randNum(int min, int max){
    return (min + rand()%(max+1-min)); 
}
