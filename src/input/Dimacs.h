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

#include "../util/ErrorMessage.h"
#include "../util/Istream.h"
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

    private:

        void insertVariables( unsigned int numberOfVariables );

        void readAllClauses( Istream& input );

        bool readClause( Istream& input );

        inline void readNextLiteral( Istream& input, int& next );

        Solver& solver;
        
        unsigned int numberOfClauses;

        unsigned int numberOfVariables;
};

void
Dimacs::readNextLiteral(
    Istream& input,
    int& next )
{
    if( !input.read( next ) )
        ErrorMessage::errorDuringParsing( "Unexpected symbol." );
}

#endif
