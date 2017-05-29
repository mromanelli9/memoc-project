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
	private:
		TSPProblem* problem;
		vector<Node> path; // Percorso
		double fitness;	// cost of the solution

	public:
		TSPSolution(TSPProblem* problem, vector<Node> path);
		// TSPSolution(TSPProblem* problem);
		// ~TCPSolution() { problem = nullptr;}

		double get_fitness();
		void print_path();
};

#endif
