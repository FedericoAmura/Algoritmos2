#ifndef MUNDIAL_H
#define MUNDIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "cola.h"

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

// Agrega un equipo a la base
// Pre: la base fue creada
// Post: se agrego el equipo a la base
int agregar_equipo(base_t* base,char* equipo);

// Agrega un jugador al equipo
// Pre: la base fue creada
// Post: se agrego el jugador al equipo
int agregar_jugador(base_t* base,char* equipo,char* jugador,int dorsal);

// Le agrega un gol al contador de goles del jugador
// Pre: la base fue creada
// Post: se incremento el contador de goles
int jugador_metio_gol(base_t* base,char* equipo,int dorsal);

// Imprime una lista de los jugadores de un equipo de la base
// Pre: la base fue creada
// Post: se imprime la lista
//int listar_jugadores(base_t* base,char* parametros);
int listar_jugadores(base_t* base,char* filtro,char* equipo);

// Imprime la informacion de un jugador
// Pre: la base fue creada
// Post: se imprime la informacion
int goles_jugador(base_t* base,char* jugador);

/* ******************************************************************
 *                    PRIMITIVAS DEL FIXTURE
 * *****************************************************************/

// Agrega un resultado a la base de datos de los partidos
// Pre: la informacion recibida es valida y con el formato correspondiente
// Post: se agrego el resultado
//int agregar_resultado(base_t* base,char* informacion);
int agregar_resultado(base_t* base, char* idr, char* gloc, char* gvis, cola_t* dorsales);

// Agrega un resultado a la base de datos de los partidos
// Pre: la informacion recibida es valida y con el formato correspondiente
// Post: se agrego el resultado
int listar_goleador(base_t* base);

// Agrega un resultado a la base de datos de los partidos
// Pre: la informacion recibida es valida y con el formato correspondiente
// Post: se agrego el resultado
int mostrar_resultado(base_t* base,char* informacion);

#endif // MUNDIAL_H
