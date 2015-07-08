#!/usr/bin/env python
'''
Created on 30/11/2013

@author: Flor
'''

import sys


from oficinaPapaNoel import Oficina


def main():
    #sys.setrecursionlimit(30000) #temporal

    # lectura de argumentos

    pesoMaximo = sys.argv[1]
    idEsquinaPoloNorte = sys.argv[2]
    archivoFabricas = sys.argv[3]
    archivoJugutes = sys.argv[4]
    archivoMapa = sys.argv[5]

    oficina = Oficina(archivoMapa,archivoFabricas,archivoJugutes,idEsquinaPoloNorte,pesoMaximo)

    # lectura de parametros

    for linea in sys.stdin.readlines():
        linea = linea.replace("\n","").replace("\r","")
        parametros = linea.split(' ')
        comando = parametros[0]
        if (comando == "listar_fabricas"):
            resultado = oficina.seleccionarFabricas()
            print("Cantidad: "+ str(resultado[0]))
            for fab in resultado[1]:
                print(fab.getHorarioConId())
        elif (comando == "camino_optimo"):
            resultado = oficina.getCaminoOptimoFabrica(parametros[1])
            if (resultado[0] == -1):
                print("Error : la fabrica con id " + parametros[1] + " no existe")
            else:
                print("Distancia: " + str(resultado[1]) + " Metros")
                for esquina in resultado[2]:
                    print(esquina.getUbicacion())
        elif (comando == "valuar_juguetes"):
            resultado = oficina.valuarJuguetes(parametros[1])
            if (resultado[0] == -1):
                print("Error : la fabrica con id "+parametros[1]+ " no existe")
            else:
                print("Total: " + str(resultado[1][0]) + " Sonrisas") # tupla dentro de tupla
        elif (comando == "valuar_juguetes_total"):
            resultado = oficina.valuarJugutesTotal()
            print("Total: "+ str(resultado[1])+" Sonrisas")

        elif (comando == "listar_juguetes"):
            resultado = oficina.valuarJuguetes(parametros[1])
            if (resultado[0] == -1):
                print("Error : la fabrica con id "+parametros[1]+ " no existe")
            else:
                print("Total: "+ str(resultado[1][0])+" Sonrisas")
                for juguete in resultado[1][1]:
                    print(juguete.getId())
		
############ Pruebas #############
if __name__ == "__main__":main()
