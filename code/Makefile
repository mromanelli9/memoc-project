#
# TODO: -
#

CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := bin/main
PYTARGET := scripts/gen_instances.py
PYCC := python
DATASET := vlsi-dataset/bcl380.tsp
INSTANCESDIR := instances
PYARGS := -f $(INSTANCESDIR)/bcl380.tsp -n 100

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -O2 -Wsign-compare -std=c++0x
CPX_INCDIR := /opt/ibm/ILOG/CPLEX_Studio1263/cplex/include/
CPX_LIBDIR := /opt/ibm/ILOG/CPLEX_Studio1263/cplex/lib/x86-64_linux/static_pic
LIB := -L$(CPX_LIBDIR) -lcplex -lm -pthread  # -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
INC := -I$(CPX_INCDIR)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(BINDIR)
	@echo "$(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

run: ${TARGET}
	@mkdir -p $(OUTPUTDIR)
	@echo "Launching ${TARGET}"; ${TARGET} ${ARGS}

gen-instances: ${PYTARGET} ${DATASET}
	@mkdir -p $(INSTANCESDIR)
	@cp ${DATASET} $(INSTANCESDIR)/bcl380.tsp
	@echo "Generating instances.\n $(PYCC) ${PYTARGET} ${PYARGS}"; $(PYCC) ${PYTARGET} ${PYARGS}
	@rm $(INSTANCESDIR)/bcl380.tsp

clean:
	@echo "Cleaning...";
	@echo "$(RM) -rf $(BUILDDIR) $(BINDIR) $(INSTANCESDIR)"; $(RM) -rf $(BUILDDIR) $(BINDIR) $(INSTANCESDIR)

.PHONY: clean
