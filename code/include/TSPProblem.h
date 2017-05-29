/**
 *  @file    TSPProblem.h
 *  @author  Marco Romanelli
 *  @date    29/05/2017
 *  @version 1.0
 *
 *  @brief TSP Problem class header
 *
 *  @section DESCRIPTION
 *
 *  This class represents a TSP Problem and includes
 *	variables and methods useful for its use outside.
 *
 */

#ifndef TSP_PROBLEM_H_
#define TSP_PROBLEM_H_

#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

typedef int Node;	// It's an integer, used to distinguish between nodes and indexes

class TSPProblem {
	private:
		vector< vector<double> > C;	// costs
		unsigned int N;	// dimension
		vector<Node> nodes;

	public:
		TSPProblem(unsigned int N);
		TSPProblem(string fileName);
		vector< vector<double> > getCosts();
		unsigned int getSize();
		void printCosts();
		vector<Node> getNodes();
};

#endif	// TSP_PROBLEM_H_
