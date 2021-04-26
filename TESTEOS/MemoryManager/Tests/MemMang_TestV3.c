#include "MemMang.h"
#include <time.h>

int randNum(int min, int max);

int main(int argc, char const *argv[])
{
    //test 1
    /*
    char *aux1 = (char*)my_malloc(100);
    char *aux2 = (char*)my_malloc(100);
    char *aux3 = (char*)my_malloc(100);
    char *aux4 = (char*)my_malloc(100);
    char *aux5 = (char*)my_malloc(100);
    printMemoryBLock(0);
    printMemoryBLock(1);
    checkMemory();
    
    my_free(aux1);
    my_free(aux2);
    my_free(aux2);
    my_free(aux3);
    my_free(aux4);
    my_free(aux5);
    printMemoryBLock(0);
    checkMemory();
    
    aux1 = (char*)my_malloc(100);
    aux2 = (char*)my_malloc(50);
    aux3 = (char*)my_malloc(876);
    for (int i = 0; i < 100; i++){aux1[i] = i;}
    for (int i = 0; i < 50; i++){aux2[i] = i;} 
    for (int i = 0; i < 876; i++){aux3[i] = i;}
    //printMemoryBLock(0);
    checkMemory();

    my_free(aux2);
    checkMemory();

    char *auxT = (char*)my_malloc(38);
    auxT = (char*)my_malloc(529);
    auxT = (char*)my_malloc(43);
    auxT = (char*)my_malloc(456);
    auxT = (char *)my_malloc(352);
    auxT = (char*)my_malloc(5263);
    auxT = (char*)my_malloc(50);
    auxT = (char*)my_malloc(69);
    auxT = (char*)my_malloc(89);
    printMemoryBLock(0);
    checkMemory();
    */

    /*
    //uniones del free
    char *aux1 = (char*)my_malloc(100);
    char *aux2 = (char*)my_malloc(100);
    printMemoryBLock(0);
    my_free(aux1);
    printMemoryBLock(0);
    my_free(aux2);
    printMemoryBLock(0);
    char *aux3 = (char*)my_malloc(100);
    char *aux4 = (char*)my_malloc(100);
    char *aux5 = (char*)my_malloc(100);
    char *aux6 = (char*)my_malloc(100);
    printMemoryBLock(0);
    my_free(aux6);
    printMemoryBLock(0);
    my_free(aux4);
    printMemoryBLock(0);
    my_free(aux5);
    printMemoryBLock(0);
    my_free(aux3);
    printMemoryBLock(0);
    checkMemory();
    */

    /*//busqueda del bloque libre adecuado
    char *aux1 = (char*)my_malloc(100);
    char *aux2 = (char*)my_malloc(75);
    char *aux3 = (char*)my_malloc(100);
    char *aux4 = (char*)my_malloc(50);
    char *aux5 = (char*)my_malloc(100);
    char *aux6 = (char*)my_malloc(25);
    printMemoryBLock(0);
    my_free(aux2);
    my_free(aux4);
    my_free(aux6);
    printMemoryBLock(0);

    aux2 = (char*)my_malloc(20);
    printMemoryBLock(0);
    aux4 = (char*)my_malloc(70);
    printMemoryBLock(0);
    aux6 = (char*)my_malloc(100);//lo mete a final
    printMemoryBLock(0);
    char* aux7 = (char*)my_malloc(45);
    printMemoryBLock(0);
    checkMemory();
    */

    /*//limites del heaps
    char *aux = (char*)my_malloc(200);
    while(aux != NULL)
       aux = my_malloc(200);
    printMemoryBLock(1);
    checkMemory();
    */

    //syscallManager()
    /*
    char *aux1 = (char *)my_malloc(MIN_BYTES_REQUEST);
    char *aux2 = (char *)my_malloc(MIN_BYTES_REQUEST);
    char *aux3 = (char *)my_malloc(MIN_BYTES_REQUEST);
    char *aux4 = (char *)my_malloc(MIN_BYTES_REQUEST);
    printMemoryBLock(0);
    checkMemory();

    my_free(aux1);
    my_free(aux2);
    my_free(aux3);
    my_free(aux4);
    printMemoryBLock(0);
    checkMemory();

        //limite de heap
    aux1 = (char *)my_malloc(MIN_BYTES_REQUEST*5);
    if(aux1 == NULL)
        printf("el puntero aux1 es NULL\n");
    aux2 = (char *)my_malloc(MIN_BYTES_REQUEST*10);
    if(aux2 == NULL)
        printf("el puntero aux2 es NULL\n");
    aux3 = (char *)my_malloc(MIN_BYTES_REQUEST*MIN_BYTES_REQUEST);
    if(aux3 == NULL)
        printf("el puntero aux3 es NULL\n");
    printMemoryBLock(0);
    checkMemory();
    */

    //fragmentacion
    srand(time(NULL) );
    while (1){
        
        char *aux[1000];
        int flag = 1, dimMaxAux;

        //creo todos los punteros
        for (int i = 0; i < 1000 && flag; i++){
            aux[i] = (char *)my_malloc(randNum(0,500));
            if (aux[i] == NULL){
                flag = 0;
                dimMaxAux = i;
            }
            //if(checkMemory()<0){
            //    return;
            //}
        }

        printMemoryBLock(0);
        if (checkMemory() < 0){
            printf("\n---------------------------------------------------------------------------------\n");
            printf("                                   ERROR\n");
            printf("---------------------------------------------------------------------------------\n\n");
            return -1;
        }else{
            printf("\n---------------------------------------------------------------------------------\n");
            printf("                                   sin Errores\n");
            printf("-----------------------------------------------------------------------------------\n\n");
        }

        //libreo algunos
        srand(time(NULL) * time(NULL));
        for (int i = 0; i < dimMaxAux/2; i++){
            int auxIndex = randNum(0, dimMaxAux - 1);
            my_free(aux[auxIndex]);
            //if(checkMemory()<0){
            //    return;
            //}
        }

        printMemoryBLock(0);
        if (checkMemory() < 0){
            printf("\n---------------------------------------------------------------------------------\n");
            printf("                                   ERROR\n");
            printf("-----------------------------------------------------------------------------------\n");
            return -1;
        }else{
            printf("\n------------------------------------------------------------------------------------\n");
            printf("                                   sin Errores\n");
            printf("------------------------------------------------------------------------------------\n\n");
        }
        sleep(1);
    }

    void MM_end();
    return 0;
}

int randNum(int min, int max)
{
    return (min + rand() % (max + 1 - min));
}