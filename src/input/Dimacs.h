/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef DIMACS_H
#define	DIMACS_H

#include <iostream>
#include <unordered_set>
using namespace std;

class Solver;

class Dimacs
{

public:

    Dimacs( Solver& solver );
  
    /**
     * This function read instruction from standard input and
     * build the program.
     */
    void parse();

    /**
     * This function read instruction from input and
     * build the program.
     *
     * @param input The istream input.
     */
    void parse( istream& input );
    
private:

    void insertVariables( unsigned int numberOfVariables );
    
    void readAllClauses( istream & input );
    
    void readComment( istream & input );
    
    void readClause( istream & input );
    
//    void readClauseWeighted( istream & input );
//    
//    void readClauseCnfWeighted( istream & input );
//        
    void readFormulaInfo( istream & input );        
    
    Solver& solver;
    
    unsigned int numberOfClauses;
    
    unsigned int numberOfVariables;
    
    unordered_set< int > addedLiterals;
    
//    unsigned int maxWeight;
//    
//    bool weighted;
//    
//    bool cnfWeighted;
//    
//    unordered_set< AtomId > variablesAppearingWithBothPolarity;
//    
//    unordered_set< int > readVariables;
};


#endif	/* DIMACS_H */

