package pso;

/**
 * The main class of PSO solver
 * The example works for the following optimization problem
 * 
 * variables:
 *   x1,x2,x3,x4,x5 in [1,100]
 * 
 * objective function:
 *   max f(x) = x1^10 + x2^4 + x3^3 + x4^2 + x5^2
 * 
 * PSO algorithm:
 * 
 * 		For each particle 
 *   		Initialize particle
 *  	End 
 * 
 * 		While maximum iterations or stop criteria is not reached
 *   		For each particle 
 *       		Calculate fitness value
 *       		If the fitness value is better than the best fitness value (pBest) in history
 *           		set current value as the new pBest
 *              End
 *          End
 *   
 *   		Choose the particle with the best fitness value of all the particles as the gBest
 *   		For each particle 
 *       		Calculate particle velocity according equation (a)
 *       		Update particle position according equation (b)
 *       	End
 *   	End 
 *   
 * 
 * @author Ricardo Soto
 * @since 1.5
 */

public class Runner {


    public static void main(String args[]) { 
        try { 
            System.out.println("Solving...");
            Swarm swarm = new Swarm();
            swarm.execute();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
   
    
}

