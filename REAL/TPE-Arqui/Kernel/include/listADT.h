#ifndef LISTADT_H_
#define LISTADT_H_

#include <stddef.h>
#include <MemMang.h>
#include <lib.h>

typedef struct listCDT *listADT;


listADT newList(int elemSize, int (*equals)(void* elem1, void* elem2));

//IMPOTANTE: se debe hacer un free de lo devuelto por ListToArray
void * ListToArray(listADT list);

//si hubo error devulve -1
int insertBeforeNext(listADT list, void* element);  

//si hubo error devulve -1
int addToTheEnd(listADT list, void* element);

//inserta los elementos al principio de la lista
//si hubo error devulve -1
int insert(listADT  list, void* element);

//devuleve le primer elemento y lo elimina de la lista
//IMPOTANTE: se debe hacer un free de lo devuelto por pop
void* pop(listADT list);

//se obtiene un puntero del elemento, si hubo un error devulve NULL
//se debe implementa la funcion equals para poder usar esta funcion
void* getElem(const listADT list, void* element);

//se obtiene el elemento en el iterador
void* getCurrentElem(const listADT list);

//se elimina el elemento del iterador
int deleteCurrentElem(listADT list);

//si hubo error devulve -1
int deleteFirstElem(listADT list);

//retorna 1 si lo elimino, sino no lo encontro devuelve 0
//se debe implementa equals para poder usar esta funcion
int delete(listADT list, void* element);

//en el caso que el elemento guarde un tipo de dato que necesite un free, 
//solo se pude usar este funcion para borrarlo.
//se debe implementa equals para poder usar esta funcion
int deleteElem(listADT list, void* element, void (*deleteElemValue)(void* value)); //falta testear

int isEmpty(const listADT list);

int size(const listADT list);

//borra todos los elemntos de la lista, permite seguir usando la lista
void cleanList(listADT list);

//borra la lista en si, no se puede seguir usando la lista
void freeList(listADT list);

//se debe implementa equals para poder usar esta funcion
int elementBelongs(const listADT list, void* element); 

//Funciones para poder iterar sobre la lista:
void toBegin(listADT list);

int hasNext(listADT list);

//no es necesario usar el hasNext(), al llegar al final hace un toBegin()
//se comporta como un iterador circular
void* next(listADT list);

#endif 
