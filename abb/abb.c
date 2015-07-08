#ifndef ABB_C
#define ABB_C
#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo_abb nodo_abb_t;

struct nodo_abb{
    char* clave;
    void* dato;
    nodo_abb_t* izq;
    nodo_abb_t* der;
};

struct abb{
    size_t cantidad;
    nodo_abb_t* raiz;
    abb_destruir_dato_t destruir_dato;
    abb_comparar_clave_t comparar_clave;
};

struct abb_iter{
    pila_t* pila;
    nodo_abb_t* nodo;
};

/* ******************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

// Funcion auxiliar que crea un nuevo nodo para el abb
nodo_abb_t* nodo_crear(const char* clave, void* dato){
    nodo_abb_t* nodo_abb = malloc(sizeof(nodo_abb_t));
    if (nodo_abb==NULL) return NULL;
    nodo_abb->clave = malloc(strlen(clave)+1);
    if (nodo_abb->clave==NULL){
        free(nodo_abb);
        return NULL;
    }
    nodo_abb->dato = dato;
    strcpy(nodo_abb->clave,clave);
    nodo_abb->izq = NULL;
    nodo_abb->der = NULL;
    return nodo_abb;

}

//funcion auxiliar para obtener un nodo segun su clave dentro del arbol
nodo_abb_t* buscar_clave(nodo_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp){
    if (nodo==NULL) return NULL;
    int comparacion = cmp(nodo->clave,clave);
    //printf("*");
    if (comparacion==0) return nodo;
    //printf("D");
    if (comparacion<0) return buscar_clave(nodo->der,clave,cmp);
    //printf("I");
    return buscar_clave(nodo->izq,clave,cmp);
}

//Funcion auxiliar que devuelve el padre del nodo indicado
nodo_abb_t* buscar_padre(nodo_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp){
    //si el nodo recibido tiene la misma clave entonces es la raiz y no tiene padre
    if (cmp(nodo->clave,clave)==0) return NULL;
    //busco el padre
    bool seguir = true;
    nodo_abb_t* padre;
    //avanzo hasta dar con el nodo
    while(seguir){
        if (nodo->der!=NULL) if (cmp(clave,nodo->der->clave)==0) seguir=false;
        if (nodo->izq!=NULL) if (cmp(clave,nodo->izq->clave)==0) seguir=false;
        //cuando seguir sea falso, el nodo apuntado es el padre
        padre = nodo;
        //avanza el nodo para seguir comparando, en el ultimo caso apuntaria al nodo hijo
        if (cmp(nodo->clave,clave)<0) nodo=nodo->der; else nodo=nodo->izq;
    }
    return padre;
}

//funcion auxiliar que va destruyendo todo
void destruir_nodo(nodo_abb_t* nodo, abb_destruir_dato_t destruir_dato){
    if (nodo->der!=NULL) destruir_nodo(nodo->der,destruir_dato);
    if (nodo->izq!=NULL) destruir_nodo(nodo->izq,destruir_dato);
    if (destruir_dato!=NULL) destruir_dato(nodo->dato);
    free(nodo->clave);
    free(nodo);
    return;
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* arbol = malloc(sizeof(abb_t));
    if (arbol==NULL) return NULL;
    arbol->raiz = NULL;
    arbol->cantidad = 0;
    arbol->comparar_clave = cmp;
    arbol->destruir_dato = destruir_dato;
    return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    //veo si la clave ya esta
    nodo_abb_t* nodo = buscar_clave(arbol->raiz,clave,arbol->comparar_clave);
    //si ya esta actualizo el dato
    if (nodo!=NULL){
        if (arbol->destruir_dato!=NULL) arbol->destruir_dato(nodo->dato);
        nodo->dato = dato;
        return true;
    }
    //como no esta, lo agrego
    //creo el nuevo nodo
    nodo = nodo_crear(clave,dato);
    //si no hay raiz lo agrego ahi
    if (arbol->raiz==NULL){
        arbol->raiz = nodo;
        arbol->cantidad++;
        return true;
    }
    //consigo un puntero a un nodo
    nodo_abb_t* actual = arbol->raiz;
    //lo avanzo hasta el anterior
    bool seguir = true;
    while (seguir==true){
        if (arbol->comparar_clave(actual->clave,clave)<0 && actual->der!=NULL){
            //el apuntado es menor al a agregar y puedo seguir
            actual = actual->der;
        }
        if (arbol->comparar_clave(actual->clave,clave)>0 && actual->izq!=NULL){
            //el apuntado es mayor al a agregar y puedo seguir
            actual = actual->izq;
        }
        if (arbol->comparar_clave(actual->clave,clave)<0 && actual->der==NULL){
            //el apuntado es menor al agregar y no puedo seguir
            actual->der = nodo;
            arbol->cantidad++;
            return true;
        }
        if (arbol->comparar_clave(actual->clave,clave)>0 && actual->izq==NULL){
            //el apuntado es mayor al agregar y no puedo seguir
            actual->izq = nodo;
            arbol->cantidad++;
            return true;
        }
    }
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    //veo si el arbol esta vacio
    if (arbol->raiz==NULL) return NULL;
    //veo si existe el nodo con la clave a borrar
    nodo_abb_t* eliminar = buscar_clave(arbol->raiz,clave,arbol->comparar_clave);
    if (eliminar==NULL) return NULL;
    //consigo el dato a devolver
    void* devolucion = eliminar->dato;
    //consigo el padre del nodo a borrar
    nodo_abb_t* padre = buscar_padre(arbol->raiz,clave,arbol->comparar_clave);
    //veo si el nodo es una hoja y lo elimino de serlo
    if (eliminar->der==NULL && eliminar->izq==NULL){
        destruir_nodo(eliminar,arbol->destruir_dato);
        if (padre==NULL) arbol->raiz=NULL; else{
            if (arbol->comparar_clave(padre->clave,clave)>0) padre->izq=NULL; else padre->der=NULL;
        }
        arbol->cantidad--;
        return devolucion;
    }
    //veo si el nodo solo tiene una hoja a la derecha
    if (eliminar->der!=NULL && eliminar->izq==NULL){
        if (padre==NULL) arbol->raiz=eliminar->der; else{
            if (arbol->comparar_clave(padre->clave,clave)>0) padre->izq=eliminar->der; else padre->der=eliminar->der;
        }
        free(eliminar->clave);
        free(eliminar);
        arbol->cantidad--;
        return devolucion;
    }
    //veo si el nodo solo tiene una hoja a la izquierda
    if (eliminar->der==NULL && eliminar->izq!=NULL){
        if (padre==NULL) arbol->raiz=eliminar->izq; else{
            if (arbol->comparar_clave(padre->clave,clave)>0) padre->izq=eliminar->izq; else padre->der=eliminar->izq;
        }
        free(eliminar->clave);
        free(eliminar);
        arbol->cantidad--;
        return devolucion;
    }
    //veo si el nodo tiene hojas a ambos lados
    if (eliminar->der!=NULL && eliminar->izq!=NULL){
        nodo_abb_t* temporal;
        char* clavetemporal;
        void* datotemporal;
        bool avance; //es true si el nodo a eliminar esta a la izquierda del padre, false si esta a la derecha
        while(eliminar->der!=NULL){
            padre = eliminar;
            temporal = eliminar;
            eliminar = eliminar->der;
            avance = false;
            while(eliminar->izq!=NULL){
                padre = eliminar;
                eliminar = eliminar->izq;
                avance = true;
            }
            //intercambio datos
            clavetemporal = eliminar->clave;
            datotemporal = eliminar->dato;
            eliminar->clave = temporal->clave;
            eliminar->dato = temporal->dato;
            temporal->clave = clavetemporal;
            temporal->dato = datotemporal;
        }
        //el nodo ahora es una hoja
        destruir_nodo(eliminar,arbol->destruir_dato);
        if (avance) padre->izq=NULL; else padre->der=NULL;
        arbol->cantidad--;
        return devolucion;
    }
    return devolucion;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_abb_t* nodo = buscar_clave(arbol->raiz,clave,arbol->comparar_clave);
    if (nodo==NULL) return NULL;
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    if (buscar_clave(arbol->raiz,clave,arbol->comparar_clave)!=NULL) return true;
    return false;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
    if (arbol->raiz!=NULL) destruir_nodo(arbol->raiz,arbol->destruir_dato);
    free(arbol);
}

/* ******************************************************************
 *                  PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

 void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    bool seguir = true;
    void* datoactual;
    char* claveactual;
    abb_iter_t* iterador = abb_iter_in_crear(arbol);
    if (iterador==NULL) return;
    while(!(abb_iter_in_al_final(iterador)) && seguir){
        claveactual = malloc(strlen(abb_iter_in_ver_actual(iterador))+1);
        strcpy(claveactual,abb_iter_in_ver_actual(iterador));
        datoactual = abb_obtener(arbol,claveactual);
        if (visitar!=NULL) seguir=visitar(claveactual,datoactual,extra);
        abb_iter_in_avanzar(iterador);
        free(claveactual);
    }
    abb_iter_in_destruir(iterador);
 }

/* ******************************************************************
 *                  PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (iter==NULL) return NULL;
    iter->pila = pila_crear();
    if (iter->pila==NULL){
        free(iter);
        return NULL;
    }
    iter->nodo = NULL;
    nodo_abb_t* provisorio = arbol->raiz;
    while (provisorio!=NULL){
        pila_apilar(iter->pila,provisorio);
        iter->nodo = provisorio;
        provisorio = provisorio->izq;
    }
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if (abb_iter_in_al_final(iter)) return false;
    iter->nodo = (nodo_abb_t*) pila_desapilar(iter->pila);
    nodo_abb_t* provisorio = iter->nodo->der;
    while (provisorio!=NULL){
        pila_apilar(iter->pila,provisorio);
        iter->nodo = provisorio;
        provisorio = provisorio->izq;
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    nodo_abb_t* nodo = (nodo_abb_t*) pila_ver_tope(iter->pila);
    if (nodo==NULL) return NULL;
    return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    while(!pila_esta_vacia(iter->pila)) pila_desapilar(iter->pila);
    pila_destruir(iter->pila);
    free(iter);
}

#endif // ABB_C
