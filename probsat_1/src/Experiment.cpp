#include "Experiment.h"

#include <algorithm>
#include <numeric>
#include <cfloat>
#include <cmath>

// PUBLIC METHODS -------------------------------------------------------------

Experiment::Experiment( float c_m, float c_b, long seed, int maxFlips ) 
    : c_m(c_m), c_b(c_b), seed(seed), maxFlips(maxFlips) {}

int Experiment::probSAT(Cnf & formula) {
    srand(seed);
    int iterations = 0;

    vector<bool> assignment(formula.getVariables()); 
    while (true) {
        if( iterations > 1 )
            return -1;

        getRandomAssignment(assignment);

        for (int j = 0; j < maxFlips; j++) {
            vector<int> unsatClauses;
            vector<int> satClauses;
            for (int k = 1; k <= formula.getClauses().size(); k++) {
                if (find(unsatClauses.begin(), unsatClauses.end(), k) == unsatClauses.end())
                    satClauses.push_back(k);
            }

            if (formula.isSatisfiedGetUnsatClauses(assignment, unsatClauses))
                return iterations;
        
            int rndUnsatClauseIndex = unsatClauses[rand() % unsatClauses.size()];
            const vector<int> & clause = formula.getClauses()[rndUnsatClauseIndex];

            // Compute probabilities
            vector<double> probVector(clause.size());
            computeProbabilities(formula, assignment, clause, probVector, unsatClauses, satClauses);

            // Normalize probabilities
            double sumProbabilities = accumulate(probVector.begin(), probVector.end(), 0.0);
            for (double & prob : probVector) {
                prob /= sumProbabilities;
            }

            // Generate a random number between 0 and 1
            double randomValue = (double)rand() / RAND_MAX;
            
            // Choose rndLiteralIndex based on cumulative probability
            double cumulativeProbability = 0.0;
            int rndLiteralIndex = -1;
            for (int i = 0; i < probVector.size(); i++) {
                cumulativeProbability += probVector[i];
                if (randomValue <= cumulativeProbability) {
                    rndLiteralIndex = abs(clause[i]) - 1;
                    break;
                }
            }

            // Flip the chosen literal in the assignment
            assignment[rndLiteralIndex] = !assignment[rndLiteralIndex];

            // Clear the unsatClauses for the next iteration
            unsatClauses.clear();
            iterations++;
        }
    }

    return -1;
}


// PRIVATE METHODS ------------------------------------------------------------

void Experiment::getRandomAssignment( vector<bool> & assignment ) {
    for( int i = 0; i < assignment.size(); i++ )
        assignment[i] = rand() % 2;
}

void Experiment::computeProbabilities( Cnf & formula, vector<bool> & assignment, const vector<int> & clause, vector<double> & probVector, vector<int> & unsatClauses, vector<int> & satClauses ) {
    for ( int literalIndex = 0; literalIndex < clause.size(); literalIndex++ ) {
        int literal = clause[literalIndex];
        int literalIndexAbs = abs(literal) - 1;
        
        assignment[literalIndexAbs] = !assignment[literalIndexAbs];
    
        vector<int> unsatClausesAfterFlip;
        formula.isSatisfiedGetUnsatClauses(assignment, unsatClausesAfterFlip);
        int make_param = 0;
        for ( int unsatClause : unsatClauses ) {
            if ( find(unsatClausesAfterFlip.begin(), unsatClausesAfterFlip.end(), unsatClause) == unsatClausesAfterFlip.end() )
                make_param++;
        }

        int break_param = 0;
        for ( int satClause : satClauses ) {
            if ( find(unsatClausesAfterFlip.begin(), unsatClausesAfterFlip.end(), satClause) != unsatClausesAfterFlip.end() )
                break_param++;
        }

        double prob = pow(make_param, c_m) / pow(break_param + DBL_EPSILON, c_b);
        
        assignment[literalIndexAbs] = !assignment[literalIndexAbs];
        probVector[literalIndex] = prob;
    }

}