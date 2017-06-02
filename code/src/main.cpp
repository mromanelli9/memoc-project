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
#include <sys/time.h>
#include "../include/CPLEXSolver.h"
#include "../include/GASolver.h"

using namespace std;

// Prototypes
long long current_time();

void single_test();

void run_istances();

 /**
 *	@brief	Main function
 */
 int main(int argc, char* argv[]) {
	// if (argc != 2) { return 1; }

	single_test();

	return 0;
}

void single_test() {
	unsigned int cplex_time_limit = 10;	// 5 minutes

	int size = 20;
	TSPProblem* problem = new TSPProblem(size);
	cout << "Problema: N = " <<size<<endl;
	// problem->print_costs();

	long long e_time, s_time;

	s_time = current_time();
	CPLEXSolver* cplexSolver = new CPLEXSolver(problem, cplex_time_limit);
	TSPSolution* cplexSol = cplexSolver->solve();
	e_time = current_time();

	cout << "Soluzione di CPLEX - Costo "<<cplexSol->get_solution_cost() <<endl;
	cplexSol->print_path();
	cout << "Tempo " << (e_time - s_time) << endl;
	cout << "--------------------------------------"<<endl;

	unsigned int ga_time_limit = 3;
	unsigned int ga_population_size_factor = 5;
	double ga_mutation_probability = 0.02;

	cout << "Risolvo con GA" <<endl;
	//GASolver* gaSolver = new GASolver(problem, problem->get_size()*10, 3, 0.05, 2);

	// per debug prendiamone una piccola
	s_time = current_time();
	GASolver* gaSolver = new GASolver(problem,\
						problem->get_size()*ga_population_size_factor,\
						ga_time_limit,\
						ga_mutation_probability);
	GAIndividual* gaSol = gaSolver->solve();
	e_time = current_time();

	cout << "Soluzione di GA - Costo "<<gaSol->get_fitness() <<endl;
	gaSol->print_path();
	cout << "Tempo " << (e_time - s_time) << endl;
	cout << "--------------------------------------" <<endl;
	cout << "Gap dall'ottimo: " << (1-(gaSol->get_fitness() / cplexSol->get_solution_cost()))*100 <<"%"<<endl;
}

void run_istances() {
	// unsigned int cplex_time_limit = 10;	// 5 minutes

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
}


// [!DEBUG]
long long current_time() {
    struct timeval te;
    gettimeofday(&te, NULL);	// get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;	// caculate milliseconds
    return milliseconds;
}
