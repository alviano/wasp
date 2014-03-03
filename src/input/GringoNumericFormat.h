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

#ifndef GRINGONUMERICFORMAT_H
#define GRINGONUMERICFORMAT_H

#include "../Solver.h"
#include "../stl/Trie.h"
#include <unordered_set>

using namespace std;

class GringoNumericFormat
{
public:
    inline GringoNumericFormat( Solver& s );
    inline ~GringoNumericFormat();

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
    void readNormalRule( istream& input );
    void addFact( unsigned head, Clause* body );
    void addNormalRule( unsigned head, Literal body );
    void addConstraint( Clause* body  );
    Clause* readBody( istream& input );
    Literal getBodyLiteral( Clause* body );
    
    void addSupportClauses();
    void programIsNotTight();
    
    void readAtomsTable( istream& input );

    void readTrueAtoms( istream& input );
    void readFalseAtoms( istream& input );

    void readErrorNumber( istream& input );
    
    Literal getLiteralForInputVar( unsigned int id, unsigned int sign );
    Literal getLiteralForAuxVar( unsigned int id, unsigned int sign );

    Solver& solver;
    
    bool consistent;
    Trie bodiesDictionary;
    
    vector< unsigned int > inputVarId;
    vector< unsigned int > auxVarId;
    
    vector< Clause* > supportVectorInputVar;
    vector< Clause* > supportVectorAuxVar;
};

GringoNumericFormat::GringoNumericFormat(
    Solver& s ) : solver( s ), consistent( true )
{
    //push an empty position
    inputVarId.push_back( 0 );
    auxVarId.push_back( 0 );
    supportVectorInputVar.push_back( NULL );
    supportVectorAuxVar.push_back( NULL );
    
    Literal skip = getLiteralForInputVar( 1, NEGATIVE );
    supportVectorInputVar[ 1 ]->addLiteral( skip );
}

GringoNumericFormat::~GringoNumericFormat()
{
}

#endif
