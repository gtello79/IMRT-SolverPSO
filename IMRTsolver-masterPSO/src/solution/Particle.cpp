#include "Particle.h"
#include "Plan.h"
using namespace std;

namespace imrt{

	Particle::Particle(Plan& P, float c_1, float c_2, float iner) : Pcurrent(P), PBest(P)
	{
		w = iner;
		c1 = c_1;
		c2 = c_2;
		//calculateFitness();
		PBest.newCopy(Pcurrent);
		fitness = Pcurrent.eval();
		bfitness = Pcurrent.eval();
		for(int i = 0; i < Pcurrent.getStationSize(); i++){
			accept_value.push_back(0);
		}
  };

	void Particle::Velocityupdate(Plan &GBest, int change){
		for(int i = 0; i < Pcurrent.getStationSize(); i++) accept_value[i]=0;
		int stationCount =  Pcurrent.getStationSize();
		int random = (rand())%(stationCount);

		//Giving access to all the beam for the change
    if(change == stationCount){
      for(int i = 0; i < stationCount; i++) accept_value[i]=1;
    }else{
			for(int j = 0; j < change ; j++){
				while(accept_value[random] == 1) random = (rand())%(stationCount);
		  	accept_value[random] = 1;
		  }
		}
    Pcurrent.updateVelocity(GBest, PBest, Pcurrent, w, c1, c2, accept_value);
	};

	void Particle::updatePbest()
	{
		bfitness = Pcurrent.getEvaluation();
		PBest.newCopy(Pcurrent);
	};

	void Particle::calculateFitness()
	{
		fitness = Pcurrent.eval();
		calculateDeltaFitness();

	};

	void Particle::calculateDeltaFitness(){
		for(int i = 0; i < Pcurrent.getStationSize(); i++){
			if(accept_value[i] == 1){
				pair<int,double> pivote;
				pivote.first = 0;
				pivote.second = 0.0;
				list<pair<int,double>> lista;
				lista.push_back(pivote);
				Station *stationRandom;
				stationRandom = Pcurrent.get_station(i);
				cout << fitness <<" "<<Pcurrent.incremental_eval(*stationRandom, lista)<<endl;
				//return Pcurrent.incremental_eval(*stationRandom, lista);
			}
		}
	};

	void Particle::updatePosition(int max_intensity)
	{
		Pcurrent.updatePosition(max_intensity, accept_value);
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
