#ifndef POLONORTE_C
#define POLONORTE_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "polonorte.h"
#include "lista.h"
#include "hash.h"
#include "heap.h"
#include "grafo.h"

typedef struct juguete{
    int idjuguete;
    int valor;
    int peso;
}juguete_t;

typedef struct fabrica{
    char* idfabrica;
    int idesquina;
    int horarioentrada;
    int horariosalida;
    lista_t* juguetes;
}fabrica_t;


struct base{
    grafo_t* mapa;
    hash_t* fabricas;
    heap_t* cerradofabricas;
    lista_t* fabricasavisitar;
    int capacidad;
    int polonorte;
};

/* ******************************************************************
 *                    PRIMITIVAS DE LA BASE
 * *****************************************************************/
//funciones auxiliares para crear las fabricas y juguetes
fabrica_t* crear_fabrica(char* idfabrica,char* idesquina,char* horaentrada,char* horasalida){
    fabrica_t* fabrica = malloc(sizeof(fabrica_t));
    if (fabrica==NULL) return NULL;
    fabrica->juguetes = lista_crear();
    if (fabrica->juguetes==NULL){
        free(fabrica);
        return NULL;
    }
    fabrica->idfabrica = malloc(strlen(idfabrica)+1);
    strcpy(fabrica->idfabrica,idfabrica);
    fabrica->idesquina = atoi(idesquina);
    fabrica->horarioentrada = atoi(horaentrada);
    fabrica->horariosalida = atoi(horasalida);
    return fabrica;
}

juguete_t* crear_juguete(char* idjuguete,char* valor,char* peso){
    juguete_t* juguete = malloc(sizeof(juguete_t));
    if (juguete==NULL) return NULL;
    juguete->idjuguete = atoi(idjuguete);
    juguete->valor = atoi(valor);
    juguete->peso = atoi(peso);
    return juguete;
}

void destruir_juguete(juguete_t* juguete){
    free(juguete);
}

void destruir_fabrica(fabrica_t* fabrica){
    while (!lista_esta_vacia(fabrica->juguetes)) destruir_juguete(lista_borrar_primero(fabrica->juguetes));
    free(fabrica->idfabrica);
    lista_destruir(fabrica->juguetes,NULL);
    free(fabrica);
}

//funcion auxiliar que ordena las fabricas por horario de cerrado
int fabrica_cierra_primero(fabrica_t* fabrica1,fabrica_t* fabrica2){
    if ((fabrica1->horariosalida-fabrica2->horariosalida)!=0) return (fabrica2->horariosalida-fabrica1->horariosalida);
    //las dos fabricas cierran al mismo tiempo
    if ((fabrica1->horarioentrada-fabrica2->horarioentrada)!=0) return (fabrica2->horarioentrada-fabrica1->horarioentrada);
    //las dos fabricas abren al mismo tiempo
    if ((atoi(fabrica1->idfabrica)-atoi(fabrica2->idfabrica))!=0) return (atoi(fabrica2->idfabrica)-atoi(fabrica1->idfabrica));
    //son iguales las fabricas
    return 0;
}

base_t *base_crear(){
    //creo la base
    base_t* base = malloc(sizeof(base_t));
    if (base == NULL) return NULL;
    //creo los componentes de la base
    base->mapa = grafo_crear();
    base->fabricas = hash_crear((hash_destruir_dato_t)destruir_fabrica);
    base->cerradofabricas = heap_crear((cmp_func_t)fabrica_cierra_primero);
    base->fabricasavisitar = lista_crear();
    //verifico si los componentes se crearon correctamente
    if ((base->mapa==NULL) || (base->fabricas==NULL) || (base->cerradofabricas==NULL) || (base->fabricasavisitar==NULL)){
        if (base->mapa!=NULL) grafo_destruir(base->mapa);
        if (base->cerradofabricas!=NULL) heap_destruir(base->cerradofabricas,NULL);
        if (base->fabricasavisitar!=NULL) lista_destruir(base->fabricasavisitar,NULL);
        if (base->fabricas!=NULL) hash_destruir(base->fabricas);
        return NULL;
    }
    base->capacidad = 0;
    base->polonorte = 0;
    //todo exitoso
    return base;
}

void base_destruir(base_t* base){
    //destruyo lo interior a la base
    grafo_destruir(base->mapa);
    heap_destruir(base->cerradofabricas,NULL);
    lista_destruir(base->fabricasavisitar,NULL);
    hash_destruir(base->fabricas);
    //destruyo la base
    free(base);
}

void cargar_capacidadyorigen(base_t* base,int capacidad,int polonorte){
    base->capacidad = capacidad;
    base->polonorte = polonorte;
}

int agregar_fabrica(base_t* base,char* idfabrica,char* idesquina,char* horaentrada,char* horasalida){
    fabrica_t* fabrica = crear_fabrica(idfabrica,idesquina,horaentrada,horasalida);
    if (fabrica==NULL) return -1;
    if (!hash_guardar(base->fabricas,idfabrica,(void*)fabrica)) return -1; //no se pudo guardar la fabrica en el hash
    if (!heap_encolar(base->cerradofabricas,(void*)fabrica)) return -2;; //no se pudo guardar la fabrica en el heap
    return 0;
}

int agregar_juguete(base_t* base,char* idfabrica,char* idjuguete,char* valor,char* peso){
    if (!hash_pertenece(base->fabricas,idfabrica)) return -1; //verifico si la fabrica existe
    //creo el juguete
    juguete_t* juguete = crear_juguete(idjuguete,valor,peso);
    if (juguete==NULL) return -2; //no se pudo crear el juguete
    fabrica_t* fabrica = hash_obtener(base->fabricas,idfabrica);
    if (lista_insertar_ultimo(fabrica->juguetes,(void*)juguete)!=true) return -3; //agrego el juguete y verifico si se pudo
    return 0;
}

//funcion auxiliar que convierte el lapso desde las 00:00 al horario correspondiente
void conversion_horario(int* hora,int* minutos,int lapso){
    *minutos = lapso%60;
    lapso = lapso/60;
    *hora = lapso;
}

int listar_fabricas(base_t* base,bool imprimir){
    int hora,minutos;
    fabrica_t* fabrica;
    fabrica_t* anterior;
    if (lista_esta_vacia(base->fabricasavisitar)){
        //creo una lista con las fabricas que voy a visitar
        anterior = heap_desencolar(base->cerradofabricas);
        lista_insertar_ultimo(base->fabricasavisitar,(void*)anterior);
        while (!heap_esta_vacio(base->cerradofabricas)){
            fabrica = heap_desencolar(base->cerradofabricas);
            if (fabrica->horarioentrada>=anterior->horariosalida){
                lista_insertar_ultimo(base->fabricasavisitar,(void*)fabrica);
                anterior = fabrica;
            }
        }
    }
    if (imprimir){
        printf("Cantidad: %zu\n",lista_largo(base->fabricasavisitar));
        lista_iter_t* iterador = lista_iter_crear(base->fabricasavisitar);
        while(!lista_iter_al_final(iterador)){
            fabrica = lista_iter_ver_actual(iterador);
            conversion_horario(&hora,&minutos,fabrica->horarioentrada);
            printf("%s,%02d:%02d,",fabrica->idfabrica,hora,minutos);
            conversion_horario(&hora,&minutos,fabrica->horariosalida);
            printf("%02d:%02d\n",hora,minutos);
            lista_iter_avanzar(iterador);
        }
        lista_iter_destruir(iterador);
    }
    return 0;
}

int valuar_juguetes(base_t* base,char* idfabrica){
    if (!hash_pertenece(base->fabricas,idfabrica)) return -1;
    int sonrisas = 0;
    int i;
    //creo la mochila y la inicializo
    int* mochila = calloc(base->capacidad+1,sizeof(int));
    fabrica_t* fabrica = hash_obtener(base->fabricas,idfabrica);
    //creo el iterador para recorrer los juguetes que tiene la fabrica
    lista_iter_t* iterador = lista_iter_crear(fabrica->juguetes);
    juguete_t* juguete = lista_iter_ver_actual(iterador);
    while(juguete!=NULL){
        for(i=base->capacidad;i>0;i--){
            if (i-(juguete->peso)>=0){
                if (mochila[i]<(mochila[i-juguete->peso]+juguete->valor)) mochila[i]=(mochila[i-juguete->peso]+juguete->valor);
            }
        }
        lista_iter_avanzar(iterador);
        juguete = lista_iter_ver_actual(iterador);
    }
    sonrisas = mochila[base->capacidad];
    lista_iter_destruir(iterador);
    free(mochila);
    return sonrisas;
}

int valuar_juguetes_total(base_t* base){
    int totalsonrisas = 0;
    int sonrisasfabricas;
    fabrica_t* fabrica;
    //if (lista_esta_vacia(base->fabricasavisitar)) return -1; //la lista esta vacia y hay que ver cuales visitar antes
    if (lista_esta_vacia(base->fabricasavisitar)) listar_fabricas(base,false); //la lista esta vacia y hay que ver cuales visitar antes
    lista_iter_t* iterador = lista_iter_crear(base->fabricasavisitar);
    while (!lista_iter_al_final(iterador)){
        fabrica = (fabrica_t*)lista_iter_ver_actual(iterador);
        sonrisasfabricas = valuar_juguetes(base,fabrica->idfabrica);
        totalsonrisas = (totalsonrisas + sonrisasfabricas);
        lista_iter_avanzar(iterador);
    }
    lista_iter_destruir(iterador);
    return totalsonrisas;
}

int camino_optimo(base_t* base,char* idfabrica){
    printf("camino optimo\n");
    return 15;
}

#endif // POLONORTE_C
