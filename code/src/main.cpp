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
 int main() {
	 string model_filename = "output/model.lp";	// path of the file in which store the model
	 string sol_filename = "output/tsp.sol";	// path of the file in which store the solution

     cout << "######################################" << endl;

     int size = 5;	// dimension of the problem
     TSPProblem* tspProblem = new TSPProblem(size);
     cout << "TSP Problem: N = " << tspProblem->getSize() << endl;
     tspProblem->printCosts();

	 cout << "######################################"<<endl;
     cout << "Solving using CPLEX... " << endl;
     CPLEXSolver* cplexSolver = new CPLEXSolver(tspProblem, model_filename);
     TSPSolution* cplexSol = cplexSolver->solve(sol_filename);

     cout << "CPLEX solution - cost: " << cplexSol->get_fitness() << endl;
     cplexSol->print_path();

	 cout << "######################################" << endl;
	 return 0;
}
