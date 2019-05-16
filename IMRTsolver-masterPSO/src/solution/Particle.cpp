#include "Particle.h"
#include "Plan.h"
using namespace std;

namespace imrt{

	Particle::Particle(Plan& P) : Pcurrent(P), PBest(P) 
	{
		w = 1;
		c1 = 1;
		c2 = 1;
		r1 = 1;//((double)rand()/(RAND_MAX));
		r2 = 1;//((double)rand()/(RAND_MAX));
		calculateFitness();
		//mejor = new Plan(Pcurrent);
		PBest.newCopy(Pcurrent);
		bfitness = Pcurrent.eval();
	};

	void Particle::Velocityupdate(Plan &GBest,float w, float c1, float c2){
		Pcurrent.updateVelocity(GBest, PBest, Pcurrent ,w, c1, c2);
	};
	
	void Particle::updatePbest()
	{		
		bfitness = Pcurrent.getEvaluation();	
		PBest.newCopy(Pcurrent);
	};

	void Particle::calculateFitness()
	{
		fitness = Pcurrent.eval();
	};	

	void Particle::updatePosition(int max_intensity)
	{
		Pcurrent.updatePosition(max_intensity);
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

	double Particle::getbfitness()
	{
		return bfitness;
	}

	void Particle::setbfitness()
	{
		bfitness = PBest.eval();
	};

	void Particle::printIntensities()
	{
		Pcurrent.printIntensities();
	};
	
	void Particle::printVelocities()
	{
		Pcurrent.printVelocities();
	}

}
