/*
 * Plan.cpp
 *
 *  Created on: 1 ago. 2018
 *      Author: leslie
 */

#include "Plan.h"

namespace imrt {

  Plan::Plan(EvaluationFunction &ev) : ev(ev), last_changed(NULL) {};

  Plan::Plan(EvaluationFunction &ev, vector<double> w, vector<double> Zmin, vector<double> Zmax): ev(ev), w(w), Zmin(Zmin), Zmax(Zmax) {
    last_changed=NULL;
  }

  Plan::Plan(vector<double> w, vector<double> Zmin, vector<double> Zmax, Collimator& collimator,
             vector<Volume>& volumes, int max_apertures, int max_intensity, int initial_intensity,
             int step_intensity, int open_apertures, int setup) : ev(volumes), w(w), Zmin(Zmin), Zmax(Zmax) {

    cout << "##Initilizing plan."<< endl;

    for (int i=0;i<collimator.getNbAngles();i++) {
      Station* station = new Station(collimator, volumes, collimator.getAngle(i), max_apertures,
                                     max_intensity, initial_intensity, step_intensity, open_apertures, setup);
      //real_stations.push_back(*station);
      add_station(*station);
      //station->printIntensity();
    }
    cout << "##  Created " << stations.size() << " stations."<< endl;
    eval();
    cout << "##  Initial evaluation: " << evaluation_fx << "."<< endl;
    last_changed=NULL;
  };

  Plan::Plan(const Plan &p): ev(p.ev), w(p.w), Zmin(p.Zmin), Zmax(p.Zmax), last_changed(NULL) {
    //EvaluationFunction aux_ev(p.ev);
    //ev=aux_ev;
    for (list<Station*>::const_iterator it=p.stations.begin();it!=p.stations.end();it++) {
      Station* aux = new Station(**it);
      if (p.last_changed && p.last_changed->getAngle()==aux->getAngle()) last_changed=aux;
      add_station(*aux);
      //real_stations.push_back(*aux);
    }
    evaluation_fx=p.evaluation_fx;
  }

  void Plan::newCopy(Plan& p) {
    last_changed=NULL;
    EvaluationFunction* aux_ev= new EvaluationFunction (p.ev);
    ev=*aux_ev;
    w=p.w;
    Zmin=p.Zmin;
    Zmax=p.Zmax;
    stations.clear();
    //real_stations.clear();
    for (list<Station*>::const_iterator it=p.stations.begin();it!=p.stations.end();it++) {
      Station* aux = new Station (**it);
      if (p.last_changed!=NULL && p.last_changed->getAngle()==aux->getAngle()) last_changed=aux;
      stations.push_back(aux);
      //real_stations.push_back(*aux);
    }
    evaluation_fx=p.evaluation_fx;
  }

  void Plan::add_station(Station& s){
    stations.push_back(&s);
  }

  double Plan::eval(vector<double>& w, vector<double>& Zmin, vector<double>& Zmax) {
    double eval=ev.eval(*this,w,Zmin,Zmax);
    ev.generate_voxel_dose_functions ();
    evaluation_fx=eval;
    return eval;
  };

  double Plan::eval() {
    double eval=ev.eval(*this,w,Zmin,Zmax);
    ev.generate_voxel_dose_functions ();
    evaluation_fx=eval;
    return eval;
  };

  double Plan::incremental_eval (Station& station, list< pair< int, double > >& diff) {
    evaluation_fx=ev.incremental_eval(station, w, Zmin, Zmax, diff);
    last_changed=&station;
    last_diff= diff;
    return(evaluation_fx);
  };

  double Plan::getEvaluation(){
    return(evaluation_fx);
  }

  void Plan::undoLast() {
    if (last_changed==NULL) return;
    list< pair< int, double > > diff = last_changed->undoLast();
    if (diff.size()>0)
      incremental_eval (*last_changed, diff);
  }

  //Lepi's version
  void Plan::undoLast2() {
    last_changed->revert(last_diff);
    ev.undo_last_eval(w,Zmin,Zmax);
  }

  const list<Station*>& Plan::get_stations() const{
    return stations;
  }

  void Plan::write_open_beamlets(){
    ofstream myfile;
    myfile.open ("openbeamlets.txt");

    myfile << "Angles\t";
    for(auto s:stations)
      myfile << s->getAngle() << "\t";
    myfile << endl;
    int k=0;
    for(auto s:stations){
      set<int> open_beamlets;
      myfile << endl << "Station Angle\t" << s->getAngle() << endl;
      for(int i=0; i<s->getNbApertures(); i++){
        myfile << "Aperture\t" << i << endl;
        myfile << "Intensity\t" << s->intensity[i] << endl;

        myfile << "OpenBeamlets\t" ;
        bool first=true;
        for(auto beam:s->open_beamlets(i)){
          if(!first) myfile << "\t";
          first=false;
          myfile << k+beam;
        }
        myfile << endl;
      }
      k+=s->getNbBeamlets();
    }
    myfile.close();

  }

  set < pair< pair<double,bool>, pair<Station*, int> >,
        std::greater < pair< pair<double,bool>, pair<Station*, int> > > >
    Plan::best_beamlets(int n, int nv, int mode) {

    return(ev.best_beamlets(*this, n, nv));
  }

  void Plan::printIntensity(int n) {
    list<Station*>::iterator s= stations.begin();
    advance(s,n);
    (*s)->printIntensity(false);
  }

  void Plan::writeIntensities(string file, int n) {
  	ifstream myfile;
  	myfile.open(file);

    std::string line;
    for(int i=0; i<n; i++) std::getline(myfile, line);

  	int angle1, angle2, angle3, angle4, angle5;
  	double F;
  	myfile >> angle1;
  	myfile >> angle2;
  	myfile >> angle3;
  	myfile >> angle4;
  	myfile >> angle5;
  	myfile >> F;

  	for(auto station:stations){
  		station->writeIntensity(myfile);
  	}

  }
  /*void calculateVelocityupdate(plan &global, plan PBest){
    int i=0, j=0, index = 0;
    List aux;
    stations pivote;
    aux = this.get_stations();
    for (list<Station*>::const_iterator it=aux.begin();it!=aux.end();it++) {
      MatrixI = get_intensity();
      MatrixV = getVelocity();
      Matrix NuevaVel, NuevaPos;
      for(i=0;i<MatrixI.length;i++){
        for(j=0;j<MatrixI.length;j++){
            NuevaVel[i][j] = w*MatrixV[i][j] + ((P.c1 * P.r1)*(PBest[i][j]-MatrixI[i][j])) + ((P.c2 * P.r2)*(GBest[i][j] - MatrixI[i][j]))
        }
      }
      index++;
      setVelocity(NuevaVel);
    }
  }*/
  void Plan::updatePosition(){
    list <Station*> aux;
    aux = get_stations();
    Matrix MatrixI, MatrixV, NuevaPos;
    for (list<Station*>::const_iterator it=aux.begin(); it!=aux.end() ;it++) {
      MatrixI = (*it)->get_Intensity();
      MatrixV = (*it)->get_Velocity();
      NuevaPos = MatrixI + MatrixV;
      (*it)->set_Intensity(NuevaPos);
    }
  }

  void Plan::updateVelocity(Plan *Bglobal, Plan *Pbest, Plan *current, int w, int c1, int c2, int r1, int r2){
    Station *auxCurrent, *auxGlobal, *auxBest;
    Matrix *ICurrent, *VCurrent, *IGlobal, *VGlobal, *IBest, *VBest;
    for (int i=0; i<5; i++){

      auxCurrent = get_station(i);
      auxGlobal = Bglobal->get_station(i);
      auxBest = Pbest->get_station(i);

      ICurrent = &auxCurrent->get_Intensity();
      VCurrent = &auxCurrent->get_Velocity();

      IGlobal = &auxGlobal->get_Intensity();
      //VGlobal = auxGlobal->get_Velocity();

      IBest = &auxBest->get_Intensity();
      //VBest = auxBest->get_Velocity();

      *VCurrent = w* *VCurrent + c1*r1*(*IBest - *ICurrent) + c2*r2*(*IGlobal - *ICurrent);
      //auxCurrent->set_Velocity(NuevaVel);
    }
  }
}
