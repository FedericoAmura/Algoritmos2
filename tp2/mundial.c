#ifndef MUNDIAL_C
#define MUNDIAL_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "mundial.h"
#include "hash.h"
#include "heap.h"
#include "cola.h"
#define LNOMBRE 150
#define LEQUIPO 100
#define LFILTRO 10

typedef struct jugador{
    char* nombre;
    int dorsal;
    int goles;
    char* equipo;
}jugador_t;

typedef struct equipo{
    char* nombre;
    size_t jugadoresanotados;
    jugador_t** jugadoresdorsal;
    jugador_t** jugadoresnombre;
}equipo_t;

typedef struct partido{
    equipo_t* local;
    equipo_t* visitante;
    int goleslocal;
    int golesvisitante;
}partido_t;

struct base{
    hash_t* equipos;
    hash_t* jugadores;
    heap_t* goleadores;
    partido_t** partidos;
    int equiposanotados;
};

/* ******************************************************************
 *                    PRIMITIVAS DE LA BASE
 * *****************************************************************/

// Funcion auxiliar para ordenar los jugadores por dorsal
int comparar_dorsales(const jugador_t* jugador1, const jugador_t* jugador2){
    return (jugador1->dorsal-jugador2->dorsal);
}

// Funcion auxiliar para ordenar los jugadores por nombre
int comparar_nombres(const jugador_t* jugador1, const jugador_t* jugador2){
    return strcmp(jugador1->nombre,jugador2->nombre);
}

// Funcion auxiliar que imprime el vector de jugadores
void listar_vector_jugadores(jugador_t** vectorjugadores){
    for(int i = 0;i<23;i++) printf("%s,%d: Goles: %d\n",vectorjugadores[i]->nombre,vectorjugadores[i]->dorsal,vectorjugadores[i]->goles);
}

// Funcion auxiliar que crea un nuevo jugador
jugador_t* crear_jugador(char* nombre, int dorsal, char* equipo){
    jugador_t* jugador = malloc(sizeof(jugador_t));
    if (jugador == NULL) return NULL;
    jugador->nombre = malloc(LNOMBRE*sizeof(char));
    strcpy(jugador->nombre,nombre);
    jugador->dorsal = dorsal;
    jugador->goles = 0;
    jugador->equipo = malloc(LEQUIPO*sizeof(char));
    strcpy(jugador->equipo,equipo);
    return jugador;
}

// Funcion auxiliar que crea un nuevo equipo
equipo_t* crear_equipo(char* nombre){
    equipo_t* equipo = malloc(sizeof(equipo_t));
    if (equipo == NULL) return NULL;
    equipo->nombre = malloc(LEQUIPO*sizeof(char));
    strcpy(equipo->nombre,nombre);
    equipo->jugadoresanotados = 0;
    equipo->jugadoresdorsal = malloc(23*sizeof(jugador_t*));
    if (equipo->jugadoresdorsal == NULL){
        free(equipo->nombre);
        free(equipo);
        return NULL;
    }
    equipo->jugadoresnombre = malloc(23*sizeof(jugador_t*));
    if (equipo->jugadoresnombre == NULL){
        free(equipo->nombre);
        free(equipo->jugadoresdorsal);
        free(equipo);
        return NULL;
    }
    return equipo;
}

// Funcion auxiliar que destruye un jugador
void destruir_jugador(jugador_t* jugador){
    free(jugador->nombre);
    free(jugador->equipo);
    free(jugador);
}

// Funcion auxiliar que destruye un equipo
void destruir_equipo(equipo_t* equipo){
    free(equipo->nombre);
    free(equipo->jugadoresdorsal);
    free(equipo->jugadoresnombre);
    free(equipo);
}

// Funciion auxiliar que comparar dos jugadores segun sus goles
int comparar_goleador(const jugador_t* jugador1, const jugador_t* jugador2){
    return ((jugador1->goles)-(jugador2->goles));
}

/* Implementaciones de primitivas */

base_t *base_crear(){
    base_t* base = malloc(sizeof(base_t));
    if (base==NULL) return NULL;
    base->equiposanotados=0;
    base->jugadores = hash_crear((hash_destruir_dato_t)&destruir_jugador);
    base->equipos = hash_crear((hash_destruir_dato_t)&destruir_equipo);
    base->goleadores = heap_crear((cmp_func_t)&comparar_goleador);
    base->partidos = malloc(15*sizeof(partido_t*));
    //verifico que todo se haya creado correctamente
    if ((base->jugadores==NULL) || (base->equipos==NULL) || (base->goleadores==NULL) || (base->partidos==NULL)){
        //alguna cosa se creo mal
        if (base->jugadores!=NULL) hash_destruir(base->jugadores);
        if (base->equipos!=NULL) hash_destruir(base->equipos);
        if (base->goleadores!=NULL) heap_destruir(base->goleadores,NULL);
        if (base->partidos!=NULL) free(base->partidos);
        free(base);
        return NULL;
    }
    for(int i=0;i<15;i++) base->partidos[i] = calloc(1,sizeof(partido_t)); //falta verificar, tengamos fe
    return base;
}

void base_destruir(base_t* base){
    heap_destruir(base->goleadores,NULL);
    hash_destruir(base->jugadores);
    hash_destruir(base->equipos);
    for(int i=0;i<15;i++) free(base->partidos[i]);
    free(base->partidos);
    free(base);
    return;
}

int agregar_equipo(base_t* base,char* pais){
    equipo_t* equipo = crear_equipo(pais);
    if (equipo==NULL) return -1; //no se pudo crear el equipo
    if (!hash_guardar(base->equipos,pais,equipo)){
        destruir_equipo(equipo);
        return -2; //no se pudo guardar el equipo
    }
    int resto = (base->equiposanotados%2);
    int posicion = 7 + (base->equiposanotados)/2;
    //printf("posicion: %d,resto:%d\n",posicion,resto);
    if (resto==0){
        base->partidos[posicion]->local = equipo;
    }else{
        base->partidos[posicion]->visitante = equipo;
    }
    base->equiposanotados++;
    return 0; //todo bien
}

int agregar_jugador(base_t* base,char* equipo,char* nombre,int dorsal){
    jugador_t* jugador = crear_jugador(nombre,dorsal,equipo);
    if (jugador==NULL) return -1; //no se pudo crear el jugador
    //veo si esta anotado el pais
    if (!hash_pertenece(base->equipos,equipo)){
        destruir_jugador(jugador);
        return -2; //el pais no esta anotado
    }
    equipo_t* pais = hash_obtener(base->equipos,equipo);
    //agrego el jugador al hash de jugadores y verifico
    if (!hash_guardar(base->jugadores,nombre,jugador)){
        destruir_jugador(jugador);
        return -3; //no se pudo guardar el jugador en el hash de jugadores
    }
    //agrego el jugador al pais en los vectores y si esta completo ordeno
    pais->jugadoresdorsal[pais->jugadoresanotados] = jugador;
    pais->jugadoresnombre[pais->jugadoresanotados] = jugador;
    pais->jugadoresanotados++;
    if (pais->jugadoresanotados==23){
        heap_sort((void*)pais->jugadoresdorsal,23,(cmp_func_t)comparar_dorsales);
        heap_sort((void*)pais->jugadoresnombre,23,(cmp_func_t)comparar_nombres);
    }
    //agrego el jugador al heap de goleadores
    if (!heap_encolar(base->goleadores,jugador)){
        hash_borrar(base->jugadores,nombre);
        destruir_jugador(jugador);
        return -5; //no se pudo agregar el jugador al heap de goleadores
    }
    return 0;
}

int jugador_metio_gol(base_t* base,char* pais,int dorsal){
    if (dorsal<1 || dorsal>23) return -2; //el jugador tiene un dorsal invalido
    if (!hash_pertenece(base->equipos,pais)) return -1; //el pais no esta en el hash
    equipo_t* equipo = hash_obtener(base->equipos,pais);
    if (equipo->jugadoresanotados<dorsal) return -2; //el jugador tiene un dorsal invalido
    equipo->jugadoresdorsal[dorsal-1]->goles++;
    return 0;
}

//int listar_jugadores(base_t* base,char* parametros){
int listar_jugadores(base_t* base,char* filtro,char* pais){
    if (!hash_pertenece(base->equipos,pais)) return -1;
    equipo_t* equipo = hash_obtener(base->equipos,pais);
    if (strcmp(filtro,"dorsal")==0) listar_vector_jugadores(equipo->jugadoresdorsal);
    if (strcmp(filtro,"nombre")==0) listar_vector_jugadores(equipo->jugadoresnombre);
    return 0;
}

int goles_jugador(base_t* base,char* nombre){
    if (!hash_pertenece(base->jugadores,nombre)) return -1; //el jugador no esta en el hash
    jugador_t* jugador = hash_obtener(base->jugadores,nombre);
    fprintf(stdout,"%s,%d: %s Goles: %d\n",jugador->nombre,jugador->dorsal,jugador->equipo,jugador->goles);
    return 0;
}

/* ******************************************************************
 *                    PRIMITIVAS DEL FIXTURE
 * *****************************************************************/

// Funcion auxiliar que devuelve la posicion del partido segun su clave
int posicion_partido(char* idr){
    if (strlen(idr)>2) return -1; // caso en el que idr tenga mas de dos caracteres
    // notar que en las pruebas no hay partidos como 16a, por ejemplo
    return ((idr[0]-'1')+(idr[1]-'a'));
}


int agregar_resultado(base_t* base, char* idr, char* gloc, char* gvis, cola_t* cola) {
    int goleslocal;
    int golesvisitante;
    int posicion;
    //consigo la posicion del partido
    posicion = posicion_partido(idr);

    if (posicion == -1) return -3; // si el idr tiene mas de dos caracteres

    //verifico si no esta cargado ya (no puede haber partidos que hayan salido 0-0 y esa es la condicion inicial)
    if (((base->partidos[posicion]->goleslocal)!=0) || ((base->partidos[posicion]->golesvisitante)!=0)){
        return -1;
    }// ya esta cargado el resultado de ese partido

    //verifico que esten los equipos cargados correctamente
    if ((base->partidos[posicion]->local==NULL) || (base->partidos[posicion]->visitante==NULL)){
        return -2;
    }// faltan resultados previos, no se sabe que equipos corresponden

    //consigo el puntero hacia el partido para modificarlo
    partido_t* partido = base->partidos[posicion];
    goleslocal = atoi(gloc);
    partido->goleslocal = goleslocal;
    golesvisitante = atoi(gvis);
    partido->golesvisitante = golesvisitante;

    //actualizo los goles del local en la base de datos
    char* dorsal;
    int numdorsal;
    int m = 0;
    while (!cola_esta_vacia(cola) && (m<goleslocal)) {
        dorsal = (char*)cola_desencolar(cola);
        numdorsal = atoi(dorsal);
        jugador_metio_gol(base,partido->local->nombre,numdorsal);
        free(dorsal);
        m++;
    }
    int n = 0;
    while (!cola_esta_vacia(cola) && (n<golesvisitante)) {
        dorsal = cola_desencolar(cola);
        numdorsal = atoi(dorsal);
        jugador_metio_gol(base,partido->visitante->nombre,numdorsal);
        free(dorsal);
        n++;
    }
    heap_heapify(base->goleadores);
    int k,siguientepartido;
    if (posicion==0) return 0; // si estoy en la final no hay siguiente partido
    //como hay siguiente partido cargo el ganador segun corresponda
    equipo_t* ganador;
    if (goleslocal>golesvisitante) ganador=partido->local; else ganador=partido->visitante; //guardo el equipo ganador
    siguientepartido = (posicion-1)/2; //calculo la posicion del siguiente partido en la base

    k = posicion%2; //veo si viene de una posicion par o impar para saber si lo asigno como local o visitante
    partido = base->partidos[siguientepartido]; //muevo el puntero hacia el siguiente partido
    if (k==0) partido->visitante=ganador; else partido->local=ganador; //asigno el ganador segun corresponda en el siguiente partido

    return 0;
}

int listar_goleador(base_t* base){
    jugador_t* goleador = heap_ver_max(base->goleadores);
    printf("%s: %s Goles: %d\n",goleador->nombre,goleador->equipo,goleador->goles);
    return 0;
}

int mostrar_resultado(base_t* base,char* idr){
    int posicion = posicion_partido(idr);
    if (posicion == -1) return -3;
    partido_t* partido = base->partidos[posicion];
    //verifico que esten los equipos cargados correctamente
    if ((base->partidos[posicion]->local==NULL) || (base->partidos[posicion]->visitante==NULL))return -2;//faltan resultados previos, no se sabe que equipos corresponden
    //verifico si esta cargado el resultado (no puede haber partidos que hayan salido 0-0 y esa es la condicion inicial)
    if (((base->partidos[posicion]->goleslocal)==0) && ((base->partidos[posicion]->golesvisitante)==0)) return -1; //no esta cargado el resultado
    printf("resultado: %s:%d vs %s:%d\n",partido->local->nombre,partido->goleslocal,partido->visitante->nombre,partido->golesvisitante);
    return 0;
}


#endif // MUNDIAL_C
