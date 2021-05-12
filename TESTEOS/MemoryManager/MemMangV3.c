/*
** Basado en:
https://mgarciaisaia.github.io/tutorial-c/blog/2014/12/26/un-tutorial-rapido-para-implementar-y-debuggear-malloc/
** Se agregaron las siguientes mejoras:
**   -> Se reducieron las llamas al sistema con la funcion MemManager() la cual pide MIN_BYTES_REQUEST bytes que luego administra
**   -> Si el tamaño del bloque devuelto por findFreeBlock() supera en MAX_DIFF_SIZE al tamaño neceasrio entonces se divide el bolque
**   ->pasar int free a char free
     ->mejora de diseño en la funciones de debugger y se le agrego agumento que
permite imprimir falta:
        ->que la memoria este alineada
        ->
*/
#include "MemMang.h"


// INFO_BLOCK_SIZE = 32 bytes
void *firstInfoBlock = NULL;
void *memorySize = NULL; //(memorySize-firstInfoBlock) tamaño total de la memoria
void *memoryDim = NULL; //(memoryDim-firstInfoBlock) bytes usados

// solo visible para syscall
#define HEAP_SIZE 10000 // 10 Kb
void *startMemory = NULL;
void *currentMemoryLimit = NULL;

struct infoBlock {
  size_t size; // 12 bytes
  char free;
  struct infoBlock *next;
  struct infoBlock *previous;
};
typedef struct infoBlock *infoBlockPtr;

// private:
void *sysCall(size_t size);
infoBlockPtr findFreeBlock(infoBlockPtr *last, size_t size);
infoBlockPtr requestSpace(infoBlockPtr last, size_t size);
void splitBlock(infoBlockPtr block, size_t size);
infoBlockPtr getBlockPtr(void *ptr);
void *syscallManager(size_t size);
void joinNextBlock(infoBlockPtr current);
void* joinPreviousBlock(infoBlockPtr current);

void *my_malloc(size_t size) {
  if (size <= 0)
    return NULL;

  infoBlockPtr block;
  
  if (firstInfoBlock == NULL) { // primer llamado
    block = requestSpace(NULL, size);
    if (block == NULL)
      return NULL;
    firstInfoBlock = block;
  } else {
    infoBlockPtr last = firstInfoBlock;
    block = findFreeBlock(&last, size);
    if (block == NULL) {                // no se encontro un bloque
      block = requestSpace(last, size); // agrega un nuevo bloque al final
      if (block == NULL)
        return NULL;
    } else { // se encontro un boque
      block->free = 0;

      //if ((int)block->size - (int)size - (int)INFO_BLOCK_SIZE >(int)MAX_DIFF_SIZE )
      splitBlock(block, size);
    }
  }
  return (block + 1); // la direccion justo despues de infoBlock
}

/*----------------------------------------------------------------*/
//solo lo divide si sobran MAX_DIFF_SIZE bytes no usados
void splitBlock(infoBlockPtr block, size_t size){
  if ( !((int)block->size - (int)size - (int)INFO_BLOCK_SIZE > (int)MAX_DIFF_SIZE))
    return;

  infoBlockPtr newBlock = (void*)(block + 1) + size;
  newBlock->free = 1;
  newBlock->size = (block->size) - size - INFO_BLOCK_SIZE;
  newBlock->previous = block;
  newBlock->next = block->next;

  if (block->next != NULL)
    block->next->previous = newBlock;

  block->next = newBlock;
  block->size = size;
  return;
}

/*----------------------------------------------------------------*/
infoBlockPtr findFreeBlock(infoBlockPtr *last, size_t size) {
  infoBlockPtr current = firstInfoBlock;
  infoBlockPtr blockAptSize = NULL;
  size_t aptSize = SIZE_MAX;
  while (current != NULL) {
    if (current->free && current->size >= size && current->size < aptSize) {
      if (current->size == size)
        return current; // current tiene el tamaño mas apto
      aptSize = current->size;
      blockAptSize = current;
    }
    *last = current;
    current = current->next;
  }
  if (blockAptSize != NULL)
    return blockAptSize;
  return current;
}

/*----------------------------------------------------------------*/
infoBlockPtr requestSpace(infoBlockPtr last, size_t size) {
  void* ptr = syscallManager(size + INFO_BLOCK_SIZE);

  long intPtr = (long) ptr;
  long align = (intPtr+INFO_BLOCK_SIZE)%64;
  if(align != 0){
    if(align < (64-INFO_BLOCK_SIZE))
      align += (64-INFO_BLOCK_SIZE) - align;
    else
      align += 64-align;
  }

  void* newPtr = (void*)(intPtr+align); 

  //en align queda guardad la cantidad de bits/bytes que se movio el bloque hacia a delante
  //newPtr es el puntero que hay que usar 
  

  infoBlockPtr block = newPtr;
  if (block == NULL) // no hay mas espacio en el heap
    return NULL;

  if (last != NULL) { // si last==NULL es el primer llamado
    last->next = block;
  }
  block->size = size;
  block->free = 0;
  block->next = NULL;
  block->previous = last;

  return block;
}

/*----------------------------------------------------------------*/
void *syscallManager(size_t size) {
  if (memoryDim == NULL) { // primer llamado
    memorySize = sysCall(0);
    memoryDim = memorySize;
  }

  void *resutl = memoryDim;
  if (memorySize - memoryDim < size) {
    void *check = NULL;
    if (size <= MIN_BYTES_REQUEST){
      check = sysCall(MIN_BYTES_REQUEST);
    } else {
      check = sysCall(size + MIN_BYTES_REQUEST);
    }
    if (check == NULL)
      return NULL; // no hay mas espacio en el heap
    memorySize = check;
    memoryDim += size;
  }else
    memoryDim += size;
  
  return resutl;
}

/*----------------------------------------------------------------*/
infoBlockPtr getBlockPtr(void *ptr) { return (infoBlockPtr)ptr - 1; }

/*----------------------------------------------------------------*/
void my_free(void *ptr) {
  if (!ptr)
    return;
  infoBlockPtr current = getBlockPtr(ptr);
  current->free = 1;
  //si es posible lo uno con el siguiente bloque 
  joinNextBlock(current);

  //si es posible lo uno con el bloque anterior
  joinPreviousBlock(current);
  
  return;
}

/*----------------------------------------------------------------*/
void joinNextBlock(infoBlockPtr current){
  infoBlockPtr aux = current->next;
  if (aux != NULL && aux->free){ // el siguiente esta libre
    current->size += aux->size + INFO_BLOCK_SIZE;
    if (aux->next != NULL)
      aux->next->previous = current;
    current->next = aux->next;
    aux->next = NULL;
    aux->previous = NULL;
  }
}

/*----------------------------------------------------------------*/
void* joinPreviousBlock(infoBlockPtr current){
  infoBlockPtr aux = current->previous;
  if (aux != NULL && aux->free){ // el anterior esta libre
    aux->size += current->size + INFO_BLOCK_SIZE;
    if (current->next != NULL)
      current->next->previous = aux;
    aux->next = current->next;
    current->next = NULL;
    current->previous = NULL;
    return aux;
  }
  return NULL; //no se union 
}

/*----------------------------------------------------------------*/
void* my_realloc(void *ptr, size_t newSize){
  if (!ptr)
    return NULL;
  infoBlockPtr current = getBlockPtr(ptr);
  
  if(current->size >= newSize) //entra en el bloque actual
    return ptr;

  //veo si hay espacio despues
  joinNextBlock(current);
  if(current->size >= newSize){
    splitBlock(current, newSize);
    return (current+1);
  }
   
  /*
  por la dificultad de la implementacion, si el bloque anterior esta libre no se unen

  infoBlockPtr newCurrent = joinPreviousBlock(current);
  if (current->size >= newSize){
    splitBlock(current, newSize);
    return (current + 1);
  }
  */

  //no se puede amplear el tamaño actual, se crea un nuevo bloque
  void* newPtr = malloc(newSize);
  if(newPtr == NULL)//si no hay espacio se devulve null y no se modifica ptr
    return NULL;
  memcpy(newPtr, ptr, current->size);
  free(ptr);
  return newPtr;
}


//----------------------------------------------debugger---------------------------------------------------
struct data {
  int numeberOfBlocks;
  int freeBlock;
  int blockused;

  int totalBytes;        // Total de bytes administrados por malloc
  int bytesUsedByBLocks; // Bytes usados por los infoBLock
  int bytesUsedByUser;   // Bytes usados por el usuario
  int unusedBytes;       // Bytes no usados por reutilizar bloques
  int bytesUsedByAlign;  // Bytes usados para alinear
};
typedef struct data *dataPtr;

int registerData(infoBlockPtr block, dataPtr data) {
  if (block == NULL)
    return -1;
  data->numeberOfBlocks++;
  if (block->free)
    data->freeBlock++;
  else
    data->blockused++;

  data->bytesUsedByBLocks += INFO_BLOCK_SIZE;
  data->bytesUsedByUser += block->size;
  data->bytesUsedByAlign += 0;
  if (block->next != NULL) {
    data->unusedBytes +=
        (long)block->next - (long)block - (int)block->size - INFO_BLOCK_SIZE;
    data->totalBytes += (long)block->next - (long)block;
  } else {
    data->unusedBytes +=
        (long)memoryDim - (long)block - (int)block->size - INFO_BLOCK_SIZE;
    data->totalBytes += (long)memoryDim - (long)block;
  }
  return 0;
}

void printData(struct data data) {
  printf("* Cantidad de bloques: %d\n", data.numeberOfBlocks);
  printf("    |-> Usados: %d\n", data.blockused);
  printf("    |-> Libre: %d\n", data.freeBlock);
  printf("\n");
  printf("* Cantidad total de bytes usados: %d\n", data.totalBytes);
  printf("    |-> Usados en infoBLocks: %d\n", data.bytesUsedByBLocks);
  printf("    |-> Usados por el usuario: %d\n", data.bytesUsedByUser);
  printf("    |-> No utilizados: %d\n", data.unusedBytes);
  printf("    |-> Usados para alinear: %d\n", data.bytesUsedByAlign);
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
  (*) los punteros devueltos por malloc tienen que estar alineados a 64 bit
*/
int checkMemory(int ptintFlag) {
  infoBlockPtr current = firstInfoBlock;
  struct data data = {0};
  int blockIndex = 0, freeFlag = 0, numError = 0;
  if (ptintFlag)
    printf("----------------START----------------\n");

  while (current != NULL) {
    if (registerData(current, &data) < 0) {
      if (ptintFlag)
        printf("error en el blocke %d: %p\n", blockIndex, current);
      return -1;
    }
    if (current->free) { // no puede haber dos free juntos
      if (freeFlag) {
        if (ptintFlag)
          printf("(*) Hay dos bloque libre juntos, el bloque:%d y bloque:%d\n",
                 blockIndex - 1, blockIndex);
        numError++;
      } else
        freeFlag = 1;
    } else
      freeFlag = 0;
    
    long ptr = (long)(current+1);
    if(ptr % 63 != 0) 
       printf("(*) En bloque %d; no esta alineado a 64 bits (%p)",blockIndex,(current+1));

    if (current->next != NULL) {
      long notUsed = (long)current->next - (long)current - (int)current->size - INFO_BLOCK_SIZE;
      if (notUsed > MAX_DIFF_SIZE) {
        if (ptintFlag)
          printf("(*) Entre los bloque %d y %d hay %ld bytes no usados (limite permitido: %d)\n", blockIndex, blockIndex + 1, notUsed, MAX_DIFF_SIZE);
        numError++;
      }
      if (current != current->next->previous) {
        if (ptintFlag)
          printf("(*) En bloque %d; su puntero:%p  != %p "
                 ":current->next->previous\n",
                 blockIndex, current, current->next->previous);
        numError++;
      }
    }
    blockIndex++;
    current = current->next;
  }

  if (data.totalBytes != (data.bytesUsedByBLocks + data.bytesUsedByUser +
                          data.unusedBytes + data.bytesUsedByAlign)) {
    if (ptintFlag)
      printf(
          "(*) totalBytes: %d != %d (sumatoria de todos los tipos de bytes)\n",
          data.totalBytes,
          (data.bytesUsedByBLocks + data.bytesUsedByUser + data.unusedBytes +
           data.bytesUsedByAlign));
    numError++;
  }
  if (data.numeberOfBlocks != data.blockused + data.freeBlock) {
    if (ptintFlag)
      printf("(*) numeberOfBlocks: %d  != %d (suma de los bloques libre y "
             "usados)\n",
             data.numeberOfBlocks, data.blockused + data.freeBlock);
    numError++;
  }
  if (memoryDim - firstInfoBlock != data.totalBytes) {
    if (ptintFlag)
      printf("(*) memoryDim-firstInfoBlock: %ld  != %d (sumatoria de todos los "
             "bloque->size+INFO_BLOCK_SIZE)\n",
             (memoryDim - firstInfoBlock), data.totalBytes);
    numError++;
  }
  if (currentMemoryLimit != memorySize) {
    if (ptintFlag)
      printf("(*) currentMemoryLimit: %p != memorySize: %p \n",
             currentMemoryLimit, memorySize);
    numError++;
  }
  if (firstInfoBlock != startMemory) {
    if (ptintFlag)
      printf("(*) firstInfoBlock: %p != startMemory: %p \n", firstInfoBlock,
             startMemory);
    numError++;
  }
  if (ptintFlag) {
    printf("* cantidad de Errores encontrados: %d\n", numError);
    printf("\n");
    printData(data);
    printf("-----------------END-----------------\n");
  }
  return numError > 0 ? -1 : 0;
}

void printMemoryBLock(int printFalg) {
  int cantElemPerLine =
      50; //<----- para cambiar la cantidad de elementos en una linea
  infoBlockPtr current = firstInfoBlock;
  struct data data = {0};
  int index = 0;
  printf("----------------START----------------\n");
  while (current != NULL) {
    if (registerData(current, &data) < 0) {
      printf("error en el blocke %d: %p\n", index, current);
      return;
    }
    printf("Block %d: %p\n", index, current);
    printf("  |-> size: %ld\n", current->size);
    printf("  |-> free: %s\n", (current->free == 0 ? "no" : "yes"));
    printf("  |-> next: %p\n", current->next);
    printf("  |-> previous: %p\n", current->previous);
    if (printFalg) {
      printf("  |-> datos guardados:");
      char *aux = current + 1; // NO CAMBIAR A (char*)(current+1)
      for (int i = 0; i < current->size; i++) {
        if (i % cantElemPerLine == 0) {
          printf("\n \t");
        }
        printf("%d ", *(aux + i));
      }
      printf("\n");
    }
    current = current->next;
    index++;
    printf("\n");
  }
  printf("\n");
  printData(data);
  printf("-----------------END-----------------\n");
  return;
}

/*------------------------------SYSCALL----------------------------------------*/
// sbrk
void *sysCall(size_t size) {
  if (startMemory == NULL) {
    startMemory = malloc(HEAP_SIZE);
    if (startMemory == NULL) {
      printf("fallo malloc de stdlib\n");
      exit(0);
    }
    currentMemoryLimit = startMemory;
    return currentMemoryLimit;
  }
  if ((currentMemoryLimit - startMemory) + size <= HEAP_SIZE)
    return currentMemoryLimit += size;

  return NULL;
}

void MM_end() { free(startMemory); }
