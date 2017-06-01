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
#include <cmath>

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

	cout << "pop dim " << this->population.size()<<endl;

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
*	@brief	Generate a pool of individual from another pool (crossover).
*
*	@section DESCRIPTION
*	Crossover is a 2 cut-point crossover.
*
*	@return set of individuals (dimension is: original / new_generation_ratio)
*/
vector< GAIndividual* > GAPopulation::crossover(vector< GAIndividual* > pool) {
	vector< GAIndividual* > offsprings;

	// consider two individuals at each iteration
	for (unsigned int i = 0; i < pool.size(); i += 2 ) {
		// Create a child that can mutate
		GAIndividual* child = mutate(cut_point_crossover(pool.at(i), pool.at(i+1)));
		offsprings.push_back(child);
	}

	return offsprings;
}


/**
*	@brief	Generate an individual from another two using 2-cut point crossover.
*
*	@section DESCRIPTION
*	After K cut points are defined, the first child is obtained by copying
*	the block between the cut points from the first and the second parents
*	alternatively. In the same way the second child is obtained.
*	We use K=2, so it's a 2-cyt point crossover.
*	In order to preserve feasible solutions, the order crossover is implemented.
*	the child takes the external blocks from one parent and the inner genes
*	are taken from the second parent in order of their appearance in the second parent.
*
*	@return one individual
*/
GAIndividual* GAPopulation::cut_point_crossover(GAIndividual* p1, GAIndividual* p2) {
	unsigned int k1, k2;
	unsigned int N = this->problem->get_size();

	// Computing the two k points randomly, s.t.
	// - k1, k2 >= 1, k1, k2 <= problem.N
	// - k2 - k1 > 2
	while (1) {
		k1 = (rand() % (N-1)) + 1;
		k2 = (rand() % (N-1)) + 1;

		if (( k2 <= k1) || ((k2 - k1) < 2)) { continue; }
		break;
	}

	vector<Node> p1_path = p1->get_path();
	vector<Node> p2_path = p2->get_path();

	// Create the path for the child
	vector<Node> child_path;
	child_path.resize(N + 1);

	// The nodes unused from the first parent
	// will be taken from the second
	vector<Node> available(N-1);
	for (unsigned int i = 0; i < N-1; i++) {
		available[i] = i+1;
	}

	// copying the first external block (left)
	for (unsigned int i = 0; i <= k1; i++) {
		child_path[i] = p1_path[i];
		if (i == 0) { continue; }	// skip the first (awalys at 0)
		available.erase(std::remove(available.begin(), available.end(), p1_path[i]), available.end());
	}

	// copyin the external block (right)
	for (unsigned int i = k2; i < p1_path.size(); i++) {
		child_path[i] = p1_path[i];
		if (i == N) { continue; }	// skip the last (awalys at 0)
		available.erase(std::remove(available.begin(), available.end(), p1_path[i]), available.end());
	}

	// create the inner block by copying the remaing nodes
	// from the second parent
	// Starting in the first free p1 position (k1+1)
	for (unsigned int i = k1+1; i < k2; i++) {
		// Check each p2 element from 1 to N
		for (unsigned int j = 1; j < N; j++) {
			unsigned int el = p2_path[j];
			if (std::find(available.begin(), available.end(), el) != available.end()) {
				// This element is in the avaiable list so can be used
				child_path[i] = el;

				// Delete the element and skip to the next i position in p1
				available.erase(std::remove(available.begin(), available.end(), el), available.end());
				break;
			}
		}
	}

	// final check: node 0 should be in the first and in the last positions only
	// if not, something went wrong
	unsigned int zeros = 2;
	for (unsigned int i = 0; i < child_path.size(); i++) {
		if (child_path.at(i) == 0) {
			zeros--;
		}
	}
	assert(zeros == 0);

	// Create a new child with the path created upon
	return new GAIndividual(this->problem, child_path);
}

/**
*	@brief	Mutate a individual using inversion mutation method.
*
*	@section DESCRIPTION
*	Two point are randomly generated and then swapped
*
*	@return a mutated individual
*/
GAIndividual* GAPopulation::mutate(GAIndividual* individual) {
	GAIndividual* new_individual;
	unsigned int z, t;
	unsigned int N = this->problem->get_size();

	// Computing the two points randomly, s.t.
	// - k1,k2 >= 1, k1, k2 <= problem.N
	// - k1 != k2
	z = (rand() % (N-1)) + 1;
	while (1) {
		t = (rand() % (N-1)) + 1;

		if (z != t) break;
	}

	cout << z << " "<< t<<endl;

	return new_individual;
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
