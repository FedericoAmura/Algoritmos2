#ifndef COMANDOS_H
#define COMANDOS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "lista.h"
/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct sesion sesion_t;
typedef struct curso curso_t;
typedef struct base base_t;

// Inicializa la base de datos.
// Pre: ninguna.
// Post: Se inicializo la base de datos necesaria para todo el programa.
base_t* inicializar_base_de_datos();

// Se cierra adecuadamente la base de datos.
// Pre: La base de datos fue creada.
// Post: La base de datos fue cerrada adecuadamente.
void cerrar_base_de_datos(base_t* basedatos);

// Crea una sesion.
// Pre: Ninguna.
// Post: se creo una sesion
sesion_t* sesion_crear();

// Destruye una sesion
// Pre: la sesion fue creada.
// Post: se destruyo la sesion.
void sesion_destruir(sesion_t* sesion);

// Obtiene el padron de la sesion en curso.
// Pre: La sesion fue creada.
// Post: Devuelve el padron de la sesion en curso
char* obtener_padron(sesion_t* sesion);

/* ******************************************************************
 *                    COMANDOS DEL PROGRAMA
 * *****************************************************************/

 /* Primitivas de administracion */

// Agrega un nuevo curso a la base de datos.
// Pre: La base de datos fue creada.
// Post: Devuelve 0 si se ejecuto correctamente, -1 si el curso ya existia, -2 si no se pudo agregar.
int agregar_curso(char* idc,char* descripcion,char* materia,char* vacantes, base_t* basedatos);

// Inscribe un alumno en un curso.
// Pre: El curso se encuentra en la base de datos, la cual fue creada.
// Post: Devuelve 1 si el alumno quedo como regular, 2 si quedo como condicional y
// -1 si el alumno ya estaba inscripto
int inscribir(char* padron, char* idc, base_t* basedatos);

// Elimina un curso de la base de datos.
// Pre: La base de datos fue creada.
// Post: Devuelve 0 si se elimino el curso, -1 si el curso no existe.
int eliminar_curso(char* idc,base_t* basedatos);

// Elimina un alumno de un curso.
// Pre: La base de datos fue creada.
// Post: Devuelve 0 si desinscribio al alumno, -1 si el curso no existe y
// -2 si el alumno no estaba inscripto
int desinscribir(char* padron, char* idc, base_t* basedatos);

// Muestra un listado de los inscriptos al curso.
// Pre: La base de datos fue creada.
// Post: Devuelve 0 si no hubo problema, -1 si el curso no existe.
int listar_inscriptos(char* idc, base_t* basedatos);

/* Primitivas para alumnos */

// Muestra una lista de los cursos filtrando por materia de ser especificada.
// Pre: La base de datos fue creada.
// Post: Se listan todos los cursos o los de la materia especificada.
void listar_cursos(char* filtro,base_t* basedatos);

// Inicia una nueva sesion asociada a un padron
// Pre: La sesion fue creada.
// Post: Devuelve 0 si inicio la sesion exitosamente, -1 si ya habia una
// sesion abierta.
int sesion_iniciar(sesion_t* sesion,char* padron);

// Selecciona un curso para inscribirse al finalizar la sesion
// Pre: La base de datos y la sesion fueron creadas.
// Post: Se agrega el curso a la lista de inscripciones para aplicar luego.
// Devuelve 1 si se inscribio como regular, 2 si fue como condicional,
// -1 si ya estaba inscripto en el curso, -2 si no existe el curso, -3 si no
// hay sesion abierta, -4 si ya estaba pendiente la inscripcion, -5 si hubo un error.
int sesion_inscribir(char* idc,base_t* basedatos, sesion_t* sesion);

// Muestra el estado actual de la sesion y los cursos seleccionados en sesion_inscribir
// Pre: La sesion fue creada.
// Post: Muestra el estado actual de la sesion y los cursos a inscribirse de haberlos.
void sesion_ver(sesion_t* sesion);

// Des-selecciona el ultimo curso seleccionado con sesion_inscribir. Puede ser
// utilizado varias veces seguidas.
// Pre: La base de datos y la sesion fueron creadas.
// Post: Devuelve 0 si pudo quitar la ultima inscripcion, -1 si no hay sesion en
// curso, -2 si no hay acciones para deshacer.
int sesion_deshacer(sesion_t* sesion);

// Inscribe al alumno en los cursos seleccionados y cierra la sesion.
// Pre: La base de datos y la sesion fueron creadas.
// Post: Devuelve 0 si fue exitoso, -1 si no hay sesion iniciada.
int sesion_aplicar(sesion_t* sesion,base_t* basedatos);

#endif // COMANDOS_H
