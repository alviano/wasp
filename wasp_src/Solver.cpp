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

#include <vector>

#include "PositiveLiteral.h"
#include "NegativeLiteral.h"
#include "Solver.h"

Solver::~Solver()
{
    assignedLiterals.clear();
    undefinedLiterals.clear();
    literalsToPropagate.clear();
    
    for( unsigned int i = 1; i < positiveLiterals.size(); i++ )
    {
        delete positiveLiterals[ i ];
    }
    
    while( !clauses.empty() )
    {
        delete clauses.back();
        clauses.pop_back();
    }
    
    while( !learnedClauses.empty() )
    {
        delete learnedClauses.back();
        learnedClauses.pop_back();
    }
        
    if( learningStrategy )
        delete learningStrategy;
}

void
Solver::addVariable( 
    const string& name )
{    
    PositiveLiteral* positiveLiteral = new PositiveLiteral( name );
    addVariableInternal( positiveLiteral );
}

void
Solver::addVariable()
{
    PositiveLiteral* positiveLiteral = new PositiveLiteral();
    addVariableInternal( positiveLiteral );
}

void
Solver::addVariableInternal(
    PositiveLiteral* positiveLiteral )
{
    NegativeLiteral* negativeLiteral = new NegativeLiteral();
    
    negativeLiteral->setOppositeLiteral( positiveLiteral );
    positiveLiteral->setOppositeLiteral( negativeLiteral );
    
    positiveLiterals.push_back( positiveLiteral );
    undefinedLiterals.insert( positiveLiteral );    
}

void 
Solver::onLiteralAssigned(
    Literal* literal,
    TruthValue truthValue,
    Clause* implicant )
{
    assert( "Assigned literal is NULL." && literal != NULL );
    assert( "TruthValue has an invalid value." && ( truthValue == TRUE || truthValue == FALSE ) );
    
    if( !conflict )
    {
        PositiveLiteral* positiveLiteral = literal->getPositiveLiteral();
        
        assert( positiveLiteral != NULL );
        if( undefinedLiterals.erase( positiveLiteral ) )
        {
            assignedLiterals.push_back( positiveLiteral );
            literalsToPropagate.push_back( literal );            
            literal->setDecisionLevel( currentDecisionLevel );
            literal->setImplicant( implicant );
            truthValue == TRUE ? !literal->setTrue() : !literal->setFalse();
        }
        else
        {
            conflict = truthValue == TRUE ? !literal->setTrue() : !literal->setFalse();
            if( conflict )
            {
                literal->setImplicant( implicant ); 
                conflictLiteral = literal;
            }
        }
    }
}

void
Solver::unroll(
    unsigned int level )
{
    assert( "Level is not valid." && level < unrollVector.size() && currentDecisionLevel >= level );
    assert( "Vector for unroll is inconsistent" && assignedLiterals.size() >= unrollVector[ level ] );    
    unsigned int toUnroll = assignedLiterals.size() - unrollVector[ level ];
    unsigned int toPop = currentDecisionLevel - level;
    
    currentDecisionLevel = level;
    
    while( toUnroll > 0 )
    {
        PositiveLiteral* tmp = assignedLiterals.back();
        assignedLiterals.pop_back();
        tmp->setUndefined();        
        this->undefinedLiterals.insert( tmp );
        toUnroll--;
    }
    
    while( toPop > 0 )
    {
        unrollVector.pop_back();
        toPop--;
    }
    literalsToPropagate.clear();
}

Literal*
Solver::getLiteral(
    int lit )
{
    assert( "Lit is out of range." && abs( lit ) < positiveLiterals.size() );
    Literal* literal;
    if( lit > 0 )
        literal = positiveLiterals[ lit ];
    else
        literal = positiveLiterals[ -lit ]->getNegativeLiteral();
    
    assert( literal != NULL );
    return literal;
}
