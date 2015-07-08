#ifndef GRAFO_H
#define GRAFO_H
#include <stdbool.h>
#include <stddef.h>
#include "lista.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct grafo grafo_t;

/* ******************************************************************
 *                    PRIMITIVAS DEL GRAFO
 * *****************************************************************/

/* Primitivas de creacion y destruccion */

// Crea un grafo.
// Post: Devuelve un nuevo grafo vacio.
grafo_t* grafo_crear();

// Destruye un grafo.
// Pre: Se recibe un grafo creado.
// Post: Devuelve un nuevo grafo vacio.
void grafo_destruir(grafo_t* grafo);

// Agrega un vertice al grafo.
// Pre: El grafo fue creado.
// Post: Se le agrego un vertice al grafo.
bool grafo_agregar_vertice(grafo_t* grafo,char* idvertice,int posicionx,int posiciony,int latitudx,int latitudy,void* dato);

// Elimina un vertice del grafo.
// Pre: El grafo fue creado.
// Post: Se le quito,si estaba, el vertice al grafo.
void* grafo_destruir_vertice(grafo_t* grafo,char* idvertice);

// Agrega una arista al grafo.
// Pre: El grafo fue creado.
// Post: Se le agrego una arista al grafo.
bool grafo_agregar_arista(grafo_t* grafo,char* idarista,char* idvertice1,char* idvertice2);

// Elimina una arista del grafo.
// Pre: El grafo fue creado.
// Post: Se le quito, si estaba, la arista al grafo.
bool grafo_destruir_arista(grafo_t* grafo,char* idarista);

/* Primitivas de utilizacion */

// Verifica si existe un vertice del grafo.
// Pre: El grafo fue creado.
// Post: Devuelve true si el vertice existe.
bool grafo_existe_vertice(grafo_t* grafo,char* idvertice);

// Recupera el dato de un vertice del grafo.
// Pre: El grafo fue creado.
// Post: Devuelve el dato del vertice si este existe.
void* grafo_dato_vertice(grafo_t* grafo,char* idvertice);

// Verifica si existe una arista del grafo.
// Pre: El grafo fue creado.
// Post: Devuelve true si la arista existe.
bool grafo_existe_arista(grafo_t* grafo,char* idarista);

// Recupera el peso de uns arista del grafo.
// Pre: El grafo fue creado.
// Post: Devuelve el peso de la arista si esta existe.
int grafo_peso_arista(grafo_t* grafo,char* idvertice);

// Devuelve el camino optimo en una lista con su peso como el primer elemento y luego los idvertices.
// Pre: El grafo fue creado
// Post: Se devuelve la lista
lista_t* camino_dijkstrak(grafo_t* grafo,char* origen,char* destino);

#endif // GRAFO_H
