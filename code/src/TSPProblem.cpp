/**
 *  @file    TSPProblem.cpp
 *  @author  Marco Romanelli
 *  @date    29/05/2017
 *  @version 1.0
 *
 *  @brief  TSP Problem class
 *
 *  @section DESCRIPTION
 *
 *  This class represents a TSP Problem and includes
 *	variables and methods useful for its use outside.
 *
 */

#include "../include/TSPProblem.h"
#include <fstream>
#include <iostream>

using namespace std;

 /**
 *	@brief	Random instances constructor
 *
 *	@section DESCRIPTION
 *	Create a random instance of a problem with a dimension of N.
 *	Costs are randomly generated within a range of 0-100,
 *	preserving matrix simmetry.
 */
TSPProblem::TSPProblem(unsigned int N) {
	this->N = N;
	nodes.reserve(N);	// make space for the nodes and then create them
	for (unsigned int i = 0; i < N; ++i) { nodes.push_back(i); }

	this->C.resize(N);
	srand(time(NULL));

	for (unsigned int i = 0; i < N; ++i) {
		if (C[i].size() == 0) { C[i].resize(N); }
		C[i][i] = 0; // fill the diagonal with zeros
		for (unsigned int j = i+1; j < N; j++){
			C[i][j] = rand() % 99 +1;	// C_i,j: random cost (non negative)
			if (C[j].size() == 0) { C[j].resize(N); }
			C[j][i] = C[i][j];	// it's a Symmetric matrix, so C_j,i = C_i,j
		}
	}
}

/**
*	@brief	(constructor) Load an istance from a file
*/
TSPProblem::TSPProblem(string filename) {
	fstream file(filename, ios_base::in);
	file >> N;	// read problem dimension value

	nodes.reserve(N);	// create nodes for the problem
	for (unsigned int i = 0; i < N; ++i) { nodes.push_back(i); }
	C.resize(N);

	// reads costs from file
	for (unsigned int i = 0; i < N; ++i) {
		C[i].resize(N);
		for (unsigned int j = 0; j <N; j++){
			file >> C[i][j];
		}
	}
}

/**
*   @brief  Return a matrix with the costs of the problem.
*
*   @return costs matrix
*/
vector< vector<double> > TSPProblem::getCosts() {
	return this->C;
}

/**
*   @brief  Return the value of the problem's dimension
*
*   @return dimension of the problem
*/
unsigned int TSPProblem::getSize() {
	return this->N;
}

/**
*   @brief	Print the matrix of the costs
*
*   @return void
*/
void TSPProblem::printCosts() {
	cout << "matrix of costs [C_i,j]" << endl;
	cout << "i\\j\t";
	for (unsigned int i = 0; i < N; ++i)
	{
		cout << i <<"\t";
	}
	cout << endl;
	for (unsigned int i = 0; i < N; ++i) {
		cout << i <<"\t";
		for (unsigned int j = 0; j <N; j++){
			cout << C[i][j]<<"\t";
		}
		cout << endl;
	}
}

/**
*   @brief	Return the list(vector) of nodes of the problem
*
*   @return vector of nodes
*/
vector<Node> TSPProblem::getNodes() {
	return nodes;
}
