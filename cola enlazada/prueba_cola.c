#include <stdio.h>
#include "stdlib.h"
#include <stdbool.h>
#include "cola.h"
//#include "pila.h"

/*******************************************************************
 *                        PRUEBAS UNITARIAS
 ******************************************************************/

typedef void (*destruccion_dato)(void*);

void print_test(char* name, bool result){
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void pruebas_cola_vacia(){
    printf("Pruebas con la cola vacia:\n\n");
    cola_t* cola = cola_crear();
    print_test("Prueba crear cola vacia", cola);
	print_test("Prueba cola esta vacia", cola_esta_vacia(cola));
	print_test("Prueba cola ver primero", cola_ver_primero(cola)==NULL);
	print_test("Prueba cola desencolar", cola_desencolar(cola)==NULL);
	cola_destruir(cola,NULL);
    printf("\n");
}

void pruebas_cola_basicas(){
    int i;
    int* numeros = malloc(16*sizeof(int));
	for(i=0;i<=15;i++){
        numeros[i]=i;
	};

    printf("Pruebas con la cola:\n\n");
    cola_t* cola = cola_crear();
    print_test("Prueba crear cola", cola);
	print_test("Prueba cola esta vacia", cola_esta_vacia(cola));
	for(i=0;i<=15;i++){
        print_test("Encolo un elemento", cola_encolar(cola,&numeros[i]));
	};
	print_test("Prueba no cola esta vacia", !cola_esta_vacia(cola));
	for(i=0;i<=15;i++){
        print_test("Verifico el primer elemento", *((int*)cola_ver_primero(cola))==numeros[i]);
        print_test("Desencolo un elemento verificando", *((int*)cola_desencolar(cola))==numeros[i]);
	};
    print_test("Prueba cola esta vacia", cola_esta_vacia(cola));
    free(numeros);
	cola_destruir(cola,NULL);
    printf("\n");
}
/*
void pruebas_cola_datos_complejos(){
    pila_t* pila1 = pila_crear();
	pila_t* pila2 = pila_crear();

    printf("Pruebas con la cola y datos complejos:\n\n");
    cola_t* cola = cola_crear();
    print_test("Prueba crear cola", cola);
	print_test("Prueba cola esta vacia", cola_esta_vacia(cola));
    print_test("Encolo una pila", cola_encolar(cola,pila1));
	print_test("Prueba no cola esta vacia", !cola_esta_vacia(cola));
    print_test("Encolo otra pila", cola_encolar(cola,pila2));
    print_test("Verifico el primer elemento", cola_ver_primero(cola)==pila1);
    print_test("Prueba no cola esta vacia", !cola_esta_vacia(cola));
    print_test("Destruyo la cola con todavia una pila dentro", true);
    cola_destruir(cola,(destruccion_dato)&pila_destruir);
    printf("\n");
}
*/
void pruebas_cola_volumen(){
int i;
    int* numeros = malloc(2000*sizeof(int));
	for(i=0;i<2000;i++){
        numeros[i]=i;
	};

    printf("Pruebas de volumen con la cola:\n\n");
    cola_t* cola = cola_crear();
    print_test("Prueba crear cola", cola);
	print_test("Prueba cola esta vacia", cola_esta_vacia(cola));
	for(i=0;i<2000;i++){
        if (i%200==0) print_test("Encolo un elemento", cola_encolar(cola,&numeros[i])); else cola_encolar(cola,&numeros[i]);
	};
	print_test("Prueba no cola esta vacia", !cola_esta_vacia(cola));
	for(i=0;i<1000;i++){
        if (i%200==0) print_test("Verifico el primer elemento", *((int*)cola_ver_primero(cola))==numeros[i]);
        if (i%200==0) print_test("Desencolo un elemento verificando", *((int*)cola_desencolar(cola))==numeros[i]); else cola_desencolar(cola);
	};
    print_test("Prueba cola no esta vacia para luego destruirla", !cola_esta_vacia(cola));
    printf("\n");
    free(numeros);
	cola_destruir(cola,NULL);
    printf("\n");
}

int main(void){
	pruebas_cola_vacia();
	pruebas_cola_basicas();
	//pruebas_cola_datos_complejos();
	pruebas_cola_volumen();
	return 0;
}
