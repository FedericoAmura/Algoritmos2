'''
Created on 05/12/2013

@author: Flor
'''

import math # importa funciones matematicas para poder usar sqrt y pow

class Esquina(object):
    '''
    classdocs
    '''

    def __init__(self, idEsquina , x, y, latitud, longitud):
        '''
        Constructor

        '''
        self.id = idEsquina
        self.latitud = latitud
        self.longitud = longitud
        self.x = float(x) 
        self.y = float(y)
        self.fabricas = dict()

    # Toma el id de la esquina
    def getId(self):
        return self.id

    # Anade una nueva fabrica al diccionario de fabricas
    def addFabrica(self,fabrica):
        self.fabricas[fabrica.getId()] = fabrica

    # Devuelve las fabricas
    def getFabricas(self):
        return self.fabricas

    # Toma la longitud
    def getLongitud(self):
        return self.longitud

    # Toma la latitud
    def getLatitud(self):
        return self.longitud

    # Devuelve la fabrica con el idFabrica
    def getFabrica(self, idFabrica):
        return self.fabricas[idFabrica]

    # Devuelve latitud y longitud con el formato correcto
    def getUbicacion(self):
        return self.latitud + "," + self.longitud

    # Si hay fabrica de id k en la esquina.
    # has_key devuelve true si esta k en el dict, false en caso contrario
    def tieneFabricaEnVertice(self, k):
        return self.fabricas.has_key(k)

    # Calcula distancia entre esquinas
    # math.pow eleva al cuadrado el primer parametro
    def calcularDistancia(self, otraEsquina):
        return math.sqrt(math.pow(self.x - otraEsquina.x, 2) + math.pow(self.y - otraEsquina.y, 2))
