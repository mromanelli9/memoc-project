CC = g++
CPPFLAGS = -g -Wall -O2 -Wsign-compare
LDFLAGS =

CPX_INCDIR  = /opt/ibm/ILOG/CPLEX_Studio1263/cplex/include
CPX_LIBDIR  = /opt/ibm/ILOG/CPLEX_Studio1263/cplex/lib/x86-64_linux/static_pic
CPX_LDFLAGS = -lcplex -lm -pthread

OBJ = TSPSolver.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -I$(CPX_INCDIR) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o TSPSolver -L$(CPX_LIBDIR) $(CPX_LDFLAGS)

clean:
		rm -rf $(OBJ) TSPSolver tsp.sol TSPModel.lp

.PHONY: clean
