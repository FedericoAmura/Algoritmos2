#ifndef COMANDOS_C
#define COMANDOS_C
#include "lista.h"
#include "comandos.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#define LIDC 11
#define LDESCRIPCION 81
#define LMATERIA 11
#define LVACANTES 4
#define LPADRON 6

typedef struct inscripcion{
    char* curso;
    bool estado;
}inscripcion_t;

struct sesion{
    char* padron;
    lista_t* inscripciones;
};

struct curso{
    char* idc;
    char* descripcion;
    char* materia;
    int vacantes;
    lista_t* inscriptos;
};

struct base{
    lista_t* listadecursos;
};

typedef bool(*funcioniterador)(void*, void*);
typedef void (*destruccion_dato)(void*);

void destruir_inscripto(char* padron){
    free(padron);
}

void destruir_inscripcion(inscripcion_t* inscripcion){
    free(inscripcion->curso);
    free(inscripcion);
}

curso_t* crear_curso(char* idc,char* descripcion,char* materia,char* vacantes){
    curso_t* nuevocurso = calloc(1,sizeof(curso_t));
    if (nuevocurso==NULL) return NULL;
    nuevocurso->inscriptos = lista_crear();
    if (nuevocurso->inscriptos==NULL){
        free(nuevocurso);
        return NULL;
    }
    nuevocurso->idc = malloc(LIDC*sizeof(char));
    nuevocurso->descripcion = malloc(LDESCRIPCION*sizeof(char));
    nuevocurso->materia = malloc(LMATERIA*sizeof(char));
    if ((nuevocurso->idc==NULL) || (nuevocurso->descripcion==NULL) || (nuevocurso->materia==NULL)){
        if (nuevocurso->idc==NULL) free(nuevocurso->idc);
        if (nuevocurso->descripcion==NULL) free(nuevocurso->descripcion);
        if (nuevocurso->materia==NULL) free(nuevocurso->materia);
        free(nuevocurso);
    }
    strcpy(nuevocurso->idc,idc);
    strcpy(nuevocurso->descripcion,descripcion);
    strcpy(nuevocurso->materia,materia);
    nuevocurso->vacantes = atoi(vacantes);
    return nuevocurso;
}

void destruir_curso(curso_t* curso){
    free(curso->idc);
    free(curso->descripcion);
    free(curso->materia);
    lista_destruir(curso->inscriptos,(destruccion_dato)&destruir_inscripto);
    free(curso);
}

base_t* inicializar_base_de_datos(){
    base_t* basededatos = malloc(sizeof(base_t));
    if (basededatos==NULL) return NULL;
    basededatos->listadecursos = lista_crear();
    if (basededatos->listadecursos==NULL){
        free(basededatos);
        return NULL;
    }
    return basededatos;
}

void cerrar_base_de_datos(base_t* basededatos){
    lista_destruir(basededatos->listadecursos,(destruccion_dato)&destruir_curso);
    free(basededatos);
}

int existe_curso(char* idc,lista_t* lista){
    int vuelta = -1;
    int i = 0;
    lista_iter_t* iterador = lista_iter_crear(lista);
    curso_t* curso;
    do{
        curso = (curso_t*) lista_iter_ver_actual(iterador);
        if (curso!=NULL){
            if (strcmp(idc,curso->idc)==0) vuelta=i;
        }
        i++;
    }while(lista_iter_avanzar(iterador) && (vuelta==-1));
    lista_iter_destruir(iterador);
    return vuelta;
}

int alumno_inscripto(char* padron,lista_t* lista){
    int i = 0;
    int estainscripto = -1;
    char* inscripto;
    lista_iter_t* iterador = lista_iter_crear(lista);
    while((!lista_iter_al_final(iterador)) && (estainscripto==-1)){
        inscripto = (char*) lista_iter_ver_actual(iterador);
        if (strcmp(inscripto,padron)==0) estainscripto=i;
        lista_iter_avanzar(iterador);
        i++;
    }
    lista_iter_destruir(iterador);
    return estainscripto;
}

curso_t* conseguir_curso(char* idc, lista_t* lista){
    int i;
    int posicion = existe_curso(idc,lista);
    lista_iter_t* iterador = lista_iter_crear(lista);
    for(i=0;i<posicion;i++) lista_iter_avanzar(iterador);
    curso_t* curso = NULL;
    curso = lista_iter_ver_actual(iterador);
    lista_iter_destruir(iterador);
    return curso;
}

bool imprimir_inscripto(char* padron, int* estado){
    fprintf(stdout,"%s ",padron);
    (*estado>0) ? fprintf(stdout,"regular\n") : fprintf(stdout,"condicional\n");
    (*estado)--;
    return true;
}

int agregar_curso(char* idc,char* descripcion,char* materia,char* vacantes, base_t* basedatos){
    if (existe_curso(idc,basedatos->listadecursos)!=-1) return -1;
    curso_t* curso = crear_curso(idc,descripcion,materia,vacantes);
    lista_insertar_ultimo(basedatos->listadecursos,curso);
    return 0;
}

int inscribir(char* padron, char* idc, base_t* basedatos){
    int pos = existe_curso(idc,basedatos->listadecursos);
    if (pos==-1) return -2;
    char* anotar = malloc(LPADRON*sizeof(char));
    strcpy(anotar,padron);
    curso_t* curso = conseguir_curso(idc,basedatos->listadecursos);
    if (alumno_inscripto(anotar,curso->inscriptos)!=-1){
        free(anotar);
        return -1;
    }
    lista_insertar_ultimo(curso->inscriptos,anotar);
    if (curso->vacantes<lista_largo(curso->inscriptos)) return 2;
    return 1;
}

int eliminar_curso(char* idc,base_t* basedatos){
    int i;
    int pos = existe_curso(idc,basedatos->listadecursos);
    if (pos==-1) return -1;
    lista_iter_t* iterador = lista_iter_crear(basedatos->listadecursos);
    for(i=0;i<pos;i++) lista_iter_avanzar(iterador);
    destruir_curso((curso_t*)lista_borrar(basedatos->listadecursos,iterador));
    lista_iter_destruir(iterador);
    return 0;
}

int desinscribir(char* padron, char* idc, base_t* basedatos){
    int i,posicion;
    curso_t* curso = conseguir_curso(idc,basedatos->listadecursos);
    if (curso==NULL) return -1;
    posicion = alumno_inscripto(padron,curso->inscriptos);
    if (posicion==-1) return -2;
    lista_iter_t* iterador = lista_iter_crear(curso->inscriptos);
    for(i=0;i<posicion;i++) lista_iter_avanzar(iterador);
    destruir_inscripto((char*)lista_borrar(curso->inscriptos,iterador));
    lista_iter_destruir(iterador);
    return 0;
}

int listar_inscriptos(char* idc, base_t* basedatos){
    int pos = existe_curso(idc,basedatos->listadecursos);
    if (pos==-1) return -1;
    curso_t* curso = conseguir_curso(idc,basedatos->listadecursos);
    int regularocondicional = curso->vacantes;
    lista_iterar(curso->inscriptos,(funcioniterador)&imprimir_inscripto,(void*)&regularocondicional);
    return 0;
}

void listar_cursos(char* filtro,base_t* basedatos){
    lista_iter_t* iterador = lista_iter_crear(basedatos->listadecursos);
    curso_t* curso;
    int inscriptos;
    while(!lista_iter_al_final(iterador)){
        curso = lista_iter_ver_actual(iterador);
        inscriptos = lista_largo(curso->inscriptos);
        if (filtro[0]=='\0'){
            fprintf(stdout,"%s: %s (%s) Vacantes: %d Inscriptos: %d\n",curso->idc,curso->descripcion,curso->materia,curso->vacantes,inscriptos);
        }else{
            if (strcmp(filtro,curso->materia)==0) fprintf(stdout,"%s: %s (%s) Vacantes: %d Inscriptos: %d\n",curso->idc,curso->descripcion,curso->materia,curso->vacantes,inscriptos);
        }
        lista_iter_avanzar(iterador);
    }
    lista_iter_destruir(iterador);
}

sesion_t* sesion_crear(){
    sesion_t* sesion = calloc(1,sizeof(sesion_t));
    if (sesion==NULL) return NULL;
    sesion->padron = calloc(LPADRON+1,sizeof(char));
    sesion->inscripciones = lista_crear();
    if (sesion->inscripciones== NULL || sesion->padron==NULL){
        if (sesion->inscripciones!=NULL) free(sesion->inscripciones);
        if (sesion->padron!=NULL) free(sesion->padron);
        free(sesion);
        return NULL;
    }
    strcpy(sesion->padron,"00000");
    return sesion;
}

void sesion_destruir(sesion_t* sesion){
    lista_destruir(sesion->inscripciones,(destruccion_dato)&destruir_inscripcion);
    free(sesion->padron);
    free(sesion);
}

int sesion_iniciar(sesion_t* sesion,char* padron){
    if (strcmp(sesion->padron,"00000")!=0) return -1;
    strcpy(sesion->padron,padron);
    return 0;
}

char* obtener_padron(sesion_t* sesion){
    char* padron = malloc(LPADRON*sizeof(char));
    strcpy(padron,sesion->padron);
    return padron;
}

int sesion_inscribir(char* idc,base_t* basedatos, sesion_t* sesion){
    //verifico si hay sesion iniciada
    if (strcmp(sesion->padron,"00000")==0) return -3;
    //verifico si existe el curso
    int pos = existe_curso(idc,basedatos->listadecursos);
    if (pos==-1) return -2;
    //verifico si ya estaba programada la inscripcion
    bool yaanotado = false;
    inscripcion_t* inscripcion;
    lista_iter_t* iterador = lista_iter_crear(sesion->inscripciones);
    while(!lista_iter_al_final(iterador) && !yaanotado){
        inscripcion = (inscripcion_t*)lista_iter_ver_actual(iterador);
        if (strcmp(inscripcion->curso,idc)==0) yaanotado=true;
        lista_iter_avanzar(iterador);
    }
    lista_iter_destruir(iterador);
    if (yaanotado==true) return -4;
    //consigo el curso
    int i;
    iterador = lista_iter_crear(basedatos->listadecursos);
    for(i=0;i<pos;i++) lista_iter_avanzar(iterador);
    curso_t* curso = (curso_t*) lista_iter_ver_actual(iterador);
    lista_iter_destruir(iterador);
    //verifico si ya esta anotado en el curso
    if (alumno_inscripto(sesion->padron,curso->inscriptos)!=-1) return -1;
    //creo la inscripcion
    inscripcion = malloc(sizeof(inscripcion_t));
    if (inscripcion==NULL) return -5;
    //programo la inscripcion
    inscripcion->curso = malloc(LIDC*sizeof(char));
    strcpy(inscripcion->curso,idc);
    ((curso->vacantes)>lista_largo(curso->inscriptos)) ? (inscripcion->estado=false) : (inscripcion->estado=true);
    lista_insertar_ultimo(sesion->inscripciones,inscripcion);
    if (inscripcion->estado) return 1;
    return 2;
}

void sesion_ver(sesion_t* sesion){
    if (strcmp(sesion->padron,"00000")==0){
        fprintf(stdout,"Error: no hay una sesion en curso\n");
        return;
    }
    fprintf(stdout,"Padron: %s\n",sesion->padron);
    if (lista_largo(sesion->inscripciones)==0){
        fprintf(stdout,"No hay inscripciones\n");
        return;
    }
    lista_iter_t* iterador = lista_iter_crear(sesion->inscripciones);
    inscripcion_t* inscripcion;
    while(!lista_iter_al_final(iterador)){
        inscripcion = (inscripcion_t*)lista_iter_ver_actual(iterador);
        fprintf(stdout,"%s ",inscripcion->curso);
        (inscripcion->estado) ? fprintf(stdout,"(condicional)\n") : fprintf(stdout,"(regular)\n");
        lista_iter_avanzar(iterador);
    }
    lista_iter_destruir(iterador);
}

int sesion_deshacer(sesion_t* sesion){
    if (strcmp(sesion->padron,"00000")==0) return -1;
    if (lista_largo(sesion->inscripciones)==0) return -2;
    lista_iter_t* iteradordeprueba = lista_iter_crear(sesion->inscripciones);
    lista_iter_t* iteradorreal = lista_iter_crear(sesion->inscripciones);
    while(!lista_iter_al_final(iteradordeprueba)){
        lista_iter_avanzar(iteradordeprueba);
        if (!lista_iter_al_final(iteradordeprueba)) lista_iter_avanzar(iteradorreal);
    }
    destruir_inscripcion((inscripcion_t*)lista_borrar(sesion->inscripciones,iteradorreal));
    //destruir_inscripcion((inscripcion_t*)lista_borrar_primero(sesion->inscripciones));
    lista_iter_destruir(iteradordeprueba);
    lista_iter_destruir(iteradorreal);
    return 0;
}

int sesion_aplicar(sesion_t* sesion,base_t* basedatos){
    inscripcion_t* anotado;
    if (strcmp(sesion->padron,"00000")==0) return -1;
    while(!lista_esta_vacia(sesion->inscripciones)){
        anotado = (inscripcion_t*)lista_borrar_primero(sesion->inscripciones);
        inscribir(sesion->padron,anotado->curso,basedatos);
        destruir_inscripcion(anotado);
    }
    strcpy(sesion->padron,"00000");
    return 0;
}

#endif // COMANDOS_C
