#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "hash.h"
#include "heap.h"
#include "mundial.h"

#include "cola.h"

void procesar_comando(base_t* base, char* comando, char* parametro1, char* parametro2, char* parametro3, cola_t* cola) {
   if (strcmp(comando,"agregar_resultado") == 0) {
        int i = agregar_resultado(base,parametro1,parametro2,parametro3,cola);
        if (i == 0) printf("OK\n");
        if (i == -1) printf("Error: el resultado con id %s ya existe\n",parametro1);
        if ((i == -2) || (i == -3)) printf("Error: el resultado con id %s no existe\n",parametro1);
    }


    if (strcmp(comando,"listar_jugadores") == 0) {
        int i =	listar_jugadores(base,parametro1,parametro2);
        if (i == -1) printf("Error: el equipo %s no esta inscripto en el fixture\n",parametro2);
    }


    if (strcmp(comando,"listar_goleador") == 0) {
        listar_goleador(base);
    }

    if (strcmp(comando,"goles_jugador") == 0) {
        int i = goles_jugador(base,parametro1);
        if (i == -1) printf("Error: el jugador %s no esta inscripto en el fixture\n",parametro1);
    }

    if (strcmp(comando,"mostrar_resultado") == 0) {
        int i = mostrar_resultado(base,parametro1);
        if ((i == -1) || (i == -2) || (i == -3)) printf("Error: el resultado con id %s no existe\n",parametro1);
    }

}


void destruir_dato(void* dato) {
    if (dato != NULL)
        free(dato);
}

bool leer_consola(cola_t *cola)
{
    char c;
    char lectura[100];
    char limitador = ' ';
    bool condLimitador = true;
    char limitador2 = ',';
    int i;
    i = 0;

    while ((c = getchar()) != '\n' && c != EOF)
    {
        if ((c != limitador || !condLimitador) && c != limitador2) {
            lectura[i]=c;
            i++;
        }
        if ((c == limitador && condLimitador) || c == limitador2) {
            lectura[i] = '\0';//LE AGREGO EL CARACTER DE FIN DE STRING PARA ASEGURARME QUE NO META BASURA
            i = 0;
            if (strcmp("listar_jugadores", lectura) != 0)
                condLimitador = false;
            char* param = malloc(strlen(lectura)+1);
            strcpy(param,lectura);
            cola_encolar(cola,param);
        }
    }
    lectura[i] = '\0';
    char* param = malloc(strlen(lectura)+1);
    strcpy(param,lectura);
    cola_encolar(cola,param);
    if (c == EOF)
    {
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {


    base_t* base_de_datos = base_crear();

    cola_t* cola = cola_crear();
    bool seguir;

    char* param1 = NULL;
    char* param2 = NULL;
    char* param3 = NULL;
    char* param4 = NULL;

    if (argc!=2) return 0; //no se indico el archivo a abrir o estan mal pasados los argumentos
    FILE* file;
	char str[60];
	char str_jugadores[60];
    file = fopen(argv[1],"r");
    while ( fgets(str,60,file) != NULL ) {
        char* equipo = strtok(str,"\r\n");
	    agregar_equipo(base_de_datos,equipo);
 	    for (int i = 1; i < 24; i ++) {
		if ( fgets(str_jugadores,60,file) != NULL ) {
		    char* jugad = strtok(str_jugadores,"\r\n");
		    agregar_jugador(base_de_datos,equipo,jugad,i);
		}
	    }
        }
	fclose(file);


    do {
        seguir = leer_consola(cola);

        param1 = (char*)cola_desencolar(cola);
        param2 = (char*)cola_desencolar(cola);
        param3 = (char*)cola_desencolar(cola);
        param4 = (char*)cola_desencolar(cola);

        procesar_comando(base_de_datos,param1,param2,param3,param4,cola);

        destruir_dato(param1);
        destruir_dato(param2);
        destruir_dato(param3);
        destruir_dato(param4);
        while (!cola_esta_vacia(cola)) free(cola_desencolar(cola));
        fflush(stdin);

    }while (seguir);

    cola_destruir(cola,(void(*)(void*))destruir_dato);
    base_destruir(base_de_datos);
    return 0;

}




