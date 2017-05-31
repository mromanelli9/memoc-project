/**
 *  @file    GASolver.h
 *  @author  Marco Romanelli
 *  @date    31/05/2017
 *  @version 1.0
 *
 *  @brief GA Solver class header
 *
 *  @section DESCRIPTION
 *
 *  This class represents a solver for the TSP problem
 *	using a genetic algorithm.
 *
 */

#ifndef GA_SOLVER_H_
#define GA_SOLVER_H_

#include "GAPopulation.h"

class GASolver {
	private:
	    TSPProblem* problem;
	    unsigned int population_size;
	    unsigned int time_limit;
	    double mutation_probability;
	    double new_generation_ratio;
	    GAPopulation* population;
	    unsigned int last_iterations_count;
	public:
	    GASolver(TSPProblem *problem, unsigned int pop_size, unsigned int t_limit, double mutation_pr, double new_gen_r);
	    // ~GASolver(){ problem = NULL; delete population;}
	    GAIndividual* solve();
	   	GAPopulation* get_last_population();
	    unsigned int get_last_iterations_count();
};

#endif	// GA_SOLVER_H_
