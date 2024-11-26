#include "Experiment.h"
#include "Cnf.h"

#include <filesystem>
#include <iostream>
#include <numeric>
#include <vector>
#include <string>
using namespace std;

const string DATA_DIR = "data/";
const vector<int> POSSIBLE_NUMBER_VARIABLES = {20, 36, 50, 75};


void printUsage( const string & programName ) {
    cout << "Usage: " << programName << " -i max_iteratiosn -r seed [cnf_file]" << endl;
}

int parse_arguments( int argc, char* argv[], int * maxIterations, long * seed, string * cnfFile ) {
    if( argc != 6 ) {
        printUsage(argv[0]);
        return 1;
    }


    if (string(argv[1]) != "-i" || string(argv[3]) != "-r") {
        printUsage(argv[0]);
        return 1;
    }

    *maxIterations = stoi(argv[2]);
    *seed = stol(argv[4]);
    *cnfFile = argv[5];
    return 0;
}

int main( int argc, char* argv[] ) {
    int maxIterations;
    long seed;
    string cnfFile;


    int ret = parse_arguments(argc, argv, &maxIterations, &seed, &cnfFile);
    if( ret != 0 )
        return ret;

    float c_m = 0;
    float c_b = 2.3;
    Experiment experiment(c_m, c_b, seed, maxIterations);

    Cnf cnf(cnfFile);
    int iterations = experiment.probSAT(cnf);

    int satisfied_clauses = cnf.getClauses().size();
    if( iterations == -1 )
        satisfied_clauses = 0;
    
    // iters, max_iters, satisfied_clauses, total_clauses = result_ints
    cerr << iterations << " " << maxIterations << " " << satisfied_clauses << " " << cnf.getClauses().size() << endl; 
    return 0;
}