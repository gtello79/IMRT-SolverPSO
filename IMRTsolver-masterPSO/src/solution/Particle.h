/*
*
* Particle.h
*
* Created on: 04-01-2018
* Author: Felipe Jorquera - Gonzalo Tello
*
*/

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Station.h"
#include "Plan.h"
#include <vector>

namespace imrt{

  class Particle{ //Swarm

    private:
      int w;
      int c1; //FACTOR PERSONAL POR AHORA FIJO, DESPUES POR PARAMETRO
      int c2; //FACTOR GLOBAL, POR AHORA FIJO LUEGO POR PARAMETRO
      double fitness; //EVALUATION OF PCURRENT
      double bfitness; //EVALUATION OF PBEST;
      double r1;
      double r2;
      Plan PBest; //BEST GLOBAL VALUE
      Plan Pcurrent; //BEST PERSONAL VALUE
      Plan *aux;

    public:
  	    //Builder of Particle class
  	    Particle(Plan& P);

  	    //Function with the process for calculate the velocity of the particle using the stations
        void Velocityupdate(Plan &GBest, int type, int w, int c1, int c2);
        //Function for calculate the position of the solution using PSO
        void calculateFitness();
        //Function for update Pbest
        void updatePosition();

        void updatePbest();

        Plan& GetPbest();

        Plan& GetPCurrent();

        double Getfitness();

        void setbfitness();

        void printIntensities();

        void printVelocities();


  };
}
#endif /*PARTICLE.H*/
