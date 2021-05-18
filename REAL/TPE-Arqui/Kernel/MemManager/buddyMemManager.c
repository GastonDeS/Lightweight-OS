#include <buddyMemManager.h>
//para test con el malloc del heap hacer free a final


//void *myStartMemory = (void *)0x600000;
void *myStartMemory = NULL;

/*
** Basado en:
http://brokenthorn.com/Resources/OSDev26.html
*/

/*
   El buddy manager trabaja con listas de bloques e diferente tamaño.
   El tamaño de los bloques depende del nivel de estos. En el nivel 0,
   un bloque ocupa toda la memora disponible, en el nivel 1, dos bloques la ocupan (cada
   uno una mitad), y en el nivel n la ocupan 2^n bloques. De tamaño = tamaño total /2^n.

   Para poder dividir los bloques por niveles en potencias de dos, el tamaño maximo y el 
   minimo tambien deben estar en potencia de 2.
   El tamaño máximo elegido es el del heap que tomamos como 2^22 bytes.
   El mínimo es de 2^5 (32 bytes).
   Supongamos que el máximo es de 2^8 (256 bytes). En este caso hay 4 niveles (del 0 l 3).
   Sin embargo si el usuario pide por 32 bytes, no podemos elegir un bloque del último nivel
   ya que debemos sumarle el tamaño del header. De esta manera, no se busca en la liste del
   nivel 3, sino en la de nivel 2.
*/



static BUDDY_HEADER *blocks[LEVELS]; //arreglo de listas de punteros a Bloques
static char initialized = 0;
static uint64_t remainingBytes = MAX_BLOCK_SIZE;
static BUDDY_HEADER *occupiedBlocks;

void *malloc(uint64_t size){
   //en el primer llamado hay que inicializarlo
   if (!initialized) {
      initialize();
      initialized = 1;
   }
   size += BUDDY_HEADER_SIZE;
   //no tengo espacio
   if (size > remainingBytes) {
      return NULL;
   }
   int64_t level = getLevel(size);
   //No tego espacio
   if (level == -1) {
      return NULL;
   }
   //el puntero a devolver debe saltearse el header del bloque
   void *toReturn = recursiveMalloc(level) + BUDDY_HEADER_SIZE;
   remainingBytes -= SIZE_OF_BLOCKS_AT_LEVEL(level);
   addOccupied(toReturn, level);
   return toReturn;
}

void *recursiveMalloc(uint64_t level) {
   void *toReturn;
   if (blocks[level] == NULL) {
      toReturn = recursiveMalloc(level -1);

      if(toReturn == NULL) {
         return NULL; //no tengo más espacio
      }

      uint64_t blockSize = SIZE_OF_BLOCKS_AT_LEVEL(level);
      
      //Lo divido en 2 porque es un nivel más
      insertBlock(toReturn, level);
      insertBlock(toReturn + blockSize, level);
   }
   return removeHeadBlock(level);
}

void free(void *ptr) { //si el puntero no es válido no hago nada
   
   if (removeOccupied(ptr) == -1) {
      return;
   }
   void *freePtr = ptr - BUDDY_HEADER_SIZE;
   BUDDY_HEADER *freeBlock = (BUDDY_HEADER *)freePtr;
   remainingBytes += SIZE_OF_BLOCKS_AT_LEVEL(freeBlock->level);
   recursiveFree(freeBlock, freeBlock->level);
}

void recursiveFree(void *header, uint64_t level) {
   void * buddy;
   uint64_t blockNumber = INDEX_OF_POINTER_IN_LEVEL(header,level, myStartMemory);
   uint64_t blockSize = SIZE_OF_BLOCKS_AT_LEVEL(level);

   /*chequeo si el índice es par o impar, de esta manera encuentro su buddy (si es par,
   el buddy está a la derecha, si es impar, a la izquierda (recordar que los índices empiezan en 0))
   */
   if (blockNumber % 2 == 0) {
      buddy = (void *)header + blockSize;
   }
   else {
      buddy = (void *)header - blockSize;
   }
   
   insertBlock(header, level); //se inserta ya que ahora este bloque vuelve a estar libre
   BUDDY_HEADER *block = blocks[level];

   while (block != NULL) { //busco el buddy
   
      if ((void *)block == buddy) { //si el budy tambien existe, elimino a los dos y subo de nivel
         removeBlock(header, level);
         removeBlock(buddy, level);
         if (blockNumber %2 == 0) {
            recursiveFree(header, (level - 1));
         }
         else {
            recursiveFree(buddy, (level - 1));
         }
         return;
      }
      block = block->next;
   }
}

void insertBlock(void *header, uint64_t level) {
   //no es una lista ordenada por ende se agrega al final
   
   BUDDY_HEADER *block = blocks[level];

   if (block == NULL) {
      block = (BUDDY_HEADER *) header;
      block -> level = level;
      block -> next = NULL;
      blocks[level] = block;
   }
   else {
      while (block->next != NULL) {
         block = block -> next;
      }
      block -> next = (BUDDY_HEADER *) header;
      block = block -> next;
      block -> level = level;
      block -> next = NULL;
   }
}

void addOccupied(void *header, uint64_t level) {
   //no es una lista ordenada por ende se agrega al final

   if (occupiedBlocks == NULL) {
      occupiedBlocks = (BUDDY_HEADER *) header;
      occupiedBlocks -> level = level;
      occupiedBlocks -> next = NULL;
   }
   else {
      BUDDY_HEADER *block = occupiedBlocks;
      while (block->next != NULL) {
         block = block -> next;
      }
      block -> next = (BUDDY_HEADER *) header;
      block = block -> next;
      block -> level = level;
      block -> next = NULL;
   }
}

int8_t removeOccupied(void *header) {
   if (occupiedBlocks == NULL || myStartMemory > header || (myStartMemory+HEAP_SIZE) < header) {
      return -1;
   }
   BUDDY_HEADER *blockHeader = (BUDDY_HEADER *)header;
   if (occupiedBlocks == blockHeader) {
      occupiedBlocks = occupiedBlocks->next;
      return 1;
   }
   BUDDY_HEADER *block = occupiedBlocks;
   while (block -> next != NULL || block -> next !=blockHeader) {
      block = block->next;
   }
   if (block->next == NULL) {
      return -1;
   }
   block -> next = blockHeader -> next;
   return 1;
}

void removeBlock(void * header, uint64_t level) {
   BUDDY_HEADER *block = blocks[level];
   BUDDY_HEADER *toRemove = (BUDDY_HEADER *) header;
   BUDDY_HEADER *aux;

   if (block == toRemove) { //si es el primero
      blocks[level] = block->next;
      return;
   }

   while (block -> next != NULL) { //ni primero ni último
      if (block -> next == toRemove) {
         block -> next = toRemove->next;
         return;
      }
      aux = block;
      block = block->next;
   }
   if (block == toRemove) { //último
      aux -> next = NULL;
      return;
   }
   //el puntero no esta en la lista, tiro excepcion?
}

void *removeHeadBlock(uint64_t level) {
   if (blocks[level] == NULL) { //list no iicializada
      return NULL;
   }
   //remuevo el primer elemento
   void *toReturn = (void *)blocks[level];
   blocks[level] = blocks[level] ->next;
   return toReturn;
}

int64_t getLevel(uint64_t size){ //devuelvo el nivel adecuado para el tamaño requerido
   int64_t level = 0;
   uint64_t totalBytes = MAX_BLOCK_SIZE;
   if (size > totalBytes){
      return -1;
   }
   while (level < LEVELS) {
      if (size < totalBytes) {
         totalBytes >>= 1;
         level++;
      }
      else {
         return size == totalBytes? level : level - 1;
      }
   }
   return LEVELS -1;
}

void initialize() {
   myStartMemory = sbrk(HEAP_SIZE);
   if (myStartMemory == NULL) {
      //lo cierro
   }
   insertBlock((void *)myStartMemory, 0);
}

void *realloc(void *ptr, uint64_t newSize){
    if (removeOccupied(ptr) == -1) {
      return NULL;
    }
    void *currentPtr = ptr - BUDDY_HEADER_SIZE;
    BUDDY_HEADER * current = (BUDDY_HEADER *) currentPtr;
    uint64_t size = SIZE_OF_BLOCKS_AT_LEVEL(current->level);
    if(size> newSize +BUDDY_HEADER_SIZE) {
         return ptr;
    }
    void* newPtr = malloc(newSize);
    if(newPtr == NULL)//si no hay espacio se devulve null y no se modifica ptr
        return NULL;
    memcpy(newPtr, ptr, size);
    free(ptr);
    return newPtr;
}

void reallocSyscall(void *ptr, uint64_t newSize, void* result){
    result = realloc(ptr, newSize);
}

void mallocSyscall(uint64_t size, void* result){
    result = malloc(size);
}