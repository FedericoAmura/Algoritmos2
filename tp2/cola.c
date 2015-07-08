#include <stdlib.h>
#include <stdbool.h>
#include "cola.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo_cola {
    void *dato;
    struct nodo_cola *siguiente; // puntero al siguiente nodo
} nodo_cola_t ;

struct cola {
    nodo_cola_t *primero;
    nodo_cola_t *ultimo;
};

/* ******************************************************************
 *                        IMPLEMENTACION
 * *****************************************************************/

// Crea una cola.
// Post: devuelve una nueva cola vacia.
cola_t* cola_crear() {
    cola_t *cola = malloc(sizeof(cola_t));
    if (cola == NULL) return NULL;
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}


// Destruye la cola. Si se recibe la funcion destruir_dato por parametro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void destruir_dato(void*)) {
    while(!cola_esta_vacia(cola)) {
        if (destruir_dato != NULL) {
            destruir_dato(cola_desencolar(cola));
        } else {
            cola_desencolar(cola); //agrego esto en caso de que sea NULL la funcion de destruccion
        }
    }
    free(cola);
}

// Devuelve verdadero o falso, segun si la cola tiene o no elementos encolados.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola) {
    if ((cola->primero == NULL) && (cola->ultimo == NULL)) return true;
    return false;
}


//Funcion auxiliar que crea un nodo y lo devuelve
//asignandole el dato que le pasan
//El nodo creado no sabe cual es su siguiente por lo tanto se asigna NULL
nodo_cola_t* crear_nod(void* valor) {
    nodo_cola_t* nodo_nuevo = malloc(sizeof(nodo_cola_t));
    if (nodo_nuevo == NULL) return NULL;

    nodo_nuevo->dato = valor; //asigno directamente el dato
    nodo_nuevo->siguiente = NULL;
    return nodo_nuevo;
} //Barbara se encarga de pedir memoria para los datos


// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agrego un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void* valor) {
    nodo_cola_t* nodo_nuevo = crear_nod(valor);
    if (nodo_nuevo == NULL) return false;

    if (cola_esta_vacia(cola)) {
        cola->primero = nodo_nuevo;
        cola->ultimo = nodo_nuevo;
    } else {
        cola->ultimo->siguiente = nodo_nuevo;
        cola->ultimo = nodo_nuevo;
    }
    return true;
}


// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si esta vacia devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvio el primer elemento de la cola, cuando no esta vacia.
void* cola_ver_primero(const cola_t *cola) {
    if (cola_esta_vacia(cola)) return NULL;
    return cola->primero->dato;
}


// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si esta vacia, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvio el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacia.
void* cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) return NULL;

    void* dato_desencolado;
    dato_desencolado = cola_ver_primero(cola);

    nodo_cola_t* nodo_auxiliar;
    nodo_auxiliar = cola->primero->siguiente;

    nodo_cola_t* nodo_primero_actual;
    nodo_primero_actual = cola->primero;
    free(nodo_primero_actual);

    cola->primero = nodo_auxiliar;
    //ultimo y primero tienen que quedar en NULL al final
    if (nodo_auxiliar == NULL) {
        cola->primero = NULL;
        cola->ultimo = NULL;
    }
    return dato_desencolado;
}
