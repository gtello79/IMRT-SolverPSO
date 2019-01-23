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


/*void searchGlobal(vector<Particle> &solution, int size, Plan &BGlobal){
	int i = 0;
	for (int i = 1 ; i < size;i++){
		if (solution[i].Getfitness() <= BGlobal.getEvaluation()){
			BGlobal = (solution[i].GetPCurrent());
		};
	}
}*/

int main(){
	int i,j; 
	int n = 1;
	int size  = 20;
	int max_iter = 10;
	int _type_ = 1;
	int initial_setup = 5;

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
  int max_intensity=28;
  int step_intensity=2;
  
  // ls params
  double prob_intensity=0.2;
  double temperature, initial_temperature=10;
  double min_temperature=0;
  double alphaT=0.95;
  int perturbation=2;

	string strategy="dao_ls";
	string file="data/testinstance_0_70_140_210_280.txt";
  string file2="data/test_instance_coordinates.txt";

	vector <Particle> solution ;//inicializar con parametro sizeB
	vector<double> w={1,1,1};
  vector<double> Zmin={0,0,76};
  vector<double> Zmax={65,60,1000};
	Collimator collimator(file2, get_angles(file, 5));
  vector<Volume> volumes= createVolumes (file, collimator);
	
	Plan *BGlobal;
	BGlobal = new Plan(w, Zmin, Zmax, collimator, volumes, max_apertures, max_intensity, initial_intensity, step_intensity, open_apertures, initial_setup);

	//Formation of the particle set
	Plan *Opc;
	cout<<"\n "<<endl;
	for(i = 0; i < size; i++)
	{	
    //Agregar condiciones nueva para generar un plan
		cout << "Particula N° " << i+1 << endl;
		Opc = new Plan(w, Zmin, Zmax, collimator, volumes, max_apertures, max_intensity, initial_intensity, step_intensity, open_apertures, initial_setup);
		solution.push_back(Particle(*Opc));
		if(solution[i].Getfitness()<solution[i].getbfitness()){
      solution[i].updatePbest();
    }
	}
  //searchGlobal(solution, size, *BGlobal);
  for(int k = 0; k < size ; k++)
  {
      if(solution[k].Getfitness() < BGlobal->getEvaluation())
      {
        BGlobal = &solution[k].GetPCurrent();
      }
  }

  cout << "###############################################################################" << endl;
	cout << "#########################Particles Created#####################################" << endl;
	cout << "###############################################################################" << endl;
	//The Begining of PSO using max_iter how the total of iterations
  for(j = 0; j < max_iter ; j++)
	{	
		cout << "\n Iteracion: "<< j+1 <<endl;
		//We calculate the Intensity and the Velocity using PSO
    for(int i = 0; i < size ; i++)
		{	
      cout << "Particula N°" << i+1 <<" " ;
      //if(solution[i].Getfitness() != BGlobal->getEvaluation() )
      //{  
			  solution[i].Velocityupdate(*BGlobal, _type_,1,1,1);
			  solution[i].updatePosition();
        solution[i].calculateFitness();
        if(solution[i].Getfitness()<solution[i].getbfitness())
        {
          solution[i].updatePbest();
        }
      //}
      cout << solution[i].Getfitness() <<"\n"<<endl;
		};
    //Calculate the new Best Global of the particle
    for(int k = 0; k < size ; k++){
      if(solution[k].Getfitness() < BGlobal->eval()){
        BGlobal = &solution[k].GetPCurrent();
      }
    }
		cout<<"Best Global Iteration n°"<<j<<": " << BGlobal->getEvaluation()<<endl;
	};
	cout << "##**************************************************************************"<< endl;
  cout << "##******************************* RESULTS **********************************"<< endl;
  cout << "##**************************************************************************"<< endl;

  cout << "##"<<endl;
  cout << "## Best solution found: " <<  BGlobal->getEvaluation() << endl;
  BGlobal->printIntensities();
  return 0;
}
