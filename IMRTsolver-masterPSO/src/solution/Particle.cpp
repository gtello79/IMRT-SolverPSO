#include "Particle.h"
#include "Plan.h"
using namespace std;

namespace imrt{

	Particle::Particle(Plan& P, float c_1, float c_2, float iner) : Pcurrent(P), PBest(P) 
	{
		w = iner;
		c1 = c_1;
		c2 = c_2;
		calculateFitness();
		PBest.newCopy(Pcurrent);
		bfitness = Pcurrent.eval();
  };

	void Particle::Velocityupdate(Plan &GBest, int change){
    Pcurrent.updateVelocity(GBest, PBest, Pcurrent, w, c1, c2, change);
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
		cout << "Best Personal: " << bfitness<<endl;
	}

	Plan& Particle::GetPCurrent()
	{
		return Pcurrent;
	};

	Plan& Particle::GetPbest()
	{
		return PBest;
	};

	double Particle::getFitness()
	{
		return fitness;
	};

	double Particle::getBfitness()
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
