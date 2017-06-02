/**
 *  @file    GAIndividual.h
 *  @author  Marco Romanelli
 *  @date    31/05/2017
 *  @version 1.0
 *
 *  @brief GA individual class header
 *
 *  @section DESCRIPTION
 *
 *  An individual is basically a solution.
 *	A new class was created in order to deal
 *	with possibile future changes and modifications.
 *
 */

#ifndef GA_INDIVIDUAL_H_
#define GA_INDIVIDUAL_H_

#include "TSPSolution.h"
#include "TSPProblem.h"

class GAIndividual : public TSPSolution {
	public:
	    GAIndividual(TSPProblem *problem);
		GAIndividual(TSPProblem* problem, vector<Node> path);

		double get_fitness();
};

#endif	// GA_INDIVIDUAL_H_
