#if !defined MM_BUDDY
#include <MemMang.h>


//INFO_BLOCK_SIZE = 32 bytes
void *firstInfoBlock = NULL; 
void *memorySize = NULL; //(memorySize-firstInfoBlock) tamaño total de la memoria
void *memoryDim = NULL;  //(memoryDim-firstInfoBlock) bytes usados 

int index = 0; //variable auxiliar del printMem

struct infoBlock{
    uint64_t size;  //12 bytes
    char free;  
    struct infoBlock *next;
    struct infoBlock *previous;
};
typedef struct infoBlock *infoBlockPtr;

//private:
infoBlockPtr findFreeBlock(infoBlockPtr *last, uint64_t size);
infoBlockPtr requestSpace(infoBlockPtr last, uint64_t size);
void splitBlock(infoBlockPtr block, uint64_t size);
infoBlockPtr getBlockPtr(void *ptr);
void *syscallManager(uint64_t size);

void *malloc(uint64_t size){
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
            if((int)block->size-(int)size-(int)INFO_BLOCK_SIZE > (int)MAX_DIFF_SIZE)//si es muy grande lo divido  
                splitBlock(block, size); //no esta testiado
        }
    }
    return (block + 1); //la direccion justo despues de infoBlock
}

void splitBlock(infoBlockPtr block, uint64_t size){
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

infoBlockPtr findFreeBlock(infoBlockPtr* last, uint64_t size){ 
    infoBlockPtr current = firstInfoBlock;
    infoBlockPtr blockAptSize = NULL;
    uint64_t aptSize = SIZE_MAX;
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

infoBlockPtr requestSpace(infoBlockPtr last, uint64_t size){
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

void *syscallManager(uint64_t size){
    if(memoryDim == NULL){ //primer llamado
        memorySize = sbrk(0);
        memoryDim = memorySize;
    }
    void* resutl = memoryDim;
    if(memorySize-memoryDim < size){
        void *check = NULL;
        if(size <= MIN_BYTES_REQUEST){
            check= sbrk(MIN_BYTES_REQUEST);
        }else{
            check= sbrk(size+MIN_BYTES_REQUEST);
        }
        if(check == NULL)
           return NULL; //no hay mas espacio en el heap
        memorySize = check;
        memoryDim += size; //ver si funciona bien
    }else
        memoryDim += size;
    return resutl;
}

infoBlockPtr getBlockPtr(void *ptr) {
    return (infoBlockPtr)ptr - 1;
    
}

void free(void *ptr){
    if (ptr == NULL || ptr < getStartMemory())
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


void *realloc(void *ptr, uint64_t newSize){
    if (!ptr)
        return NULL;
    infoBlockPtr current = getBlockPtr(ptr);

    if(current->size > newSize)
        return ptr;
    
    void* newPtr = malloc(newSize);
    if(newPtr == NULL)//si no hay espacio se devulve null y no se modifica ptr
        return NULL;
    memcpy(newPtr, ptr, current->size);
    free(ptr);
    return newPtr;
}



void reallocSyscall(void *ptr, uint64_t newSize, void** result){
    (*result) = realloc(ptr, newSize);
}

void mallocSyscall(uint64_t size, void** result){
    (*result) = malloc(size);
}


/*
  (*) No puede haber dos free juntos
  (*) distancia entre bloques - size - INFO_BLOCK_SIZE < MAX_DIFF_SIZE
  (*) memoryDim-firstInfoBlock = la sumatoria de "block->next - block" en todos
  los bloques
  (*) en todo los bloques pasa que, current = current->next->previous
  (*) currentMemoryLimit = memorySize
  (*) firstInfoBlock = startMemory
  (*) totalBytes es igual a la suma de bytesUsedByBLocks, bytesUsedByUser,
  unusedBytes y bytesUsedByAlign
  (*) numeberOfBlocks es igual a los bloque libre y usados
*/
void checkMemory(struct checkMemdata *data){
    infoBlockPtr current = firstInfoBlock;
    int freeFlag = 0;

    while (current != NULL){
        data->numeberOfBlocks++;
        if (current->free){
            data->freeBlock++;
        }else{
            data->blockused++;
        }
        data->bytesUsedByBLocks += INFO_BLOCK_SIZE;
        
        if (current->next != NULL){
            data->lostBytes += (long)current->next - (long)current - (int)current->size - INFO_BLOCK_SIZE;
            data->totalBytes += (long)current->next - (long)current;
        }else{
            data->lostBytes += (long)memoryDim - (long)current - (int)current->size - INFO_BLOCK_SIZE;
            data->totalBytes += (long)memoryDim - (long)current;
        }
        if (current->free){ // no puede haber dos free juntos
            data->freeBytes += current->size;
            if (freeFlag){
                data->freeBlocksTogether ++;
                data->numError ++;
            }else
                freeFlag = 1;
        }else
            data->bytesUsedByUser += current->size;
            freeFlag = 0;

        if (current->next != NULL){
            long notUsed = (long)current->next - (long)current - (int)current->size - INFO_BLOCK_SIZE;
            if (notUsed > MAX_DIFF_SIZE){
                data->lostBytes += (notUsed - MAX_DIFF_SIZE);
                data->numError ++;
            }
            if (current != current->next->previous){
                data->curNextPrev++;
                data->numError ++;
            }
        }
        current = current->next;
    }
    if (data->totalBytes != (data->bytesUsedByBLocks + data->bytesUsedByUser + data->lostBytes + data->freeBytes)){
        data->bytesError = 1;
        data->numError ++;
    }
    if (data->numeberOfBlocks != data->blockused + data->freeBlock){
        data->numblocksError = 1;
        data->numError++;
    }
    if (memoryDim - firstInfoBlock != data->totalBytes){
        data->memError = 1;
        data->numError++;
    }
    return;
}

void printMem(char *str, int strSize){
    int i=0, j,buffDim=10, aux;
    strSize--; //reservo el lugar del \n
    char *title = "\n#block  free  size      addr\n";
    char auxBuf[buffDim];    
    infoBlockPtr current = firstInfoBlock;

    //avanzo hasta donde imprimi la ultima vez
    for(j= 0; j < index && current != NULL; j++)
        current = current->next;

    //armado del title
    strcat2(str, &i, strSize, title);

    while (current != NULL && i < strSize ){
        
        aux = intToString(index, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 8-aux);

        if(current->free)
            aux = strcat2(str, &i, strSize, "yes");
        else
            aux = strcat2(str, &i, strSize, "no");
        addSpace(str, &i, strSize, 6-aux);

        aux = intToString(current->size, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 10-aux);

        aux = intToBase((unsigned long long)(current+1), 16, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        strcat2(str, &i, strSize, "\n");

        current = current->next;
        index++;
    }
    if(current == NULL && i != strSize)
        index = 0;
    if(i == strSize)
        index--;
    str[i] = '\0'; 
}
#endif