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
 
#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>

#include "Variable.h"
#include "Literal.h"
#include "outputBuilders/OutputBuilder.h"
#include "util/Assert.h"
#include "util/VariableNames.h"

using namespace std;

class Variables
{
    public:
        inline Variables();
        inline ~Variables();
    
        inline void init();
    
        inline void push_back( Variable* v );
        
        inline bool hasNextVariableToPropagate() const { return nextVariableToPropagate < assignedVariablesSize; }
        inline Variable* getNextVariableToPropagate();
        
        inline Literal getOppositeLiteralFromLastAssignedVariable();        
        inline bool hasNextAssignedVariable() const;
        inline void startIterationOnAssignedVariable();
        
        inline Variable* unrollLastVariable();
        inline void resetLiteralsToPropagate() { nextVariableToPropagate = assignedVariablesSize; }
        
        inline unsigned numberOfAssignedLiterals() const { return assignedVariablesSize; }
        inline unsigned numberOfVariables() const { assert( variables[ 0 ] == NULL ); return variables.size() - 1; }
        
        inline Variable* getAssignedVariable( unsigned idx ) { assert( idx < assignedVariables.size() ); return assignedVariables[ idx ]; }
        
        inline Variable* getFirstUndefined();
        inline Variable* getNextUndefined( Variable* v );
        
        inline void printAnswerSet( OutputBuilder* outputBuilder ) const;
        
        inline bool assign( int level, Literal literal );
        inline bool assign( int level, Clause* implicant );
        inline void onEliminatingVariable( Variable* variable, unsigned int sign, Clause* definition );

        inline Variable* operator[]( unsigned idx ) { return variables[ idx ]; }
        inline Variable const* operator[]( unsigned idx ) const { return variables[ idx ]; }
        
        inline void onUnroll();
        
    private:
        //Variable** assignedVariables;
		vector< Variable* > assignedVariables;
        unsigned assignedVariablesSize;
        int iteratorOnAssignedVariables;
//        unsigned iteratorOnUndefinedVariables;
        unsigned nextVariableToPropagate;
        unsigned noUndefinedBefore;
        
        /* Data structures */
        vector< Variable* > variables;
        
        inline bool checkNoUndefinedBefore( unsigned idx ) const;

        /**
         * Assign a literal with the truth value TRUE.
         * 
         * @param level the level of the inference.
         * @param literal the literal which is true.
         * @param implicant the clause which is the reason of the literal assignment.
         * @return true if no conflict occurs, false otherwise. 
         */
        inline bool assign( int level, Literal literal, Clause* implicant );
        
        inline bool checkVariableHasBeenAssigned( Variable* var );
};

Variables::Variables()
: //assignedVariables ( NULL ),
  assignedVariablesSize( 0 ),
//  iteratorOnUndefinedVariables( 0 ),
  nextVariableToPropagate( 0 ),
  noUndefinedBefore( 1 )
{
    //Add a fake position.
    variables.push_back( NULL );
}

Variables::~Variables()
{
    assert( variables[ 0 ] == NULL );
    for( unsigned int i = 1; i < variables.size(); i++ )
    {
        delete variables[ i ];
    }
    //if( assignedVariables != NULL )
    //    delete[] assignedVariables;
}

void
Variables::init()
{
    //assert( assignedVariables == NULL );
	//assignedVariables = new Variable*[ variables.size() * 120 / 100 ];
//    assert( noUndefinedBefore == 1 );
    assert( variables.size() >= 2 );
    assert( variables[ 1 ] != NULL );
}

void
Variables::push_back(
    Variable* v )
{
    assert( v != NULL );
//    assert( assignedVariables == NULL );
    variables.push_back( v );
	assignedVariables.push_back( NULL );	
}

Variable*
Variables::getNextVariableToPropagate()
{ 
    assert( hasNextVariableToPropagate() );
    return assignedVariables[ nextVariableToPropagate++ ];
}

Variable*
Variables::unrollLastVariable()
{
    assert( assignedVariablesSize > 0 && assignedVariablesSize <= assignedVariables.size() );
    Variable* variable = assignedVariables[ --assignedVariablesSize ];
    variable->setUndefined();
    return variable;
//    if( variable->getId() < noUndefinedBefore )
//    {
//        noUndefinedBefore = variable->getId();
//    }
//    assert( checkNoUndefinedBefore( noUndefinedBefore ) );
}

Literal
Variables::getOppositeLiteralFromLastAssignedVariable()
{
    assert( iteratorOnAssignedVariables >= 0 && static_cast< unsigned >( iteratorOnAssignedVariables ) < assignedVariablesSize );    
    return Literal::createOppositeFromAssignedVariable( assignedVariables[ iteratorOnAssignedVariables-- ] );    
}

bool
Variables::hasNextAssignedVariable() const
{
    return iteratorOnAssignedVariables >= 0;
}

void
Variables::startIterationOnAssignedVariable()
{
    assert( assignedVariablesSize > 0 );
    iteratorOnAssignedVariables = assignedVariablesSize - 1;
}

Variable* 
Variables::getFirstUndefined()
{
    assert( variables[ 0 ] == NULL );
    assert( noUndefinedBefore >= 1 );
    assert( checkNoUndefinedBefore( noUndefinedBefore ) );
    while( noUndefinedBefore < variables.size() )
    {
        if( variables[ noUndefinedBefore ]->isUndefined() )
        {
            return variables[ noUndefinedBefore ];
        }
        ++noUndefinedBefore;
    }
    assert( 0 );
    return NULL;
}

Variable*
Variables::getNextUndefined(
    Variable* v )
{
    assert( v != NULL );
    for( unsigned i = v->getId() + 1; i < variables.size(); ++i )
        if( variables[ i ]->isUndefined() )
            return variables[ i ];
    return NULL;
}

void
Variables::printAnswerSet(
    OutputBuilder* outputBuilder ) const
{
    outputBuilder->startModel();
    for( unsigned int i = 0; i < assignedVariablesSize; ++i )
    {
        const Variable* v = assignedVariables[ i ];
        outputBuilder->printVariable( v );
    }
    outputBuilder->endModel();
}

bool
Variables::assign( 
    int level, 
    Literal literal,
    Clause* implicant )
{
    Variable* variable = literal.getVariable();
    assert( variable != NULL );
    assert( assignedVariablesSize < variables.size() );
    if( literal.setTrue() )
    {
        assert_msg( !checkVariableHasBeenAssigned( variable ), "The variable " << *variable << " has been already assigned." );
        assignedVariables[ assignedVariablesSize++ ] = variable;        
        variable->setDecisionLevel( level );
        variable->setImplicant( implicant );
        return true;
    }
    return false;
}

void
Variables::onEliminatingVariable(
    Variable* variable,
    unsigned int sign,
    Clause* definition )
{
    assert( variable != NULL );
    assert( assignedVariablesSize < variables.size() );
    assert_msg( !checkVariableHasBeenAssigned( variable ), "The variable " << *variable << " has been already assigned." );
    assignedVariables[ assignedVariablesSize++ ] = variable;        
    variable->setEliminated( sign, definition );    
}

bool
Variables::assign( 
    int level, 
    Literal literal )
{
    return assign( level, literal, NULL );
}

bool
Variables::assign( 
    int level, 
    Clause* implicant )
{
    assert( implicant != NULL );
    return assign( level, implicant->getAt( 0 ), implicant );
}

bool
Variables::checkNoUndefinedBefore(
    unsigned idx ) const
{
    for( unsigned i = 1; i < idx; ++i )
        if( variables[ i ]->isUndefined() )
            return false;
    return true;
}

void
Variables::onUnroll()
{
    resetLiteralsToPropagate();
    
    noUndefinedBefore = 1;
//    while( !variables[ noUndefinedBefore ]->isUndefined() )
//    {
//        ++noUndefinedBefore;
//        assert( noUndefinedBefore < variables.size() );
//    }
}

bool
Variables::checkVariableHasBeenAssigned( 
    Variable* var )
{
    for( unsigned int i = 0; i < assignedVariablesSize; i++ )
    {
        if( assignedVariables[ i ] == var )
            return true;
    }
    return false;
}


#endif
