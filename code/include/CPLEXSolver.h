/**
 *  @file    CPLEXSolver.h
 *  @author  Marco Romanelli
 *  @date    29/05/2017
 *  @version 1.0
 *
 *  @brief TSP Solver class header
 *
 *  @section DESCRIPTION
 *
 *  This class is used to solve a TSP problem with CPLEX
 *
 */

#ifndef CPLEX_SOLVER_H_
#define CPLEX_SOLVER_H_

#include "TSPProblem.h"
#include "TSPSolution.h"
#include "cpxmacro.h"

class CPLEXSolver {
	private:
		static const int NAME_SIZE = 512;

		Env env;	// CPLEX Enviroment
		Prob lp;	// CPLEX Problem

		int status;	// error status and messagge buffer
		char errmsg[BUF_SIZE];
		char name[NAME_SIZE];

		std::vector< std::vector<int> > xMap;	// used for store variables x_i,j
		std::vector< std::vector<int> > yMap;	// used for store variables y_i,j

		TSPProblem* problem;	// the TSP problem to solve

		vector<Node> extractPath(vector<double> vals, int start=0, int cnt=0);

	public:
		CPLEXSolver(TSPProblem* problem);
		CPLEXSolver(TSPProblem* problem, unsigned int time_limit);
		Solution* solve();
};

#endif	// CPLEX_SOLVER_H_
