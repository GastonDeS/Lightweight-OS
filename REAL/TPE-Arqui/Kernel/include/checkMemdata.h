#ifndef CHECKMEMDATA_H
#define CHECKMEMDATA_H

#include <stdint.h>

struct checkMemdata{
    int numeberOfBlocks;
    int freeBlock;
    int blockused;

    int totalBytes;        // Total de bytes administrados por malloc
    int bytesUsedByBLocks; // Bytes usados por los infoBLock
    int bytesUsedByUser;   // Bytes usados por el usuario
    int freeBytes;       // Bytes no usados por reutilizar bloques
    int lostBytes;         //bytes que por algun error no se toman en cuenta
    
    int freeBlocksTogether; //numero de bloques libre que estan junto a otro bloque libre
    int curNextPrev;        //cantidad de bloques que no cumplen: current != current->next->previous
    int numError;           //numeros de errores encontrados

    char bytesError;      //si hubo errores en la administracion de los byts
    char numblocksError;  //no conidene la suma de bloques libre y en uso con el total de bloques
    char memError;        //la memoria entregada por sbrk y la susada son distanitas
};


#endif
