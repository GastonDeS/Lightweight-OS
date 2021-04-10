/*  (*) checkMemory(): Terminar
**  (*) my_malloc(): Si el bloque que se encntro es muy grande dividirlo 
**  (*) Reducir la cantidad de veces que se llama a la syscall pidiendole grandes cantidades de memoria
*/
#include "MemMang.h"

//INFO_BLOCK_SIZE = 32
void *firstInfoBlock = NULL; 

//para la syscall
#define HEAP_SIZE 8192 
void *startMemory = NULL;  
void *currentMemoryLimit = NULL; 

struct infoBlock{
    size_t size;
    int free;
    struct infoBlock *next;
    struct infoBlock *previous;
};
typedef struct infoBlock *infoBlockPtr;


//private:
void *sysCall(size_t size);
infoBlockPtr findFreeBlock(infoBlockPtr *last, size_t size);
infoBlockPtr requestSpace(infoBlockPtr last, size_t size);
infoBlockPtr getBlockPtr(void *ptr);


/*----------------------------------------------------------------*/
void *my_malloc(size_t size){
    if (size <= 0)
        return NULL;
    
    infoBlockPtr block; 
    if (firstInfoBlock == NULL){ // primer llamado
        block = requestSpace(NULL,size);
        if(block == NULL)
            return NULL;
        firstInfoBlock = block;
    }else{
        infoBlockPtr last = firstInfoBlock;
        block = findFreeBlock(&last, size);
        if (block == NULL){ 
            block = requestSpace(last, size);//agrego al final un nuevo bloque
            if(block == NULL)
                return NULL;
        }else{ // se encontro un boque
            // se podria dividir el bloque si es muy grande
            block->free = 0;
        }
    }
    return (block + 1); //la direccion justo despues de infoBlock
}

/*----------------------------------------------------------------*/
infoBlockPtr findFreeBlock(infoBlockPtr* last, size_t size){ 
    infoBlockPtr current = firstInfoBlock;
    infoBlockPtr aux = NULL;
    size_t bestSize = SIZE_MAX;
    while (current != NULL){ 
        if(current->free && current->size >= size && current->size < bestSize){
            if(current->size == size)
                return current;
            bestSize = current->size;
            aux = current; 
        }
        *last = current;
        current = current->next;
    }
    if(aux != NULL)
        return aux;
    return current;
}

/*----------------------------------------------------------------*/
//agregar un nuevo bloque al final de la lista
infoBlockPtr requestSpace(infoBlockPtr last, size_t size){
    infoBlockPtr block;
    block = sysCall(0); //block = sbrk(0); 
    void* check = sysCall(size + INFO_BLOCK_SIZE);
    if(check == NULL) //no hay mas espacio en el heap
        return NULL;

    if (last != NULL){ // si last==NULL es el primer llamado
        last->next = block;
    }
    block->size = size;
    block->free = 0;
    block->next = NULL;
    block->previous = last;

    return block;
}

/*----------------------------------------------------------------*/
infoBlockPtr getBlockPtr(void *ptr) {
    return (infoBlockPtr)ptr - 1;
}

/*----------------------------------------------------------------*/
void my_free(void *ptr){
    if (!ptr)
        return;
    infoBlockPtr current = getBlockPtr(ptr);
    current->free = 1;
    infoBlockPtr aux = current->next;
    if(aux != NULL && aux->free){//el siguiente esta libre
        current->size += aux->size+INFO_BLOCK_SIZE;
        if(aux->next != NULL) 
            aux->next->previous = current;
        current->next = aux->next;
    }
    aux = current->previous;
    if(aux != NULL && aux->free){//el siguiente esta libre
        aux->size += current->size+INFO_BLOCK_SIZE; 
        if(current->next != NULL)
        current->next->previous = aux;
        aux->next = current->next;
    }
    return;
}

/*--------------------------------debugger--------------------------------------------------------------
  (*) No puede haber dos free juntos
  (*) La diferencia entre dos punteros bloque consecutivos = size del bloque1 + INFO_BLOCK_SIZE
  (*) currentMemoryLimit - startMemory = a la suma de todos los size + INFO_BLOCK_SIZE * cantidad de bloques
*/
void checkMemory(void){
    infoBlockPtr current = firstInfoBlock;
    infoBlockPtr last = NULL;
    int numeberOfBlocks= 0;
    int totalMemUsed= 0;
    while (current != NULL){ 
        if((current->next-current) != current->size+INFO_BLOCK_SIZE) //distancia entre block actual y siguiente
            printf("Error1 en bloque: %d \n",numeberOfBlocks);             // tiene que ser igual a size+INFO_BLOCK_SIZE
        
        //if(last == NULL && current != last+last->size+INFO_BLOCK_SIZE) 
        //     printf("Error2 en bloque : %d",numeberOfBlocks); 
        

        numeberOfBlocks++;
        last = current;
        current = current->next;
    }
}

void printMemoryBLock(void){
    infoBlockPtr current = firstInfoBlock;
    int numeberOfBlocks= 0;
    int freeBlock = 0;
     printf("----------------START----------------\n");
    while (current != NULL){
        printf("Block %d: %p\n",numeberOfBlocks, current);
        printf("  |-> size:%ld\n",current->size);
        printf("  |-> free:%s\n",(current->free == 0 ? "no" : "yes"));
        printf("  |-> next:%p\n",current->next);
        printf("\n");
        if(current->free == 1)
            freeBlock++;
        current = current->next;
        numeberOfBlocks++;
    }
    printf("\n");
    printf("cantidad de bloques: %d\n",numeberOfBlocks);
    printf("cantidad de bloques libres: %d\n",freeBlock);
    printf("-----------------END-----------------\n");
    return;
}
void printAllMemory(void){
    int cantElemPerLine = 50; //<----- para cambiar la cantidad de elementos en una linea 
    infoBlockPtr current = firstInfoBlock;
    int numeberOfBlocks= 0;
    int freeBlock = 0;
    printf("----------------START----------------\n");
    while (current != NULL){
        printf("Block %d: %p\n",numeberOfBlocks, current);
        printf("  |-> size:%ld\n",current->size);
        printf("  |-> free:%s\n",(current->free == 0 ? "yes" : "no"));
        printf("  |-> next:%p\n",current->next);
        printf("  |-> datos guardados:");
        char *aux = current+1; //NO CAMBIAR A (char*)(current+1)
        for (int i = 0; i < current->size; i++){
            if(i%cantElemPerLine == 0){
                printf("\n \t");
            }
            printf("%d ", *(aux+i));
        }
        printf("\n");
        if(current->free == 1)
            freeBlock++;
        current = current->next;
        numeberOfBlocks++;
    }
    printf("\n");
    printf("cantidad de bloques: %d\n",numeberOfBlocks);
    printf("cantidad de bloques libres: %d\n",freeBlock);
    printf("-----------------END-----------------\n");
    return;
}

/*------------------------------SYSCALL----------------------------------------*/
void *sysCall(size_t size){ 
    if(startMemory == NULL){
        startMemory = malloc(HEAP_SIZE);
        if(startMemory == NULL){
            printf("fallo syscall \n");
            exit(0);
        }
        currentMemoryLimit = startMemory;
        return currentMemoryLimit;
    }
    if((currentMemoryLimit-startMemory)+size <= HEAP_SIZE)
        return currentMemoryLimit+=size;

    return NULL;
}

void MM_end(){
    free(startMemory);
}



