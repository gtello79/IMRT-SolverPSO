  package pso;

import java.util.ArrayList;

/**
 * Represents the swarm of the PSO solver
 * 
 * @author Ricardo Soto
 * @since 1.5
 */


public class Swarm {
	
	private ArrayList<Particle> particles  = new ArrayList<Particle>(); 
	
	public double[] gBest={1,1,1,1,1}; //global best solution
	public double gfitness=0;          //global best fitness
	public int nParticles=20;          //number of particles
	
	private int iterations = 100 ;     //number of iterations
	
	
	
	public void execute(){
		
		// add particles to the Arraylist
		for (int i=1; i<=nParticles; i++)
			particles.add(new Particle(gBest, gfitness)); 

		//PSO algorithm
		for(int i=0; i<=iterations;i++) {
			for (Particle p : particles)
				p.calculateFitnessAndUpdatePBest();
			
			updateGBest(i);	

			for (Particle p : particles)
				p.calculateVelocityAndUpdatePos(gBest);
			
		}
		//System.out.println("final fitness: " + gfitness);
	}
	
	
	public void updateGBest(int i){
		for (Particle p : particles){
			double temp = p.getFitness();
			if (temp > gfitness){
				gBest = p.getpBest();
				gfitness = temp;
			}
			
		}
		//print solution and fitness each 10 iterations
		if (i%10==0) {
			System.out.println("fitness it " + i + ": " + gfitness);
			System.out.println("gBest: " + gBest[0] +"," + gBest[1]+"," + gBest[2]+"," + gBest[3]+"," + gBest[4]);
		}
		
	}
	


	
	
	

}
