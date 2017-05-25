/**
 * @file ironrods.cpp
 * @brief
 */

#include <cstdio>
#include <iostream>
#include <vector>
#include "cpxmacro.h"

using namespace std;

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

// data
const int I = 3;
const int J = 3;
const char nameI[I] = { 'A', 'B', 'C' }; // origins
const char nameJ[J] = { '1', '2', '3' }; // destinations
const double D[I] = { 7000.0, 6000.0, 4000.0 }; // Disponibilita' (origins)
const double R[J] = { 8000.0, 5000.0, 4000.0 }; // Richieste (destinations)
const double C[I*J] = { 9.0, 6.0, 5.0,
                        7.0, 4.0, 9.0,
                        4.0, 6.0, 3.0 }; // costs(origin, destination) LINEARIZED!
const double K = 10000.0;
const double F = 50.0;
const double N = 4.0;
const double L = 65.0;

const int NAME_SIZE = 512;
char name[NAME_SIZE];

void setupLP(CEnv env, Prob lp)
{
	// Problem setup
    cout << "Init model ..."<<endl;

	int createdVars = 0; 			// Numero di variabili inserite in CPLEX
    unsigned int N = problem->getSize();
    vector< Node > nodes = problem->getNodes();
    vector< vector<double> > C = problem->getCosts();

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

		// TODO: da esplicitare
		// vector<Node> path = extractPath(varVals);
	    // assert(path.size() == problem->getSize()+1);

		//
        CHECKED_CPX_CALL( CPXsolwrite, env, lp, "tsp.sol" );

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
