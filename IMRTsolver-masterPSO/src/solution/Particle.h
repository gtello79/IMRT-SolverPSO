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
      float w;
      float c1; //FACTOR PERSONAL POR AHORA FIJO, DESPUES POR PARAMETRO
      float c2; //FACTOR GLOBAL, POR AHORA FIJO LUEGO POR PARAMETRO
      double fitness; //EVALUATION OF PCURRENT
      double deltafitness;
      double bfitness; //EVALUATION OF PBEST;
      Plan PBest; //BEST GLOBAL VALUE
      Plan Pcurrent; //BEST PERSONAL VALUE
      vector<int> accept_value;
      int mejora = 0;
      float prob_aperture;

    public:
  	    //Builder of Particle class
  	    Particle(Plan& P, float c_1 , float c_2 , float iner, float prob_aperture);
  	    //Function with the process for calculate the velocity of the particle using the stations
        void Velocityupdate(Plan &GBest, int change);
        //Function for calculate the position of the solution using PSO
        void calculateFitness(int change);
        //Function for update Pbest
        void updatePosition(int max_intensity);

        void updatePbest();

        Plan& GetPbest();

        Plan& GetPCurrent();

        double getFitness();

        double getBfitness();

        double getDeltaFitness();

        void setbfitness();

        void printVelocities();

        void printIntensities();

        void calculateDeltaFitness();

        

  };
}
#endif /*PARTICLE.H*/
