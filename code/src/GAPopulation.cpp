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
GAPopulation::GAPopulation(unsigned int size, double mutation_probability, TSPProblem *problem) {
	// Store parameters
	this->population_size = size;
	this->problem = problem;
	this->new_generation_ratio = 2;	// Fixed!
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
*	After K cut points are defined, the child is obtained by copying
*	the block between the cut points from the first and the second parents
*	alternatively.
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
*	Two point are randomly generated and then the sublist between them is revered.
*
*	@return a mutated individual
*/
GAIndividual* GAPopulation::mutate(GAIndividual* individual) {
	// mutate with probability <mutation_probability>
	double prob = rand() / double(RAND_MAX);
    if (prob >= mutation_probability) {
		return individual;
	}

	unsigned int z, t;
	unsigned int N = this->problem->get_size();

	// Computing the two points randomly, s.t.
	// - z,t >= 1, z, t <= problem.N
	// - z < t
	while (1) {
		z = (rand() % (N-1)) + 1;
		t = (rand() % (N-1)) + 1;

		if (z < t) break;
	}

	vector<Node> p = individual->get_path();
	vector<Node> new_path(N+1);	// new path for the individual
	new_path[0] = 0;	// as always the first and the last
	new_path[N] = 0;	// node are set to 0

	// the first and the last part are the same
	for (unsigned int i = 1; i < z; i++ )
		new_path[i] = p[i];

	for (unsigned int i = t+1; i < N+1; i++ )
		new_path[i] = p[i];

	// do the magic
	unsigned int j = 0;
	for (unsigned int i = z; i <= t; i++ ) {
		new_path[i] = p[t-j];
		j++;
	}

	return new GAIndividual(this->problem, new_path);
}

/**
*	@brief	Merge the old population with the offspring and produce a new generation.
*
*	@section DESCRIPTION
*	Method used: selection of the bests.
*	N individuals are selected from a pool of N+R individuals where
*	R is the dimension of the offspring.
*
*	@return void
*/
void GAPopulation::population_management(vector< GAIndividual* > pool) {
	vector< GAIndividual* > new_population;

	// Create a new (provisional) population with the old one and the offspring
	// with dimension N + R (actually: population_size * new_generation_ratio)
	this->population.insert( this->population.end(), pool.begin(), pool.end() );

	for (unsigned int i = 0; i < this->population_size; i++) {
		GAIndividual* selected = montecarlo_selection(this->population);

		// Remove the individual from the current population
		this->population.erase(std::remove(this->population.begin(), \
									this->population.end(), selected), this->population.end());


		// Add the individual to the new population
		new_population.push_back(selected);
	}

	this->population = new_population;
}

/**
*	@brief	Select the best individual in the pool using the Montecarlo method.
*
*	@section DESCRIPTION
*	The probability p_i of picking the individual i is proportional to its
*	fitness value f_i:
*		p_i = (f_i / sum of f_k)  where k=1,_,N
*
*
*	@return an individual
*/
GAIndividual* GAPopulation::montecarlo_selection(vector< GAIndividual* > pool) {
	// Sort the population by their fitness value
	std::sort(pool.begin(), pool.end(), GAPopulation::sort_by_fitness);

	vector< double > probabilities;	// probability of a node to be selected
	// Compute sum of f_k
	{
		vector< double > f_i;	// list of all fiteness values
		long double f_k_sum;	// sum of f_k

		for (auto & element : pool) {
			f_i.push_back(element->get_fitness());
		}
		f_k_sum = std::accumulate(f_i.begin(), f_i.end(), 0.0);

		// Compute probabilities
		for (auto & el : f_i) {
			probabilities.push_back(el / f_k_sum);
		}
	}

	// Choose a random number k s.t. 0<k<1
	double k = (double) rand() / (RAND_MAX);

	double tot = 0;	// p_i so far
	unsigned int i = 0;
	while (i < probabilities.size()) {
		tot += probabilities.at(i);
		if (tot >= k) {
			// Pick this element
			break;
		}
		i++;
	}
	if (i==0) {
		i = 1;
	}

	return pool.at(i-1);
}




/**
*	@brief	Return the best individual from the given set of individuals
*
*	@return an individual
*/
GAIndividual* GAPopulation::choose_best(vector<GAIndividual*> pool) {
	double bestValue = INT_MAX;
	unsigned long index = pool.size()+1;
	for (unsigned long i = 0; i < pool.size(); ++i) {
		if (pool.at(i)->get_fitness() < bestValue){
			bestValue = pool.at(i)->get_fitness();
			index = i;
		}
	}
	return pool.at(index);
	// double max = 0;
	// unsigned int best = 0;
	// double value;
	//
	// for (unsigned int i = 0; i < pool.size(); i++) {
	// 	value = pool.at(i)->get_fitness();
	// 	if (value > max) {
	// 		max = value;
	// 		best = i;
	// 	}
	// }
	//
	// return pool.at(best);
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

/**
*	@brief	Return true if an individual p1 fitness value is grater than
*			the fiteness value of individual p2
*
*	@return true or false
*/
bool GAPopulation::sort_by_fitness(GAIndividual* p1, GAIndividual* p2) {
    return p1->get_fitness() < p2->get_fitness();
}

/**
*	@brief	Retrieve the best individual in the population
*
*	@return true or false
*/
GAIndividual* GAPopulation::get_best_individual() {
	return *min_element(this->population.begin(), this->population.end(), GAPopulation::sort_by_fitness);
}

/**
*	@brief	Retrieve the worst individual in the population
*
*	@return true or false
*/
GAIndividual* GAPopulation::get_worst_individual() {
	return *max_element(this->population.begin(), this->population.end(), GAPopulation::sort_by_fitness);
}
