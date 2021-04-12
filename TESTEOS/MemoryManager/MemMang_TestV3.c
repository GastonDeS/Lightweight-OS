#include "MemMang.h"
#include<time.h>

int randNum(int min, int max);

int main(int argc, char const *argv[]){
    /*//test 1
    char *aux1 = (char*)my_malloc(100);
    char *aux2 = (char*)my_malloc(100);
    char *aux3 = (char*)my_malloc(100);
    char *aux4 = (char*)my_malloc(100);
    char *aux5 = (char*)my_malloc(100);
    printMemoryBLock();
    checkMemory();

    my_free(aux1);
    my_free(aux2);
    my_free(aux2);
    my_free(aux3);
    my_free(aux4);
    my_free(aux5);
    printAllMemory();
    checkMemory();
    
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
    
    /*//uniones del free
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
    checkMemory();
    */

    /*//busqueda del bloque libre adecuado
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
    checkMemory();
    */

    /*//limites del heaps
    char *aux = (char*)my_malloc(200);
    while(aux != NULL)
       aux = my_malloc(200);
    printAllMemory(); 
    checkMemory();
    */

    //fragmentacion 
    srand(time(NULL)); 
    char *aux[1000];
    int flag = 1, dimMaxAux;
    for (int i = 0; i < 10000 && flag; i++){
        aux[i] = (char*)my_malloc(randNum(1, 20));
        if(aux[i] == NULL){
            flag = 0;
            dimMaxAux = i;
        }
    }
    printMemoryBLock();
    if(checkMemory() < 0){
        printf("\n---------------------------------------------------------------------------------\n");
        printf("                                   ERROR\n");
        printf("---------------------------------------------------------------------------------\n\n");
        return -1;
    }else{
        printf("\n---------------------------------------------------------------------------------\n");
        printf("                                   sin Errores\n");
        printf("-----------------------------------------------------------------------------------\n\n");
    }
    srand(time(NULL)*time(NULL)); 
    for (int i = 0; i < dimMaxAux; i++){
        int auxIndex = randNum(0, dimMaxAux-1);
        my_free(aux[auxIndex]);  
    }
    printMemoryBLock();
    if(checkMemory()<0){
            printf("\n---------------------------------------------------------------------------------\n");
            printf("                                   ERROR\n");
            printf("-----------------------------------------------------------------------------------\n");
            return -1;
    }else{
        printf("\n------------------------------------------------------------------------------------\n");
        printf("                                   sin Errores\n");
        printf("------------------------------------------------------------------------------------\n\n");
    }

    
    void MM_end();
    return 0;
}

int randNum(int min, int max){
    return (min + rand()%(max+1-min)); 
}