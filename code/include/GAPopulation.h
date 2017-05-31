/**
 *  @file    GAPopulation.h
 *  @author  Marco Romanelli
 *  @date    31/05/2017
 *  @version 1.0
 *
 *  @brief GA pupulation class header
 *
 *  @section DESCRIPTION
 *
 *  A population is a set of individuals
 *	with some features (genes) in common.
 *
 */

#ifndef GA_POPULATION_H_
#define GA_POPULATION_H_

#include "GAIndividual.h"

class GAPopulation {
	private:
		// Algorithm parameters
		double mutation_probability;
		double new_generation_ratio;

		// Population parameters
		unsigned int size;
		TSPProblem* problem;

		// Info about the population
		// Statistiche sulla popolazione
		GAIndividual* best;
		GAIndividual* worst;
		double average_fitness;

		vector< GAIndividual* > individuals;	// Actual population
		vector< GAIndividual* > mating_pool;	// current parents set


		// Methods to handle the population
		GAIndividual* montecarlo_selection(vector<GAIndividual *> individuals);
		GAIndividual* mutate(TSPSolution *individual);
		vector<GAIndividual*> extract(int n);
		GAIndividual* crossover(GAIndividual* s1, GAIndividual* s2);
		GAIndividual* choose_random(vector<GAIndividual*> pool, vector<GAIndividual*>  avoid);
		GAIndividual* choose_best(vector<GAIndividual*>  pool);

		// Methods to handle the vector of individuals
		Node weighted_choice(Node v1, Node v2, double p);
		Node best_avaible_node(Node from, vector<Node> partialPath, TSPProblem *p);
		bool is_good_choice(Node n, vector<Node> path);
		bool is_bad_choice(Node n, vector<Node> path);
		bool node_in_path(Node node, vector<Node> path);

	public:
	    GAPopulation(unsigned int size, double mutation_probability, double new_generation_ratio,  TSPProblem* problem);
	    GAIndividual* get_best_individual();
	    GAIndividual* get_worst_individual();
	    double get_average_fitness();
		void create_mating_pool(unsigned int type);
		vector< GAIndividual* > k_tournament(unsigned int n);
		void crossover(unsigned int n);
		void uniform_crossover(double probability);

		static const unsigned int K_TOURNAMENT = 0;
		static const unsigned int LINEAR_RANKING = 1;
		static const unsigned int UNIFORM_CROSSOVER = 0;
		static const unsigned int CUT_POINT_CROSSOVER = 1;
};

#endif	// GA_POPULATION_H_
