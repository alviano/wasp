/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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

/* 
 * File:   Variables.h
 * Author: Mario Alviano
 *
 * Created on 14 July 2013, 18.25
 */
 
#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>

#include "Variable.h"
#include "Literal.h"
#include "outputBuilders/OutputBuilder.h"

using namespace std;

class Variables
{
    public:
        inline Variables();
        inline ~Variables();
    
        inline void init();
    
        inline void push_back( Variable* v );
        
        inline bool hasNextLiteralToPropagate() const { return static_cast< unsigned >( nextLiteralToPropagate ) < assignedVariablesSize; }
        inline Literal getNextLiteralToPropagate();
        
        inline Literal getOppositeLiteralFromLastAssignedVariable();        
        inline bool hasNextAssignedVariable() const;
        inline void startIterationOnAssignedVariable();
        
        inline void unrollLastVariable();
        inline void resetLiteralsToPropagate() { nextLiteralToPropagate = assignedVariablesSize; }
        
        inline unsigned numberOfAssignedLiterals() const { return assignedVariablesSize; }
        inline unsigned numberOfVariables() const { assert( variables[ 0 ] == NULL ); return variables.size() - 1; }
        
//        inline Variable* getNextUndefinedVariable();
//        inline bool hasNextUndefinedVariable();
//        inline void startIterationOnUndefinedVariables();
        inline Variable* getFirstUndefined();
        inline Variable* getNextUndefined( Variable* v );
        
        inline void printAnswerSet( OutputBuilder* outputBuilder ) const;
        
        inline bool assign( Literal literal, int level, Clause* implicant );

        inline Variable* operator[]( unsigned idx ) { return variables[ idx ]; }
        
        inline void onUnroll();
        
    private:
        Variable** assignedVariables;
        unsigned assignedVariablesSize;
        int iteratorOnAssignedVariables;
//        unsigned iteratorOnUndefinedVariables;
        int nextLiteralToPropagate;
        unsigned noUndefinedBefore;
        
        /* Data structures */
        vector< Variable* > variables;
        
        inline bool checkNoUndefinedBefore( unsigned idx ) const;
};

Variables::Variables()
: assignedVariables ( NULL ),
  assignedVariablesSize( 0 ),
//  iteratorOnUndefinedVariables( 0 ),
  nextLiteralToPropagate( 0 ),
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
    if( assignedVariables != NULL )
        delete[] assignedVariables;
}

void
Variables::init()
{
    assert( assignedVariables == NULL );
    assignedVariables = new Variable*[ variables.size() * 120 / 100 ];
    assignedVariablesSize = 0;
    
    assert( noUndefinedBefore == 1 );
    assert( variables.size() >= 2 );
    assert( variables[ 1 ] != NULL );
}

void
Variables::push_back(
    Variable* v )
{
    assert( v != NULL );
    assert( assignedVariables == NULL );
    variables.push_back( v );
}

Literal
Variables::getNextLiteralToPropagate()
{ 
    assert( hasNextLiteralToPropagate() );
    return Literal::createFromAssignedVariable( assignedVariables[ nextLiteralToPropagate++ ] );
}

void
Variables::unrollLastVariable()
{
    assert( assignedVariables > 0 );
    Variable* variable = assignedVariables[ --assignedVariablesSize ];
    variable->setUndefined();
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
    assert( assignedVariablesSize >= 0 );
    iteratorOnAssignedVariables = assignedVariablesSize - 1;
}

//Variable*
//Variables::getNextUndefinedVariable()
//{
//    assert( variables[ 0 ] == NULL );
//    assert( iteratorOnUndefinedVariables >= 1 && iteratorOnUndefinedVariables < variables.size() );
//    assert( variables[ iteratorOnUndefinedVariables ]->isUndefined() );
//    return variables[ iteratorOnUndefinedVariables ];
//}

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

//void
//Variables::startIterationOnUndefinedVariables()
//{
//    assert( variables[ 0 ] == NULL );
//    assert( noUndefinedBefore >= 1 );
//    assert( checkNoUndefinedBefore( noUndefinedBefore ) );
//    while( noUndefinedBefore < variables.size() )
//    {
//        if( variables[ noUndefinedBefore ]->isUndefined() )
//        {
//            iteratorOnUndefinedVariables = noUndefinedBefore;
//            return;
//        }
//        ++noUndefinedBefore;
//    }
//    assert( 0 );
//}
// 
//bool
//Variables::hasNextUndefinedVariable()
//{
//    assert( variables[ 0 ] == NULL );
//    assert( 0 <= iteratorOnUndefinedVariables && iteratorOnUndefinedVariables < variables.size() );
//    while( ++iteratorOnUndefinedVariables < variables.size() )
//    {
//        assert( variables[ iteratorOnUndefinedVariables ] != NULL );
//        if( variables[ iteratorOnUndefinedVariables ]->isUndefined() )
//            return true;
//    }
//    return false;
//}

void
Variables::printAnswerSet(
    OutputBuilder* outputBuilder ) const
{
    outputBuilder->startModel();
    for( unsigned int i = 0; i < assignedVariablesSize; ++i )
    {
        const Variable* v = assignedVariables[ i ];
        if( !v->isHidden() )
            outputBuilder->printVariable( v );
    }
    outputBuilder->endModel();
}

bool
Variables::assign( 
    Literal literal, 
    int level, 
    Clause* implicant )
{
    Variable* variable = literal.getVariable();
    assert( variable != NULL );
    assert( assignedVariablesSize < variables.size() );
    if( literal.setTrue() )
    {
        assignedVariables[ assignedVariablesSize++ ] = variable;
        variable->setDecisionLevel( level );
        variable->setImplicant( implicant );
        return true;
    }
    return false;
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

#endif
