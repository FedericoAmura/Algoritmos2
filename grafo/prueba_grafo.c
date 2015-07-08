#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "grafo.h"

void print_test(char* name, bool result) {
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void destruir_dato(int* dato){
    free(dato);
}

void pruebas_primitivas_basicas_grafo_vacio(){
    printf("\nPruebas con el grafo vacio:\n\n");
    grafo_t* grafo = grafo_crear();
    print_test("Prueba grafo crear grafo vacio", grafo);
	print_test("Prueba grafo existe vertice", !grafo_existe_vertice(grafo,"vertice"));
	print_test("Prueba grafo existe arista", !grafo_existe_arista(grafo,"arista"));
	print_test("Prueba grafo conseguir dato de vertice que no existe", grafo_dato_vertice(grafo,"vertice")==NULL);
	print_test("Prueba grafo conseguir peso de arista que no existe", grafo_peso_arista(grafo,"arista")==-1);
	print_test("Prueba grafo destruir", true);

    grafo_destruir(grafo);
}

void pruebas_primitivas_basicas(){
    char idvertice1[5] = "abcd";
    char idvertice2[5] = "efgh";
    char idvertice3[5] = "ijkl";
    char idvertice4[5] = "mnop";
    char idvertice5[6] = "qrstu";
    char idvertice6[6] = "vwxyz";
    int* numeros = malloc(6*sizeof(int));
    for(int i=0;i<6;i++) numeros[i]=i;
    printf("\nPruebas con datos en el grafo:\n\n");
    grafo_t* grafo = grafo_crear();
    print_test("Prueba grafo crear grafo", grafo);
    print_test("Prueba grafo agregar vertice 1", grafo_agregar_vertice(grafo,idvertice1,0,0,12.00,12.00,(void*)&(numeros[1])));
    print_test("Prueba grafo agregar vertice 1 de vuelta", !grafo_agregar_vertice(grafo,idvertice1,0,0,12.00,12.00,(void*)&(numeros[1])));
    print_test("Prueba grafo agregar vertice 2", grafo_agregar_vertice(grafo,idvertice2,3,4,13.00,13.00,(void*)&(numeros[2])));
    print_test("Prueba grafo agregar vertice 3", grafo_agregar_vertice(grafo,idvertice3,10,28,15.00,16.00,(void*)&(numeros[3])));
    print_test("Prueba grafo agregar vertice 4", grafo_agregar_vertice(grafo,idvertice4,-5,-12,12.00,20.00,(void*)&(numeros[4])));
    print_test("Prueba grafo agregar vertice 4 de vuelta", !grafo_agregar_vertice(grafo,idvertice4,-2,-1,12.00,20.00,(void*)&(numeros[4])));
    print_test("Prueba grafo agregar vertice 5", grafo_agregar_vertice(grafo,idvertice5,2,4,21.00,21.00,(void*)&(numeros[5])));
    print_test("Prueba grafo existe vertice 1", grafo_existe_vertice(grafo,idvertice1));
    print_test("Prueba grafo existe vertice 4", grafo_existe_vertice(grafo,idvertice4));
    print_test("Prueba grafo no existe vertice 6", !grafo_existe_vertice(grafo,idvertice6));
    print_test("Prueba grafo obtener dato vertice 2", grafo_dato_vertice(grafo,idvertice2)==(void*)&(numeros[2]));
    print_test("Prueba grafo obtener dato vertice 5", grafo_dato_vertice(grafo,idvertice5)==(void*)&(numeros[5]));
    print_test("Prueba grafo agregar arista 1-2", grafo_agregar_arista(grafo,"12",idvertice1,idvertice2));
    print_test("Prueba grafo agregar arista 2-3", grafo_agregar_arista(grafo,"23",idvertice2,idvertice3));
    print_test("Prueba grafo agregar arista 2-3 de vuelta", !grafo_agregar_arista(grafo,"23",idvertice2,idvertice3));
    print_test("Prueba grafo agregar arista 3-4", grafo_agregar_arista(grafo,"34",idvertice3,idvertice4));
    print_test("Prueba grafo agregar arista 4-5", grafo_agregar_arista(grafo,"45",idvertice4,idvertice5));
    print_test("Prueba grafo agregar arista 1-4", grafo_agregar_arista(grafo,"14",idvertice1,idvertice4));
    print_test("Prueba grafo agregar arista 2-5", grafo_agregar_arista(grafo,"25",idvertice2,idvertice5));
    print_test("Prueba grafo agregar arista 2-5 de vuelta", !grafo_agregar_arista(grafo,"25",idvertice2,idvertice5));
    print_test("Prueba grafo agregar arista 3-1", grafo_agregar_arista(grafo,"31",idvertice3,idvertice1));
    //cargamos el peso de estas aristas con las ternas pitagoricas para que sea facil probarlas
    print_test("Prueba grafo obtener peso arista 1-2", grafo_peso_arista(grafo,"12")==5);
    print_test("Prueba grafo obtener peso arista 2-3", grafo_peso_arista(grafo,"23")==25);
    print_test("Prueba grafo obtener peso arista 1-4", grafo_peso_arista(grafo,"14")==13);
    print_test("Prueba grafo obtener peso arista inexistente 4-2", grafo_peso_arista(grafo,"42")==-1);

	print_test("Prueba grafo destruir", true);
    grafo_destruir(grafo);
    free(numeros);
}

int main(void){
    pruebas_primitivas_basicas_grafo_vacio();
    pruebas_primitivas_basicas();
    return 0;
}
