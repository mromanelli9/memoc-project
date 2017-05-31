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

	// [!DEBUG]
	cout << "Popolazione iniziale:" <<endl;
    GAIndividual* worstSol = population->get_worst_individual();
    cout << "Worst: " <<worstSol->get_fitness();
    delete worstSol;
    cout << "\t Avg: " <<population->get_average_fitness();
    GAIndividual* bestSol = population->get_best_individual();
    cout << "\t Best: " <<bestSol->get_fitness() << endl;
    delete bestSol;


	// Start main loop. Stop when time exceed
	long long t_start = current_timestamp();	// time in milliseconds
	unsigned int i = 0;	// current itereation
	// while ((current_timestamp() - t_start) < this->time_limit) {
		i++;

		// Phase 2: select the mating pool
		cout << "Creo i genitori "<<endl;
		population->create_mating_pool(GAPopulation::K_TOURNAMENT);

		// Phase 3: crossover to generate offsprings
		cout << "li accoppio" <<endl;
		population->crossover(GAPopulation::UNIFORM_CROSSOVER);

		// Phase 4: mutation

		// Phase 5: population management

		// [!DEBUG]
		// if (i % 500 == 0) {
		// 	cout << "Iterazione "  <<i <<endl;
		// 	worstSol = population->get_worst_individual();
		// 	cout << "Worst: " <<worstSol->get_fitness();
		// 	delete worstSol;
		// 	cout << "\t Avg: " <<population->get_average_fitness();
		// 	bestSol = population->get_best_individual();
		// 	cout << "\t Best: " <<bestSol->get_fitness() << endl;
		// 	delete bestSol;
		// }
	// }

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

/**
*	@brief	get time in milliseconds
*
*	@return return time [milliseconds]
*/
long long GASolver::current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);	// get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;	// caculate milliseconds
    return milliseconds;
}
