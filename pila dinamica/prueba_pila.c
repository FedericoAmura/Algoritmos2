#include <stdio.h>
#include "stdlib.h"
#include <stdbool.h>
#include "pila.h"

/*******************************************************************
 *                        PRUEBAS UNITARIAS
 ******************************************************************/

void print_test(char* name, bool result){
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void pruebas_pila_vacia(){
    printf("Pruebas con la pila vacia:\n\n");
    pila_t* pila = pila_crear();
    print_test("Prueba crear pila vacia", pila);
	print_test("Prueba pila esta vacia", pila_esta_vacia(pila));
	print_test("Prueba pila ver primero", pila_ver_tope(pila)==NULL);
	print_test("Prueba pila desencolar", pila_desapilar(pila)==NULL);
	pila_destruir(pila);
    printf("\n");
}

void pruebas_pila_basicas(){
    int i;
    int* numeros = malloc(16*sizeof(int));
	for(i=0;i<=15;i++){
        numeros[i]=i;
	};

    printf("Pruebas con la pila:\n\n");
    pila_t* pila = pila_crear();
    print_test("Prueba crear pila", pila);
	print_test("Prueba pila esta vacia", pila_esta_vacia(pila));
	for(i=0;i<=15;i++){
        print_test("Apilo un elemento", pila_apilar(pila,&numeros[i]));
	};
	print_test("Prueba pila no esta vacia", !pila_esta_vacia(pila));
	for(i=0;i<=15;i++){
        print_test("Verifico el primer elemento", *((int*)pila_ver_tope(pila))==numeros[15-i]);
        print_test("Desapilo un elemento verificando", *((int*)pila_desapilar(pila))==numeros[15-i]);
	};
    print_test("Prueba pila esta vacia", pila_esta_vacia(pila));
    free(numeros);
	pila_destruir(pila);
    printf("\n");
}

void pruebas_pila_volumen(){
    int i;
    int* numeros = malloc(2000*sizeof(int));
	for(i=0;i<2000;i++){
        numeros[i]=i;
	};

    printf("Pruebas de volumen con la pila:\n\n");
    pila_t* pila = pila_crear();
    print_test("Prueba crear pila", pila);
	print_test("Prueba pila esta vacia", pila_esta_vacia(pila));
	for(i=0;i<2000;i++){
        if (i%200==0) print_test("Apilo un elemento", pila_apilar(pila,&numeros[i])); else pila_apilar(pila,&numeros[i]);
	};
	print_test("Prueba pila no esta vacia", !pila_esta_vacia(pila));
	for(i=0;i<1000;i++){
        if (i%200==0) print_test("Verifico el primer elemento", *((int*)pila_ver_tope(pila))==numeros[1999-i]);
        if (i%200==0) print_test("Desapilo un elemento verificando", *((int*)pila_desapilar(pila))==numeros[1999-i]); else pila_desapilar(pila);
	};
    print_test("Prueba pila no esta vacia para luego destruirla de todos modos", !pila_esta_vacia(pila));
    free(numeros);
	pila_destruir(pila);
    printf("\n");
}

int main(void)
{
	pruebas_pila_vacia();
	pruebas_pila_basicas();
	pruebas_pila_volumen();
	return 0;
}
