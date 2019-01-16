package pso;

/**
 * Represents the particle of the swarm
 * 
 * @author Ricardo Soto
 * @since 1.5
 */


public class Particle {
	
	private double[] pBest={0,0,0,0,0}; //particle best position
	private double[] pos={0,0,0,0,0};  //particle position
	private double fitness;          //particle fitness
	private double pfitness;          //particle best fitness
	private double[] velocity = {0,0,0,0,0};
	public int nVariables=5;        //number of variables of the problem

	/*particle parameters*/
	private double vMax = 100;
	private double inertia = 1;
	private int c1 = 2;
	private int c2 = 2;
	

	public Particle (double[] pBest, double fitness) {
		this.pBest = pBest;
		this.pfitness = fitness;
		//initialize particle position
		for (int i=0; i<=nVariables-1; i++){
			pos[i] = Math.random()*99+1;
		}
	}

	public void calculateFitnessAndUpdatePBest() {
		/*completar*/
	}
	
	public void calculateVelocityAndUpdatePos(double[] gBest) {
		/*completar*/
	}
	
	public double[] getpBest() {
		return pBest;
	}
	
	public double getFitness() {
		return fitness;
	}
	
	
	public String toString() {
		return pos[0] +"," + pos[1]+"," + pos[2]+"," + pos[3]+"," + pos[4];
	}
}
