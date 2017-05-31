/**
 *  @file    GAPopulation.cpp
 *  @author  Marco Romanelli
 *  @date    31/05/2017
 *  @version 1.0
 *
 *  @brief GA population class file
 *
 *  @section DESCRIPTION
 *
 *  A population is a set of individuals
 *	with some features (genes) in common.
 *
 */

#include "../include/GAPopulation.h"
#include "../include/GAIndividual.h"
#include <assert.h>
#include <algorithm>
#include <climits>
#include <stdexcept>

using namespace std;

/**
*	@brief	Default contructor
*/
GAPopulation::GAPopulation(unsigned int size, double mutation_probability, double new_generation_ratio, TSPProblem *problem) {
	// Store parameters
	this->size = size;
    this->problem = problem;
    this->new_generation_ratio = new_generation_ratio;
    this->mutation_probability = mutation_probability;
    this->average_fitness = 0;
    double best = INT_MAX;
    double worst = 0;
    int bi = 0, wi = 0;

	// Populate the population (= set of individuals)
    for (unsigned int i = 0; i < size; ++i) {
        individuals.push_back(new GAIndividual(problem));
        double c_fitness = individuals.at(i)->get_fitness();
        if (c_fitness < best){
            bi = i;
            best = c_fitness;
        }
        if (c_fitness > worst){
            wi = i;
            worst = c_fitness;
        }

        this->average_fitness += c_fitness;
    }
    this->average_fitness /= size;

    this->best = new GAIndividual(problem, individuals.at(bi)->get_path());
    this->worst = new GAIndividual(problem, individuals.at(wi)->get_path());
}

/**
*	@brief	create a mating pool based on type
*
*	@return set of individuals
*/
void GAPopulation::create_mating_pool(unsigned int type) {
	// Init a mating pool (size: 2*real-size)
	switch (type) {
		case GAPopulation::K_TOURNAMENT:
			this->mating_pool = k_tournament(10);
			break;
		case GAPopulation::LINEAR_RANKING:
			throw std::runtime_error("Function not implemeted.");
			break;
	}

	// Filter the current mating pool with montecarlo method
	//montecarlo_selection(mating_pool)
}

/**
*	@brief	K-tournament selection method,
*			where K = size / n
*
*	@return set of individuals
*/
vector< GAIndividual* > GAPopulation::k_tournament(unsigned int n) {
	vector< GAIndividual* > parents;		// current set of individuals

	// loop (rounds)
	for (unsigned int i = 0; i < this->size; ++i) {

		vector<GAIndividual*> winners;	// Select two individuals at each round
	    int K = this->size / n;

	    for (unsigned int j = 0; j < this->new_generation_ratio; ++j) {
	        vector<GAIndividual*> participants(K);
	        vector<GAIndividual*> avoid(winners);	// avoid past individuals
	        for (unsigned int j = 0; j < K; ++j) {
	            participants[j] = choose_random(this->individuals, avoid);
	            avoid.push_back(participants[j]);
	        }
	        winners.push_back(choose_best(participants));
	    }

		for (unsigned int j = 0; j < this->new_generation_ratio; ++j) {
	    	parents.push_back(winners.at(j));
		}
	}

	// std::replace(parents.begin(), parents.end(), parents, pool);
	// vector< GAIndividual* >().swap(parents);
	return parents;
}

/**
*	@brief	select a crossover on type
*
*	@return void
*/
void GAPopulation::crossover(unsigned int type) {
	switch (type) {
		case GAPopulation::UNIFORM_CROSSOVER:
			uniform_crossover(this->mutation_probability);
			break;
		case GAPopulation::CUT_POINT_CROSSOVER:
			throw std::runtime_error("Function not implemeted.");
			break;
	}

	// Filter the current mating pool with montecarlo method
	//montecarlo_selection(mating_pool)
}

/**
*	@brief	Uniform crossover: genes from first parent
*			are inherited by the son with probability p
*			and from the second parent with probability
*			(1-p).
*
*	@return void
*/
void GAPopulation::uniform_crossover(double probability) {
	for (unsigned int i = 0; i < this->size*this->new_generation_ratio; i += 2) {
		GAIndividual* p1 = this->mating_pool.at(i);
		GAIndividual* p2 = this->mating_pool.at(i+1);

		unsigned int N = this->problem->get_size();
		vector<Node> newPath(N+1);
		newPath[0] = newPath[N] = 0;
		vector<Node> path1(p1->get_path());
		vector<Node> path2(p2->get_path());

		for (int i = 1; i < N; ++i) {
			Node currentNode = newPath[i-1];
			Node s1Next = p1->next_node(currentNode);
			Node s2Next = p2->next_node(currentNode);
			/* Casi possibili
			 * s1Next = 0 e s2Next != 0 e notInPath => scelgo s2Next
			 * s1Next = 0 e s2Next = 0 oppure s2Next inPath => correggo scegliendo tra quelli rimasti il migliore
			 * (^ ci sono anche i casi simmetrici)
			 * s1Next e s2Next entrambi in path => correggo scegliendo tra quelli rimasti il migliore
			 * s1Next e s2Next entrambi buoni => scelta pesata
			 * */

			// Solo uno dei due è una buona scelta
			if (is_bad_choice(s1Next, newPath) && is_good_choice(s2Next, newPath)) { newPath[i] = s2Next; continue; }
			if (is_bad_choice(s2Next, newPath) && is_good_choice(s1Next, newPath)) { newPath[i] = s1Next; continue; }
			// Entrambi sono una brutta scelta
			if (is_bad_choice(s1Next, newPath) && is_good_choice(s2Next, newPath) ) { newPath[i] = best_avaible_node(currentNode, newPath, problem); continue; }


			// Entrambi i nodi sono nodi buoni
			double tot = p1->get_fitness() + p2->get_fitness();
			double prob = p2->get_fitness()/tot;
			newPath[i] = weighted_choice(s1Next, s2Next, 1-prob);
		}

		this->individuals.push_back(new GAIndividual(problem, newPath));
	}
}


/**
*	@brief	return the best individual in the population
*
*	@return value of the best individual
*/
GAIndividual* GAPopulation::get_best_individual() {
    return this->best;
}

/**
*	@brief	return the worst individual in the population
*
*	@return value of the worst individual
*/
GAIndividual* GAPopulation::get_worst_individual() {
    return this->worst;
}

/**
*	@brief	return the averge individual in the population
*
*	@return value of the average individual
*/
double GAPopulation::get_average_fitness() {
    return this->average_fitness;
}

/**
*	@brief	get the best individual
*
*	@return an individual
*/
GAIndividual* GAPopulation::choose_best(vector<GAIndividual*> pool){
    double max = INT_MAX;
    unsigned long index = pool.size()+1;
    for (unsigned long i = 0; i < pool.size(); ++i) {
        if (pool.at(i)->get_fitness() < max){
            max = pool.at(i)->get_fitness();
            index = i;
        }
    }
    return pool.at(index);
}

/**
*	@brief	get a random individual
*
*	@return an individual
*/
GAIndividual* GAPopulation::choose_random(vector<GAIndividual*> pool, vector<GAIndividual*>  avoid){
    while(true) {
        unsigned int val = rand() % (unsigned int)pool.size();
        GAIndividual* candidate = pool.at(val);

        bool good_candidate = true;
        for (unsigned int i = 0; i < avoid.size(); ++i) {
            if (avoid.at(i) == candidate) {
                good_candidate = false;
                break;
            }
        }
        if (good_candidate){
            return candidate;
        }
    }
}

/**
*	@brief	choose a node based on his probability
*
*	@return a node
*/
Node GAPopulation::weighted_choice(Node v1, Node v2, double p) {
    double r = rand() / double(RAND_MAX);
    assert(r <= 1);
    if (r < p) {
        return v1;
    } else {
        return v2;
    }
}

 /**
 *	@brief	search for the best node to choose
 *
 *	@return a node
 */
Node GAPopulation::best_avaible_node(Node from, vector<Node> partialPath, TSPProblem* p){
    vector<Node > availableNodes;
    vector<Node> problemNodes = p->get_nodes();
    for (unsigned int i = 0; i < problemNodes.size(); ++i) {
        if (!node_in_path(problemNodes[i], partialPath)) {
            availableNodes.push_back(problemNodes[i]);
        }
    }
    double minCost = (double)INT_MAX;
    int minIndex = -1;
    for (unsigned int i = 0; i < availableNodes.size(); ++i) {
        double c = p->get_costs()[from][availableNodes[i]];
        if ( c < minCost){
            minCost = c;
            minIndex = i;
        }
    }
    return availableNodes[minIndex];
}

 /**
 *	@brief	check if node n can be used or not in path
 *
 *	@return boolean
 */
bool GAPopulation::is_good_choice(Node n, vector<Node> path){
    return n != 0 && !node_in_path(n, path);
}

 /**
 *	@brief	check if node n can't be used  in path
 *
 *	@return boolean
 */
bool GAPopulation::is_bad_choice(Node n, vector<Node> path){
    return n == 0 || node_in_path(n, path);
}

/**
*	@brief	check if node is in the path
*
*	@return boolean
*/
bool GAPopulation::node_in_path(Node node, vector< Node > path){
    return std::find(path.begin(), path.end(), node) != path.end();
}
