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
#include "../include/TSPProblem.h"
//TODO: include "../include/CPLEXSolver.h"

 using namespace std;

 /**
 *	@brief	Main function
 */
 int main() {
     cout << "######################################" << endl;

     int size = 5;	// dimension of the problem
     TSPProblem* tspProblem = new TSPProblem(size);
     cout << "TSP Problem: N = " << tspProblem->getSize() << endl;
     tspProblem->printCosts();

	 cout << "######################################" << endl;
	 return 0;
}
