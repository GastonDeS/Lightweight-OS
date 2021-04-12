/* 
**  Basado en: https://mgarciaisaia.github.io/tutorial-c/blog/2014/12/26/un-tutorial-rapido-para-implementar-y-debuggear-malloc/
**  Implemtacion del codigo del like, solo se remplazo la llamada al sistema sbrk por la funcion syscall que simula su 
**  funcionamiento sin llamar al sistema y ademas se agregaron funciones de debbuger       
*/
#include "MemMang.h"

//INFO_BLOCK_SIZE = 24 
void *firstInfoBlock = NULL; 

//solo visible para syscall
#define HEAP_SIZE 10000  //10 Kb 
void *startMemory = NULL;  
void *currentMemoryLimit = NULL; 

struct infoBlock{
    size_t size;
    int free;
    struct infoBlock *next;
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
            block->free = 0;
        }
    }
    return (block + 1); //la direccion justo despues de infoBlock
}

/*----------------------------------------------------------------*/
infoBlockPtr findFreeBlock(infoBlockPtr* last, size_t size){
    infoBlockPtr current = firstInfoBlock;
    while (current != NULL && !(current->free && current->size >= size)){
        *last = current;
        current = current->next;
    }
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
    block->next = NULL;
    block->free = 0;

    return block;
}

/*----------------------------------------------------------------*/
infoBlockPtr getBlockPtr(void *ptr) { //direcciond de 
    return (infoBlockPtr)ptr - 1;
}

/*----------------------------------------------------------------*/
void my_free(void *ptr){
    if (!ptr)
        return;
    infoBlockPtr blockPtr = getBlockPtr(ptr);
    blockPtr->free = 1;
}


/*--------------------------------debugger------------------------------------
  (*) bloque->next - bloque = bloque->size + INFO_BLOCK_SIZE 
  (*) currentMemoryLimit - firstInfoBlock = a la suma de todos los size + INFO_BLOCK_SIZE * cantidad de bloques
  (*) firstInfoBlock = startMemory
*/
int checkMemory(void){
    infoBlockPtr current = firstInfoBlock;
    int blockIndex=0, cantFreeBlock = 0, numError = 0;
    size_t totalSize = 0;
    printf("----------------START----------------\n");
    while (current != NULL){ 
        if(current->next != NULL){
            long diff = (long)current->next-(long)current;
            if(diff != (current->size+INFO_BLOCK_SIZE)){
                int notUsed = diff - (current->size + INFO_BLOCK_SIZE);
                printf("(*) Entre los bloque %d y %d hay %d bytes no usados\n",blockIndex, blockIndex+1, notUsed);
                numError++;
            }
        }
        blockIndex++;
        totalSize += current->size + INFO_BLOCK_SIZE;
        current = current->next;
    }
    if(currentMemoryLimit - firstInfoBlock != totalSize){
        printf("(*) currentMemoryLimit-firstInfoBlock = %ld  != %ld (sumatoria: bloque->size+INFO_BLOCK_SIZE)\n",(currentMemoryLimit - firstInfoBlock), totalSize);
        numError++;
    }
    if(firstInfoBlock != startMemory){
        printf("(*) firstInfoBlock: %p != startMemory: %p \n", firstInfoBlock, startMemory);
        numError++;
    }
    printf("cantidad de Errores encontrados: %d\n", numError);
    printf("cantidad de bloques: %d\n",blockIndex);
    printf("cantidad de bloques libres: %d\n",cantFreeBlock);
    printf("-----------------END-----------------\n");
    return numError>0?-1:0;
}

void printMemoryBLock(void){
    infoBlockPtr current = firstInfoBlock;
    int numeberOfBlocks= 0;
    int freeBlock = 0;
     printf("----------------START----------------\n");
    while (current != NULL){
        printf("Block %d: %p\n",numeberOfBlocks, current);
        printf("  |-> size: %ld\n",current->size);
        printf("  |-> free: %s\n",(current->free == 0 ? "no" : "yes"));
        printf("  |-> next: %p\n",current->next);
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
    int cantElemPerLine = 10; //<----- para cambiar la cantidad de elementos en una linea 
    infoBlockPtr current = firstInfoBlock;
    int numeberOfBlocks= 0;
    int freeBlock = 0;
    printf("----------------START----------------\n");
    while (current != NULL){
        printf("Block %d: %p\n",numeberOfBlocks, current);
        printf("  |-> size: %ld\n",current->size);
        printf("  |-> free: %s\n",(current->free == 0 ? "no" : "yes"));
        printf("  |-> next: %p\n",current->next);
        printf("  |-> datos guardados:");
        char *aux = current+1; //NO CAMBIAR (char*)(current+1)
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
//sbrk
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



