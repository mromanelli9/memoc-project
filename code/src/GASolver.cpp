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
#include <iostream>

using namespace std;

/**
*	@brief	Default constructor
*/
GASolver::GASolver(TSPProblem *problem, unsigned int pop_size, unsigned int t_limit, double mutation_pr, double new_gen_r) {
    this->problem = problem;
    this->population_size = pop_size;
    this->time_limit = t_limit;
    this->mutation_probability = mutation_pr;
    this->new_generation_ratio = new_gen_r;
}

/**
*	@brief	Main function: solve the problem using a gen. algo.
*/
GAIndividual* GASolver::solve() {
	/**
	*	Phase 1: create the first population
	*/
	GAPopulation* population = new GAPopulation(this->population_size,\
											this->mutation_probability, \
											this->new_generation_ratio,
											this->problem);

	// [!DEBUG]
	cout << "Popolazione iniziale:" <<endl;
    GAIndividual* worstSol = population->get_worst_individual();
    cout << "Worst: " <<worstSol->get_fitness();
    delete worstSol;
    cout << "\t Avg: " <<population->get_average_fitness();
    GAIndividual* bestSol = population->get_best_individual();
    cout << "\t Best: " <<bestSol->get_fitness() << endl;
    delete bestSol;

	this->population = population;


	/**
	*	Phase 2: select the mating pool
	*/


	/**
	*	Phase 3: crossover to generate offsprings
	*/

	/**
	*	Phase 4: mutation
	*/

	/**
	*	Phase 5: population management
	*/

	return bestSol;
}

/**
*	@brief	method to retrieve the last population
*
*	@return return the population
*/
GAPopulation* GASolver::get_last_population() {
    return this->population;
}

/**
*	@brief	method to get the last iteration count
*
*	@return return the last itr. count
*/
unsigned int GASolver::get_last_iterations_count() {
    return this->last_iterations_count;
}
