#!/usr/bin/env python
import sys
import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument("-size", "--size",type=int ,help=" Number of particles for the swarm")
parser.add_argument("-max_iter", "--max_iter",type=int, help = " Number of iterations that the program will run")
parser.add_argument("-initial_setup", "--initial_setup", help = "Type of creation for the particles")
parser.add_argument("-iner", "--iner", type=int , help = "Inertia Parameter")
parser.add_argument("-c1", "--c1",type=int, help="Social Factor Parameter")
parser.add_argument("-c2", "--c2",type=int, help="Personal Factor Parameter")
parser.add_argument("-bsize", "--bsize" , type=int, help = "Type of creation for the particles")
parser.add_argument("-vsize","--vsize",type=int, help = "Number of considered worst voxels")
parser.add_argument("-maxdelta","--maxdelta",type=float, help = "Max delta")
parser.add_argument("-maxratio","--maxratio",type=float, help = "Max Ratio")
parser.add_argument("-alpha","--alpha",type=float, help = "Initial temperature for intensities")
parser.add_argument("-beta","--beta",type=float, help = "Initial temperature for ratio ")
parser.add_argument("-maxtime","--maxtime",type=int , help = "Maximum time in seconds")
parser.add_argument("-seed","--seed",type=int ,help = "Seed")
parser.add_argument("-max_apertures","--max_apertures",type=int, help = "Initial intensity for the station ")
parser.add_argument("-open_apertures","--open_apertures",type=int , help = "Number of initialized open apertures")
parser.add_argument("-initial_intensity", "--initial_intensity",type=int , help = "Initial value aperture")
parser.add_argument("-max_intensity","--max_intensity",type=int , help = "Step size for aperture intensity")
parser.add_argument("-pso_iter", "--pso_iter", type=int, help = "Number of iteration that the pso will run")

args = parser.parse_args()

size  = 10
max_iter = 100
initial_setup = 5
iner=1 
c1=1 
c2 = 1
bsize=20  
vsize=50
maxdelta=5.0
maxratio=3.0
alpha=1.0
beta=1.0
maxtime=0
max_apertures=5
open_apertures=-1
initial_intensity=2
max_intensity=28
step_intensity=2
seed = 0
write_report = False
pso_iter = 10

# Aquí procesamos lo que se tiene que hacer con cada argumento
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
if args.pso_iter:
    pso_iter = args.pso_iter

os.system("mkdir Values")
for i in range(0,pso_iter):
    os.system("./PSO --max_iter "+str(max_iter)+"--size "+str(size)+"--initial_setup "+str(initial_setup)+"--iner "+str(iner)" --c1 "+str(c1)+" --c2 "+str(c2)+" --bsize "+str(bsize)+" --vsize "+str(vsize)+" --maxdelta "+str(maxdelta)+" --maxratio "+str(maxratio)+" --alpha "+str(alpha)+" --beta "+str(beta)+" --maxtime "+str(maxtime)+" --max_apertures "+str(max_apertures)+" --open_apertures "+str(open_apertures)+" --initial_intensity "+str(initial_intensity)+" --max_intensity "+str(max_intensity) +" --step_intensity "+str(step_intensity)+ " --seed "+str(seed) +" >>Values/resultado"+str(i)+".txt")
    seed = seed + 1
