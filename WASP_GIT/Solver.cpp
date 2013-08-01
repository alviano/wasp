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
#include "Variable.h"
#include "Solver.h"

Solver::~Solver()
{
//    assignedLiterals.clear();
//    undefinedLiterals.clear();
    assignedVariables.clear();
    undefinedVariables.clear();
    literalsToPropagate.clear();
    
//    assert( positiveLiterals.size() == negativeLiterals.size() );
//    for( unsigned int i = 1; i < positiveLiterals.size(); i++ )
//    {
//        delete negativeLiterals[ i ];
//        delete positiveLiterals[ i ];
//    }
    
    for( unsigned int i = 1; i < variables.size(); i++ )
    {
        delete variables[ i ];
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
    Variable* var = new Variable( name );
    addVariableInternal( var );    
}

void
Solver::addVariable()
{
    Variable* var = new Variable();
    addVariableInternal( var );
}

void
Solver::addVariableInternal(
    Variable* var )
{
    NegativeLiteral* negativeLiteral = new NegativeLiteral( var );
    PositiveLiteral* positiveLiteral = new PositiveLiteral( var );
    
    var->setNegativeLiteral( negativeLiteral );
    var->setPositiveLiteral( positiveLiteral );
    
    variables.push_back( var );
    undefinedVariables.insert( var );
    
//    assert( positiveLiterals.size() == negativeLiterals.size() );
//    PositiveLiteral* positiveLiteral = new PositiveLiteral();
//    positiveLiterals.push_back( positiveLiteral );
//    NegativeLiteral* negativeLiteral = new NegativeLiteral();
//    negativeLiterals.push_back( negativeLiteral );
//    
//    positiveLiteral->setOppositeLiteral( negativeLiteral );
//    negativeLiteral->setOppositeLiteral( positiveLiteral );
//    this->undefinedLiterals.insert( positiveLiteral );
}

void 
Solver::onLiteralAssigned(
    Literal* literal,
    TruthValue truthValue,
    Clause* implicant )
{
    assert( "Assigned literal is NULL." && literal != NULL );
    assert( "TruthValue has an invalid value." && ( truthValue == TRUE || truthValue == FALSE ) );
    
    literalsToPropagate.push_back( pair< Literal*, TruthValue >( literal, truthValue ) );
    Variable* var = literal->getVariable();
    assert( var != NULL );
    var->setDecisionLevel( currentDecisionLevel );
    literal->setImplicant( implicant );
//    literal->setImplicant( implicant );
//    literal->setDecisionLevel( currentDecisionLevel );    
}

void
Solver::unroll(
    unsigned int level )
{
    assert( "Level is not valid." && level < unrollVector.size() && currentDecisionLevel >= level );
    assert( "Vector for unroll is inconsistent" && assignedVariables.size() >= unrollVector[ level ] );    
    unsigned int toUnroll = assignedVariables.size() - unrollVector[ level ];
    unsigned int toPop = currentDecisionLevel - level;
    
    currentDecisionLevel = level;
    
    while( toUnroll > 0 )
    {
        Variable* variable = assignedVariables.back();
        assignedVariables.pop_back();
        variable->setUndefined();
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
//    assert( "Checking if lit is in the range." && lit < 0 || lit < positiveLiterals.size() );
//    assert( "Checking if lit is in the range." && lit > 0 || -lit < negativeLiterals.size() );
    assert( "Lit is out of range." && abs( lit ) < variables.size() );
    return lit > 0 ? variables[ lit ]->getPositiveLiteral() : variables[ -lit ]->getNegativeLiteral();//positiveLiterals[ lit ] : negativeLiterals[ -lit ];
}
