/**
 *  @file    TSPSolution.h
 *  @author  Marco Romanelli
 *  @date    29/05/2017
 *  @version 1.0
 *
 *  @brief TSP Solution class header
 *
 *  @section DESCRIPTION
 *
 *  This class represents a solution for the TSP problem.
 *
 */

#ifndef TSP_SOLUTION_H_
#define TSP_SOLUTION_H_

#include <cstdlib>
#include <vector>
#include <iostream>
#include "TSPProblem.h"

using namespace std;

class TSPSolution {
	protected:
		TSPProblem* problem;
		vector<Node> path;
		double solution_cost;
		Node choose_node(Node from, vector<Node> nodes, vector< vector<double> >& C);

	public:
		TSPSolution(TSPProblem* problem, vector<Node> path);
		TSPSolution(TSPProblem* problem);
		// ~TCPSolution() { problem = nullptr;}

		double get_solution_cost();
	    vector<Node> get_path();
	    void print_path();
	    Node next_node(Node from);
	    bool equals(TSPSolution& sol);
};

#endif	// TSP_SOLUTION_H_
