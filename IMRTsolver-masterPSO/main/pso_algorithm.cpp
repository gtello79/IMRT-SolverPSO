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

int main(int argc, char** argv){
  //Belong to the PSO-ALGORITHM
	int size  = 10;
	int max_iter = 100;
	int initial_setup = 4;
  int diff_setup = 0;                   //this params use a particle to guide the rest of the particles
  int best_particle;

  //Best parametres values
  float iner=1;
  float c1 = 1;
  float c2 = 1;

  //Marks of Best Global
  double actual_global = 0;
  int Best_iteration = 0;

  //Belong to the other algorithms.
	int bsize=20;
 	int vsize=50;
  double maxdelta=5.0;
  double maxratio=3.0;
  double alpha=1.0;
  double beta=1.0;
  int maxiter=5000;
  int maxtime=0;
  int max_apertures=5;
  int open_apertures=-1;
  int initial_intensity=2;
  int max_intensity=28;
  int step_intensity=2;
  int changes_beam = 1;                   //number of beam to change
  float prob_aperture = 1;                //Probability to move one aperture respect all the set of max_apertures

  bool search_aperture=false;
  bool search_intensity=false;

  // ls params
  double prob_intensity=0.2;
  double temperature, initial_temperature=10;
  double min_temperature=0;
  double alphaT=0.95;
  int perturbation=2;
	string strategy="dao_ls";
	string file="data/testinstance_0_70_140_210_280.txt";
  string file2="data/test_instance_coordinates.txt";
  string path=".";
  int seed=time(NULL);
  srand(seed);
  cout << seed << endl;


  args::ArgumentParser parser("********* IMRT-Solver (PSO-Algorithm) *********", "Example.\n./PSO  --maxiter=400 --maxdelta=8 --maxratio=6 --alpha=0.999 --beta=0.999 --bsize=5 --vsize=20 --max-apertures=4 --seed=0 --open-apertures=1 --initial-intensity=4 --step-intensity=1 --file-dep=data/Equidistantes/equidist00.txt --file-coord=data/Equidistantes/equidist-coord.txt");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
  args::ValueFlag<int> _size(parser, "int", "Number of particles for the swarm("+to_string(size)+")", {"size"});
  args::ValueFlag<int> _max_iter(parser, "int", "Number of iterations that the program will run ("+to_string(max_iter)+")", {"max_iter"});
  args::ValueFlag<int> _initial_setup(parser, "int", "Type of creation for the particles ("+to_string(initial_setup)+")", {"initial_setup"});
  args::ValueFlag<int> _diff_setup(parser, "int", "Type of creation for a particle ("+to_string(diff_setup)+")", {"diff_setup"});
  args::ValueFlag<float> _iner(parser, "int", "Inertia Parameter ("+to_string(iner)+")", {"iner"});
  args::ValueFlag<float> _c1(parser, "int", "Social Factor Parameter ("+to_string(c1)+")", {"c1"});
  args::ValueFlag<float> _c2(parser, "int", "Personal Factor Parameter ("+to_string(c2)+")", {"c2"});
  args::ValueFlag<int> _changes_beam(parser, "int","Size of beam to change("+to_string(changes_beam)+")",{"changes_beam"});
  args::ValueFlag<float> _prob_aperture(parser, "int", "Probability to move one aperture respect all the set of max_apertures ("+to_string(prob_aperture)+")", {"prob_aperture"});

  args::ValueFlag<int> _bsize(parser, "int", "Number of considered beamlets for selection ("+to_string(bsize)+")", {"bsize"});
	args::ValueFlag<int> _vsize(parser, "int", "Number of considered worst voxels ("+to_string(vsize)+")", {"vsize"});
  args::ValueFlag<int> _maxdelta(parser, "int", "Max delta  ("+to_string(maxdelta)+")", {"maxdelta"});
  args::ValueFlag<int> _maxratio(parser, "int", "Max ratio  ("+to_string(maxratio)+")", {"maxratio"});
  args::ValueFlag<double> _alpha(parser, "double", "Initial temperature for intensities  ("+to_string(alpha)+")", {"alpha"});
  args::ValueFlag<double> _beta(parser, "double", "Initial temperature for ratio  ("+to_string(beta)+")", {"beta"});
  args::ValueFlag<int> _maxiter(parser, "int", "Number of iterations ("+to_string(maxiter)+")", {"maxiter"});
  args::ValueFlag<int> _maxtime(parser, "int", "Maximum time in seconds ("+to_string(maxtime)+")", {"maxtime"});
  args::ValueFlag<int> _seed(parser, "int", "Seed  ("+to_string(seed)+")", {"seed"});
  args::ValueFlag<int> _max_apertures(parser, "int", "Initial intensity for the station  ("+to_string(max_apertures)+")", {"max_apertures"});
  args::ValueFlag<int> _open_apertures(parser, "int", "Number of initialized open apertures (-1: all, default:"+to_string(open_apertures)+")", {"open_apertures"});
  args::ValueFlag<int> _initial_intensity(parser, "int", "Initial value aperture intensity  ("+to_string(initial_intensity)+")", {"initial_intensity"});
  args::ValueFlag<int> _max_intensity(parser, "int", "Max value aperture intensity  ("+to_string(max_intensity)+")", {"max_intensity"});
  args::ValueFlag<int> _step_intensity(parser, "int", "Step size for aperture intensity  ("+to_string(step_intensity)+")", {"step_intensity"});

  args::Group setup (parser, "Initial solution setup (these override all provided configurations):", args::Group::Validators::DontCare);
  args::Flag open_max(setup, "open_max", "Open aperture setup with max intensity", {"open-max-setup"});
  args::Flag open_min(setup, "open_min", "Open aperture setup with min intensity", {"open-min-setup"});
  args::Flag closed_min(setup, "closed_min", "Closed aperture setup with min intensity", {"closed-min-setup"});
  args::Flag closed_max(setup, "closed_max", "Closed aperture setup with max intensity", {"closed-max-setup"});
  args::Flag all_rand(setup, "all_rand", "Random aperture setup with random intensity", {"rand-setup"});

  args::Flag _plot(parser, "bool", "Generate plot and save in file", {"plot"});
args::ValueFlag<string> _file(parser, "string", "File with the deposition matrix", {"file-dep"});
  args::ValueFlag<string> _file2(parser, "string", "File with the beam coordinates", {"file-coord"});
  args::ValueFlag<string> _path(parser, "string", "Absolute path of the executable (if it is executed from other directory)", {"path"});

 	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help&)
	{
		std::cout << parser;
		return 0;
	}
	catch (args::ParseError& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}
	catch (args::ValidationError& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}

  if(_size) size = _size.Get();
  if(_max_iter) max_iter = _max_iter.Get();
  if(_initial_setup) initial_setup =_initial_setup.Get();
  if(_diff_setup) diff_setup = _diff_setup.Get();
  if(_iner) iner =_iner.Get();
  if(_c1) c1 =_c1.Get();
  if(_c2) c2 =_c2.Get();
  if(_changes_beam) changes_beam = _changes_beam.Get();
  if(_bsize) bsize=_bsize.Get();
  if(_vsize) vsize=_vsize.Get();
  if(_maxdelta) maxdelta=_maxdelta.Get();
  if(_maxratio) maxratio=_maxratio.Get();
  if(_alpha) alpha=_alpha.Get();
  if(_beta) beta=_beta.Get();
  if(_maxiter) maxiter=_maxiter.Get();
  if(_maxtime) maxtime=_maxtime.Get();
  if(_max_apertures) max_apertures=_max_apertures.Get();
  if(_open_apertures) open_apertures=_open_apertures.Get();
  if(_seed) seed =_seed.Get();
  if(_initial_intensity) initial_intensity=_initial_intensity.Get();
  if(_max_intensity) max_intensity=_max_intensity.Get();
  if(_step_intensity) step_intensity=_step_intensity.Get();
  if(_prob_aperture) prob_aperture = _prob_aperture.Get();

  vector <Particle> solution ;
  vector<double> w={1,1,1};
  vector<double> Zmin={0,0,76};
  vector<double> Zmax={65,60,1000};
	Collimator collimator(file2, get_angles(file, 5));
  vector<Volume> volumes = createVolumes (file, collimator);

	Plan *BGlobal;                                                                //Se genera una particula especial que puede servir inicialmente de guia
	Plan *Opc;                                                                    //Se comienza a generar el resto de particulas
  for(int i = 0; i < size; i++)                                                 /*Se genera la poblacion total de particulas*/
	{
    cout << "Particula N " <<i+1 << endl;
    if(i == 0){
      BGlobal = new Plan(w, Zmin, Zmax, collimator, volumes, max_apertures, max_intensity, initial_intensity, step_intensity, open_apertures, diff_setup);
      solution.push_back(Particle(*BGlobal,c1,c2,iner,prob_aperture));
      best_particle=i+1;
      actual_global = solution[i].getFitness();
    }else{
      Opc = new Plan(w, Zmin, Zmax, collimator, volumes, max_apertures, max_intensity, initial_intensity, step_intensity, open_apertures, initial_setup);
		  solution.push_back(Particle(*Opc,c1,c2,iner,prob_aperture));
    }
	}
  for(int k = 0; k < size ; k++)                                                /*Se realiza una nueva busqueda para encontrar una nueva solución global*/
  {
    if(solution[k].getFitness() <= actual_global)
    {
      BGlobal->newCopy(solution[k].GetPCurrent());
      best_particle = k+1;
      actual_global = solution[k].getFitness();
    }
  }

	cout << "#########################Particles Created#####################################" << endl;
  cout<<"Best Global Iteration n "<<0<<": " << actual_global << " at the iteration " << Best_iteration <<"on particle "<< best_particle<<endl ;

  for(int j = 0; j < max_iter ; j++)                                            //The Begining of PSO using max_iter how the total of iterations
	{
		cout << "Iteracion: "<< j+1 <<endl;
    for(int i = 0; i < size ; i++)
		{
      cout << "Particula N°" << i+1 <<" " <<endl;
      solution[i].Velocityupdate(*BGlobal, changes_beam);
      solution[i].updatePosition(max_intensity);
      solution[i].calculateFitness(changes_beam);
      if(solution[i].getFitness() < solution[i].getBfitness())
        solution[i].updatePbest();
      cout <<"Actual Value: " <<solution[i].getFitness() << endl;
      cout << endl;
		};

    for(int k = 0; k < size ; k++)                                              //Calculate the new Best Global of the particle
    {
      if(solution[k].getFitness() < actual_global)
      {
        BGlobal->newCopy(solution[k].GetPCurrent());
        actual_global = solution[k].getFitness();
        cout << endl;
        best_particle = k+1;
        Best_iteration = j;
      }
    }
	  cout<<"Best Global Iteration n "<<j+1<<": " << actual_global << " at the iteration " <<Best_iteration <<" on particle "<< best_particle<<endl ;
  };

  cout << "********************************** RESULTS **********************************"<< endl;
  cout << "## Best Global Iteration n "<<max_iter<<": " << actual_global << " at the iteration " <<Best_iteration <<"on particle "<< best_particle<<endl;
  return 0;
}
