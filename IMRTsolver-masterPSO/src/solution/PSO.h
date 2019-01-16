/*
*
* PSO.h
*
* Created on: 04-01-2018
* Author: Felipe Jorquera - Gonzalo Tello
*
*/

#ifndef PSO_H_
#define PSO_H_

#include "Station.h"
#include "Plan.h"
#include "vector"
using namespace std;

class PSO{


	public:
		PSO(Plan &first);
		void actualizarvelocidad(Plan &elemento);
		void actualizarposicion()
	private:
		/*vector with the solutions of all the particles */
		vector <Plan> particulas;
		/*the best solution of the iteration */
		Plan best_sol;

}


#endif /*PSO*/
