#ifndef PILA_C
#define PILA_C
#define VALORINICIAL 5
#include "pila.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdbool.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* Se trata de una pila que contiene datos de tipo void*
 * (punteros genéricos).  La pila en sí está definida en el .c.  */

struct _pila {
    void** datos;
    size_t tamanio;
    size_t cantidad;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear(){
  pila_t* pila = (pila_t*) malloc(sizeof(pila_t));
  if (pila == NULL) return NULL;
  pila->datos = (void**) malloc(VALORINICIAL*sizeof(void*));
  if (pila->datos == NULL){
    free(pila);
    return NULL;
  }
  pila->tamanio=VALORINICIAL;
  pila->cantidad=0;
  return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila){
	bool puedodestruir;
	puedodestruir=pila_esta_vacia(pila);
	if (puedodestruir){
		free(pila->datos);
		free(pila);
		pila=NULL;
	}else{
	printf("no pude destruir la pila\n");
	}
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila){
    if (pila->cantidad==0) {
        return true;
    }
    return false;
}

void pila_redimensionar(pila_t *pila){
	void** temporal;
	//printf("redimensiono\n");
	if (pila->tamanio==pila->cantidad){
        //printf("el tamanio a redimensionar es:%d\n",(int)pila->tamanio);
        temporal=realloc(pila->datos,(pila->tamanio)*sizeof(void*)+5*sizeof(void*));
        if (temporal!=NULL){
            pila->datos=temporal;
            pila->tamanio=(pila->tamanio+5);
            //printf("el tamanio ahora es:%d\n",(int)pila->tamanio);
            }
	}
	if (pila->tamanio>((pila->cantidad)+10)){
        //printf("el tamanio a redimensionar es:%d\n",(int)pila->tamanio);
        temporal=realloc(pila->datos,(pila->tamanio)*sizeof(void*)-5*sizeof(void*));
        if (temporal!=NULL){
            pila->datos=temporal;
            pila->tamanio=(pila->tamanio-5);
            //printf("el tamanio ahora es:%d\n",(int)pila->tamanio);
        }
	}
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor){
    //printf("inicio apilar - tamanio de pila:%zu cantidad de pila:%zu\n",pila->tamanio,pila->cantidad);
	if (pila->tamanio==pila->cantidad) pila_redimensionar(pila);
	if (pila->tamanio==pila->cantidad) return false;			//si la pila sigue siendo del mismo tamanio fallo el redimensionar//
    //printf("redimensionando(si hizo falta) - tamanio de pila:%zu cantidad de pila:%zu\n",pila->tamanio,pila->cantidad);
	pila->datos[(pila->cantidad)]=valor;
	pila->cantidad=(pila->cantidad+1);
    //printf("fin apilar - tamanio de pila:%zu cantidad de pila:%zu\n",pila->tamanio,pila->cantidad);
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila){
	if (pila->cantidad==0) return NULL;
	return pila->datos[pila->cantidad-1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila){
    //printf("inicio desapilar - tamanio de pila:%zu cantidad de pila:%zu\n",pila->tamanio,pila->cantidad);
	if (pila->cantidad==0){
        //printf("no puedo desapilar de algo vacio\n");
        return NULL;
	}else{
        //printf("saco el dato\n");
        void* dato = pila->datos[pila->cantidad-1];
        //printf("achico la cantidad\n");
        pila->cantidad=(pila->cantidad-1);
        if (pila->tamanio>((pila->cantidad)+10)) pila_redimensionar(pila);
        //printf("redimensionado(si hizo falta) - tamanio de pila:%zu cantidad de pila:%zu\n",pila->tamanio,pila->cantidad);
        return dato;
    }
}


#endif // PILA_H
