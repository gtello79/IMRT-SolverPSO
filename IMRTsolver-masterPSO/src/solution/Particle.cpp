

#include "Particle.h"
#include "Plan.h"
using namespace std;

namespace imrt{

	Particle::Particle(Plan& P) : Pcurrent(P), PBest(P) 
	{
		w = 1;
		c1 = 1;
		c2 = 1;
		r1 = rand();
		r2 = rand();
		calculateFitness();
		bfitness = Pcurrent.getEvaluation();
	};

	void Particle::Velocityupdate(Plan &GBest, int type, int w, int c1, int c2)
	{
		if(type == 1)
		{
			Pcurrent.updateVelocity(&GBest, &PBest, &Pcurrent ,w, c1, c2, r1, r2);

		}else if(type == 2)
		{
			/*
			*	Segunda implementacion de velocidad
			*/
		}
	}
	void Particle::calculateFitness()
	{
		fitness = Pcurrent.getEvaluation();
	};

	void Particle::updatePbest()
	{
		PBest.newCopy(Pcurrent);
	}

	void Particle::updatePosition()
	{
		Pcurrent.updatePosition();
	}

	Plan& Particle::GetPCurrent()
	{
		return Pcurrent;
	};

	Plan& Particle::GetPbest()
	{
		return PBest;
	};

	double Particle::Getfitness()
	{
		return fitness;
	};

	void Particle::setbfitness()
	{
		bfitness = PBest.getEvaluation();
	};
}
