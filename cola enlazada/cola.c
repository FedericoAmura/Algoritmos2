#ifndef COLA_C
#define COLA_C
#include "cola.h"
#include <stdlib.h>


/* *****************************************************************
 *                ESTRUCTURA DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo_cola {
    void* dato;
    struct nodo_cola* siguiente;
}nodo_cola_t;

struct cola {
    struct nodo_cola* primero;
    nodo_cola_t* ultimo;
};

/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t* cola_crear(){
    cola_t* cola = malloc(sizeof(cola_t));
    if (cola == NULL) return NULL;
    cola->primero=NULL;
    cola->ultimo=NULL;
    return cola;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)){
    while (!cola_esta_vacia(cola)){
        if (destruir_dato!=NULL) destruir_dato(cola->primero->dato); //aca se destruye el dato
        cola_desencolar(cola);  //devuelve el puntero al dato que se acaba de destruir, pero al no usarse no se accede a memoria invalida
    }
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
    return (cola->primero==NULL);
}

bool cola_encolar(cola_t *cola, void* valor){
    if (valor==NULL) return false;
    nodo_cola_t* nuevonodo = malloc(sizeof(nodo_cola_t));
    if (nuevonodo==NULL) return false;
    nuevonodo->dato = valor;
    nuevonodo->siguiente = NULL;
    if (cola_esta_vacia(cola)){
        cola->primero = nuevonodo;
    }
    else{
        cola->ultimo->siguiente = nuevonodo;
    }
    cola->ultimo = nuevonodo;
    return true;
}

void* cola_ver_primero(const cola_t *cola){
    if (cola_esta_vacia(cola)) return NULL;
    return cola->primero->dato;
}

void* cola_desencolar(cola_t *cola){
    void* devolucion = cola_ver_primero(cola);
    if (devolucion==NULL) return NULL;
    nodo_cola_t* nuevoprimero = cola->primero->siguiente;
    free(cola->primero);
    cola->primero = nuevoprimero;
    if (nuevoprimero==NULL) cola->ultimo=NULL;
    return devolucion;
}

#endif
