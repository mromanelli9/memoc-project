/**
 *  @file    main.cpp
 *  @author  Marco Romanelli
 *  @date    29/05/2017
 *  @version 1.0
 *
 *  @brief  Main file
 *
 *  @section DESCRIPTION
 *
 *  This file is the main file, it creates/loads instances,
 *	solve the problems, prints results and some statistics.
 *
 */

#include <iostream>
#include <string>
#include "../include/TSPProblem.h"
#include "../include/CPLEXSolver.h"

 using namespace std;

 /**
 *	@brief	Main function
 */
 int main(int argc, char* argv[]) {
	if (argc != 2) { return 1; }

	unsigned int time_limit = 300;	// 5 minutes

	// Create a problem from the instance provided with the argument
	try {
		 TSPProblem* tspProblem;
		 std::string instance_filename = argv[1];
		 tspProblem = new TSPProblem(instance_filename);

		 // Solving problem using CPLEX
		 CPLEXSolver* cplexSolver = new CPLEXSolver(tspProblem, time_limit);
		 TSPSolution* cplexSol = cplexSolver->solve();
	} catch(std::exception& e) {
		// std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
		return 2;
	}
	return 0;
}
