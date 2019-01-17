

#include "Particle.h"
#include "Plan.h"
using namespace std;

namespace imrt{

	Particle::Particle(Plan& P){
        Pcurrent.newCopy(P);
		w = 1;
		c1 = 1;
		c2 = 1;
		r1 = 1;
		r2 = 1;
		calculateFitness();
		bfitness = 99999999;
		PBest.newCopy(P);; //colocar new o no?
	};
	void Particle::Velocityupdate(Plan &GBest, int type, int w, int c1, int c2, int r1, int r2){
		if(type == 1){
			Pcurrent.updateVelocity(&GBest, &PBest, &Pcurrent ,w, c1, c2, r1, r2);
		}else if(type == 2){
			/*
			*	Segunda implementacion de velocidad
			*/
		}
	}

	void Particle::calculateFitness(){
		fitness = Pcurrent.eval();
	}

	void Particle::updatePbest(Plan &New){
		PBest.newCopy(New);
	}

	void Particle::updatePosition(){
		Pcurrent.updatePosition();
	}
	Plan& Particle::GetPCurrent(){
		return Pcurrent;
	};
	Plan& Particle::GetPbest(){
		return PBest;
	};
	int Particle::Getfitness(){
		return fitness;
	};
	void Particle::setbfitness(int insert){
		bfitness = insert;
	}
}
