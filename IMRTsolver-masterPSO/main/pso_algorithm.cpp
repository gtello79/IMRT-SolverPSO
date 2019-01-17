#include <iostream>
#include <vector>
#include <stack>
#include "Particle.h"
#include "Plan.h"
#include "Collimator.h"
#include "Volume.h"
#include "EvaluationFunction.h"

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
set<int> get_angles(string file, int n){
  ifstream _file(file.c_str(), ios::in);
  string line;

  if (! _file)
    cerr << "ERROR: unable to open instance file: " << file << ", stopping! \n";

  //cout << "##Reading volume files." << endl;
  getline(_file, line);
  _file.close();

  set<int> angles;
  stack<string> q;
  char delim=' ';
  std::size_t current, previous = 0;
  current = line.find(delim);
  //cout << line << endl;
  while (current != std::string::npos) {
    angles.insert(atoi(line.substr(previous, current - previous).c_str()));
    previous = current + 1;
    current = line.find(delim, previous);
  }
  angles.insert(atoi(line.substr(previous, current - previous).c_str()));

  return angles;
}

vector<Volume> createVolumes (string organ_filename, Collimator& collimator){
  ifstream organ_file(organ_filename.c_str(), ios::in);
  vector<string> organ_files;
  vector<Volume> volumes;
  string line;

  if (! organ_file)
    cerr << "ERROR: unable to open instance file: " << organ_filename << ", stopping! \n";

  cout << "##Reading volume files." << endl;
  getline(organ_file, line);
  while (organ_file) {
    getline(organ_file, line);
    if (line.empty()) continue;
    cout << "##  " << line << endl;
    //Assuming one data point
    organ_files.push_back(line);
  }
  organ_file.close();
  cout << "##  Read " << organ_files.size() << " files"<< endl;

  for (int i=0; i<organ_files.size(); i++)
    volumes.push_back(Volume(collimator, organ_files[i]));

  return(volumes);
}

void searchGlobal(vector<Particle> &solution, int size){
	int i = 0;
	Particle* aux = &solution[0];
	BGlobal->newCopy(aux->GetPCurrent());
	for (int i = 0 ; i < size;i++){
		if ((solution[i].GetPbest()).eval() < BGlobal->eval()){
			BGlobal->newCopy(aux->GetPbest());
		};
	}
}

int main(){
	int i; 
	int size;
	int max_iter = 3;
	int _type_ = 1;
	int vsize=50;
 	int bsize=20;
  	int maxiter=5000;
  	int maxtime=0;
  	int max_apertures=5;
  	int open_apertures=-1;
  	double alpha=1.0;
  	double beta=1.0;
  	double maxdelta=5.0;
  	double maxratio=3.0;
  	bool search_aperture=false;
  	bool search_intensity=false;
	string file="data/testinstance_0_70_140_210_280.txt";
  	string file2="data/test_instance_coordinates.txt";
	int initial_intensity=2;
  	int max_intensity=28;
  	int step_intensity=2;
  	int initial_setup;


	vector <Particle> solution[size] ;//inicializar con parametro sizeB
	vector<double> w={1,1,1};
  	vector<double> Zmin={0,0,76};
  	vector<double> Zmax={65,60,1000};
	Collimator collimator(file2, get_angles(file, 5));
  	vector<Volume> volumes= createVolumes (file, collimator);
	
	//Formation of the particle set
	for(i = 0; i < 5; i++){
		//Agregar condiciones nueva para generar un plan
		Plan ADD(w, Zmin, Zmax, collimator, volumes, max_apertures, max_intensity, initial_intensity, step_intensity, open_apertures, 4);

		Particle* nueva_part = new Particle(ADD);
		if(nueva_part->GetPCurrent().eval() < nueva_part->GetPbest().eval()){
			nueva_part->setbfitness(nueva_part->Getfitness());
			nueva_part->updatePbest(nueva_part->GetPCurrent()); // nueva.bestglobal
			//BGlobal = newCopy(nueva.solucion);
		};
		solution->insert(nueva_part);
	}
	searchGlobal(solution);

	//for e iteracionces
	for(i=0 ; i!= max_iter; i++){
		for(i = 0; i<5; i++){

			(solution[i])->Velocityupdate(&BGlobal, _type_,1,1,1,1,1);
			solution[i]->updatePosition();
			solution[i]->calculateFitness(); 	//Una funcion que calcule los fitness y lo asigna
			if(solution[i]->Getfitness() < solution[i]->GetPbest().eval()){
				solucion[i]->setbfitness(solucion[i].Getfitness());
				//Que es nueva?
				nueva->updatePbest(nueva.solucion());
			}
			searchGlobal(solution);
		}
	}
	return 0;
}
