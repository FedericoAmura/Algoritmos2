#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef bool(*funcioniterador)(void*, void*);

void print_test(char* name, bool result) {
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

int comparar_claves(const char* clave1, const char* clave2){
    return strcmp(clave1,clave2);
}

void destruir_dato(int* dato){
    free(dato);
}

void pruebas_primitivas_basicas_abb_vacio(){
    printf("\nPruebas con el arbol vacio:\n\n");
    abb_t* arbol = abb_crear(&comparar_claves,NULL);
    print_test("Prueba abb crear abb vacio", arbol);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);
	print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(arbol, "A"));
	print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(arbol, "A"));
	print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(arbol, "A"));

	abb_destruir(arbol);
}

void pruebas_iteradores_abb_vacio(){
    printf("\nPruebas con el iterador externo y el arbol vacio:\n\n");
    abb_t* arbol = abb_crear(&comparar_claves,NULL);
    abb_iter_t* iter = abb_iter_in_crear(arbol);

	print_test("Prueba abb iter crear iterador abb vacio", iter);
	print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
	print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
	print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));
    abb_in_order(arbol,NULL,NULL);
    print_test("Prueba abb iter interno no colapsa", true);

	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
}

void pruebas_primitivas_basicas(){
    printf("\nPruebas agregando cosas al arbol:\n\n");
    int i;
    abb_t* arbol = abb_crear(&comparar_claves,NULL);
    char* clave[26] = {"q","w","e","r","t","y","u","i","o","p","l","k","j","h","g","f","d","s","a","z","x","c","v","b","n","m"};
    int dato[26] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};

    print_test("Prueba abb crear abb vacio", arbol);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);
	for(i=0;i<26;i++){
        print_test("Prueba agregar un elemento", abb_guardar(arbol,clave[i],&dato[i]));
        printf("La cantidad de elementos es %d",i+1);
        print_test("", abb_cantidad(arbol) == i+1);
	}
	printf("\n");
	for(i=0;i<26;i++){
        print_test("Prueba agregar un elemento repetido", abb_guardar(arbol,clave[i],&dato[i]));
        //como pisa el dato esta bien que de abb_guardar devuelva verdadero
        printf("La cantidad de elementos es %d",26);
        print_test("", abb_cantidad(arbol) == 26);
	}
	for(i=0;i<26;i++){
        printf("Verifico que exista la clave %d", i+1);
        print_test("", abb_pertenece(arbol,clave[i]));
	}
    for(i=0;i<26;i++){
        printf("Verifico obtener el elemento %d", i+1);
        print_test("", abb_obtener(arbol,clave[i])==&dato[i]);
    }

    print_test("Prueba borrar un elemento que no existe", abb_borrar(arbol,"aa")==NULL);
    print_test("Prueba borrar un elemento tipo hoja", abb_borrar(arbol,clave[20])==&dato[20]);
    print_test("Prueba abb la cantidad de elementos es 25", abb_cantidad(arbol) == 25);
    print_test("Prueba borrar un elemento con hijo izquierdo", abb_borrar(arbol,clave[21])==&dato[21]);
    print_test("Prueba abb la cantidad de elementos es 24", abb_cantidad(arbol) == 24);
    print_test("Prueba borrar un elemento con hijo derecho", abb_borrar(arbol,clave[3])==&dato[3]);
    print_test("Prueba abb la cantidad de elementos es 23", abb_cantidad(arbol) == 23);
    print_test("Prueba borrar un elemento cualquiera", abb_borrar(arbol,clave[13])==&dato[13]);
    print_test("Prueba abb la cantidad de elementos es 22", abb_cantidad(arbol) == 22);
    print_test("Prueba abb obtener clave 10", abb_obtener(arbol, clave[10]));
    print_test("Prueba borrar un elemento cualquiera", abb_borrar(arbol,clave[10])==&dato[10]);
    print_test("Prueba abb la cantidad de elementos es 21", abb_cantidad(arbol) == 21);
    print_test("Prueba abb obtener clave 10, es NULL, no existe", !abb_obtener(arbol, clave[10]));
    print_test("Prueba borrar un elemento cualquiera", abb_borrar(arbol,clave[19])==&dato[19]);
    print_test("Prueba abb la cantidad de elementos es 20", abb_cantidad(arbol) == 20);
    print_test("Prueba borrar un elemento cualquiera", abb_borrar(arbol,clave[22])==&dato[22]);
    print_test("Prueba abb la cantidad de elementos es 19", abb_cantidad(arbol) == 19);
    print_test("Prueba borrar un elemento cualquiera", abb_borrar(arbol,clave[1])==&dato[1]);
    print_test("Prueba abb la cantidad de elementos es 18", abb_cantidad(arbol) == 18);
    print_test("Prueba borrar un elemento ya borrado", !abb_borrar(arbol,clave[20]));
    print_test("Prueba abb la cantidad de elementos es 18", abb_cantidad(arbol) == 18);

    abb_destruir(arbol);
}

void pruebas_iterador_externo(){
    printf("\nPruebas con el iterador externo del arbol:\n\n");

    int i;
    abb_t* arbol = abb_crear(&comparar_claves,NULL);
    char* clave[26] = {"q","w","e","r","t","y","u","i","o","p","l","k","j","h","g","f","d","s","a","z","x","c","v","b","n","m"};
    char* clavesordenadas[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    int dato[26] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
    const char* actual;

    print_test("Prueba abb crear abb vacio", arbol);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);
	for(i=0;i<26;i++) abb_guardar(arbol,clave[i],&dato[i]);
	print_test("Prueba abb la cantidad de elementos es 26", abb_cantidad(arbol) == 26);

	abb_iter_t* iterador = abb_iter_in_crear(arbol);
	print_test("Prueba crear el iterador", iterador!=NULL);
	print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iterador));

	for(i=0;i<26;i++){
        printf("Verifico que el iterador este sobre el elemento %d", i+1);
        actual = abb_iter_in_ver_actual(iterador);
        print_test("", strcmp(actual,clavesordenadas[i])==0);
        print_test("Avanzo el iterador", abb_iter_in_avanzar(iterador));
    }
    print_test("Prueba abb iterador esta al final", abb_iter_in_al_final(iterador));

	abb_iter_in_destruir(iterador);
	abb_destruir(arbol);
}

bool visitar1(const char *clave, void *dato, void *extra){
    printf("La clave %s esta en la posicion %d\n",clave,*(int*)dato);
    return true;
}

bool visitar2(const char *clave, void *dato, void *extra){
    printf("La clave %s esta en la posicion %d\n",clave,*(int*)dato);
    if (*(int*)dato==*(int*)extra) return false;
    return true;
}

void pruebas_iterador_interno(){
    printf("\nPruebas con el iterador interno del arbol:\n\n");

    int i;
    abb_t* arbol = abb_crear(&comparar_claves,NULL);
    char* clave[26] = {"q","w","e","r","t","y","u","i","o","p","l","k","j","h","g","f","d","s","a","z","x","c","v","b","n","m"};
    int dato[26] = {17,23,5,18,20,25,21,9,15,16,12,11,10,8,7,6,4,19,1,26,24,3,22,2,14,13};
    int* corte = malloc(sizeof(int));
    *corte=5;

    print_test("Prueba abb crear abb vacio", arbol);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);
	for(i=0;i<26;i++) abb_guardar(arbol,clave[i],&dato[i]);
	print_test("Prueba abb la cantidad de elementos es 26", abb_cantidad(arbol) == 26);
	printf("Prueba abb iterador interno, debe aparecer listados en orden las letras y sus posiciones:\n");
	abb_in_order(arbol,visitar1,NULL);
    printf("Prueba abb iterador interno interrumpido mediante extra, deben aparecer las primeras 5 claves con sus posiciones:\n");
	abb_in_order(arbol,visitar2,corte);

	abb_destruir(arbol);
	free(corte);
}

int main(void){
    pruebas_primitivas_basicas_abb_vacio();
    pruebas_iteradores_abb_vacio();
    pruebas_primitivas_basicas();
    pruebas_iterador_externo();
    pruebas_iterador_interno();
    return 0;
}
