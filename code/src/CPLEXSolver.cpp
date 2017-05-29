/**
 *  @file    CPLEXSolver.cpp
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

 using namespace std;


 /**
 *	@brief	Default constructor
 *
 *	@section DESCRIPTION
 *	TODO
 *
 */
 CPLEXSolver::CPLEXSolver(TSPProblem *problem) {
	 this->problem = problem;

	// Setup CPLEX eviroment
	this->env = CPXopenCPLEX(&status);
	if (status){
		CPXgeterrorstring(NULL, status, errmsg);
		int trailer = std::strlen(errmsg) - 1;
	 	if (trailer >= 0) errmsg[trailer] = '\0';
		throw std::runtime_error(std::string(__FILE__) + ":" + STRINGIZE(__LINE__) + ": " + errmsg);
	}

	this->lp = CPXcreateprob(env, &status, "");
	if (status){
		CPXgeterrorstring(NULL, status, errmsg);
		int trailer = std::strlen(errmsg) - 1;
		if (trailer >= 0) errmsg[trailer] = '\0';
		throw std::runtime_error(std::string(__FILE__) + ":" + STRINGIZE(__LINE__) + ": " + errmsg);
	}

	cout << "Init model ..." << endl;	// Model setup

	/*
        Variables setup
    */
	int created_vars = 0;	// total number of variables used in cplex

	unsigned int N = problem->getSize();
	vector< Node > nodes = problem->getNodes();
	vector< vector<double> > C = problem->getCosts();

	xMap.resize(N);	// variables: x_i,j
	for (unsigned int i = 0; i < N; i++) {
		xMap[i].resize(N);

	 	for (unsigned int j = 0; j < N; ++j) { xMap[i][j] = -1; }
	}

	for (unsigned int i = 0; i < N; ++i) {
		for (unsigned int j = 0; j < N; ++j) {
			if (i == j) continue; // variables with same i-j indexes will not be set

			char htype = 'I';
		 	double obj = 0.0;
		 	double lb = 0.0;
		 	double ub = CPX_INFBOUND;
		 	snprintf(name, NAME_SIZE, "x_%d,%d", nodes[i], nodes[j]);
			char* xname = &name[0];
			CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &htype, &xname );
			xMap[i][j] = created_vars;
			created_vars++;
		}
	}

	yMap.resize(N);		// variables: y_i,j
	for (unsigned int i = 0; i < N; i++){
		yMap[i].resize(N);
		for (unsigned int j = 0; j < N; ++j){ yMap[i][j] = -1; }
	}

	for (unsigned int i = 0; i < N; ++i) {
	 for (unsigned int j = 0; j < N; ++j) {
		 if (i == j) continue;	// skip same indexes

		 char ytype = 'B';
		 double obj = C[i][j];	// coefficient of object function
		 double lb = 0.0;
		 double ub = CPX_INFBOUND;
		 snprintf(name, NAME_SIZE, "y_%d,%d", nodes[i], nodes[j]);
		 char* yname = &name[0];
		 CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &ytype, &yname );
		 yMap[i][j] = created_vars;
		 created_vars++;
	 }
	}

	const Node STARTING_NODE = 0;	// index of the node to start with (set to 0)

	/*
        Contraints setup
    */
	// Contraint: flow coming out from first node
	{
		std::vector<int> varIndex(N-1);	// starting postions of x-es variables
		std::vector<double> coef(N-1);

		int idx = 0;
		for (unsigned int j = 0; j < N; ++j) {
			if (j == STARTING_NODE) continue;
			varIndex[idx] = xMap[STARTING_NODE][j];	// index
			coef[idx] = 1; // coefficient
			idx++;
		}

	 	char sense = 'E';
	 	double rhs = N;
	 	snprintf(name, NAME_SIZE, "flux");
	 	char* cname = (char*)(&name[0]);

	 	int matbeg = 0;
	 	CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, varIndex.size(), &rhs, &sense, &matbeg, &varIndex[0], &coef[0], NULL, &cname );
	}

	// Constraints: flow demand
	for (unsigned int k = 0; k < N; ++k) {
		if (k == STARTING_NODE) continue;

	 	std::vector<int> varIndex((N - 1) * 2);
	 	std::vector<double> coef((N - 1) * 2);

	 	int idx = 0;
	 	for (unsigned int i = 0; i < N; ++i) {
			if (i == k) continue;
			varIndex[idx] = xMap[i][k];
			coef[idx] = 1;
			idx++;
	 	}

	 	for (unsigned int j = 0; j < N; ++j){
			if (k == j) continue;
			varIndex[idx] = xMap[k][j];
			coef[idx] = -1;
			idx++;
	 	}

		char sense = 'E';
		double rhs = 1;
		snprintf(name, NAME_SIZE, "delta_%d",k+1);
		char* cname = (char*)(&name[0]);

		int matbeg = 0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, varIndex.size(), &rhs, &sense, &matbeg, &varIndex[0], &coef[0], NULL, &cname );
	}

	// Constraints: outflow
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
	 CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, varIndex.size(), &rhs, &sense, &matbeg, &varIndex[0], &coef[0], NULL, &cname );
	}

	// Constraints: inflow
	for (unsigned int j = 0; j < N; ++j){
		std::vector<int> varIndex(N-1);
	 	std::vector<double> coef(N-1);

	 	int idx = 0;
	 	for (unsigned int i = 0; i < N; ++i) {
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
	 	CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, varIndex.size(), &rhs, &sense, &matbeg, &varIndex[0], &coef[0], NULL, &cname );
	}

	// enabling constraints
	for (unsigned int i = 0; i < N; i++){
		for (unsigned int j = 0; j < N; j++){
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
		 	CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, &cname );
	 	}
	}

	// Print model on file
	CHECKED_CPX_CALL( CPXwriteprob, env, lp, this->model_filename, 0 );
 }

 /**
 *	@brief	use this when constructor time limit is specified
 */
 CPLEXSolver::CPLEXSolver(TSPProblem *p, unsigned int time_limit) : CPLEXSolver(p) {
	 CPXsetdblparam(env, CPX_PARAM_TILIM, time_limit);	// Setup time limit
 }
