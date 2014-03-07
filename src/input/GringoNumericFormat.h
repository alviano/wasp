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
    void readNormalRule( istream& input, unsigned head );
    void readConstraint( istream& input );
    void addFact( unsigned head );
//    void addNormalRule( unsigned head, Literal body );
//    void addConstraint( Clause* body );
//    Clause* readBody( istream& input, vector< Variable* >& truePositiveLiterals );
//    Literal getBodyLiteral( Clause* body );
    
//    void addSupportClauses();
//    void programIsNotTight();
    void computeSCCs();
    void computeCompletion();
    
    void readAtomsTable( istream& input );

    void readTrueAtoms( istream& input );
    void readFalseAtoms( istream& input );

    void readErrorNumber( istream& input );
    
    void createStructures( unsigned id );
    
    void propagate( Variable* var );
    
//    Literal getLiteralForInputVar( unsigned int id, unsigned int sign );
//    Literal getLiteralForAuxVar( unsigned int id, unsigned int sign );

    Solver& solver;
    
//    Trie bodiesDictionary;
    
//    vector< unsigned int > inputVarId;
//    vector< unsigned int > auxVarId;
    
    unsigned propagatedLiterals;
    
    vector< bool > facts;
    vector< vector< int > > normalRules;
    vector< vector< unsigned > > headOccurrences;
    vector< vector< unsigned > > posOccurrences;
    vector< vector< unsigned > > negOccurrences;
};

GringoNumericFormat::GringoNumericFormat(
    Solver& s ) : solver( s ), propagatedLiterals( 0 )
{
    facts.push_back( false );
    headOccurrences.push_back( vector< unsigned >() );
    posOccurrences.push_back( vector< unsigned >() );
    negOccurrences.push_back( vector< unsigned >() );
    createStructures( 1 );
    solver.addClause( Literal( solver.getVariable( 1 ), NEGATIVE ) );
}

GringoNumericFormat::~GringoNumericFormat()
{
}

#endif
