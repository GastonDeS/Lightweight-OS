#include <MemMang.h>
#include <syscallsASM.h>

//INFO_BLOCK_SIZE = 32 bytes
void *firstInfoBlock = NULL; 
void *memorySize = NULL; //(memorySize-firstInfoBlock) tamaño total de la memoria
void *memoryDim = NULL;  //(memoryDim-firstInfoBlock) bytes usados 

struct infoBlock{
    uint64_t size;  //12 bytes
    char free;  
    struct infoBlock *next;
    struct infoBlock *previous;
};
typedef struct infoBlock *infoBlockPtr;

//private:
void *sbrk(uint64_t size);
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
            if((int)block->size-(int)size-(int)INFO_BLOCK_SIZE > (int)MAX_DIFF_SIZE)//si es muy grande lo divido  ------------------------------------------------  CAMBIO!!!!!!
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

void *sbrk(uint64_t size){
    void * answer = NULL;
    sbrkSyscall(size, &answer);
    if (answer==NULL){
        drawRectSyscall(150,150,150,150,0xff00ff);
    } else {
        drawRectSyscall(150,300,150,150,0xff00ff);
    }
    
    return answer;
}