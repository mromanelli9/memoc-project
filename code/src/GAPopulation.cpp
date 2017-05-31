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
	this->population_size = size;
	this->problem = problem;
	this->new_generation_ratio = new_generation_ratio;
	this->mutation_probability = mutation_probability;

	// Populate the population (= set of individuals)
	for (unsigned int i = 0; i < this->population_size; ++i) {
		this->population.push_back(new GAIndividual(this->problem));
	}
}

/**
*	@brief	Create a mating pool based on the mathod called K-tournament.
*			K is set to <ratio>% of population-size;
*
*	@section DESCRIPTION
*	Each individual is selected by choosing uniformly n individuals (2<=k<=N)
*	and then picking up the best among them.
*
*	@return set of individuals
*/
vector< GAIndividual* > GAPopulation::create_mating_pool(unsigned int ratio) {
	double ratio_val = this->population_size /(double)100 * ratio;
	unsigned int K = (int) ratio_val;
	vector< GAIndividual* > parents;		// current set of individuals

	cout << "K =  " << K <<endl;

	// loop (rounds)
	for (unsigned int i = 0; i < this->population_size; i++) {
		vector<GAIndividual*> winners;	// Select two individuals (parents) at each round

		for (unsigned int j = 0; j < this->new_generation_ratio; j++) {
			vector<GAIndividual*> candidates;	// individuals in this round
			vector<GAIndividual*> no_good;	// avoid past individuals
			GAIndividual* winner;

			for (unsigned int j = 0; j < K; j++) {
				GAIndividual* c = choose_random(this->population, no_good);
				candidates.push_back(c);

				no_good.push_back(candidates.at(j));
			}
			winner = choose_best(candidates);
			winners.push_back(winner);
		}

		// Saving the winning parents into the set (vector)
		for (unsigned int j = 0; j < this->new_generation_ratio; j++) {
			parents.push_back(winners.at(j));
		}

	}

	return parents;
}

/**
*	@brief	Return the best individual from the given set of individuals
*
*	@return an individual
*/
GAIndividual* GAPopulation::choose_best(vector<GAIndividual*> pool) {
	double max = 0;
	unsigned int best = 0;
	double value;

	for (unsigned int i = 0; i < pool.size(); i++) {
		value = pool.at(i)->get_fitness();
		if (value > max) {
			max = value;
			best = i;
		}
	}

	return pool.at(best);
}

/**
*	@brief	Return a random individual from the given set of individuals
*
*	@return an individual
*/
GAIndividual* GAPopulation::choose_random(vector<GAIndividual*> pool, vector<GAIndividual*>  not_good) {
	GAIndividual* candidate;
	unsigned int val;

	while (1) {
		// Choose one randomly
		val = rand() % (unsigned int) pool.size();
		candidate = pool.at(val);

		// Check if it's a good candidate, that is,
		// it's not int the not_good pool
		bool good_candidate = true;
		unsigned int i = 0;
		while ((i < not_good.size()) && good_candidate) {
			if (not_good.at(i) == candidate) {
				good_candidate = false;
			}

			i++;
		}

		if (good_candidate) { break; }
	}

	return candidate;
}
