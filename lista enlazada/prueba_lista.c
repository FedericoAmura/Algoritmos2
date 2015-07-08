#include "lista.h"
//#include "cola.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef bool(*funcioniterador)(void*, void*);

void print_test(char* name, bool result) {
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void pruebas_primitivas_basicas(){
    int* numeros = malloc(5*sizeof(int));
    int i;
    for(i=0;i<5;i++){
        numeros[i]=i;
	};

    printf("\nPruebas con datos simples:\n");

    lista_t* lista = lista_crear();
    print_test("Prueba crear lista", (lista!=NULL));

    print_test("Veo que la lista este vacia", lista_esta_vacia(lista));

    print_test("Inserto el primero elemento al principio", lista_insertar_primero(lista,&numeros[2]));

    print_test("Inserto el segundo elemento al principio", lista_insertar_primero(lista,&numeros[1]));

    print_test("Inserto el tercer elemento al principio", lista_insertar_primero(lista,&numeros[0]));

    print_test("Verficio el largo de la lista", lista_largo(lista)==3);

    print_test("Inserto el cuarto elemento al final", lista_insertar_ultimo(lista,&numeros[3]));

    print_test("Inserto el quinto elemento al final", lista_insertar_ultimo(lista,&numeros[4]));

    print_test("Verficio el largo de la lista", lista_largo(lista)==5);

    print_test("Verifico el primer elemento", (lista_ver_primero(lista)==&numeros[0]));

    print_test("Saco el primero elemento", lista_borrar_primero(lista)==&numeros[0]);

    print_test("Verficio el largo de la lista", lista_largo(lista)==4);

    i=1;
    while(!lista_esta_vacia(lista)){
        print_test("Verifico el siguiente elemento", (lista_ver_primero(lista)==&numeros[i]));
        print_test("Saco el siguiente elemento", (lista_borrar_primero(lista)==&numeros[i]));
        print_test("Verficio el largo de la lista", (lista_largo(lista)==(4-i)));
        i++;
    };

    print_test("Veo que la lista este vacia", lista_esta_vacia(lista));

    lista_destruir(lista,NULL);
    print_test("Destruyo la lista", true);
    free(numeros);
}
/*
void pruebas_primitivas_basicas_datos_complejos(){
    cola_t* cola1 = cola_crear();
    cola_t* cola2 = cola_crear();
    cola_t* cola3 = cola_crear();
    cola_t* cola4 = cola_crear();
    typedef void (*destruccion_dato)(void*);

    printf("\nPruebas con datos complejos:\n");

    lista_t* lista = lista_crear();
    print_test("Prueba crear lista", (lista!=NULL));

    print_test("Veo que la lista este vacia", lista_esta_vacia(lista));

    print_test("Verficio el largo de la lista", lista_largo(lista)==0);

    print_test("Inserto la segunda cola al principio", lista_insertar_primero(lista,cola2));

    print_test("Inserto la primera cola al principio", lista_insertar_primero(lista,cola1));

    print_test("Verficio el largo de la lista", lista_largo(lista)==2);

    print_test("Inserto la tercer cola al final", lista_insertar_ultimo(lista,cola3));

    print_test("Inserto la cuarta cola al final", lista_insertar_ultimo(lista,cola4));

    print_test("Verficio el largo de la lista", lista_largo(lista)==4);

    print_test("Verifico la primera cola", (lista_ver_primero(lista)==cola1));

    print_test("Saco el primero elemento", lista_borrar_primero(lista)==cola1);
    cola_destruir(cola1,NULL);

    lista_destruir(lista,(destruccion_dato)&cola_destruir);
    print_test("Elimino todo", true);
}
*/
void pruebas_iterador_externo(){

    int* numeros = malloc(5*sizeof(int));
    int i;
    for(i=0;i<5;i++){
        numeros[i]=i;
	};

    printf("\nPruebas con iterador externo:\n");
    lista_t* lista = lista_crear();
    print_test("Prueba crear lista", (lista!=NULL));
    //lista://
    print_test("Veo que la lista este vacia", lista_esta_vacia(lista));
    print_test("Verifico el largo de la lista", lista_largo(lista)==0);
    lista_iter_t* iterador = lista_iter_crear(lista);
    print_test("Creo un iterador a una lista vacia", (iterador!=NULL));
    //lista:/--/
    print_test("Agrego un elemento al principio", lista_insertar(lista,iterador,&numeros[3]));
    //lista:/-3-/
    print_test("Verifico el largo de la lista", lista_largo(lista)==1);
    print_test("Agrego otro elemento al principio", lista_insertar(lista,iterador,&numeros[1]));
    //lista:/-1-3/
    print_test("Agrego otro elemento al principio", lista_insertar(lista,iterador,&numeros[0]));
    //lista:/-0-13/
    print_test("Verifico el elemento actual", (lista_iter_ver_actual(iterador)==&numeros[0]));
    print_test("Me muevo al siguiente elemento", (lista_iter_avanzar(iterador)));
    //lista:/0-1-3/
    print_test("Verifico el elemento actual", (lista_iter_ver_actual(iterador)==&numeros[1]));
    print_test("Verifico el largo de la lista", lista_largo(lista)==3);
    print_test("Me muevo al siguiente elemento", (lista_iter_avanzar(iterador)));
    //lista:/01-3-/
    print_test("Agrego un elemento en el medio", (lista_insertar(lista,iterador,&numeros[2])));
    //lista:/01-2-3/
    print_test("Verifico el largo de la lista", lista_largo(lista)==4);
    print_test("Me muevo al siguiente elemento", (lista_iter_avanzar(iterador)));
    //lista:/012-3-/
    print_test("Verifico no estar al final", (!lista_iter_al_final(iterador)));
    print_test("Me muevo al siguiente elemento", (lista_iter_avanzar(iterador)));
    //lista:/0123--/
    print_test("Verifico estar al final", (lista_iter_al_final(iterador)));
    print_test("Agrego un elemento al final", lista_insertar(lista,iterador,&numeros[4]));
    //lista:/0123-4-/
    print_test("Me muevo al siguiente nodo", (lista_iter_avanzar(iterador)));
    //lista:/01234--/
    print_test("Verifico estar al final", (lista_iter_al_final(iterador)));
    lista_iter_destruir(iterador);
    print_test("Destruyo el iterador", true);
    //lista:/01234/
    iterador = lista_iter_crear(lista);
    print_test("Creo un iterador nuevo a la lista", (iterador!=NULL));
    //lista:/-0-1234/
    print_test("Verifico que apunte al elemento correcto", (lista_iter_ver_actual(iterador)==&numeros[0]));
    for(i=0;i<5;i++) print_test("Saco el elemento apuntado verificandolo", (lista_borrar(lista,iterador)==&numeros[i]));
    //lista:/--/
    lista_iter_destruir(iterador);
    print_test("Destruyo el iterador", true);
    //lista://
    lista_destruir(lista,NULL);
    print_test("Destruyo la lista vacia", true);
    //lista:
    free(numeros);
}

bool imprimirinterrumpido(char *caracter, void *extra){
    if (*caracter=='-') return false;
    printf("%c",*caracter);
    return true;
}

bool imprimircondato(char *caracter, char *resultado){
    printf("%c",*caracter);
    if (*caracter==':') printf("%s",resultado);
    return true;
}

void pruebas_iterador_interno(){
    char* impresion1 = "\nPrueba de volumen con iterador interno limitado: OK-ERROR";
    char* impresion2 = "\nPrueba de iterador interno hasta el final:";
    char* resultado = " OK\n";
    lista_t* lista1 = lista_crear();
    lista_t* lista2 = lista_crear();
    int i,largocadena;
    printf("\nPruebas con iterador interno:\n");
    largocadena = strlen(impresion1);
    for(i=0;i<=largocadena;i++) lista_insertar_ultimo(lista1,(void*)(impresion1+i));
    largocadena = strlen(impresion2);
    for(i=0;i<=largocadena;i++) lista_insertar_ultimo(lista2,(void*)(impresion2+i));
    lista_iterar(lista1,(funcioniterador)imprimirinterrumpido,NULL);
    lista_iterar(lista2,(funcioniterador)imprimircondato,(void*)resultado);
    lista_destruir(lista1,NULL);
    lista_destruir(lista2,NULL);
}

int main(void){
    pruebas_primitivas_basicas();
    //pruebas_primitivas_basicas_datos_complejos();
    pruebas_iterador_externo();
    pruebas_iterador_interno();
    return 0;
}
