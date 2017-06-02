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
	    long long time_limit;
		unsigned int iterations_limit;
	    double mutation_probability;
	    GAPopulation* population;

	public:
	    GASolver(TSPProblem *problem, unsigned int pop_size,\
				unsigned int t_limit, unsigned int itr_limit, \
				double mutation_pr);
	    // ~GASolver(){ problem = NULL; delete population;}
	    GAIndividual* solve();

		long long current_timestamp();
};

#endif	// GA_SOLVER_H_
