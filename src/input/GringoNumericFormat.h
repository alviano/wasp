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


#ifndef _GRINGONUMERICFORMAT_H
#define	_GRINGONUMERICFORMAT_H

#include "../Solver.h"
#include "../stl/Trie.h"

using namespace std;

// TODO: the program has to be built in this class! No need of a separate builder. Revise this class.
class GringoNumericFormat
{
    public:

        inline GringoNumericFormat( Solver& s );
        ~GringoNumericFormat();

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

        void dealWithSupport();
        void readAtomsTable( istream& input );
        void readNormalRule( istream& input );
        void readDisjunctiveRule( istream& input );
        void readChoiceRule( istream& input );
        void readConstraintRule( istream& input );

        void readTrueAtoms( istream& input );
        void readFalseAtoms( istream& input );

        void readErrorNumber( istream& input );

        void postprocessing();

        void addNewVariable( unsigned int variable );
        void addNewAuxVariable( unsigned int auxVariable );
        bool existsAuxVariable( unsigned int auxVariable ) const;

        Literal readBody( istream& input );
        void readConstraint( istream& input );

        inline unsigned int getIdInSolver( int id, bool isAux );
        inline Literal getLiteral( int id );
        inline Variable* getVariable( unsigned int id );
        
        inline void addTrueLiteralInSolver( int id );
        
        inline bool addLiteralInClause( Literal literal, Clause* clause );
        
        Solver& solver;
        unsigned int numberOfAddedVariables;
        unsigned int numberOfAddedAuxVariables;

        vector< vector< Literal > > supportVector;
        unordered_set< Variable* > supportedVariables;

        unordered_set< int > addedLiterals;
        unordered_map< int, unsigned int > gringoIdToSolverId;        
        Trie bodiesDictionary;
        
        bool ok;
};

GringoNumericFormat::GringoNumericFormat(
    Solver& s ) : solver( s ), numberOfAddedVariables( 0 ), numberOfAddedAuxVariables( 0 ), ok( true )
{
    //push an empty position
    supportVector.push_back( vector< Literal >() );
}

unsigned int
GringoNumericFormat::getIdInSolver(
    int id,
    bool isAux )
{
    assert_msg( id > 0, "Id must be positive and greater than 0. Current value " << id );
    assert_msg( gringoIdToSolverId.find( isAux ? -id : id ) != gringoIdToSolverId.end(), "Cannot obtain solver id of " << ( isAux ? "aux" : "normal" ) << " variable with internal id " << id );
    return isAux ? gringoIdToSolverId[ -id ] : gringoIdToSolverId[ id ];
}

Literal
GringoNumericFormat::getLiteral(
    int id )
{
    assert_msg( id != 0, "Literal with id 0 is not used" );
    return solver.getLiteral( id );
}

Variable*
GringoNumericFormat::getVariable(
    unsigned int id )
{
    assert_msg( id != 0, "Variable with id 0 is not used" );
    return solver.getLiteral( id ).getVariable();
}

void
GringoNumericFormat::addTrueLiteralInSolver(
    int id )
{
    if( addedLiterals.insert( id ).second )
    {
        Clause* clause = solver.newClause();
        clause->addLiteral( getLiteral( id ) );
        solver.addClause( clause );
        //solver.addTrueLiteral( getLiteral( id ) );
    }
}

bool
GringoNumericFormat::addLiteralInClause(
    Literal literal,
    Clause* clause )
{
    if( literal.isUndefined() )
        clause->addLiteral( literal );
    else if( literal.isTrue() )
    {
        solver.releaseClause( clause );
        return false;
    }
    
    return true;
}

#endif	/* _GRINGONUMERICFORMAT_H */
