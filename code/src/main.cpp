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
 *  This file is the main file: it loads all the instances,
 *	solve the problems, prints results and some statistics.
 *
 */

#include "../include/CPLEXSolver.h"
#include "../include/GASolver.h"
#include <iostream>
#include <string>
#include <sys/time.h>
#include <dirent.h>
#include <algorithm>
#include <sys/stat.h>

using namespace std;

int getdir (string dir, vector<string> &files);
bool is_file(const char* path);
bool is_dir(const char* path);
void single_test(string filename);
void run_tests(vector<string> &files);


 /**
 *	@brief	Main function
 *
 */
 int main(int argc, char* argv[]) {
	 if (argc != 2) { return -1; }

	 std::string input = argv[1];
	 vector<std::string> files = vector<std::string>();

	 if (is_dir(input.c_str())) {
		 // Read instances filenames
		 getdir(input, files);
		 std::sort(files.begin(), files.end());

		 // Remove . and ..
		 files.erase(files.begin(), files.begin()+2);

		 // Go!
		 run_tests(files);
	 } else if (is_file(input.c_str())) {
		 // GO!
		 single_test(input);
	 } else {
		 cout << "Error." << endl;
		 return -2;
	 }

	 return 0;
}

void run_tests(vector<string> &files) {
	for (unsigned int i = 0;i < files.size();i++) {
		 cout << files[i] << endl;
	}
}

void single_test(string filename) {
	cout << "######################################"<< endl;

	// Create a new problem based on date provided in the file
	TSPProblem* tspProblem = new TSPProblem(filename);
	cout << "Problem dimension: " << tspProblem->get_size() << "." << endl;

	// Solving problem using CPLEX
	TSPSolution* cplexSol;
	try {
		// Initialize the solver
		cout << "Solving with CPLEX..." <<endl;
		unsigned int cplex_time_limit = 10;	// 5 minutes
		CPLEXSolver* cplexSolver = new CPLEXSolver(tspProblem, cplex_time_limit);

		// Solve the problem with CPLEX
		cplexSol = cplexSolver->solve();
	} catch(std::exception& e) {
		cplexSol = NULL;
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
	cout << " Done." << endl;

	// Solving problem using the heuristic (GA)

	// Parameters
	unsigned int ga_time_limit = 10;	// 10 seconds
	unsigned int ga_iteration_limit = 1000;	// maximum number of iterations
	unsigned int ga_population_size_factor = 5;	// the population will have a number
												// of individuals set to 5 * problem-size
	double ga_mutation_probability = 0.02;	// probability of mutation

	cout << "Solving with GA..." <<endl;
	GASolver* gaSolver = new GASolver(tspProblem,\
									ga_population_size_factor,\
									ga_time_limit,\
									ga_iteration_limit,\
									ga_mutation_probability);
	GAIndividual* gaSol = gaSolver->solve();

	cout << endl;
	cout << "--------------  CPLEX	--------------"<<endl;
	if ( cplexSol != NULL) {
		cout << " Solution found." << endl;
		cout << " Cost: " << cplexSol->get_solution_cost() << "." << endl;
		cout << " Path: ";

		if (tspProblem->get_size() <= 20) {
			cplexSol->print_path();
		} else {
			cout << " too long. " << endl;
		}
	} else {
		cout << " No solution found.";
	}
	cout << endl;

	cout << "--------------    GA 	--------------" << endl;
	cout << " Solution found." << endl;
	cout << " Cost: " << gaSol->get_fitness() << "." << endl;
	cout << " Path: ";
	if (tspProblem->get_size() <= 20) {
		gaSol->print_path();
	} else {
		cout << " too long. " << endl;
	}
	cout << endl;

	if ( cplexSol != NULL) {
		cout << "Gap between solutions : " << \
			(1-(gaSol->get_fitness() / cplexSol->get_solution_cost())) * 100 << \
			"%" << "." << endl;
	}
}


/**
*	@brief	Read files in a given directory
*/
int getdir (string dir, vector<string> &files) {
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}

/**
*	@brief	Return true if the argument is a regular file
*
*	@return boolean
*/
bool is_file(const char* path) {
	struct stat buf;
	stat(path, &buf);
	return S_ISREG(buf.st_mode);
}

/**
*	@brief	Return true if the argument is a directory
*
*	@return boolean
*/
bool is_dir(const char* path) {
	struct stat buf;
	stat(path, &buf);
	return S_ISDIR(buf.st_mode);
}
