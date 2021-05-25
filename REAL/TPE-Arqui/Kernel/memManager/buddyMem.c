// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef MM_BUDDY
#include "MemMang.h"
//para test con el malloc del heap hacer free a final

#define HEAP_SIZE (1024*1024*4)  // 4Mb entran aprox 1024 4kb sirve masomenos 

//void *myStartMemory = (void *)0x600000;
void *myStartMemory =(void *) 0x600000;

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

typedef struct BUDDY_HEADER {
   struct BUDDY_HEADER* next;
   uint64_t level;
}BUDDY_HEADER;

#define BUDDY_HEADER_SIZE sizeof(BUDDY_HEADER)

#define BLOCKS_PER_LEVEL(level) (int64_t)(1<<(level))
#define SIZE_OF_BLOCKS_AT_LEVEL(level) (uint64_t)(1 << (MAX_BLOCK_SIZE_LOG2 - (level)))

#define MIN_BLOCK_SIZE_LOG2 5
#define MIN_BLOCK_SIZE ((uint64_t) 1<<MIN_BLOCK_SIZE_LOG2)

#define MAX_BLOCK_SIZE_LOG2 22
#define MAX_BLOCK_SIZE ((uint64_t)1<<MAX_BLOCK_SIZE_LOG2)


#define LEVELS (MAX_BLOCK_SIZE_LOG2 - MIN_BLOCK_SIZE_LOG2)+1

#define INDEX_OF_POINTER_IN_LEVEL(pointer,level,memory_start) ((pointer)-(memory_start)) / (SIZE_OF_BLOCKS_AT_LEVEL(level))



static BUDDY_HEADER *blocks[LEVELS]; //arreglo de listas de punteros a Bloques
static char initialized = 0;
static BUDDY_HEADER *occupiedBlocks;

void *recursiveMalloc(uint64_t level);
void recursiveFree(void *header, uint64_t level);
void insertBlock(void *header, uint64_t level);
void removeBlock(void * header, uint64_t level);
void *removeHeadBlock(uint64_t level);
int64_t getLevel(uint64_t size);
uint64_t getBlockNumber(BUDDY_HEADER *header);
void initialize();
void addOccupied(void *header, uint64_t level);
int8_t removeOccupied(void *header);
uint64_t remainingBytes();

void *malloc(uint64_t size){
  //en el primer  hay que inicializarlo
   if (!initialized) {
      initialize();
      initialized = 1;
   }
   size += BUDDY_HEADER_SIZE;
   //no tengo espacio
   if (size > remainingBytes()) {
      return NULL;
   }
   int64_t level = getLevel(size);
   //No tego espacio
   if (level == -1) {
      return NULL;
   }
   //el puntero a devolver debe saltearse el header del bloque
   void *toReturn = recursiveMalloc(level) + BUDDY_HEADER_SIZE;
   addOccupied(toReturn -BUDDY_HEADER_SIZE, level);
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

   if (ptr == NULL ||removeOccupied(ptr - BUDDY_HEADER_SIZE) == -1) {
      return;
   }
   void *freePtr = ptr - BUDDY_HEADER_SIZE;
   BUDDY_HEADER *freeBlock = (BUDDY_HEADER *)freePtr;
   uint64_t level = freeBlock->level;
   recursiveFree(freePtr, level);
}

void recursiveFree(void *header, uint64_t level) {
   void * buddy;
   uint64_t blockNumber = INDEX_OF_POINTER_IN_LEVEL(header,level, myStartMemory);
   uint64_t blockSize = SIZE_OF_BLOCKS_AT_LEVEL(level);

   /*chequeo si el índice es par o impar, de esta manera encuentro su buddy (si es par,
   el buddy está a la derecha, si es impar, a la izquierda (recordar que los índices empiezan en 0))
   */
   if (blockNumber %2 == 0) {
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
         BUDDY_HEADER * aux;
         if (blockNumber % 2 == 0) {
            aux = (BUDDY_HEADER *)header;
            aux->level--;
            recursiveFree(aux, aux ->level);
         }
         else {
            aux = (BUDDY_HEADER *)buddy;
            aux->level--;
            recursiveFree(aux, aux->level);
         }
         return;
      }
      block = block->next;
   }
}

void insertBlock(void *header, uint64_t level) {
   //Es una lista ordenada
   
   BUDDY_HEADER *block = blocks[level];
   BUDDY_HEADER *toInsert = (BUDDY_HEADER *) header;

   if (block == NULL) {
      blocks[level] = toInsert;
      toInsert->level = level;
      toInsert ->next = NULL;
      return;
   }

   if (block > toInsert) {
      toInsert->next = block;
      blocks[level] = toInsert;
      toInsert->level =level;
      return;
   }

   while (block->next != NULL && block ->next < toInsert) {
      block = block->next;
   }
   if (block ->next == NULL) {
      block -> next = toInsert;
      toInsert -> next = NULL;
   }
   else {
      toInsert ->next =block ->next;
      block -> next = toInsert;
   }
   toInsert ->level = level;
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
   BUDDY_HEADER *block = occupiedBlocks;
   if (block == blockHeader) {
      block = block->next;
      occupiedBlocks =(void *)block;
      return 1;
   }
   while (block -> next != NULL && block -> next !=blockHeader) {
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

   if (block == toRemove) { //si es el primero
      blocks[level] = block->next;
      return;
   }

   while (block -> next != NULL && block->next != toRemove) { //ni primero ni último
      block = block->next;
   }
   if (block -> next == toRemove) {
      block -> next = toRemove->next;
      return;
   }
   if (block -> next == NULL) { //último
      return;
   }
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
   insertBlock((void *)myStartMemory, 0);
}

uint64_t remainingBytes(){
   int32_t i = 0;
   while (blocks[i] == NULL) {
      i++;
   }
   return SIZE_OF_BLOCKS_AT_LEVEL(i);
   
}

void *realloc(void *ptr, uint64_t newSize){
    if (ptr == NULL || !ptr || removeOccupied(ptr -BUDDY_HEADER_SIZE) == -1) {
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

void reallocSyscall(void *ptr, uint64_t newSize, void** result){
    result = realloc(ptr, newSize);
}

void mallocSyscall(uint64_t size, void** result){
    (*result) = malloc(size);
}

//debugger
void checkMemory(struct checkMemdata* data){
   return;
}

void printMem(char *str, int strSize){
   int i=0, buffDim=10, currentLevel, aux;
   strSize--; //reservo el lugar del \n
   char auxBuf[buffDim];
   char *title = "\n\nFREE BLOCKS";
   char *title1 = "\n#level  #index   size       addr\n";
   char *title2 = "\nUSED BLOCKS\n";
   strcat2(str, &i, strSize, title);
   strcat2(str, &i, strSize, title1);

   for (currentLevel = 0; currentLevel < LEVELS; currentLevel++) {
      BUDDY_HEADER * block = (BUDDY_HEADER *) blocks[currentLevel];
      while(block != NULL) {
         aux = intToString(currentLevel, auxBuf);
         strcat2(str, &i, strSize, auxBuf);
         addSpace(str, &i, strSize, 8-aux);

         aux = intToString(INDEX_OF_POINTER_IN_LEVEL((void *)block ,currentLevel, myStartMemory), auxBuf);
         strcat2(str, &i, strSize, auxBuf);
         addSpace(str, &i, strSize, 9-aux);

         aux = intToString(SIZE_OF_BLOCKS_AT_LEVEL(currentLevel), auxBuf);
         strcat2(str, &i, strSize, auxBuf);
         addSpace(str, &i, strSize, 11-aux);

         aux = intToBase((unsigned long long)(block), 16, auxBuf);
         strcat2(str, &i, strSize, auxBuf);
         strcat2(str, &i, strSize, "\n");
         
         block = block ->next;
      }
   }
   strcat2(str, &i, strSize, title2);
   strcat2(str, &i, strSize, title1);
   BUDDY_HEADER *ocBlock = (BUDDY_HEADER *)occupiedBlocks;
   while(ocBlock != NULL) {
      aux = intToString(ocBlock ->level, auxBuf);
      strcat2(str, &i, strSize, auxBuf);
      addSpace(str, &i, strSize, 8-aux);

      aux = intToString(INDEX_OF_POINTER_IN_LEVEL((void *)ocBlock ,currentLevel, myStartMemory), auxBuf);
      strcat2(str, &i, strSize, auxBuf);
      addSpace(str, &i, strSize, 9-aux);

      aux = intToString(SIZE_OF_BLOCKS_AT_LEVEL(ocBlock->level), auxBuf);
      strcat2(str, &i, strSize, auxBuf);
      addSpace(str, &i, strSize, 11-aux);

      aux = intToBase((unsigned long long)(ocBlock), 16, auxBuf);
      strcat2(str, &i, strSize, auxBuf);
      strcat2(str, &i, strSize, "\n");
         
      ocBlock = ocBlock ->next;
   }
}
#endif