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
#include <fstream>
#include <vector>

using namespace std;

int getdir (string dir, vector<string> &files);
bool is_file(const char* path);
bool is_dir(const char* path);
int single_test(string filename);
int run_tests(vector<string> &files);
long long current_timestamp();

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

		 // Some cleaning: filter only .tsp files
		 vector<std::string> instances;
		 for (auto & el : files) {
			 std::string ext;
			 try {
				 ext = el.substr( el.size()-4, el.size()-1);
			 } catch(std::exception& e) { continue; }

			 if (ext == ".tsp") {
				 instances.push_back( input + "/" + el );
			 }
		 }

		 files.clear();
		 std::sort(instances.begin(), instances.end());

		 // Go!
		 return run_tests(instances);
	 } else if (is_file(input.c_str())) {
		 // GO!
		 return  single_test(input);
	 } else {
		 cout << "Error." << endl;
		 return -2;
	 }

	 return -1;
}

/**
*	@brief	Run all the instance and solvs them
*
*	@return exit status (int)
*/
int run_tests(vector<string> &files) {
	long long s_time, e_time, cplex_time, ga_time;
	std::string separator = "\t";
	std::string output_file = "results.txt";
	ofstream myfile;

	cout << "############   TSP SOLVER  ############"<< endl;

	// Set up all the parameters
	unsigned int time_limit = 60 * 5;	// 5 minutes
	unsigned int ga_iteration_limit = 1000;	// maximum number of iterations
	unsigned int ga_population_size_factor = 3;	// the population will have a number
												// of individuals set to this value times problem-size
	double ga_mutation_probability = 0.05;	// probability of mutation
	bool verbose = false;	// To speed up the computation, do not show any messages

	try {
		myfile.open(output_file, ios::out);
		myfile << "\"Dimension\"" << separator;
		myfile << "\"CPLEX Time\"" << separator;
		myfile << "\"CPLEX Sol.\"" << separator;
		myfile << "\"GA Sol.\"" << separator;
		myfile << "\"GA Time\"" << separator;
		myfile << "\r\n" << std::flush;
	} catch(std::exception& e) {
		std::cout << "[!] EXCEPTION: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	// For each instance:
	for (unsigned int i = 0; i < files.size(); i++) {
		std::string instance = files[i];
		cout << "Now running on: \'" << instance << "\'..." << endl;

		// Create a new problem based on date provided in the file
		TSPProblem* tspProblem = new TSPProblem(instance);

		// Solving problem using CPLEX
		cout << "  with CPLEX..."  << endl;
		TSPSolution* cplexSol = NULL;
		try {
			s_time = current_timestamp();
			CPLEXSolver* cplexSolver = new CPLEXSolver(tspProblem, time_limit);
			cplexSol = cplexSolver->solve();
			e_time = current_timestamp();
			cplex_time = e_time - s_time;
		} catch(std::exception& e) {
			cplexSol = NULL;
			cplex_time = -1;
		}

		// Solving problem using GA
		cout << "  with GA..."  << endl;
		s_time = current_timestamp();
		GASolver* gaSolver = new GASolver(tspProblem,\
										ga_population_size_factor,\
										time_limit,\
										ga_iteration_limit,\
										ga_mutation_probability,
										verbose);
		GAIndividual* gaSol = gaSolver->solve();
		e_time = current_timestamp();
		ga_time = e_time - s_time;

		try {
			// Save cplex results
			myfile << tspProblem->get_size() << separator;
			myfile << cplex_time << separator;
			myfile <<  ((cplexSol != NULL) ? cplexSol->get_solution_cost() : -1) << separator;

			// Save ga results
			myfile << ga_time << separator;
			myfile << gaSol->get_fitness();
			// ga will always find a solution, so we're not saving the success/failure flag

			// End of the line
			myfile << "\r\n" << std::flush;
		} catch(std::exception& e) {
			std::cout << "[!] EXCEPTION: " << e.what() << std::endl;
			return EXIT_FAILURE;
		}


	}

	try {
		myfile.close();
	} catch(std::exception& e) {
		std::cout << "[!] EXCEPTION: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	cout << endl << endl;

	return 0;
}

/**
*	@brief	Run a single instance provided in the file.
*			To use for testing and examples.
*
*	@return exit status (int)

*/
int single_test(string filename) {
	long long s_time, e_time, cplex_time, ga_time;

	cout << "############   TSP SOLVER  ############"<< endl;

	unsigned int time_limit = 60 * 1;	// 1 minute

	// Create a new problem based on date provided in the file
	TSPProblem* tspProblem = new TSPProblem(filename);
	cout << "Problem dimension: " << tspProblem->get_size() << "." << endl;

	// Solving problem using CPLEX
	TSPSolution* cplexSol;
	// try {
	// 	// Initialize the solver
	// 	cout << "Solving with CPLEX..." <<endl;
	//
	// 	s_time = current_timestamp();
	// 	CPLEXSolver* cplexSolver = new CPLEXSolver(tspProblem, time_limit);
	//
	// 	// Solve the problem with CPLEX
	// 	cplexSol = cplexSolver->solve();
	// 	e_time = current_timestamp();
	//
	// 	cplex_time = e_time - s_time;
	// } catch(std::exception& e) {
	// 	cplexSol = NULL;
	// 	cplex_time = -1;
	// 	std::cout << "[!] EXCEPTION: " << e.what() << std::endl;
	// }
	// cout << " Done." << endl;

	// Solving problem using the heuristic (GA)

	// Parameters
	unsigned int ga_iteration_limit = 1000;	// maximum number of iterations
	unsigned int ga_population_size_factor = 3;	// the population will have a number
												// of individuals set to this value times problem-size
	double ga_mutation_probability = 0.05;	// probability of mutation
	bool verbose = true;

	cout << "Solving with GA..." <<endl;
	s_time = current_timestamp();
	GASolver* gaSolver = new GASolver(tspProblem,\
									ga_population_size_factor,\
									time_limit,\
									ga_iteration_limit,\
									ga_mutation_probability,
									verbose);
	GAIndividual* gaSol = gaSolver->solve();
	e_time = current_timestamp();
	ga_time = e_time - s_time;
	cout << " Done." << endl;

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

		cout << " Time: " << cplex_time << " [ms]." << endl;
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
	cout << " Time: " << ga_time << " [ms]." << endl;
	cout << endl;

	if ( cplexSol != NULL) {
		cout << "Gap between solutions : " << \
			(1-(gaSol->get_fitness() / cplexSol->get_solution_cost())) * 100 << \
			"%" << "." << endl;
	}

	return 0;
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

/**
*	@brief	Get time in milliseconds
*
*	@return return time [milliseconds]
*/
long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);	// get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;	// caculate milliseconds
    return milliseconds;
}
