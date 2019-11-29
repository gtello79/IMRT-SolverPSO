# encoding: utf-8
import matplotlib.pyplot as plt
import sys
import argparse
import os.path

archivo = sys.argv[1]
iteracion = sys.argv[2]
values = []
par = []
x = []
y = []
write = False
particles = 0
if os.path.exists(archivo):
    lectura = open(archivo,"r")
    for linea in lectura:
        nuevo = linea.strip().split(" ")
        if(nuevo[0] == "Iteracion:"):
            if(nuevo[1] == str(iteracion)):
                write = True
            else: write = False
        if(write):
            if(nuevo[0] == "Actual"):
                particles = particles+1
                y.append(float(nuevo[2]))
                x.append(particles)

    #par = (int(nuevo[-1]),float(nuevo[4]))
    #NewX = int(nuevo[-1])
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
