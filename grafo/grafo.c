#ifndef GRAFO_C
#define GRAFO_C
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "lista.h"
#include "hash.h"
#include "heap.h"
#include "grafo.h"
#define PESOMAXIMO 99999

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct arista{
    char* idarista;
    char* idvertice1;
    char* idvertice2;
    int peso;
}arista_t;

typedef struct vertice{
    char* idvertice;
    int posicionx;
    int posiciony;
    int latitudx;
    int latitudy;
    void* dato;
    lista_t* aristas;
}vertice_t;

struct grafo{
    hash_t* vertices;
    hash_t* aristas;
};

typedef struct vertice_dijkstra{
    vertice_t* vertice;
    vertice_t* padre;
    int costo;
    bool visitado;
}vertice_dijkstra_t;

/* ******************************************************************
 *                    PRIMITIVAS DEL GRAFO
 * *****************************************************************/

void destruir_arista(arista_t* arista){
    free(arista->idarista);
    free(arista->idvertice1);
    free(arista->idvertice2);
    free(arista);
}

void* destruir_vertice(vertice_t* vertice){
    void* dato = vertice->dato;
    free(vertice->idvertice);
    lista_destruir(vertice->aristas,NULL);
    free(vertice);
    return dato;
}

grafo_t* grafo_crear(){
    grafo_t* grafo = malloc(sizeof(grafo_t));
    if (grafo==NULL) return NULL;
    grafo->aristas = hash_crear((hash_destruir_dato_t)destruir_arista);
    grafo->vertices = hash_crear((hash_destruir_dato_t)destruir_vertice);
    if ((grafo->aristas==NULL) || (grafo->vertices==NULL)){
        if (grafo->aristas!=NULL) hash_destruir(grafo->aristas);
        if (grafo->vertices!=NULL) hash_destruir(grafo->vertices);
        free(grafo);
        return NULL;
    }
    return grafo;
}

void grafo_destruir(grafo_t* grafo){
    hash_destruir(grafo->aristas);
    hash_destruir(grafo->vertices);
    free(grafo);
}

//funcion auxiliar que crea un vertice
vertice_t* crear_vertice(char* idvertice,int posicionx,int posiciony,int latitudx,int latitudy,void* dato){
    vertice_t* vertice = malloc(sizeof(vertice_t));
    if (vertice==NULL) return NULL;
    vertice->idvertice = malloc((strlen(idvertice)+1)*sizeof(char));
    if (vertice->idvertice==NULL){
        free(vertice);
        return NULL;
    }
    strcpy(vertice->idvertice,idvertice);
    vertice->aristas = lista_crear();
    if (vertice->aristas==NULL){
        free(vertice->idvertice);
        free(vertice);
        return NULL;
    }
    vertice->posicionx = posicionx;
    vertice->posiciony = posiciony;
    vertice->latitudx = latitudx;
    vertice->latitudy = latitudy;
    vertice->dato = dato;
    return vertice;
}

//funcion auxiliar que crea una arista
arista_t* crear_arista(hash_t* vertices,char* idarista,char* idvertice1,char* idvertice2){
    //creo la arista
    arista_t* arista = malloc(sizeof(arista_t));
    if (arista==NULL) return NULL;
    arista->idarista = malloc((strlen(idarista)+1)*sizeof(char));
    arista->idvertice1 = malloc((strlen(idvertice1)+1)*sizeof(char));
    arista->idvertice2 = malloc((strlen(idvertice2)+1)*sizeof(char));
    if ((arista->idarista==NULL) || (arista->idvertice1==NULL) || (arista->idvertice2==NULL)){
        if (arista->idarista!=NULL) free(arista->idarista);
        if (arista->idvertice1!=NULL) free(arista->idvertice1);
        if (arista->idvertice2!=NULL) free(arista->idvertice2);
        free(arista);
        return NULL;
    }
    strcpy(arista->idarista,idarista);
    strcpy(arista->idvertice1,idvertice1);
    strcpy(arista->idvertice2,idvertice2);
    vertice_t* vertice1 = hash_obtener(vertices,idvertice1);
    vertice_t* vertice2 = hash_obtener(vertices,idvertice2);
    //calculo el peso de la arista
    arista->peso = sqrt(pow((vertice1->posicionx-vertice2->posicionx),2)+pow((vertice1->posiciony-vertice2->posiciony),2));
    return arista;
}

bool grafo_agregar_vertice(grafo_t* grafo,char* idvertice,int posicionx,int posiciony,int latitudx,int latitudy,void* dato){
    if (grafo_existe_vertice(grafo,idvertice)) return false; //podria pisar el dato pero no interesa
    vertice_t* vertice = crear_vertice(idvertice,posicionx,posiciony,latitudx,latitudy,dato);
    if (vertice==NULL) return NULL;
    if (!hash_guardar(grafo->vertices,idvertice,(void*)vertice)){
        destruir_vertice(vertice);
        return false;
    }
    return true;
}

void* grafo_destruir_vertice(grafo_t* grafo,char* idvertice){
    //no se va a usar nunca en este tp, solo se destruyen vertices al final con destruir grafo
    return NULL;
}

bool grafo_agregar_arista(grafo_t* grafo,char* idarista,char* idvertice1,char* idvertice2){
    if (grafo_existe_arista(grafo,idarista)) return false; //ya existe esta arista
    //se podria agregar la arista nuevamente con otro id, en esta implementacion es redundante ya que no es dirigido y el peso se calcula por la distancia
    //verifico si existen los vertices que une la arista
    if ((!hash_pertenece(grafo->vertices,idvertice1)) || (!hash_pertenece(grafo->vertices,idvertice2))) return NULL;
    //creo la arista y la agrego al hash
    arista_t* arista = crear_arista(grafo->vertices,idarista,idvertice1,idvertice2);
    if (arista==NULL) return false;
    hash_guardar(grafo->aristas,idarista,arista);
    //agrego la arista a los vertices que conecta
    vertice_t* vertice1 = hash_obtener(grafo->vertices,idvertice1);
    vertice_t* vertice2 = hash_obtener(grafo->vertices,idvertice2);
    lista_insertar_ultimo(vertice1->aristas,arista);
    lista_insertar_ultimo(vertice2->aristas,arista);
    return true;
}

bool grafo_destruir_arista(grafo_t* grafo,char* idarista){
    //no se va a usar nunca en este tp, solo se destruyen aristas al final con destruir grafo
    return true;
}

bool grafo_existe_vertice(grafo_t* grafo,char* idvertice){
    return hash_pertenece(grafo->vertices,idvertice);
}

void* grafo_dato_vertice(grafo_t* grafo,char* idvertice){
    if (!grafo_existe_vertice(grafo,idvertice)) return NULL;
    vertice_t* vertice = hash_obtener(grafo->vertices,idvertice);
    return vertice->dato;
}

bool grafo_existe_arista(grafo_t* grafo,char* idarista){
    return hash_pertenece(grafo->aristas,idarista);
}

int grafo_peso_arista(grafo_t* grafo,char* idarista){
    if (!grafo_existe_arista(grafo,idarista)) return -1;
    arista_t* arista = hash_obtener(grafo->aristas,idarista);
    return arista->peso;
}

//funcion auxiliar que crea un vertice dijkstrak
vertice_dijkstra_t* crear_vertice_dijkstra(vertice_t* vertice){
    vertice_dijkstra_t* verticedijkstra = malloc(sizeof(vertice_t));
    if (vertice==NULL) return NULL;
    verticedijkstra->vertice = vertice;
    verticedijkstra->padre = NULL;
    verticedijkstra->costo = PESOMAXIMO;
    verticedijkstra->visitado = false;
    return verticedijkstra;
}

void destruir_vertice_dijkstra(vertice_dijkstra_t* vertice){
    free(vertice);
}

//funcion auxiliar que ordena en un heap los vertices de dijkstrak por peso para llegar
int proximo_vertice(vertice_dijkstra_t* vertice1,vertice_dijkstra_t* vertice2){
    return vertice1->costo-vertice2->costo;
}

lista_t* camino_dijkstra(grafo_t* grafo,char* origen,char* destino){
    //verifico que existan el destino y el origen
    if ((!hash_pertenece(grafo->vertices,origen)) || (!hash_pertenece(grafo->vertices,destino))) return NULL;
    //creo las estructuras necesarias para encontrar el camino minimo
    hash_t* hashverticesdijkstra = hash_crear((hash_destruir_dato_t)destruir_vertice_dijkstra);
    heap_t* menordijkstra = heap_crear((cmp_func_t)proximo_vertice);
    lista_t* camino = lista_crear();
    vertice_dijkstra_t* verticedijkstra;
    vertice_t* vertice;
    int* distancia = calloc(1,sizeof(int));
    //creo todos los vertices para dijkstra con los vertices originales y los guardo en el hash adecuado
    hash_iter_t* iterador = hash_iter_crear(grafo->vertices);
    do{
        vertice = (vertice_t*) hash_iter_ver_actual(iterador);
        verticedijkstra = crear_vertice_dijkstra(vertice);
        hash_guardar(hashverticesdijkstra,vertice->idvertice,vertice);
    }while(hash_iter_avanzar(iterador));
    hash_iter_destruir(iterador);
    //consigo el origen
    verticedijkstra = hash_obtener(hashverticesdijkstra,origen);
    //procesoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    //devuelvo
    lista_insertar_primero(camino,distancia);
    return camino;
}

#endif // HASH_C
