#include "iostream"
#include "vector"
#include "Particle.h"
#include "Plan.h"

using namespace std;
using namespace imrt;

Plan *BGlobal; //No se si esta correcto...

/*
void calculateFitness(vector <Particle> solution, Particle *best){
	for (int i = 0; i < 5; i++)
	{
		Particle P = solution[i];
		P.calculateVelocityUpdate();

	Plan }

}
*/
void searchGlobal(vector<Particle*> &solution, int size){
	int i = 0;
	Particle *aux = solution.begin();
	BGlobal->newCopy(solution[0]->PBest);
	for (int i = 0 ; i < size;i++){
		if ((solution[i])->PBest.eval() < BGlobal->eval()){
			BGlobal->newCopy(solution[i]->PBest)
		}
	}
}

int main(){
	int i; 
	int size;
	int max_iter = 3;
	int _type_ = 1;
	vector <Particle*> solution[size] ;//inicializar con parametro sizeB

	//Formation of the particle set
	for(i = 0; i < 5; i++){
		//Agregar condiciones nueva para generar un plan
		Plan ADD;
		Particle* nueva_part = new Particle(ADD);
		if(nueva_part->fitness < nueva_part.bfitness){
			nueva_part.bfitness = nueva_part.fitness;
			nueva_part.updatePbest(nueva_part.PCurrent); // nueva.bestglobal
			//BGlobal = newCopy(nueva.solucion);
		}
		solution.insert(nueva_part);
	}
	searchGlobal(solution);

	//for e iteracionces
	for(i=0 ; i!= max_iter; i++){
		for(i = 0; i<5; i++){

			solution[i]->Velocityupdate(&BGlobal, _type_,1,1,1,1,1);
			solution[i]->updatePosition();
			solution[i].calculateFitness(); 	//Una funcion que calcule los fitness y lo asigna
			if(solution[i].fitness < solution[i].bfitness){
				solucion[i].bfitness = solucion[i].fitness;
				nueva.updatePbest(nueva.solucion);
			}
			searchGlobal(solution);
		}
	}
	return 0;
}
