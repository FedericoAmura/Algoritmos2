#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "polonorte.h"
#define MAXCOMANDO 30
#define MAXPARAMETRO 30

void procesar_comando(base_t* base, char* comando, char* parametro){
   int i = 0;
   if (strcmp(comando,"listar_fabricas") == 0) {
        i = listar_fabricas(base,true);
        return;
    }
    if (strcmp(comando,"valuar_juguetes") == 0) {
        i =	valuar_juguetes(base,parametro);
        printf("Total: %d Sonrisas\n",i);
        return;
    }
    if (strcmp(comando,"valuar_juguetes_total") == 0) {
        i =	valuar_juguetes_total(base);
        printf("Total: %d Sonrisas\n",i);
        return;
    }

    if (strcmp(comando,"camino_optimo") == 0) {
        i = camino_optimo(base,parametro);
        return;
    }
    //printf("Comando no encontrado,comando: %s\n",comando);
}


bool leer_consola(char* comando, char* parametro) {
    char c;
    int i = 0;
    if  (feof(stdin)!=0) return false;
    while (((c=getchar())!=' ') && (c!='\n') && (c!=EOF)){
        comando[i] = c;
        i++;
    }
    comando[i] = '\0';
    //estas lineas me parece que no son necesarias
    //if (c==EOF) return false; //llegue al final del archivo
    //if (c=='\n') return true; //llegue al final de linea pero no del archivo
    i = 0;
    while (((c=getchar())!='\n') && (c!=EOF)){
        parametro[i] = c;
        i++;
    }
    parametro[i] = '\0';
    if (c==EOF) return false; //llegue al final del archivo
    //if (c=='\n') return true; //llegue al final de linea pero no del archivo
    return true;
}

int main(int argc, char *argv[]){
    bool seguir;
    int polonorte,capacidad;
    if (argc!=6) return 0; //esta mal ejecutado el programa
    //creo la base
    base_t* base = base_crear();
    //cargo los datos
    capacidad = atoi(argv[1]);
    polonorte = atoi(argv[2]);
    cargar_capacidadyorigen(base,capacidad,polonorte);
    //cargo archivo de fabricas
    FILE* file;
	char linea[40];
	char primero[10];
	char segundo[10];
	char tercero[10];
	char cuarto[10];
	int i,j;
	//abro el archivo
    file = fopen(argv[3],"r");
    //leo linea de cada fabrica y la agrego
    while ( fgets(linea,40,file) != NULL ) {
        //cargo idfabrica
        i = 0;
        while(linea[i]!=','){
            primero[i] = linea[i];
            i++;
        }
        primero[i] = '\0';
        //cargo idesquina
        i++; j = 0;
        while(linea[i]!=','){
            segundo[j] = linea[i];
            i++; j++;
        }
        segundo[j] = '\0';
        //cargo horaentrada
        i++; j = 0;
        while(linea[i]!=','){
            tercero[j] = linea[i];
            i++; j++;
        }
        tercero[j] = '\0';
        //cargo horasalida
        i++; j = 0;
        while(linea[i]!='\n'){
            cuarto[j] = linea[i];
            i++; j++;
        }
        cuarto[j] = '\0';
        agregar_fabrica(base,primero,segundo,tercero,cuarto);
    }
    //cierro el archivo
	fclose(file);

    //cargo archivo de juguetes
	//abro el archivo
    file = fopen(argv[4],"r");
    //leo linea de cada fabrica y la agrego
    while ( fgets(linea,40,file) != NULL ) {
        //cargo idfabrica
        i = 0;
        while(linea[i]!=','){
            primero[i] = linea[i];
            i++;
        }
        primero[i] = '\0';
        //cargo idjuguete
        i++; j = 0;
        while(linea[i]!=','){
            segundo[j] = linea[i];
            i++; j++;
        }
        segundo[j] = '\0';
        //cargo valor
        i++; j = 0;
        while(linea[i]!=','){
            tercero[j] = linea[i];
            i++; j++;
        }
        tercero[j] = '\0';
        //cargo peso
        i++; j = 0;
        while(linea[i]!='\n'){
            cuarto[j] = linea[i];
            i++; j++;
        }
        cuarto[j] = '\0';
        agregar_juguete(base,primero,segundo,tercero,cuarto);
    }
    //cierro el archivo
	fclose(file);

    //armo cadena para comando y parametro
    char* comando = malloc(MAXCOMANDO*sizeof(char));
    char* parametro = malloc(MAXPARAMETRO*sizeof(char));
    //leo, proceso y avanzo comandos
    do {
        seguir = leer_consola(comando,parametro);
        procesar_comando(base,comando,parametro);
        fflush(stdin);
    }while (seguir);
    free(comando);
    free(parametro);
    base_destruir(base);
    return 0;
}
