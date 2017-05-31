/**
 *  @file    GAPopulation.cpp
 *  @author  Marco Romanelli
 *  @date    31/05/2017
 *  @version 1.0
 *
 *  @brief GA population class file
 *
 *  @section DESCRIPTION
 *
 *  A population is a set of individuals
 *	with some features (genes) in common.
 *
 */

#include "../include/GAPopulation.h"
#include "../include/GAIndividual.h"
#include <assert.h>
#include <algorithm>
#include <climits>
#include <stdexcept>

using namespace std;

/**
*	@brief	Default contructor
*/
GAPopulation::GAPopulation(unsigned int size, double mutation_probability, double new_generation_ratio, TSPProblem *problem) {
	// Store parameters
	this->population_size = size;
    this->problem = problem;
    this->new_generation_ratio = new_generation_ratio;
    this->mutation_probability = mutation_probability;

	// Populate the population (= set of individuals)

}
