#include <iostream>
#include <vector>
#include <stack>
#include <iterator>
#include <set>
#include <unistd.h>

#include "Particle.h"
#include "Plan.h"
#include "Collimator.h"
#include "Volume.h"
#include "EvaluationFunction.h"
#include "args.hxx"

using namespace std;
using namespace imrt;


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
};


void searchGlobal(vector<Particle> &solution, int size, Plan &BGlobal){
	int i = 0;
	for (int i = 1 ; i < size;i++){
		if ((solution[i].GetPbest()).getEvaluation() <= BGlobal.getEvaluation()){
			BGlobal.newCopy(solution[i].GetPCurrent());
		};
	}
}

int main(){
	int i,j; 
	int n = 1;
	int size=5;
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
	int initial_intensity=2;
  int max_intensity=10;
  int step_intensity=2;
  int initial_setup;

	string strategy="dao_ls";
	string file="data/testinstance_0_70_140_210_280.txt";
  string file2="data/test_instance_coordinates.txt";
	


	vector <Particle> solution ;//inicializar con parametro sizeB
	vector<double> w={1,1,1};
  	vector<double> Zmin={0,0,76};
  	vector<double> Zmax={65,60,1000};
	Collimator collimator(file2, get_angles(file, 5));
  	vector<Volume> volumes= createVolumes (file, collimator);
	
	
	//Formation of the particle set
	Plan *Opc;
	cout<<"\n \n"<<endl;
	for(i = 0; i < 5; i++)
	{	//Agregar condiciones nueva para generar un plan
		cout << "Particula N° : " << i+1 << endl;
		Opc = new Plan(w, Zmin, Zmax, collimator, volumes, max_apertures, max_intensity, initial_intensity, step_intensity, open_apertures, 4);
		solution.push_back(Particle(*Opc));
		if(solution[i].GetPCurrent().getEvaluation() < solution[i].GetPbest().getEvaluation())
		{
			solution[i].updatePbest();
			solution[i].setbfitness();
		};
		delete(Opc);
	}
	
	Plan *BGlobal;
	BGlobal = new Plan(solution[0].GetPCurrent());
	for(int i = 1; i <5 ; i++){
			if(BGlobal->getEvaluation() > solution[i].Getfitness() ){
				BGlobal->newCopy(solution[i].GetPCurrent());
			}
	}
	
	cout << "###Particles Created, initial best: " << BGlobal->getEvaluation() <<endl;
	/*searchGlobal(solution, size, *BGlobal);*/

	for(j = 0; j < 3;j++)
	{	
		cout << "\n Iteracion: "<< j+1 <<endl;
		for(int i = 0; i<5; i++)
		{	
			cout << "Particula N°: " << i+1 ;
		  solution[i].printVelocities();
			solution[i].Velocityupdate(*BGlobal, _type_,1,1,1);
      solution[i].printVelocities();
      solution[i].printIntensities();
			solution[i].updatePosition();
			solution[i].printIntensities();
			solution[i].calculateFitness(); 
			solution[i].updatePbest();
			cout << solution[i].Getfitness() <<"\n"<<endl;
		};
		for(int i = 0; i <5 ; i++){
			if(BGlobal->getEvaluation() > solution[i].Getfitness() ){
				BGlobal->newCopy(solution[i].GetPCurrent());
			}
		}
		cout<<"Best Global Iteration n:"<<j<<": " << BGlobal->getEvaluation()<<endl;
	};
	cout << "##**************************************************************************"<< endl;
  cout << "##******************************* RESULTS **********************************"<< endl;
  cout << "##**************************************************************************"<< endl;

  cout << "##"<<endl;
  cout << "## Best solution found: " <<  BGlobal->getEvaluation() << "  " <<BGlobal->eval()<< endl;
  cout <<  BGlobal->getEvaluation() << " ";
	cout << BGlobal->eval();
	const list<Station*> stations= BGlobal->get_stations();

  int tot_alpha=0;
  for(auto s:stations){
    int alpha=s->get_sum_alpha(strategy);
    cout << alpha << " " ;
    tot_alpha+=alpha;
  }
  cout << tot_alpha << " ";

  int nb_apertures=0;
  for(auto s:stations){
    int ap=s->get_nb_apertures(strategy);
    cout << ap << " " ;
    nb_apertures+=ap;
  };
  cout << nb_apertures << endl;
	for (int i = 0; i <5 ; i++) BGlobal->printIntensity(i);
	return 0;
}
