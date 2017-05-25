/**
	@file TSPSolver.cpp
	@brief	Main file
	@author Marco Romanelli
	@version 0.1 25/22/17
 */

#include <cstdio>
#include <iostream>
#include <vector>
#include <assert.h>
#include <math.h>
#include "cpxmacro.h"

using namespace std;

typedef int Node;	// It's an integer, used to distinguish nodes and indexes

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

const int NAME_SIZE = 512;
char name[NAME_SIZE];

unsigned int N = 5;	// [!TESTING]		// an unsigned will cause a warning (comparison between signed and unsigned integer)

vector< vector<double> > xMap;
vector< vector<double> > yMap;

// Prototyping
vector<Node> extractPath(vector<double> vals, int start, int cnt);

void setupLP(CEnv env, Prob lp)
{
	// Problem setup
    cout << "Init model ..." << endl;

	int createdVars = 0;	// number of variables added to cplex

	// create nodes for the problem
	vector<Node> nodes;
	nodes.reserve(N);
    for (int i = 0; i < N; ++i) { nodes.push_back(i); }

	// Create costs for the problem
	// I costi vengono generati casualmente come valori da 1 a 100, rispettando la
    // [!TESTING] simmetria della matrice
	vector< vector<double> > C;
	C.resize(N);
    srand(time(NULL));

	// [!TESTING]
	for (int i = 0; i < N; ++i) {
        if (C[i].size() == 0) { C[i].resize(N); }
        C[i][i] = 0; // azzera la diagonale
        for (int j = i+1; j < N; j++){
            C[i][j] = rand() % 99 +1; // costo casuale C_i,j (non nullo)
            if (C[j].size() == 0) { C[j].resize(N); }
            C[j][i] = C[i][j]; // la matrice è simmetrica, C_j,i = C_i,j
        }
    }

	/*
        Variables setup
    */
	// x_i,j
    xMap.resize(N);
    for (int i = 0; i < N; i++){
        xMap[i].resize(N);
        for (int j = 0; j < N; ++j){
            xMap[i][j] = -1;
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) continue; // non creo le variabili con lo stesso indice, ma nella mappa c'è comunque il posto (rimane a -1)
            char htype = 'I';
            double obj = 0.0;
            double lb = 0.0;
            double ub = CPX_INFBOUND;
            snprintf(name, NAME_SIZE, "x_%d,%d", nodes[i], nodes[j]);
            char* xname = &name[0];
            CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &htype, &xname );
            xMap[i][j] = createdVars;
            createdVars++;
        }
    }

	// y_i,j
    yMap.resize(N);
    for (int i = 0; i < N; i++){
        yMap[i].resize(N);
        for (int j = 0; j < N; ++j){
            yMap[i][j] = -1;
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {

            if (i == j) continue;

            char ytype = 'B';
            double obj = C[i][j]; // coefficiente nella funzione obiettivo
            double lb = 0.0;
            double ub = CPX_INFBOUND;
            snprintf(name, NAME_SIZE, "y_%d,%d", nodes[i], nodes[j]);
            char* yname = &name[0];
            CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &ytype, &yname );
            yMap[i][j] = createdVars;
            createdVars++;
        }
    }

	// Starting node index [0]
	const Node STARTING_NODE = 0;

	// Vincolo sul flusso uscende dal primo nodo
    {
        std::vector<int> varIndex(N-1); // posizione d'inizio delle variabili x
        std::vector<double> coef(N-1);
        // Per scorrere gli elementi di varIndex devo usare un'altra variabile diversa da j
        // perché j va da 0 a N mentre varIndex è lungo N-1
        int idx = 0;
        for (int j = 0; j < N; ++j) {
            if (j == STARTING_NODE) continue;
            varIndex[idx] = xMap[STARTING_NODE][j]; // indice della variabile
            coef[idx] = 1; // coefficente della variabile nel vincolo
            idx++;
        }
        char sense = 'E'; // verso del vincolo
        double rhs = N; // parte destra del vincolo
        snprintf(name, NAME_SIZE, "flux");// nome del vincolo
        char* cname = (char*)(&name[0]);

        int matbeg = 0; // viene creato un solo vincolo (è la posizione d'inizio delle variabili che compaiono nei vari vincoli)
        // PXaddrows, env, lp, colcount, rowcount, nzcnt, &rhs[0], &sense[0], &rmatbeg[0], &rmatind[0], &rmatval[0], newcolnames , rownames
        CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, varIndex.size(), &rhs, &sense, &matbeg, &varIndex[0], &coef[0], NULL, &cname );
    }

    // Vincoli sulla domanda di flusso
    for (int k = 0; k < N; ++k) {
        if (k == STARTING_NODE) continue;

        std::vector<int> varIndex((N-1)*2);
        std::vector<double> coef((N-1)*2);
        int idx = 0;
        for (int i = 0; i < N; ++i) {
            if (i == k) continue; // non considera il caso i == k
            //cout << i <<" +x_"<<nodes[i]<<","<<nodes[k] <<" CPLEX addr: "<< xMap[i][k]<<endl;
            varIndex[idx] = xMap[i][k];
            coef[idx] = 1;
            idx++;
        }
        for (int j = 0; j < N; ++j){
            if (k == j) continue; // non considera il caso k == j
            //cout << idx <<" -x_"<<nodes[k]<<","<<nodes[j]<<" CPLEX addr: " << xMap[k][j]<<endl;
            varIndex[idx] = xMap[k][j]; // Parto da metà dell'array degli indici
            coef[idx] = -1;
            idx++;
        }

        char sense = 'E'; // verso del vincolo
        double rhs = 1; // parte destra del vincolo
        snprintf(name, NAME_SIZE, "delta_%d",k+1);// nome del vincolo
        char* cname = (char*)(&name[0]);

        int matbeg = 0;
        // PXaddrows, env, lp, colcount, rowcount, nzcnt, &rhs[0], &sense[0], &rmatbeg[0], &rmatind[0], &rmatval[0], newcolnames , rownames
        CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, varIndex.size(), &rhs, &sense, &matbeg, &varIndex[0], &coef[0], NULL, &cname );
    }

    // Vincoli sul flusso in uscita
    for (int i = 0; i < N; ++i){
        std::vector<int> varIndex(N-1);
        std::vector<double> coef(N-1);
        int idx = 0;
        for (int j = 0; j < N; ++j) {
            if (j == i) continue;
            varIndex[idx] = yMap[i][j];
            coef[idx] = 1;
            idx++;
        }

        char sense = 'E';
        double rhs = 1;
        snprintf(name, NAME_SIZE, "out_%d",i+1);
        char* cname = (char*)(&name[0]);

        int matbeg = 0;
        // PXaddrows, env, lp, colcount, rowcount, nzcnt, &rhs[0], &sense[0], &rmatbeg[0], &rmatind[0], &rmatval[0], newcolnames , rownames
        CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, varIndex.size(), &rhs, &sense, &matbeg, &varIndex[0], &coef[0], NULL, &cname );
    }
    // Vincoli sul flusso in ingresso
    for (int j = 0; j < N; ++j){
        std::vector<int> varIndex(N-1);
        std::vector<double> coef(N-1);
        int idx = 0;
        for (int i = 0; i < N; ++i) {
            if (i==j) continue;
            varIndex[idx] = yMap[i][j];
            coef[idx] = 1;
            idx++;
        }

        char sense = 'E';
        double rhs = 1;
        snprintf(name, NAME_SIZE, "in_%d",j+1);
        char* cname = (char*)(&name[0]);

        int matbeg = 0;
        // PXaddrows, env, lp, colcount, rowcount, nzcnt, &rhs[0], &sense[0], &rmatbeg[0], &rmatind[0], &rmatval[0], newcolnames , rownames
        CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, varIndex.size(), &rhs, &sense, &matbeg, &varIndex[0], &coef[0], NULL, &cname );
    }

    // Vincoli di attivazione
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (i == j) continue;
            std::vector<int> idx(2);
            idx[0] = xMap[i][j];
            idx[1] = yMap[i][j];
            std::vector<double> coef(2);
            coef[0] = 1;
            coef[1] = (double)N * -1;
            char sense = 'L';
            double rhs = 0;
            snprintf(name, NAME_SIZE, "att_%d_%d",i,j);
            char* cname = &name[0];

            int matbeg = 0;
            // PXaddrows, env, lp, colcount, rowcount, nzcnt, &rhs[0], &sense[0], &rmatbeg[0], &rmatind[0], &rmatval[0], newcolnames , rownames
            CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, &cname );
        }
    }

    // Print model on file
    CHECKED_CPX_CALL( CPXwriteprob, env, lp, "TSPModel.lp", NULL );

	/// status = CPXwriteprob (env, lp, "myprob", filetype_str);
}

int main (int argc, char const *argv[])
{
    try
    {
        // init
        DECL_ENV( env );
        DECL_PROB( env, lp );

        // setup LP
        setupLP(env, lp);

        // Optimize
        CHECKED_CPX_CALL( CPXmipopt, env, lp );

        // Print optimum
        double objval = 0.0;
        CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
        std::cout << "Objval: " << objval << std::endl;

		//
		int numVars = CPXgetnumcols(env, lp);
	    std::vector<double> varVals;
	    varVals.resize(numVars);
	    int fromIdx = 0;
	    int toIdx = numVars - 1;
	    CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], fromIdx, toIdx );

		// // TODO: capire cosa fa
		// int cnt = 0;	// aggiunto a caso
		// vector<Node> path = extractPath(varVals, 0, cnt);	// l'originale metteva solo il primo parametro
		// assert(path.size() == N+1);
		//
		// //
        // CHECKED_CPX_CALL( CPXsolwrite, env, lp, "tsp.sol" );
		//
		// // TODO: da esplicitare
		// // return new Solution(problem, path);
		// vector<Node> p;
	    // vector< vector<double> > C;
	    // path.resize(path.size());
	    // for (int j = 0; j < p.size(); ++j) {
	    //     p[j] = path[j];
	    // }
	    // double fitness = 0;
	    // int debug_sum = 0;
	    // int debug_sum_2 =0;
	    // for (int i = 0; i < N; ++i) {
	    //     debug_sum += path[i];
	    //     debug_sum_2 += i;
	    //     fitness += C[p[i]][p[i+1]];
	    // }
	    // assert(debug_sum == debug_sum_2);
		//
		// cout << "Soluzione di CPLEX - Costo "<< fitness <<endl;

		// free
        CPXfreeprob(env, &lp);
        CPXcloseCPLEX(&env);
    }
    catch(std::exception& e)
    {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }
    return 0;
}

// TODO: capire cosa fa
// vector<Node> extractPath(vector<double> vals, int start, int cnt){
//     unsigned int l = N + 1; // il nodo di partenza deve comparire 2 volte
//
//     if (cnt == l) { return vector<Node>(); }
//
//     for (int i = 0; i < l; ++i) {
//         if ( i != start && round(vals[yMap[start][i] ]) == 1) {
//             // i è il nodo in cui mi sposto
//             vector<Node> res;
//             res.push_back(start);
//             vector<Node> nextPath = extractPath(vals, i, ++cnt);
//             res.insert(res.end(), nextPath.begin(), nextPath.end()); // faccio l'append del resto del percorso.
//             return  res;
//         }
//     }
//
//     return vector<Node>();
// }
