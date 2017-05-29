/**
 *  @file    TSPSolution.h
 *  @author  Marco Romanelli
 *  @date    29/05/2017
 *  @version 1.0
 *
 *  @brief  TSP Problem class
 *
 *  @section DESCRIPTION
 *
 *  This class represents a solution for the TSP problem.
 *
 */

#include "../include/TSPProblem.h"
#include <fstream>
#include <iostream>

using namespace std;

/**
*	@brief	Constructor: create a solution of the model
*			from a given solution path
*/
TSPSolution::TSPSolution(TSProblem *problem, vector<Node> p) {
	this->problem = problem;
	unsigned int N = problem->getSize();

	vector< vector<double> > C = problem->getCosts();

	// create and then "copy" the given path
	this->path.resize(p.size());
	for (unsigned int j = 0; j < path.size(); ++j) { path[j] = p[j]; }

	this->fitness = 0;
	int debug_sum = 0;
	int debug_sum_2 = 0;
	for (unsigned int i = 0; i < N; ++i) {
		debug_sum += path[i];
		debug_sum_2 += i;
		this->fitness += C[path[i]][path[i+1]];
	}

	assert(debug_sum == debug_sum_2);	// note: told to do so but not sure why
}

/**
*   @brief	function to get access to the fitness value
*
*   @return return the fitness value
*/
double TSPSolution::get_fitness() {
	return this->fitness;
}

/**
*   @brief	print in the standard ouput the path
*
*   @return void
*/
void Solution::print_path() {
	cout << "Fitness: " << this->fitness << endl;
	for (unsigned int i = 0; i < path.size(); ++i) {
		cout << path[i];
		if (i != path.size()-1) { cout << " -> ";}
	}
	cout << endl;
}
