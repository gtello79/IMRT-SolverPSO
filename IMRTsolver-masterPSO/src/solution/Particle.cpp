#include "Particle.h"
#include "Plan.h"
using namespace std;

namespace imrt{

	Particle::Particle(Plan& P, float c_1, float c_2, float iner) : Pcurrent(P), PBest(P)
	{
		w = iner;
		c1 = c_1;
		c2 = c_2;
		PBest.newCopy(Pcurrent);
		fitness = Pcurrent.eval();
		bfitness = fitness;
		deltafitness = fitness;
  };

	void Particle::Velocityupdate(Plan &GBest, int change){
		//Giving access to all the beam for the change
    Pcurrent.updateVelocity(GBest, PBest, Pcurrent, w, c1, c2,change);
	};

	void Particle::updatePbest()
	{
		mejora++;
		bfitness = Pcurrent.getEvaluation();
		PBest.newCopy(Pcurrent);
	};

	void Particle::calculateFitness()
	{
		fitness = Pcurrent.eval();
	};

	void Particle::calculateDeltaFitness()
	{
		deltafitness = Pcurrent.calculateDeltaFitness();
	};

	void Particle::updatePosition(int max_intensity)
	{
		Pcurrent.updatePosition(max_intensity);
		cout << "Best Personal " << mejora <<": "<< bfitness<<endl;
	};

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
	};

	double Particle::getDeltaFitness()
	{
		return deltafitness;
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
	};
}
