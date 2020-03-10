#Author: Gonzalo Tello Valenzuela
#Day: 10-03-2020

#HINT: Considerar que en el archivo de configuraciones tanto size como max_iter deben tener la misma cantidad de datos, sino ocurrira un index error
#La forma que debe ser escrito size y max_iter es en el orden que el usuario desea usar las pruebas

import sys
import argparse
import os.path
import random
from datetime import datetime


class instance(object):                                                         #Clase que contendra todos los datos de la instancia desde el archivo "config"
    def __init__(self,configPSO):
        super(instance, self).__init__()
        self.__params = dict()
        PSO_file = open(configPSO,'r')
        for linea in PSO_file:
            par = linea.strip().split(';')
            self.__params[par[0]] = par[1]
        self.__size = self.getParam('size')
        self.__max_iter = self.getParam('max_iter')

    def getParam(self,parametro):                                               #Se obtienen los parametros que se utilizaran para correr el algoritmo
        return self.__params[parametro]

    def getSize (self,index):                                                   #Se obtiene una lista de strings con los valores utilizados para size
        cantidad = (self.__params['size']).split(',')
        return cantidad[index]

    def getIter (self,index):                                                   #Se obtiene una lista de strings con los valores utilizados para size
        Iteraciones = (self.__params['max_iter']).split(',')
        return Iteraciones[index]

tiempo_inicial = datetime.now()                                                 #Toma del tiempo inicial
instances_file = sys.argv[1]                                                    #Se lee el archivo de instancias
if(os.path.isfile(instances_file)):
    instancia = instance(instances_file)

    if(os.path.exists('ResultsPSO')):                                           #Se elimina un el archivo ResultsPSO en caso que aun se encuentren resultados de pruebas anteriores
        os.system("rm -r ResultsPSO")
    os.mkdir('ResultsPSO')

    nTest = int(instancia.getParam('nTest'))                                    #nuevo de pruebas a realizar para comparar size o iter
    cant = len(instancia.getParam('size').split(','))
    for iter in range(0, cant):
        for level in range(0,nTest):
            seed = int(instancia.getParam('seed'))
            if(seed<0):                                                         #Se genera una semilla random para ejecutar el algoritmo
                seed = random.randint(0, sys.maxint)

            file_name = 'ResultsPSO/test'+str(iter)+'.'+str(level)+'.txt'       #Se guarda el nombre del archivo a generar
            pivote_name = "ResultsPSO/resultado"+str(iter)+'.'+str(level)+'.txt'#Se guarda el nombre del archivo que guardara provisoriamente los datos del algoritmo

            os.system("./PSO --diff_setup "+instancia.getParam('diff_setup')+" --seed "+str(seed)+ " --max_iter "+instancia.getIter(iter)+" --size "+instancia.getSize(iter)+" --iner "+instancia.getParam('iner')+" --c1 "+instancia.getParam('c1')+" --c2 "+instancia.getParam('c2')+" --initial_setup "+instancia.getParam('initial_setup')+" --bsize "+instancia.getParam('bsize')+" --vsize "+instancia.getParam('vsize')+" --maxdelta "+instancia.getParam('maxdelta')+" --maxtime "+instancia.getParam('maxtime')+" --maxratio "+instancia.getParam('maxratio')+" --alpha "+instancia.getParam('alpha')+" --beta "+instancia.getParam('beta')+" --max_apertures "+instancia.getParam('max_apertures')+" --open_apertures "+instancia.getParam('open_apertures')+" --initial_intensity "+instancia.getParam('initial_intensity')+" --max_intensity "+instancia.getParam('max_intensity')+" --changes_beam "+instancia.getParam('changes_beam')+" --prob_aperture "+instancia.getParam('prob_aperture')+" >> " + pivote_name)         #Se ejecuta el algoritmo con distintas semillas

            to_write = open(file_name,'w')
            pivote = open(pivote_name,'r')

            to_write.write("diff_setup "+instancia.getParam('diff_setup')+" seed "+str(seed)+ " max_iter "+instancia.getIter(iter)+" size "+instancia.getSize(iter)+" iner "+instancia.getParam('iner')+" c1 "+instancia.getParam('c1')+" c2 "+instancia.getParam('c2')+" initial_setup "+instancia.getParam('initial_setup')+" bsize "+instancia.getParam('bsize')+" vsize "+instancia.getParam('vsize')+" maxdelta "+instancia.getParam('maxdelta')+" maxtime "+instancia.getParam('maxtime')+" maxratio "+instancia.getParam('maxratio')+" alpha "+instancia.getParam('alpha')+" beta "+instancia.getParam('beta')+" max_apertures "+instancia.getParam('max_apertures')+" open_apertures "+instancia.getParam('open_apertures')+" initial_intensity "+instancia.getParam('initial_intensity')+" max_intensity "+instancia.getParam('max_intensity')+" changes_beam "+instancia.getParam('changes_beam')+" prob_aperture "+instancia.getParam('prob_aperture')+'\n')

            for linea in pivote:                                                #Se guarda solo el valor de los mejores best Global
                segmento = linea.split(" ")
                if(segmento[0]=='Best' and segmento[1]=='Global'):
                        to_write.write(segmento[5]+'\n')

            to_write.close()
            pivote.close()
            os.remove(pivote_name)

else:
    print("FILE DON'T FOUND")
tiempo_final = datetime.now()
tiempo = tiempo_final - tiempo_inicial
print("Time: "+str(tiempo))                                                     #Se entrega el tiempo que tomo todo el experimento
