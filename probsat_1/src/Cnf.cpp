#include "Cnf.h"

#include <fstream>
#include <sstream>
#include <cassert>

// PUBLIC METHODS -------------------------------------------------------------

Cnf::Cnf( const string & filepath ) {
    ifstream input(filepath);
    if( !input.is_open() )
        throw runtime_error("Error opening file: " + filepath);

    string line;
    while( getline(input, line) ) {
        if( line[0] == 'c' )
            continue;

        vector<string> tokens;
        splitString(line, tokens);
        if( line[0] == 'p' ) {
            if( variables != NO_VARIABLES )
                throw runtime_error("Error: multiple 'p' lines in file: " + filepath);

            // 1. token is 'p'
            // 2. token is 'cnf'
            // 3. token is number of variables and 4. token is number of clauses
            if( tokens.size() != 4 )
                throw runtime_error("Error: invalid 'p' line in file: " + filepath);
            variables = stoi(tokens[2]);
        } else {
            if( variables == NO_VARIABLES )
                throw runtime_error("Error: no 'p' line in file: " + filepath);

            // 1. token is a number
            // 2. token is a number
            // 3. token is a number
            // 4. token is 0
            if( tokens.empty() || tokens.back() != "0" )
                throw runtime_error("Error: invalid clause in file: " + filepath);

            vector<int> clause;
            for( int i = 0; i < tokens.size() - 1; i++ ) {
                int variable = stoi(tokens[i]);
                clause.emplace_back(variable);
            }
            clauses.emplace_back(clause);
        }
    }

    input.close();
}

bool Cnf::isSatisfied( const vector<bool> & assignment ) const {
    for( const vector<int> & clause : clauses ) {

        bool containsTrue = false;
        for( int variable : clause ) {
            int absVariable = abs(variable);
            bool isAssigned = assignment[absVariable - 1] == true;
            bool finalValue = (variable >= 0) ? isAssigned : !isAssigned; 

            if( finalValue == true ) {
                containsTrue = true;
                break;
            }
        }

        if( !containsTrue )
            return false;
    }
    return true;
}

bool Cnf::isSatisfiedGetUnsatClauses( const vector<bool> & assignment, vector<int> & unsatClauses ) const {
    bool isUnsat = false;
    for( int i = 0; i < clauses.size(); i++ ) {
        const vector<int> & clause = clauses[i]; 

        bool containsTrue = false;
        for( int variable : clause ) {
            int absVariable = abs(variable);
            bool isAssigned = assignment[absVariable - 1] == true;
            bool finalValue = (variable >= 0) ? isAssigned : !isAssigned; 

            if( finalValue == true ) {
                containsTrue = true;
                break;
            }
        }

        if( !containsTrue ) {
            unsatClauses.emplace_back(i);
            isUnsat = true;
        }
    }

    bool isSatisfied = !isUnsat;
    // just to make sure add assert
    assert( ((isSatisfied == true) && (unsatClauses.size() == 0)) || ((isSatisfied == false) && (unsatClauses.size() != 0)) );
    return isSatisfied;
}


ostream & operator<<( ostream & os, const Cnf & cnf ) {
    os << "Variables: " << cnf.variables << "\nClauses:\n";
    for (const auto & clause : cnf.clauses) {
        for (int i = 0; i < clause.size(); i++) {
            os << clause[i];
            if( i != clause.size() - 1 )
                os << " or ";
        }
        os << endl;
    }
    return os;
}


// PRIVATE METHODS ------------------------------------------------------------

void Cnf::splitString( const string & line, vector<string> & tokens ) {
    stringstream ss(line);
    string token;

    while ( ss >> token )
        tokens.emplace_back(token);
}