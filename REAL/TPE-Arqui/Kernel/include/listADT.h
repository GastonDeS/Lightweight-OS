#ifndef LISTADT_H_
#define LISTADT_H_

#include <stddef.h>
#include <MemMang.h>
#include <lib.h>


typedef struct listCDT *listADT;


//crea una nueva lista
listADT newList(int elemSize, int (*equals)(void* elem1, void* elem2));

void * ListToArray(listADT list);

int insertBeforeNext(listADT list, void* element);  

int addToTheEnd(listADT list, void* element);

//inserta los elementos al principio de la lista
int insert(listADT  list, void* element);


//devuleve le primer elemento y lo elimina de la lista
void* pop(listADT list);

//se obtiene un puntero del elemento que se puede modificar y cambiar el elemento en la lista
void* getElem(const listADT list, void* element);

void* getCurrentElem(const listADT list);


int deleteCurrentElem(listADT list);

int deleteFirstElem(listADT list);

int delete(listADT list, void* element);

//en el caso que el elemento guarde un tipo de dato que necesite un free, solo
//se pude usar este funcion para borrar 
int deleteElem(listADT list, void* element, void (*deleteElemValue)(void* value)); //falta testear


int isEmpty(const listADT list);

int size(const listADT list);

//borra todos los elemntos de la lista, se puede seguir usando
void cleanList(listADT list);

//borra la lista en si, no se puede seguir usando
void freeList(listADT list);


int elementBelongs(const listADT list, void* element); 




/**
 * Funciones para poder iterar sobre la lista
 */
void toBegin(listADT list);

int hasNext(listADT list);

void* next(listADT list);

#endif 
