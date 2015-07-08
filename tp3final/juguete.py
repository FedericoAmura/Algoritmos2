'''
Created on 08/12/2013

@author: Flor
'''

class Juguete(object):
    '''
    classdocs
    '''


    def __init__(self, idJuguete, valor, peso):
        '''
        Constructor
        '''
        self.id = idJuguete
        self.valor = int(valor) 
        self.peso = int(peso) 

    # Devuelve el id del juguete
    def getId(self):
        return self.id

    # Devuelve el valor del juguete
    def getValor(self):
        return self.valor

    # Devuelve el peso del juguete
    def getPeso(self):
        return self.peso


    # Compara los id entre juguetes
    def __cmp__(self,juguete):
        if (self.id > juguete.id):
            return 1
        elif (self.id < juguete.id):
            return -1
        else:
            return 0


    
    # Compara valores, pesos e id
    def compare(self,juguete):
        if (self.valor > juguete.valor):
            return 1
        elif (self.valor < juguete.valor):
            return -1
        else:
            if (self.peso < juguete.peso):
                return 1
            elif (self.peso > juguete.peso):
                return -1
            else:
                if (self.id < juguete.id):
                    return 1
                if (self.id > juguete.id):
                    return -1
        return 0

