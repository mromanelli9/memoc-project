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

using namespace std;

/**
*	@brief	Default contructor
*/
GAPopulation::GAPopulation(unsigned int size, double mutation_probability, double new_generation_ratio, TSPProblem *problem) {
	// Store parameters
	this->size = size;
    this->problem = problem;
    this->new_generation_ratio = new_generation_ratio;
    this->mutation_probability = mutation_probability;
    this->average_fitness = 0;
    double best = INT_MAX;
    double worst = 0;
    int bi = 0, wi = 0;

	// Populate the population (= set of individuals)
    for (unsigned int i = 0; i < size; ++i) {
        individuals.push_back(new GAIndividual(problem));
        double c_fitness = individuals.at(i)->get_fitness();
        if (c_fitness < best){
            bi = i;
            best = c_fitness;
        }
        if (c_fitness > worst){
            wi = i;
            worst = c_fitness;
        }

        this->average_fitness += c_fitness;
    }
    this->average_fitness /= size;

    this->best = new GAIndividual(problem, individuals.at(bi)->get_path());
    this->worst = new GAIndividual(problem, individuals.at(wi)->get_path());
}
