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
#include "../include/CPLEXSolver.h"
#include "../include/GASolver.h"

using namespace std;

 /**
 *	@brief	Main function
 */
 int main(int argc, char* argv[]) {
	if (argc != 2) { return 1; }

	unsigned int cplex_time_limit = 300;	// 5 minutes
	unsigned int ga_time_limit = 5;	// 5 seconds
	unsigned int ga_population_size_factor = 7;
	double ga_mutation_probability = 0.05;
	unsigned int ga_new_generation_ratio = 2;


	// // Create a problem from the instance provided with the argument
	// try {
	// 	 TSPProblem* tspProblem;
	// 	 std::string instance_filename = argv[1];
	// 	 tspProblem = new TSPProblem(instance_filename);
	//
	// 	 // Solving problem using CPLEX
	// 	 CPLEXSolver* cplexSolver = new CPLEXSolver(tspProblem, time_limit);
	// 	 TSPSolution* cplexSol = cplexSolver->solve();
	// } catch(std::exception& e) {
	// 	// std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	// 	return 2;
	// }

	int size = 20;
    TSPProblem* problem = new TSPProblem(size);
    cout << "Problema: N = " <<size<<endl;
    // problem->print_costs();

	CPLEXSolver* cplexSolver = new CPLEXSolver(problem, cplex_time_limit);
    TSPSolution* cplexSol = cplexSolver->solve();
	cout << "Soluzione di CPLEX - Costo "<<cplexSol->get_fitness() <<endl;
    cplexSol->print_path();

	cout << "--------------------------------------"<<endl;
    cout << "Risolvo con GA" <<endl;
    //GASolver* gaSolver = new GASolver(problem, problem->get_size()*10, 3, 0.05, 2);

	// per debug prendiamone una piccola
	GASolver* gaSolver = new GASolver(problem,\
						problem->get_size()*ga_population_size_factor,\
						ga_time_limit,\
						ga_mutation_probability,\
						ga_new_generation_ratio);
    GAIndividual* gaSol = gaSolver->solve();

	cout << "Soluzione di GA - Costo "<<gaSol->get_fitness() <<endl;
    gaSol->print_path();
    cout << "--------------------------------------" <<endl;
    cout << "Gap dall'ottimo: " << (1-(gaSol->get_fitness() / cplexSol->get_fitness()))*100 <<"%"<<endl;

	return 0;
}
