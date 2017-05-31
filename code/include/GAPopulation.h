/**
 *  @file    GAPopulation.h
 *  @author  Marco Romanelli
 *  @date    31/05/2017
 *  @version 1.0
 *
 *  @brief GA pupulation class header
 *
 *  @section DESCRIPTION
 *
 *  A population is a set of individuals
 *	with some features (genes) in common.
 *
 */

#ifndef GA_POPULATION_H_
#define GA_POPULATION_H_

#include "GAIndividual.h"

class GAPopulation {
	private:
		// Algorithm parameters
		unsigned int population_size;
		double mutation_probability;
		double new_generation_ratio;

		// Population parameters
		TSPProblem* problem;

		vector< GAIndividual* > population;	// Actual population
		vector< GAIndividual* > mating_pool;	// current parents set

	public:
	    GAPopulation(unsigned int size, double mutation_probability, double new_generation_ratio,  TSPProblem* problem);
};

#endif	// GA_POPULATION_H_
