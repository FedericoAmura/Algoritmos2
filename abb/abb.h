#ifndef ABB_H
#define ABB_H
#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* Se trata de un arbol binario de busqueda que almacena datos con una clave para organizarlos. */

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;
typedef void (*abb_destruir_dato_t)(void *);
typedef int (*abb_comparar_clave_t) (const char *, const char *);

/* ******************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

/* Primitivas basicas */

// Crea un abb.
// Post: Devuelve un nuevo abb vacio.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Devuelve verdadero o falso, segun si el abb contiene la clave.
// Pre: El abb fue creado.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Agrega un dato y su clave asociada al abb.
// Pre: El abb fue creado.
// Post: El arbol es un elemento mas grande.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Borra el elemento asociado a la clave especificada.
// Pre: El abb fue creado.
// Post: Se devuelve el elemento de la clave asociada y
// se destruye su almacenamiento en el abb, ademas el abb es un elemento
// mas chico.
void *abb_borrar(abb_t *arbol, const char *clave);

// Obtiene el elemento del abb con la clave asociada.
// Pre: El abb fue creado.
// Post: Devuelve el elemento de la clave asociada.
void *abb_obtener(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de elementos del abb.
// Pre: El abb fue creado.
// Post: Devuelve la cantidad de elementos en el abb.
size_t abb_cantidad(abb_t *arbol);

// Destruye el abb.
// Pre: El abb fue creado.
// Post: Se destruyo el abb y todos sus datos almacenados.
void abb_destruir(abb_t *arbol);

/* Primitivas de iterador externo */

// Itera todo el abb in order efectuando visitar a cada elemento.
// Pre: El abb fue creado.
// Post: Se ejecuto visitar para cada elemento del abb junto con extra
// de manera in order.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* Primitivas de iterador externo */

// Crea un iterador asociado al abb.
// Pre: El abb fue creado.
// Post: Devuelve un iterador apuntando al primer elemento del abb.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Mueve el iterador hacia el siguiente elemento.
// Pre: El abb y el iterador fueron creados.
// Post: El iterador ahora apunta al siguiente elemento.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve la clave del elemento al que esta apuntando el iterador.
// Pre: El abb y el iterador fueron creados.
// Post: Se devuelve la clave apuntada.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Verifica si el iterador se encuentra al final del abb..
// Pre: El abb y el iterador fueron creados.
// Post: Devuelve verdadero si el iterador esta al final del abb.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador.
// Pre: El abb fue creado.
// Post: El iterador se destruyo.
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H
