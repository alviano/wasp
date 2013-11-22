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

#include "BerkminHeuristic.h"

#include <cassert>
#include "../../Variable.h"
#include "../../Literal.h"
#include "../../Solver.h"

BerkminHeuristic::~BerkminHeuristic()
{
}

void
BerkminHeuristic::updateMaxCounter(
    Variable* variable )
{
    assert( "Variable has not been set." && variable != NULL );
    assert( "Variable must be undefined." && variable->isUndefined() );

    assert( variable->getId() < variableCounters.size() );
    unsigned totalCounter = variableCounters[ variable->getId() ].getTotalCounter();
    if( totalCounter > maxCounter )
    {
        maxCounter = totalCounter;
        chosenVariable = variable;
    }
}

void
BerkminHeuristic::updateMaxOccurrences(
    Variable* variable )
{
    assert( "Variable has not been set." && variable != NULL );
    assert( "Variable must be undefined." && variable->isUndefined() );

    unsigned int numberOfOccurrences = variable->numberOfPositiveWatchedClauses() + variable->numberOfNegativeWatchedClauses();
    if( numberOfOccurrences > maxOccurrences )
    {
        maxOccurrences = numberOfOccurrences;
        chosenVariableByOccurrences = variable;
    }
}

void
BerkminHeuristic::pickLiteralUsingActivity()
{
    trace( heuristic, 1, "Starting pickLiteralUsingActivity.\n" );
    
    assert( "This variable should be reset at the beginning of the heuristic." && chosenVariable == NULL );
    
    resetCounters();
    //solver.startIterationOnUndefinedVariables();

    Variable* firstUndefined = solver.getFirstUndefined();
    trace_msg( heuristic, 2, "First Undefined Variable is " << *firstUndefined );
    
    Variable* variable = firstUndefined;
    do
    {
        trace_msg( heuristic, 2, "Current Variable is " << *variable );
        assert( "Variable has not been set." && variable != NULL );
        assert( "The literal must be undefined." && variable->isUndefined() );
        updateMaxCounter( variable );
        updateMaxOccurrences( variable );
        
        
    } while( ( variable = solver.getNextUndefined( variable ) ) != NULL );
    
    if( chosenVariable != NULL )
    {
        assert( Literal( chosenVariable ).isUndefined() );
        choosePolarityHigherGlobalCounter();
        return;
    }
    
    if( chosenVariableByOccurrences != NULL )
    {
        assert( Literal( chosenVariableByOccurrences ).isUndefined() );
        chosenVariable = chosenVariableByOccurrences;
        chosenPolarity = chosenVariable->numberOfPositiveWatchedClauses() > chosenVariable->numberOfNegativeWatchedClauses();
        return;
    }

    trace( heuristic, 1, "Choosing last undefined literal.\n" );
    chosenVariable = firstUndefined;
    chosenPolarity = false;
    assert( "Variable has not been set." && chosenVariable != NULL );
    assert( "The literal must be undefined." && chosenVariable->isUndefined() );
}

void
BerkminHeuristic::pickLiteralFromTopMostUnsatisfiedLearnedClause()
{
    trace( heuristic, 1, "Starting TopMostUnsatisfiedLearnedClause.\n" );
    trace( heuristic, 1, "Considering %d learned clauses.\n", numberOfLearnedClausesToConsider );

    while( topMostUnsatisfiedIt != solver.learnedClauses_rend() )
    {        
        if( ++count > numberOfLearnedClausesToConsider )
            break;

        resetCounters();
        assert( *topMostUnsatisfiedIt != NULL );
        Clause& learnedClause = **topMostUnsatisfiedIt;
        trace( heuristic, 1, "Learned clause %d: %s.\n", count, toString( learnedClause ).c_str() );
        if( checkUnsatisfiedAndOptimize( learnedClause ) )//checkUnsatisfiedAndOptimize( learnedClause ) )
        {
            assert( chosenVariable != NULL );
            assert( Literal( chosenVariable ).isUndefined() );
            assert( learnedClause.isUnsatisfied() );
            chosenPolarity = variableCounters[ chosenVariable->getId() ].getPositiveCounter() > variableCounters[ chosenVariable->getId() ].getNegativeCounter();
            --count;
            return;
        }
        ++topMostUnsatisfiedIt;        
    }

    resetCounters();    
}

bool
BerkminHeuristic::checkUnsatisfied( 
    Clause& clause )
{
    assert( "Unary clauses must be removed." && clause.size() > 1 );
    
    for( unsigned int i = 0; i < clause.size(); ++i )
    {
        Literal literal = clause.getAt( i );
        if( literal.isTrue() )
            return false;
        if( literal.isUndefined() )
            updateMaxCounter( literal.getVariable() );
    }
    
    return true;
}

bool
BerkminHeuristic::checkUnsatisfiedAndOptimize( 
    Clause& clause )
{
    assert( "Unary clauses must be removed." && clause.size() > 1 );
    
    Literal literal = clause.getAt( clause.size() - 1 );
    if( literal.isTrue() ) //literals.back().isTrue() )
        return false;
    if( literal.isUndefined() )
        updateMaxCounter( literal.getVariable() );
    
    literal = clause.getAt( 0 );
    if( literal.isTrue() )
        return false;
    if( literal.isUndefined() )
        updateMaxCounter( literal.getVariable() );
    
    unsigned size = clause.size() - 1;
    for( unsigned int i = 1; i < size; ++i )
    {
        literal = clause.getAt( i );
        if( literal.isTrue() )
        {
            if( i == 1 )
                clause.swapWatchedLiterals();
            else
                clause.swapUnwatchedLiterals( i, size );
            
            return false;
        }
        if( literal.isUndefined() )
            updateMaxCounter( literal.getVariable() );
    }
    
    return true;
}

Literal
BerkminHeuristic::makeAChoice()
{
    trace( heuristic, 1, "Starting Berkmin Heuristic.\n" );

    chosenVariable = NULL;
    pickLiteralFromTopMostUnsatisfiedLearnedClause();    
    if( chosenVariable == NULL )
        pickLiteralUsingActivity();

    assert( chosenVariable != NULL );
    assert( Literal( chosenVariable ).isUndefined() );
    
    // FIXME: chosenPolarity has to be revised
    return chosenPolarity ? Literal( chosenVariable, POSITIVE ) : Literal( chosenVariable, NEGATIVE );
}

void
BerkminHeuristic::onLearning()
{
    topMostUnsatisfiedIt = solver.learnedClauses_rbegin();
    numberOfConflicts++;
    count = 0;
    //TODO: Implement aging.
}

void
BerkminHeuristic::onRestart()
{
}

void
BerkminHeuristic::choosePolarityHigherGlobalCounter()
{
    trace( heuristic, 2, "Higher Global Counter: choose polarity.\n" );
    assert( "Variable has not been set." && chosenVariable != NULL );

    Literal positiveLiteral( chosenVariable );    
    unsigned int value1 = estimatePropagation( positiveLiteral );
    trace( heuristic, 2, "Estimated propagation of positive literal %s, value: %d.\n", toString( positiveLiteral ).c_str(), value1 );
    if( value1 == UINT_MAX )
    {
        trace( heuristic, 3, "Conflict detected by look-ahead!\n" );
        chosenPolarity = true;
        return;
    }
    
    Literal negativeLiteral( chosenVariable, NEGATIVE );
    unsigned int value2 = estimatePropagation( negativeLiteral );
    trace( heuristic, 2, "Estimated propagation of negative literal %s, value: %d.\n", toString( negativeLiteral ).c_str(), value2 );
    
    chosenPolarity = value1 > value2;
}

unsigned int
BerkminHeuristic::estimatePropagation(
    Literal literal )
{
    assert( literal.isUndefined() );
    solver.incrementCurrentDecisionLevel();
    solver.setAChoice( literal );
    
    while( solver.hasNextVariableToPropagate() )
    {
        Variable* variableToPropagate = solver.getNextVariableToPropagate();
        solver.propagate( variableToPropagate );        
        if( solver.conflictDetected() )
        {
            solver.unrollOne();
            solver.clearConflictStatus();
            return UINT_MAX;
        }
    }
    unsigned int lookaheadValue = literal.numberOfWatchedClauses() + solver.numberOfAssignedLiterals();
    
    solver.unrollOne();
    return lookaheadValue;
}

void
BerkminHeuristic::onNewVariable(
    Variable& variable )
{
    variableCounters.push_back( VariableCounters() );
}

void 
BerkminHeuristic::onLiteralInvolvedInConflict( 
    Literal literal )
{
    ++( variableCounters[ literal.getVariable()->getId() ].globalCounter[ literal.getSign() ] );
    ++( variableCounters[ literal.getVariable()->getId() ].counter[ literal.getSign() ] );
}
