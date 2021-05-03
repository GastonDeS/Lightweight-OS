#include "listADT.h"
#include <stddef.h>
#include <MemMang.h>
#include <lib.h>

typedef struct node {
	void* value;
	struct node * next;
}node;

typedef struct node * nodeP;

typedef struct listCDT{
	nodeP first;
	int valueBytes;
    int (*cmp)(void* elem1, void* elem2);
    unsigned int size; //se podria sacar
	nodeP iteradorNext; //se podria sacar
}listCDT;

//prtivate:
nodeP search(nodeP current, void* element, int (*comparator)(void*, void*));
int contains(nodeP first, void* element, int (*comparator)(void*, void*));

listADT newList(int elemSize, int (*cmp)(void* elem1, void* elem2)){
    listADT list = malloc(sizeof(struct listCDT));
    list->first = NULL;
    list->valueBytes = elemSize;
    list->cmp = cmp;
    list->size = 0;
    list->iteradorNext = NULL;
    return list;
}

//inserta los elementos al principio de la lista
int insert(listADT list, void* element){
    nodeP aux = malloc(sizeof(struct node));
        if(aux == NULL)
            return -1;
    aux->next = list->first;
    list->first = aux;
    memcpy(aux->value, element, list->valueBytes);
    //aux->value = element;
    list->size++;
    return 0;
}

//se le podria pasar un puntero a funcion del compare
//retorna 1 si lo elimino y 0 si no lo encontro 
int delete(listADT list, void* element){
    nodeP current = list->first;
    nodeP last = search(current, element, list->cmp);

    if(current == NULL)
        return 0;

    if(last == NULL){
        list->first = current->next;
    }else{
        last->next = current->next;
    }

    list->size --;
    free(current->value);
    free(current);
    return 1;
}

int listIsEmpty( listADT list){
	return list->size == 0;
}

int elementBelongs(listADT list, void* element){
	return contains(list->first, element, list->cmp);
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

int listSize(const listADT list) {
	return list->size;
}

//iterator
void toBegin(listADT list) {
	list->iteradorNext = list->first;
}

int hasNext(const listADT list) {
	return list->iteradorNext != NULL;
}

void* next(listADT list) {
	if (hasNext(list))
		toBegin(list); // NEXT CIRCULAR PREGUNTAR
	void* result = list->iteradorNext->value;
	list->iteradorNext = list->iteradorNext->next;

	return result;
}

//private:

//si current es null no lo encotro
//si last es null el elemnto buscado es el primer elemnto
nodeP search(nodeP current, void* element, int (*comparator)(void*, void*)) {
	nodeP last = NULL;
    while (current != NULL){
        if(comparator(current->value, element) == 0){
            return last;
        }
        last = current;
        current = current->next;
    }
    return last;     
}

//devuelve 1 si lo encontro sino 0
int contains(nodeP first, void* element, int (*comparator)(void*, void*)){
    search(first, element, comparator);
    if(first == NULL)
        return 0;
    return 1;
}

