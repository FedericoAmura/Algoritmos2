#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef bool(*funcioniterador)(void*, void*);

void print_test(char* name, bool result) {
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

int comparar_elementos(const char* clave1, const char* clave2){
    return strcmp(clave1,clave2);
}

void destruir_dato(int* dato){
    free(dato);
}

void pruebas_primitivas_basicas_heap_vacio(){
    printf("\nPruebas con el heap vacio:\n\n");
    heap_t* heap = heap_crear((cmp_func_t)comparar_elementos);
    print_test("Prueba heap crear heap vacio", heap);
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap)==0);
	print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
	print_test("Prueba heap ver maximo", heap_ver_max(heap)==NULL);
	print_test("Prueba heap desencolar", heap_desencolar(heap)==NULL);
	print_test("Prueba heap destruir", true);
    heap_destruir(heap,NULL);
}

void pruebas_primitivas_basicas(){
    int i;
    char* clave[26] = {"q","w","e","r","t","y","u","i","o","p","l","k","j","h","g","f","d","s","a","z","x","c","v","b","n","m"};
    char* clavesordenadas[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

    printf("\nPruebas primitivas basicas sobre el heap:\n\n");
    heap_t* heap = heap_crear((cmp_func_t)comparar_elementos);
    print_test("Prueba heap crear heap vacio", heap);
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap)==0);
	print_test("Prueba heap esta vacio", heap_esta_vacio(heap));
    print_test("prueba heap agregar elemento", heap_encolar(heap,clave[0]));
    print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap)==1);
    print_test("prueba heap ver el mayor elemento", (void*)clave[0]==heap_ver_max(heap));
    print_test("prueba heap sacar el elemento", (void*)clave[0]==heap_desencolar(heap));
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap)==0);
    for(i=0;i<26;i++){
        print_test("Prueba agregar un elemento", heap_encolar(heap,clave[i]));
        printf("La cantidad de elementos es %d",i+1);
        print_test("", heap_cantidad(heap)==i+1);
	}
	print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap)==26);
	for(i=0;i<16;i++){
        print_test("Prueba sacar un elemento", strcmp(clavesordenadas[25-i],(char*)heap_desencolar(heap))==0);
        printf("La cantidad de elementos es %d",25-i);
        print_test("", heap_cantidad(heap)==25-i);
	}
	print_test("Prueba heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Prueba heap destruir", true);
    heap_destruir(heap,NULL);
}

void pruebas_heapsort(){
    char* clave[26] = {"q","w","e","r","t","y","u","i","o","p","l","k","j","h","g","f","d","s","a","z","x","c","v","b","n","m"};
    char* clavesordenadas[26] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
    //char* clave[4] = {"b","d","a","c"};
    //char* clavesordenadas[4] = {"a","b","c","d"};

    printf("\nPruebas primitiva heapsort:\n\n");
    heap_sort((void*)clave,26,(cmp_func_t)comparar_elementos);
    bool exito = true;
    for(int i=0;i<26;i++){
        if (strcmp(clave[i],clavesordenadas[i])!=0){
            exito=false;
            printf("%d,",i);
        }
    }
    print_test("Prueba heapsort", exito);
}

int main(void){
    pruebas_primitivas_basicas_heap_vacio();
    pruebas_primitivas_basicas();
    pruebas_heapsort();
    return 0;
}
