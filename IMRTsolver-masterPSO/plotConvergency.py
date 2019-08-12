# encoding: utf-8
import matplotlib.pyplot as plt
matplotlib.use('Agg')
import sys
import argparse
import os.path

archivo = sys.argv[1]
best_values = set()
par = []
x = []
y = []
if os.path.exists(archivo):
    lectura = open(archivo,"r")

    for linea in lectura:
        nuevo = linea.strip().split(" ")
        if(nuevo[0] == "Best"):
            if(nuevo[1] == "Global"):
                #par = (int(nuevo[-1]),float(nuevo[4]))
                NewX = int(nuevo[-1])
                NewY = float(nuevo[4])
                if(not(NewX in x)):
                  x.append(int(nuevo[-1]))
                  y.append(float(nuevo[4]))
    #for i in best_values:
    #  x.append(i[0])
    #  y.append(i[1])
    print(x)
    print(y)
    plt.plot(x,y,'o-')
    plt.grid()
    plt.xlim(0)
    plt.ylim(0)
    plt.title("Plot de Convergencia")
    plt.show()
    lectura.close()
else:
    print("No existe el archivo")
