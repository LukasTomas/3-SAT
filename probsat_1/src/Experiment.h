#pragma once

#include "Cnf.h"

using namespace std;


class Experiment {
public:

    Experiment( float c_m, float c_b, long seed, int maxFlips);

    int probSAT( Cnf & formula );

private:

    const float c_m, c_b;
    const long seed;
    const int maxFlips;

    static void getRandomAssignment( vector<bool> & assignment );

    void computeProbabilities( Cnf & formula, vector<bool> & assignment, const vector<int> & clause, vector<double> & probVector,
        vector<int> & unsatClauses, vector<int> & satClauses );


};