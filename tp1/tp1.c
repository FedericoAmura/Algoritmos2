#define _GNU_SOURCE
#include "comandos.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#define LMAXCOMANDO 23
#define LPARAMETRO1 11
#define LPARAMETRO2 81
#define LPARAMETRO3 11
#define LPARAMETRO4 4
#define MAXPALABRA 81

typedef struct lectura{
    char* comando;
    char* parametro1;
    char* parametro2;
    char* parametro3;
    char* parametro4;
}lecturaformateada;

void iniciar_parser(lecturaformateada* leer){
    leer->comando = malloc(LMAXCOMANDO*sizeof(char));
    leer->parametro1 = malloc(LPARAMETRO1*sizeof(char));
    leer->parametro2 = malloc(LPARAMETRO2*sizeof(char));
    leer->parametro3 = malloc(LPARAMETRO3*sizeof(char));
    leer->parametro4 = malloc(LPARAMETRO4*sizeof(char));
}

void reiniciar_parser(lecturaformateada* leer){
    leer->comando[0] = '\0';
    leer->parametro1[0] = '\0';
    leer->parametro2[0] = '\0';
    leer->parametro3[0] = '\0';
    leer->parametro4[0] = '\0';
}

void finalizar_parser(lecturaformateada* leer){
    free(leer->comando);
    free(leer->parametro1);
    free(leer->parametro2);
    free(leer->parametro3);
    free(leer->parametro4);
}

bool conseguir_comando(lecturaformateada* leer){
    size_t bytesleidos = 0;
    size_t tamlinea = 10;
    int i,j,k;
    if ((leer->comando[0]=fgetc(stdin))==EOF) return false;
    char* linea = malloc(tamlinea*sizeof(char));
    bytesleidos = getline(&linea,&tamlinea,stdin);
    i = 0;
    while(linea[i]!=' ' && linea[i]!='\n'){
        leer->comando[i+1]=linea[i];
        i++;
        if (i>=bytesleidos) break;
    }
    leer->comando[i+1]='\0';
    if (i<bytesleidos){
        k=0;
        while(linea[i]!='\n'){
            i++;
            j = 0;
            while(linea[i]!=',' && linea[i]!='\n'){
                if (k==0) leer->parametro1[j]=linea[i];
                if (k==1) leer->parametro2[j]=linea[i];
                if (k==2) leer->parametro3[j]=linea[i];
                if (k==3) leer->parametro4[j]=linea[i];
                i++;j++;
                if (i>=bytesleidos) break;
            }
            if (k==0) leer->parametro1[j]='\0';
            if (k==1) leer->parametro2[j]='\0';
            if (k==2) leer->parametro3[j]='\0';
            if (k==3) leer->parametro4[j]='\0';
            k++;
            if (i>=bytesleidos) break;
        }
    }
    free(linea);
    return true;
}

int main(void){
    int devolucion;
    char* padron;
    base_t* base = inicializar_base_de_datos();
    sesion_t* sesion = sesion_crear();
    lecturaformateada ingreso;
    iniciar_parser(&ingreso);
    reiniciar_parser(&ingreso);
    while (conseguir_comando(&ingreso)){
        //agregar_curso
        if (strcmp(ingreso.comando,"agregar_curso")==0){
            devolucion = agregar_curso(ingreso.parametro1,ingreso.parametro2,ingreso.parametro3,ingreso.parametro4,base);
            if (devolucion==0) fprintf(stdout,"OK\n");
            if (devolucion==-1) fprintf(stdout,"Error: el curso con id \"%s\" ya existe\n",ingreso.parametro1);
        }
        //listar_cursos y listar_cursos_materia
        if (strcmp(ingreso.comando,"listar_cursos")==0 || strcmp(ingreso.comando,"listar_cursos_materia")==0){
            listar_cursos(ingreso.parametro1,base);
        }
        //inscribir
        if (strcmp(ingreso.comando,"inscribir")==0){
            devolucion = inscribir(ingreso.parametro1,ingreso.parametro2,base);
            if (devolucion==1) fprintf(stdout,"OK: padron %s inscripto como regular\n",ingreso.parametro1);
            if (devolucion==2) fprintf(stdout,"OK: padron %s inscripto como condicional\n",ingreso.parametro1);
            if (devolucion==-1) fprintf(stdout,"Error: el padron %s ya esta inscripto en el curso \"%s\"\n",ingreso.parametro1,ingreso.parametro2);
            if (devolucion==-2) fprintf(stdout, "Error: el curso con id \"%s\" no existe\n",ingreso.parametro2);
        }
        //eliminar_curso
        if (strcmp(ingreso.comando,"eliminar_curso")==0){
            devolucion = eliminar_curso(ingreso.parametro1,base);
            if (devolucion==0) fprintf(stdout,"OK\n");
            if (devolucion==-1) fprintf(stdout, "Error: el curso con id \"%s\" no existe\n",ingreso.parametro1);
        }
        //desinscribir
        if (strcmp(ingreso.comando,"desinscribir")==0){
            devolucion = desinscribir(ingreso.parametro1,ingreso.parametro2,base);
            if (devolucion==0) fprintf(stdout,"OK\n");
            if (devolucion==-1) fprintf(stdout, "Error: el curso con id \"%s\" no existe\n",ingreso.parametro2);
            if (devolucion==-2) fprintf(stdout, "Error: el padron %s no esta inscripto en el curso \"%s\"\n",ingreso.parametro1,ingreso.parametro2);
        }
        //listar_inscriptos
        if (strcmp(ingreso.comando,"listar_inscriptos")==0){
            devolucion = listar_inscriptos(ingreso.parametro1,base);
            if (devolucion==-1) fprintf(stdout, "Error: el curso con id \"%s\" no existe\n",ingreso.parametro1);
        }
        //sesion_iniciar
        if (strcmp(ingreso.comando,"sesion_iniciar")==0){
            devolucion = sesion_iniciar(sesion,ingreso.parametro1);
            if (devolucion==0) fprintf(stdout,"OK\n");
            if (devolucion==-1) fprintf(stdout, "Error: ya hay una sesion en curso\n");
        }
        //sesion_aplicar
        if (strcmp(ingreso.comando,"sesion_aplicar")==0){
            devolucion = sesion_aplicar(sesion,base);
            if (devolucion==0) fprintf(stdout,"OK\n");
            if (devolucion==-1) fprintf(stdout,"Error: no hay una sesion en curso\n");
        }
        //sesion_inscribir
        if (strcmp(ingreso.comando,"sesion_inscribir")==0){
            devolucion = sesion_inscribir(ingreso.parametro1,base,sesion);
            padron = obtener_padron(sesion);
            if (devolucion==2) fprintf(stdout,"OK: el padron %s sera inscripto como regular\n",padron);
            if (devolucion==1) fprintf(stdout,"OK: el padron %s sera inscripto como condicional\n",padron);
            if (devolucion==-3) fprintf(stdout,"Error: no hay una sesion en curso\n");
            if (devolucion==-2) fprintf(stdout,"Error: el curso con id \"%s\" no existe\n",ingreso.parametro1);
            if (devolucion==-1) fprintf(stdout,"Error: el padron %s ya esta inscripto en el curso \"%s\"\n",padron,ingreso.parametro1);
            if (devolucion==-4) fprintf(stdout,"Error: la sesion ya contiene una inscripcion al curso\"%s\"\n",ingreso.parametro1);
            free(padron);
        }
        //sesion_ver
        if (strcmp(ingreso.comando,"sesion_ver")==0){
            sesion_ver(sesion);
        }
        //sesion_deshacer
        if (strcmp(ingreso.comando,"sesion_deshacer")==0){
            devolucion = sesion_deshacer(sesion);
            if (devolucion==0) fprintf(stdout,"OK\n");
            if (devolucion==-1) fprintf(stdout,"Error: no hay una sesion en curso\n");
            if (devolucion==-2) fprintf(stdout,"Error: no hay acciones para deshacer\n");
        }
        reiniciar_parser(&ingreso);
    }
    finalizar_parser(&ingreso);
    sesion_destruir(sesion);
    cerrar_base_de_datos(base);
    return 0;
}
