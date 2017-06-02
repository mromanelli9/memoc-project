/**
 *  @file    GAIndividual.cpp
 *  @author  Marco Romanelli
 *  @date    31/05/2017
 *  @version 1.0
 *
 *  @brief GA individual class file
 *
 *  @section DESCRIPTION
 *
 *  An individual is basically a solution.
 *	A new class was created in order to deal
 *	with possibile future changes and modifications.
 *
 */

#include "../include/TSPProblem.h"
#include "../include/GAIndividual.h"

using namespace std;

/**
*	@brief	Default contructor
*/
GAIndividual::GAIndividual(TSPProblem *problem): TSPSolution(problem) {}
GAIndividual::GAIndividual(TSPProblem *problem, vector<Node> p): TSPSolution(problem, p) {}

/**
*   @brief	function to get access to the sol. cost. value,
*			here named fitness value
*
*   @return the cost of the solution
*/
double GAIndividual::get_fitness() {
	return TSPSolution::get_solution_cost();
}
