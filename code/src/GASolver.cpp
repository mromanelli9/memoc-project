/**
 *  @file    GASolver.cpp
 *  @author  Marco Romanelli
 *  @date    31/05/2017
 *  @version 1.0
 *
 *  @brief GA Solver class file
 *
 *  @section DESCRIPTION
 *
 *  This class represents a solver for the TSP problem
 *	using a genetic algorithm.
 *
 */

#include "../include/GASolver.h"
#include "../include/GAPopulation.h"
#include <iostream>
#include <sys/time.h>

using namespace std;

/**
*	@brief	Default constructor
*/
GASolver::GASolver(TSPProblem *problem, unsigned int pop_size, unsigned int t_limit, double mutation_pr, double new_gen_r) {
    this->problem = problem;
    this->population_size = pop_size;
    this->time_limit = (long long) t_limit;
    this->mutation_probability = mutation_pr;
    this->new_generation_ratio = new_gen_r;
}

/**
*	@brief	Main function: solve the problem using a gen. algo.
*/
GAIndividual* GASolver::solve() {
	// Phase 1: create the first population
	GAPopulation* population = new GAPopulation(this->population_size,\
											this->mutation_probability, \
											this->new_generation_ratio,
											this->problem);

	// Start main loop. Stop when time exceed
	// long long t_start = current_timestamp();	// time in milliseconds
	// unsigned int i = 0;	// current itereation
	// while ((current_timestamp() - t_start) < this->time_limit) {

		// Phase 2: select the mating pool

		// Phase 3: crossover to generate offsprings

		// Phase 4: mutation

		// Phase 5: population management


	//}

}

/**
*	@brief	Get time in milliseconds
*
*	@return return time [milliseconds]
*/
long long GASolver::current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);	// get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;	// caculate milliseconds
    return milliseconds;
}
