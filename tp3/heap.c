#ifndef _HEAP_C
#define _HEAP_C

#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#define TAMANIOINICIAL 15

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct heap{
    cmp_func_t cmp;
    void** dato;
    size_t tamanio;
    size_t cantidad;
};

/* ******************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

//funcion auxiliar para cambiar dos posiciones del heap
void swapposiciones(heap_t* heap, size_t pos1, size_t pos2){
    void* aux;
    aux = heap->dato[pos1];
    heap->dato[pos1] = heap->dato[pos2];
    heap->dato[pos2] = aux;
}

//funcion auxiliar para redimensionar el heap
void heap_redimensionar(heap_t *heap, int nuevotamanio){
    void** temporal;
    temporal = realloc(heap->dato,sizeof(void*)*nuevotamanio);
	if (temporal!=NULL){
        heap->dato=temporal;
        heap->tamanio=nuevotamanio;
	}
}

//funcion upheap
void heap_upheap(heap_t* heap, size_t pos){
    if (pos==0) return;  //estoy en la raiz
    size_t padre = (pos-1)/2;
    if (heap->cmp(heap->dato[padre],heap->dato[pos])<0){
        swapposiciones(heap,pos,padre);
        heap_upheap(heap,padre);
    }
    return;
}

//funcion downheap
void heap_downheap(heap_t* heap, size_t pos){
    //calculo la posicion al primer hijo
    size_t hijo = 2*pos+1;
    //veo si el hijo esta fuera del vector
    if ((hijo+1)>heap_cantidad(heap)) return; //el nodo no tiene hijos
    //veo si el nodo tiene un solo hijo
    if ((hijo+1)==heap_cantidad(heap)){
        if (heap->cmp(heap->dato[pos],heap->dato[hijo])<0) swapposiciones(heap,pos,hijo);
        return;
    }
    //el nodo tiene sus dos hijos
    if (heap->cmp(heap->dato[hijo],heap->dato[hijo+1])<0) hijo++; //si el hijo derecho es mayor corro la posicion del hijo
    if (heap->cmp(heap->dato[pos],heap->dato[hijo])<0){
        swapposiciones(heap,pos,hijo);
        heap_downheap(heap,hijo);
    }
    return;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    if (cant==0) return;
    heap_t* heap = heap_crear(cmp);
    free(heap->dato);
    heap->dato = elementos;
    heap->cantidad = cant;
    heap->tamanio = sizeof(elementos)/sizeof(void*);
    heap_heapify(heap);
    for(int i=cant-1;i>0;i--){
        swapposiciones(heap,0,i);
        heap->cantidad--;
        heap_downheap(heap,0);
    }
    free(heap);
    return;
}

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if (heap==NULL) return NULL;
    heap->dato = malloc(sizeof(void*)*TAMANIOINICIAL);
    heap->cmp = cmp;
    heap->cantidad = 0;
    heap->tamanio = TAMANIOINICIAL;
    return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
    while(!heap_esta_vacio(heap)){
        if (destruir_elemento!=NULL) destruir_elemento(heap_desencolar(heap)); else heap_desencolar(heap);
    }
    free(heap->dato);
    free(heap);
    return;
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
    return (heap->cantidad==0);
}

bool heap_encolar(heap_t *heap, void *elem){
    heap->dato[heap->cantidad] = elem;
    heap_upheap(heap,heap->cantidad);
    heap->cantidad++;
    if ((heap->tamanio)<(heap->cantidad+5)) heap_redimensionar(heap,heap->tamanio+8);
    return true;
}

void *heap_ver_max(const heap_t *heap){
    if (heap_esta_vacio(heap)) return NULL;
    return heap->dato[0];
}

void *heap_desencolar(heap_t *heap){
    if (heap_esta_vacio(heap)) return NULL;
    void* devolucion = heap_ver_max(heap);
    swapposiciones(heap,0,heap->cantidad-1);
    heap->cantidad--;
    heap_downheap(heap,0);
    if ((heap->tamanio)>(heap->cantidad+16)) heap_redimensionar(heap,heap->tamanio-8);
    return devolucion;
}

void heap_heapify(heap_t* heap){
    for(int i=heap_cantidad(heap)/2;i>=0;i--) heap_downheap(heap,i);
    return;
}

#endif // _HEAP_C
