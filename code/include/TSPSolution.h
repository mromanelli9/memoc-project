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

class Solution {
	private:
		TSPProblem* problem;
		vector<Node> path; // Percorso
		double fitness;	// cost of the solution
		Node choose_node(Node from, vector<Node> nodes, vector< vector<double> >& C);

	public:
		TSPSolution(TSPProblem* problem);
		TSPSolution(TSPProblem* problem, vector<Node> path);
		~Solution() { problem = nullptr;}

		double get_fitness();
		vector<Node> get_path();
		void print_path();
		Node next_node(Node from);
		bool equals(TSPSolution& sol);
};

#endif
