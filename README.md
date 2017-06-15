# MeMOC project
Lab exercises for the MeMOC course (Methods and Models for Combinatorial Optimization).

## Getting Started
### Prerequisites
LaTeX, Python, g++, CPLEX

### Compile
First run a terminal and move into the folder that contains the code:
```
cd code/
```
Then compile:
```
make
```
Generate the instances:
```
make gen-instances
```

### Run
Run main program on a sample instance by typing:
```
bin/main "samples/dcc1911_n025.tsp"
```
If you want to run over all instances:
```
make run-cplex
make run-ga
```

## Contributors
- None

## Licenses
Code:
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)] (LICENSE)

Report:
[![License: CC BY 4.0](https://licensebuttons.net/l/by/4.0/80x15.png)](http://creativecommons.org/licenses/by/4.0/) 
