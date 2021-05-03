#ifndef LISTADT_H_
#define LISTADT_H_

typedef struct listCDT *listADT;

/*
typedef int elemType;

static int compare(elemType elem1, elemType elem2 ){
	return elem1 - elem2;		
}
*/

listADT newList(int elemSize, int (*cmp)(void* elem1, void* elem2));

int insert(listADT  list, void* element);

int delete(listADT list, void* element);

int listIsEmpty( listADT list);

int elementBelongs(const listADT list, void* element);

void freeList(listADT list);

int listSize(const listADT list);

/**
 * Funciones para poder iterar sobre la lista
 */
void toBegin(listADT list);

int hasNext(const listADT list);

void* next(listADT list);

#endif 
