#include "listADT.h"
#include <stddef.h>
#include <MemMang.h>
#include <lib.h>

typedef struct node {
	void* value;
	struct node * next;
    struct node * previous;
}node;
typedef struct node * nodeP;

typedef struct listCDT{
	nodeP first;
    nodeP iteradorNext; 
	int valueBytes;
    int (*cmp)(void* elem1, void* elem2);
    unsigned int size;
}listCDT;

//prtivate:
int search(nodeP current, void* element, int (*comparator)(void*, void*));

listADT newList(int elemSize, int (*cmp)(void* elem1, void* elem2)){
    listADT list = malloc(sizeof(struct listCDT));
    list->first = NULL;
    list->valueBytes = elemSize;
    list->cmp = cmp;
    list->size = 0;
    list->iteradorNext = NULL;
    return list;
}

int insertBeforeNext(listADT list, void* element){
    if(!hasNext(list)) // no se inicializo iteradorNext
        toBegin(list);

    if(list->iteradorNext == list->first)
        return addToTheEnd(list, element);

    nodeP aux = malloc(sizeof(struct node)); //creo el elemento
    if(aux == NULL)
        return -1;
    memcpy(aux->value, element, list->valueBytes);

    list->iteradorNext->previous->next = aux;
    aux->previous = list->iteradorNext->previous;
    aux->next = list->iteradorNext;
    list->iteradorNext->previous = aux; 
    list->size++;

    return 0;
}

int addToTheEnd(listADT list, void* element){
    
    if(isEmpty(list))//es el primer elemento
        return insert(list, element);
        
    nodeP aux = malloc(sizeof(struct node)); //creo el elemnto
    if(aux == NULL)
        return -1;
    memcpy(aux->value, element, list->valueBytes);
    aux->next = NULL;

    nodeP current = list->first;
    while(current->next != NULL )
        current = current->next;
    current->next = aux;
    aux->previous = current;
    list->size++;
    return 0;
}

//inserta los elementos al principio de la lista
int insert(listADT list, void* element){
    nodeP aux = malloc(sizeof(struct node));
    if(aux == NULL)
        return -1;
    memcpy(aux->value, element, list->valueBytes);
    aux->previous = NULL;
    aux->next = list->first;
    list->first = aux;
    list->size++;
    return 0;
}


//si hubo un error devulve NULL
void* getFirstElem(listADT list){
    if(isEmpty(list))
        return NULL;
    
    void* result;
    memcpy(result, list->first->value, list->valueBytes);

    deletFistElem(list);
    return result;
}

int deletFistElem(listADT list){
    if(isEmpty(list))
        return -1;
    
    nodeP aux = list->first;
    list->first = aux->next;
    list->first->previous = NULL;
    list->size--;
    free(aux->value);
    free(aux);

    return 0;
}

int deleteCurrentElem(listADT list){
    if(!hasNext(list))//si no lo fue inicializado, se inicializa 
        toBegin(list);

    //variable auxiliares
    nodeP deleteNode = list->iteradorNext->previous;
    nodeP previousNode = deleteNode->previous;

    list->iteradorNext->previous = previousNode;
    if(previousNode != NULL)
        previousNode->next = list->iteradorNext;
    else
        list->first = list->iteradorNext;
    

    free(deleteNode->value);
    free(deleteNode);
}

//se le podria pasar un puntero a funcion del compare
//retorna 1 si lo elimino y 0 si no lo encontro 
int delete(listADT list, void* element){
    nodeP current = list->first;
    if(!search(current, element, list->cmp))
        return 0;

    if(current->next != NULL)
        current->next->previous = current->previous;
    if(current->previous != NULL)
        current->previous->next = current->next;
    else
        list->first = current->next;
    list->size --;
    free(current->value);
    free(current);
    return 1;
}

int isEmpty(const listADT list){
	return size(list) == 0;
}

int size(const listADT list) {
	return list->size;
}

int elementBelongs(const listADT list, void* element){
	return search(list->first, element, list->cmp);
}

void freeList( listADT list){
	nodeP current = list->first, aux;
	while (current != NULL) {
		aux = current->next;
		free(current->value);
        free(current);
		current = aux;
	}
    free(list);
}



//   ITERADOR
void toBegin(listADT list) {
	list->iteradorNext = list->first;
}

int hasNext(const listADT list) {
	return list->iteradorNext != NULL;
}

void* next(listADT list) {
	if(!hasNext(list))
		toBegin(list);
    void* result;
    memcpy(result, list->first->value, list->valueBytes);
	list->iteradorNext = list->iteradorNext->next;

	return result;
}

//private:
//si retorna 1 lo encontro sino -1 y en current esta el nodo buscado
int search(nodeP current, void* element, int (*comparator)(void*, void*)) {
    while (current != NULL){
        if(comparator(current->value, element) == 0){
            return 1;
        }
        current = current->next;
    }
    return -1;     
}

