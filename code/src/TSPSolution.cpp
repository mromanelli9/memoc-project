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

#include "../include/TSPSolution.h"
#include "../include/TSPProblem.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;

/**
*	@brief	Constructor: create a solution for the model.
*			Method: pseudo-greey
*/
// TODO: rivedere questa parte
TSPSolution::TSPSolution(TSPProblem *problem) {
	this->problem = problem;

	// Costruisco la soluzione in modo pseudo-casuale: come successore scelgo a caso tra quelli possibili, tenendo conto di quanto migliorano
	// choose the next successor randomly among the possibilies
	vector< vector<double> > C = problem->get_costs();
	unsigned int N = problem->get_size();
	fitness = 0;
	path.resize(N + 1);
	path[0] = 0;	// starting node is 0

	vector<Node> nodes(N-1);	// vector of nodes to be choosen
	for (unsigned int i = 0; i < N-1; ++i) {
		nodes[i] = i+1;
	}

	for (unsigned int i = 1; i < N; ++i) {
		Node selected_node = choose_node(path[i-1], nodes, C);
		fitness += C[path[i-1]][selected_node];	// compute the cost of the solution so far
		path[i] = selected_node;

		// re-compute the vector of possible nodes
		vector<Node> new_nodes;
		for (unsigned int j = 0; j < nodes.size(); ++j) {
			if (nodes[j] != selected_node) { new_nodes.push_back(nodes[j]);}
		}
		assert(nodes.size() == new_nodes.size()+1);
		nodes = new_nodes;
	}
	fitness += C[path[N-1]][0];	// cost of the second-to-last node
	path[N] = 0;	// ending node should be always 0
}

/**
*	@brief	Constructor: create a solution of the model
*			from a given solution path
*/
TSPSolution::TSPSolution(TSPProblem *problem, vector<Node> p) {
	this->problem = problem;
	unsigned int N = problem->get_size();

	vector< vector<double> > C = problem->get_costs();

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

	// assert(debug_sum == debug_sum_2);	// note: told to do so but not sure why
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
*   @brief	function to get access to the path value
*
*   @return return the path value
*/
vector<Node> TSPSolution::get_path() {
	return this->path;
}

/**
*   @brief	print in the standard ouput the path
*
*   @return void
*/
void TSPSolution::print_path() {
	cout << "Fitness: " << this->fitness << endl;
	for (unsigned int i = 0; i < this->path.size(); ++i) {
		cout << this->path[i];
		if (i != this->path.size()-1) { cout << " -> ";}
	}
	cout << endl;
}

/**
*   @brief	return the next node in the path
*
*   @return return a node
*/
Node TSPSolution::next_node(Node from) {
	for (unsigned int i = 0; i < this->path.size()-1; ++i) {
		if (this->path[i] == from){
			return this->path[i+1];
		}
	}

	assert(false);	// shouldn't be here --> error
	return -1;
}

bool TSPSolution::equals(TSPSolution &sol) {
	return this->path == sol.path;
}


/**
*   @brief	choose (weighted) wich node should be used
*	as destination node starting from node "from".
*
*   @return return a node
*/
// TODO: rivedere questa parte
Node TSPSolution::choose_node(Node from, vector<Node> nodes, vector< vector<double> >& C){
	if (nodes.size() == 1){
		return nodes[0];
	}

	double tot = 0;
	for (unsigned int i = 0; i < nodes.size(); ++i) {
		tot += C[from][nodes[i]];
	}
	vector<double> adjusted_costs;	// less cost --> more probability

	double adjusted_tot = 0;
	for (unsigned int i = 0; i < nodes.size(); ++i) {
		double adjusted_cost = tot - C[from][nodes[i]];
		adjusted_costs.push_back(adjusted_cost);
		adjusted_tot += adjusted_cost; // Aggiungo l'ultimo elemento al totale
	}

	double val = (rand() / (double) RAND_MAX) * adjusted_tot;	// random number in [0, tot]
	assert(val >= 0);
	assert(val <= adjusted_tot);

	// sum over all probability until they becom bigger than the value we got
	// when that appens: the index which cause that is the one randomly chosen
	unsigned int i = 0;
	double sum = 0;
	while (val > sum){
		sum += adjusted_costs[i];
		if (val > sum) {i++;} // evito di incrementare all'ultima iterazione
	}
	assert(i < nodes.size());
	return nodes[i];
}
