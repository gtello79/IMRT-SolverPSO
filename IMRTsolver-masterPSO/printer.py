#!/usr/bin/env python
import sys
import argparse
import os
import random

parser = argparse.ArgumentParser()
parser.add_argument("-size", "--size",type=int ,help=" Number of particles for the swarm")
parser.add_argument("-max_iter", "--max_iter",type=int, help = " Number of iterations that the program will run")
parser.add_argument("-initial_setup", "--initial_setup", help = "Type of creation for the particles")
parser.add_argument("-iner", "--iner", type=float , help = "Inertia Parameter")
parser.add_argument("-c1", "--c1",type=float, help="Social Factor Parameter")
parser.add_argument("-c2", "--c2",type=float, help="Personal Factor Parameter")
parser.add_argument("-bsize", "--bsize" , type=int, help = "Type of creation for the particles")
parser.add_argument("-vsize","--vsize",type=int, help = "Number of considered worst voxels")
parser.add_argument("-maxdelta","--maxdelta",type = int, help = "Max delta")
parser.add_argument("-maxratio","--maxratio",type = int, help = "Max Ratio")
parser.add_argument("-alpha","--alpha",type = float, help = "Initial temperature for intensities")
parser.add_argument("-beta","--beta",type = float, help = "Initial temperature for ratio ")
parser.add_argument("-maxtime","--maxtime", type=int , help = "Maximum time in seconds")
parser.add_argument("-seed","--seed",type=int ,help = "Seed")
parser.add_argument("-max_apertures","--max_apertures",type=int, help = "Initial intensity for the station ")
parser.add_argument("-open_apertures","--open_apertures",type=int , help = "Number of initialized open apertures")
parser.add_argument("-initial_intensity", "--initial_intensity",type=int , help = "Initial value aperture")
parser.add_argument("-max_intensity","--max_intensity",type=int , help = "Step size for aperture intensity")
parser.add_argument("-pso_iter", "--pso_iter", type=int, help = "Number of iteration that the pso will run")
parser.add_argument("-changes_beam","--changes_beam", type= int, help= "Choose the beam number to operate")
parser.add_argument("-diff_setup", "--diff_setup", type= int, help = "Add a particle with a different setup")
parser.add_argument("-prob_aperture", "--prob_aperture", type = float , help = "Add a little condition to the movement of one or all the apertures")
args = parser.parse_args()

size = 10
max_iter = 100
initial_setup = 4
iner=1.0
c1=1.0
c2 = 1.0
bsize=20
vsize=50
maxdelta=5
maxratio=3
alpha=1.0
beta=1.0
maxtime=0
max_apertures = 5
open_apertures=-1
initial_intensity=2
max_intensity=28
step_intensity=2
seed = random.randint(0, sys.maxint)
write_report = False
pso_iter = 1
changes_beam = 1
diff_setup = 0
prob_aperture = 0

# Aqui procesamos lo que se tiene que hacer con cada argumento
if args.size:
    size = args.size
if args.max_iter:
    max_iter = args.max_iter
if args.initial_setup:
    initial_setup = args.initial_setup
if args.iner:
    iner = args.iner
if args.c1:
    c1 = args.c1
if args.c2:
    c2 = args.c2
if args.bsize:
    bsize = args.bsize
if args.vsize:
    vsize = args.vsize
if args.maxdelta:
    maxdelta = args.maxdelta
if args.maxratio:
    maxratio = args.maxratio
if args.alpha:
    alpha = args.alpha
if args.beta:
    beta = args.beta
if args.maxtime:
    maxtime = args.maxtime
if args.seed:
    seed = args.seed
if args.max_apertures:
    max_apertures = args.max_apertures
if args.open_apertures:
    open_apertures = args.open_apertures
if args.initial_intensity:
    initial_intensity = args.initial_intensity
if args.max_intensity:
    max_intensity = args.max_intensity
if args.changes_beam:
	changes_beam = args.changes_beam
if args.pso_iter:
    pso_iter = args.pso_iter
if args.diff_setup:
    diff_setup = args.diff_setup
if args.prob_aperture:
    prob_aperture = args.prob_aperture
##Erasing the files of the last iteration
os.system("rm -r Values")
os.system("mkdir Values")

##It's the call to the function
detalles = open("Values/Descripcion.txt","w")
for i in range(0,pso_iter):
	descripcion = "PSO --diff_setup "+str(diff_setup)+" --max_iter "+str(max_iter)+" --size "+str(size)+" --iner "+str(iner)+" --c1 "+str(c1)+" --c2 "+str(c2)+" --initial_setup "+str(initial_setup)+" --bsize "+str(bsize)+" --vsize "+str(vsize)+" --maxdelta "+str(maxdelta)+" --maxtime "+str(maxtime)+" --maxratio "+str(maxratio)+" --alpha "+str(alpha)+" --beta "+str(beta)+" --max_apertures "+str(max_apertures)+" --open_apertures "+str(open_apertures)+" --initial_intensity "+str(initial_intensity)+" --max_intensity "+str(max_intensity)+" --changes_beam "+str(changes_beam)
	detalles.write("PSO Iteracion "+str(i)+"\n")
	detalles.write(descripcion+"\n")
    	print(seed)
	os.system("./PSO --diff_setup "+str(diff_setup)+" --seed "+str(seed)+ " --max_iter "+str(max_iter)+" --size "+str(size)+" --iner "+str(iner)+" --c1 "+str(c1)+" --c2 "+str(c2)+" --initial_setup "+str(initial_setup)+" --bsize "+str(bsize)+" --vsize "+str(vsize)+" --maxdelta "+str(maxdelta)+" --maxtime "+str(maxtime)+" --maxratio "+str(maxratio)+" --alpha "+str(alpha)+" --beta "+str(beta)+" --max_apertures "+str(max_apertures)+" --open_apertures "+str(open_apertures)+" --initial_intensity "+str(initial_intensity)+" --max_intensity "+str(max_intensity)+" --changes_beam "+str(changes_beam)+" --prob_aperture "+str(prob_aperture)+" >> Values/resultado"+str(i)+".txt")
	seed = seed+1
detalles.close()

##Now we try to show only the final line with the best solution of the iteration
Best = open("Values/BestSolutions.txt",'w')
final_line=""
for i in range(pso_iter):
    archivo = file("Values/resultado"+str(i)+".txt",'r')
    print("PSO "+str(i))
    for linea in archivo:
        reading = linea.strip().split(" ")
        if(reading[0] == "##"):
			if(len(reading) > 1):
				if(reading[1]=="Best"):
					print(linea)
    archivo.close()
Best.close()
