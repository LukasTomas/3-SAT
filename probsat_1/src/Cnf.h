#pragma once

#include <vector>
#include <iostream>
using namespace std;

class Cnf {
public:

    Cnf( const string & filepath );

    int getVariables() const { return variables; }
    const vector<vector<int>> & getClauses() const { return clauses; }

    bool isSatisfied( const vector<bool> & assignment ) const;
    bool isSatisfiedGetUnsatClauses( const vector<bool> & assignment, vector<int> & unsatClauses ) const;

    friend ostream & operator<<( ostream & os, const Cnf & cnf );

private:

    const static int NO_VARIABLES = -1;

    vector<vector<int>> clauses;
    int variables = NO_VARIABLES;

    static void splitString( const string & line, vector<string> & tokens );

};