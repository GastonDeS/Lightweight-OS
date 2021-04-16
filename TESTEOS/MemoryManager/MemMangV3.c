/* 
** Basado en: https://mgarciaisaia.github.io/tutorial-c/blog/2014/12/26/un-tutorial-rapido-para-implementar-y-debuggear-malloc/
** Se agregaron las siguientes mejoras:
**   -> Se reducieron las llamas al sistema con la funcion MemManager() la cual pide MIN_BYTES_REQUEST bytes que luego administra
**   -> Si el tamaño del bloque devuelto por findFreeBlock() supera en MAX_DIFF_SIZE al tamaño neceasrio entonces se divide el bolque 
**   alineamiento
**   ->pasar int free a char free (o de menor tamaño)
    (FALTA)
        ->hacer que printAllMemory y printMemoryBLock sean la misma y con un flag se imprime el contenido de la memorya 
        ->que la memoria este alineada
        ->en la parte de debugger se repite mucho codigo
*/
#include "MemMang.h"

//INFO_BLOCK_SIZE = 32 bytes
void *firstInfoBlock = NULL; 
void *memorySize = NULL; //(memorySize-firstInfoBlock) tamaño total de la memoria
void *memoryDim = NULL;  //(memoryDim-firstInfoBlock) bytes usados 

//solo visible para syscall
#define HEAP_SIZE 8000  //10 Kb 
void *startMemory = NULL;  
void *currentMemoryLimit = NULL; 

struct infoBlock{
    size_t size;  //12 bytes
    char free;  
    struct infoBlock *next;
    struct infoBlock *previous;
};
typedef struct infoBlock *infoBlockPtr;

//private:
void *sysCall(size_t size);
infoBlockPtr findFreeBlock(infoBlockPtr *last, size_t size);
infoBlockPtr requestSpace(infoBlockPtr last, size_t size);
void splitBlock(infoBlockPtr block, size_t size);
infoBlockPtr getBlockPtr(void *ptr);
void *syscallManager(size_t size);

/*----------------------------------------------------------------*/
void *my_malloc(size_t size){
    if (size <= 0)
        return NULL;
    
    infoBlockPtr block; 
    if (firstInfoBlock == NULL){ //primer llamado
        block = requestSpace(NULL,size);
        if(block == NULL)
            return NULL;
        firstInfoBlock = block;
    }else{
        infoBlockPtr last = firstInfoBlock;
        block = findFreeBlock(&last, size);
        if (block == NULL){ //no se encontro un bloque
            block = requestSpace(last, size);//agrega un nuevo bloque al final
            if(block == NULL)
                return NULL;
        }else{ //se encontro un boque
            block->free = 0;
            if((int)block->size-(int)size-(int)INFO_BLOCK_SIZE > (int)MAX_DIFF_SIZE)//si es muy grande lo divido  ------------------------------------------------  CAMBIO!!!!!!
                splitBlock(block, size); //no esta testiado
        }
    }
    return (block + 1); //la direccion justo despues de infoBlock
}

/*----------------------------------------------------------------*/
void splitBlock(infoBlockPtr block, size_t size){
    infoBlockPtr newBlock = (void*)(block+1) + size;
    newBlock->free = 1;
    newBlock->size = (block->size)-size-INFO_BLOCK_SIZE;
    newBlock->previous = block;
    newBlock->next = block->next;

    if (block->next != NULL)           
        block->next->previous = newBlock;

    block->next = newBlock;
    block->size = size;    
    return;
} 

/*----------------------------------------------------------------*/
infoBlockPtr findFreeBlock(infoBlockPtr* last, size_t size){ 
    infoBlockPtr current = firstInfoBlock;
    infoBlockPtr blockAptSize = NULL;
    size_t aptSize = SIZE_MAX;
    while (current != NULL){ 
        if(current->free && current->size >= size && current->size < aptSize){
            if(current->size == size)
                return current;// current tiene el tamaño mas apto
            aptSize = current->size;
            blockAptSize = current; 
        }
        *last = current;
        current = current->next;
    }
    if(blockAptSize != NULL)
        return blockAptSize;
    return current;
}

/*----------------------------------------------------------------*/
infoBlockPtr requestSpace(infoBlockPtr last, size_t size){
    infoBlockPtr block = syscallManager(size + INFO_BLOCK_SIZE);
    if(block == NULL) //no hay mas espacio en el heap
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
void *syscallManager(size_t size){
    if(memoryDim == NULL){ //primer llamado
        memorySize = sysCall(0);
        memoryDim = memorySize;
    }
    void* resutl = memoryDim;
    if(memorySize-memoryDim < size){
        void *check = NULL;
        if(size <= MIN_BYTES_REQUEST){
            check= sysCall(MIN_BYTES_REQUEST);
        }else{
            check= sysCall(size+MIN_BYTES_REQUEST);
        }
        if(check == NULL)
           return NULL; //no hay mas espacio en el heap
        memorySize = check;
        memoryDim += size; //ver si funciona bien
    }else
        memoryDim += size;
    return resutl;
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
        aux->next = NULL;
        aux->previous = NULL;
    }
    aux = current->previous;
    if(aux != NULL && aux->free){//el anterior esta libre
        aux->size += current->size+INFO_BLOCK_SIZE; 
        if(current->next != NULL)
            current->next->previous = aux;
        aux->next = current->next;
        current->next = NULL;
        current->previous = NULL;
    }
    return;
}

/*----------------------------------------------debugger---------------------------------------------------
  (*) No puede haber dos free juntos 
  (*) bloque->next-bloque = bloque->size+INFO_BLOCK_SIZE 
  (*) memoryDim-firstInfoBlock = a la suma de todos los bloque->size+(INFO_BLOCK_SIZE*cantidad de bloques)
  (*) current = current->next->previous 
  (*) currentMemoryLimit = memorySize
  (*) firstInfoBlock = startMemory
  (*) del total de memoria usada mostrar cunata memoria se uso en infoBLocke, cuanta usa el usuario y cuanta en alinear (FALTA)
*/
int checkMemory(void){
    infoBlockPtr current = firstInfoBlock;
    int blockIndex=0, freeFlag = 0, cantFreeBlock = 0, numError = 0; 
    size_t totalSize = 0;
    printf("----------------START----------------\n");
    while (current != NULL){ 
        if(current->free){
            if(freeFlag){
                printf("(*) Hay dos bloque libre juntos, el bloque:%d y bloque:%d\n",blockIndex-1, blockIndex);
                numError++;
            }else    
                freeFlag = 1;
            cantFreeBlock++;
        }else
            freeFlag=0;
        if(current->next != NULL){
            long diff = (long)current->next-(long)current;
            if(diff != (current->size+INFO_BLOCK_SIZE)){
                int notUsed = diff - (current->size + INFO_BLOCK_SIZE);
                printf("(*) Entre los bloque %d y %d hay %d bytes no usados\n",blockIndex, blockIndex+1, notUsed);
                numError++;
            }
            if(current != current->next->previous){
                printf("(*) En bloque %d; su puntero:%p  != %p :current->next->previous\n",blockIndex, current, current->next->previous);
                numError++;
            }
        }
        blockIndex++;
        totalSize += current->size + INFO_BLOCK_SIZE;
        current = current->next;
    }
    if(memoryDim - firstInfoBlock != totalSize){
        printf("(*) memoryDim-firstInfoBlock = %ld  != %ld (sumatoria: bloque->size+INFO_BLOCK_SIZE)\n",(memoryDim - firstInfoBlock), totalSize);
        numError++;
    }
    if(currentMemoryLimit != memorySize){
        printf("(*) currentMemoryLimit: %p != memorySize: %p \n", currentMemoryLimit, memorySize);
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

// (*) del total de memoria usada mostrar cunata memoria se uso en infoBLocke, cuanta usa el usuario y cuanta en alinear (FALTA)
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
        printf("  |-> previous: %p\n",current->previous);
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
        printf("  |-> size: %ld\n",current->size);
        printf("  |-> free: %s\n",(current->free == 0 ? "no" : "yes"));
        printf("  |-> next: %p\n",current->next);
        printf("  |-> previous: %p\n",current->previous);
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
//sbrk
void *sysCall(size_t size){ 
    if(startMemory == NULL){
        startMemory = malloc(HEAP_SIZE);
        if(startMemory == NULL){
            printf("fallo malloc de stdlib\n");
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



