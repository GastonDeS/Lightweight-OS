#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

#define MEM_SIZE 1024
#define BLOCK_SIZE 64 //entran 16 bloques
void *start = NULL;
void *next = NULL;

int MM_init(){
    if((start = malloc(MEM_SIZE)) == NULL)
        return -1;
    next = start;
    return 0;
}

char* my_malloc(int num_of_Blocks){   
    if ( (next+(BLOCK_SIZE*num_of_Blocks))-start <= MEM_SIZE ){
        char *result = next;
        next += BLOCK_SIZE * num_of_Blocks;
        return result;
    }
    return NULL;
}

int num_of_free_blocks(void){
    return  (MEM_SIZE - (next-start) )/ BLOCK_SIZE;
}

int num_of_blocks_used(void){
    return (next-start) / BLOCK_SIZE;
}

//-------------------------------TEST---------------------------------------
int main(int argc, char const *argv[]){

    if(MM_init() == -1 ){
        printf("Error al MM_init");
        return -1;
    }
    /*
    //verificacion de un bloque (cambiar MEM_SIZE 64)
    char* aux = my_malloc(1);
    char *aux2 = my_malloc(1);

    printf("valor del puntero: %p  \n", aux);
    printf("se espera valor NULL: %p  \n", aux2);

    printf("numero de bloques libres: %d \n", num_of_free_blocks());
    printf("numero de bloques usados: %d \n", num_of_blocks_used());
    */

    /*
    //verifico el uso completo de bloques
    char *aux1 = my_malloc(1);
    char *aux2 = my_malloc(1);
    char *aux3 = my_malloc(1);

    printf("se espera que sea 13: %d \n", num_of_free_blocks());
    printf("se espera que sea 3: %d \n", num_of_blocks_used());

    for (int i = 0; i < BLOCK_SIZE * 3; i++){aux1[i] = 0;}//completo los 3 bloque con 0 

    for (int i = 0; i < BLOCK_SIZE; i++){aux1[i] = -1;} //lleno a aux1 con -1

    for (int i = 0; i < BLOCK_SIZE+5 ; i++){ //complero aux2 con -1 y me paso en 5
        if (aux2[i] == -1)                   //si algun elemento de aux2 es -1 => error
            printf("Error 1");
        aux2[i] = -1;
    }
    int count = 0;
    for (int i = 0; i < BLOCK_SIZE; i++){//verifico que solo haya 5 -1 en aux3
        if (aux3[i] == -1)
            count++;
    }
    printf("se espera que sea 5: %d \n", count);
    */

    //
    char *aux1 = my_malloc(4);
    if (aux1 == NULL)
        printf("Error 1");
    printf("se espera que sea 12: %d \n", num_of_free_blocks());
    printf("se espera que sea 4: %d \n", num_of_blocks_used());

    aux1 = my_malloc(2);
    if (aux1 == NULL)
        printf("Error 1");
    printf("se espera que sea 10: %d \n", num_of_free_blocks());
    printf("se espera que sea 6: %d \n", num_of_blocks_used());

    aux1 = my_malloc(0);
    if (aux1 == NULL)
        printf("Error 1");
    printf("se espera que sea 10: %d \n", num_of_free_blocks());
    printf("se espera que sea 6: %d \n", num_of_blocks_used());

    aux1 = my_malloc(7);
    if (aux1 == NULL)
        printf("Error 1");
    printf("se espera que sea 3: %d \n", num_of_free_blocks());
    printf("se espera que sea 13: %d \n", num_of_blocks_used());

    aux1 = my_malloc(7);
    printf("se espera que sea NULL: %p \n", aux1);
    printf("se espera que sea 3: %d \n", num_of_free_blocks());
    printf("se espera que sea 13: %d \n", num_of_blocks_used());

    aux1 = my_malloc(3);
    if (aux1 == NULL)
        printf("Error 1");
    printf("se espera que sea 0: %d \n", num_of_free_blocks());
    printf("se espera que sea 16: %d \n", num_of_blocks_used());

    aux1 = my_malloc(3);
    printf("se espera que sea NULL: %p \n", aux1);
    printf("se espera que sea 0: %d \n", num_of_free_blocks());
    printf("se espera que sea 16: %d \n", num_of_blocks_used());

    return 0;
}