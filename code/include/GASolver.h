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

#include "TSPSolution.h"

typedef TSPSolution GAIndividual;	// an individual is basically a solution

class GASolver {
	private:
	    TSPProblem* problem;
	    unsigned int population_size;
	    unsigned int time_limit;
	    double mutation_probability;
	    double new_generation_ratio;
	    // Population* population;
	    unsigned int lastIterationsCount;
	public:
	    GASolver(TSPProblem *problem, unsigned int populationSize, unsigned int timeLimit, double mutationProbability, double newGenerationRatio);
	    // ~GASolver(){ problem = NULL; delete population;}
	    TSPSolution* solve();
	   // Population* getLastPopulation();
	    unsigned int getLastIterationsCount();
};

#endif	// GA_SOLVER_H_
