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
	 TSPProblem* tspProblem;

	  if (argc > 1) {
		  // A filename is provide, so load the problem
		  // with the instance in it
		  std::string instance_filename = argv[1];
		  tspProblem = new TSPProblem(instance_filename);
	  } else {
		  // No istance file provide, create one randomly
		  int size = 5;	// dimension of the problem
		  tspProblem = new TSPProblem(size);
	  }

     cout << "######################################" << endl;

     cout << "TSP Problem: N = " << tspProblem->getSize() << endl;
     tspProblem->printCosts();

	 cout << "######################################"<<endl;
     cout << "Solving using CPLEX... " << endl;
     CPLEXSolver* cplexSolver = new CPLEXSolver(tspProblem);
     TSPSolution* cplexSol = cplexSolver->solve();

     cout << "CPLEX solution - cost: " << cplexSol->get_fitness() << endl;
     cplexSol->print_path();

	 cout << "######################################" << endl;
	 return 0;
}
