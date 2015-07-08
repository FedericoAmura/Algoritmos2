#ifndef POLONORTE_H
#define POLONORTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct base base_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA BASE
 * *****************************************************************/

 /* Primitivas basicas */

// Crea una base de datos.
// Post: devuelve una nueva base vacía.
base_t *base_crear();

// Destruye una base de datos
// Pre: la base fue creada
// Post: se destruyo la base
void base_destruir(base_t* base);

// Carga la capacidad y la ubicacion del polo norte en la base de datos
// Pre: la base fue creada
// Post: se agregaron los datos
void cargar_capacidadyorigen(base_t* base,int capacidad,int polonorte);

// Destruye una base de datos
// Pre: la base fue creada
// Post: se destruyo la base
int agregar_fabrica(base_t* base,char* idfabrica,char* idesquina,char* horaentrada,char* horasalida);

// Destruye una base de datos
// Pre: la base fue creada
// Post: se destruyo la base
int agregar_juguete(base_t* base,char* idfabrica,char* idjuguete,char* valor,char* peso);

// Genera una lista de las fabricas ideales a visitar y las imprime si se lo pide
// Pre: la base fue creada
// Post: se creo la lista en la base y se la imprime si imprimir==true
int listar_fabricas(base_t* base,bool imprimir);

// Valua los juguetes de una fabrica
// Pre: la base fue creada
// Post: devuelve la cantidad de sonrisas acordes a los juguetes
int valuar_juguetes(base_t* base,char* idfabrica);

// Valua los juguetes de todas las fabricas a visitar
// Pre: la base fue creada
// Post: devuelve la cantidad de sonrisas acordes a todos los juguetes
int valuar_juguetes_total(base_t* base);

// Destruye una base de datos
// Pre: la base fue creada
// Post: se destruyo la base
int camino_optimo(base_t* base,char* idfabrica);

#endif // POLONORTE_H
