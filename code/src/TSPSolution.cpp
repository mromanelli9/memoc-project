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
*	@brief	Constructor: create a solution of the model.
*			Method: pseudo-greey
*/
// TODO: rivedere questa parte
TSPSolution::TSPSolution(TSPProblem *problem) {
    this->problem = problem;

    // Costruisco la soluzione in modo pseudo-casuale: come successore scelgo a caso tra quelli possibili, tenendo conto di quanto migliorano
    vector< vector<double> > C = problem->get_costs();
    unsigned int N = problem->get_size();
    fitness = 0;
    path.resize(N + 1);
    path[0] = 0; // parto sempre dal nodo 0

    vector<Node> nodes(N-1); // Creo un vettore con i nodi ancora da scegliere per il percorso (da notare che non c'è il nodo 0)
    for (unsigned int i = 0; i < N-1; ++i) {
        nodes[i] = i+1;
    }

    for (unsigned int i = 1; i < N; ++i) {
        Node selected_node = choose_node(path[i-1], nodes, C);
        fitness += C[path[i-1]][selected_node]; // man mano che proseguo calcolo il costo della soluzione
        path[i] = selected_node;

        // ricalcolo il vettore dei nodi disponibili
        vector<Node> new_nodes;
        for (unsigned int j = 0; j < nodes.size(); ++j) {
            if (nodes[j] != selected_node) { new_nodes.push_back(nodes[j]);}
        }
        assert(nodes.size() == new_nodes.size()+1);
        nodes = new_nodes;
    }
    fitness += C[path[N-1]][0]; // costo dal penultimo nodo, al quello di partenza (il +0 è per chiarezza)
    path[N] = 0; // il nodo di arrivo deve essere sempre 0

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
 * Effettua la scelta pesata di un nodo da utilizzare come destinazione a partire dal nodo from
 * */
Node TSPSolution::choose_node(Node from, vector<Node> nodes, vector< vector<double> >& C){
    if (nodes.size() == 1){
        return nodes[0];
    }

    double tot = 0;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        tot += C[from][nodes[i]];
    }
    vector<double> adjustedCosts; // "Aggiusto i costi" in modo che quelli che costano poco abbiano maggior probabilità
    // di essere scelti

    double adjustedTot = 0;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        double adjustedCost = tot - C[from][nodes[i]];
        adjustedCosts.push_back(adjustedCost);
        adjustedTot += adjustedCost; // Aggiungo l'ultimo elemento al totale
    }

    double val = (rand() / (double) RAND_MAX) * adjustedTot; // genero un numero tra 0 e tot
    assert(val >= 0);
    assert(val <= adjustedTot);
    // sommo tutte le probabilità, finché non diventano maggiori del valore ottenuto
    // quando diventano maggiori vuol dire che l'indice che ha reso maggiore in numero è quello scelto casualente
    unsigned int i = 0;
    double sum = 0;
    while (val > sum){
        sum += adjustedCosts[i];
        if (val > sum) {i++;} // evito di incrementare all'ultima iterazione
    }
    assert(i < nodes.size());
    return nodes[i];
}
