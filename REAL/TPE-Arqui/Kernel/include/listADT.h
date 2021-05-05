#ifndef LISTADT_H_
#define LISTADT_H_

typedef struct listCDT *listADT;


listADT newList(int elemSize, int (*cmp)(void* elem1, void* elem2));


int insertBeforeNext(listADT list, void* element);

int addToTheEnd(listADT list, void* element);

int insert(listADT  list, void* element);


void* getFirstElem(listADT list);


int deleteCurrentElem(listADT list);

int delete(listADT list, void* element);


int isEmpty(const listADT list);

int size(const listADT list);

int elementBelongs(const listADT list, void* element);

void freeList(listADT list);


/**
 * Funciones para poder iterar sobre la lista
 */
void toBegin(listADT list);

int hasNext(listADT list);

void* next(listADT list);

#endif 
