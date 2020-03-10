#Author: Gonzalo Tello Valenzuela
#Date: 10-03-2020

import matplotlib.pyplot as plt
import sys
import argparse
import os.path

archivo = sys.argv[1]
best_values = set()
x = []
y = []
if os.path.exists(archivo):
    lectura = open(archivo,"r")
    contador = 0
    for linea in lectura:
        if (contador !=0):
            linea_actual = linea.strip()
            y.append(float(linea_actual))
            x.append(contador-1)
        contador = contador+1
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
