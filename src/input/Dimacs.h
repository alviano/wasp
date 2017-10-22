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

#ifndef WASP_DIMACS_H
#define WASP_DIMACS_H

#include "../util/WaspErrorMessage.h"
#include "../util/Istream.h"
#include <vector>
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
        void parse( Istream& input );
        
        bool isMaxsat() const { return maxsat; }

    private:

        void readAllClauses( Istream& input );

        bool readClause( Istream& input );
        
        bool readWeightedClause( Istream& input );

        inline bool readNextLiteral( Istream& input, int& next );
        
        inline void addNewVariable();        

        Solver& solver;

        unsigned int numberOfClauses;
        unsigned int numberOfVariables;

        uint64_t maxWeight;
        bool maxsat;

        vector< Var > relaxationVars;
        vector< uint64_t > relaxationVarsWeights;
};

bool
Dimacs::readNextLiteral(
    Istream& input,
    int& next )
{
    return ( input.read( next ) || input.eof() );    
}

#endif
