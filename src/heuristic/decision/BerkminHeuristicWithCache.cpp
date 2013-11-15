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

#include "BerkminHeuristicWithCache.h"

#include <cassert>
#include "../../Variable.h"
#include "../../Literal.h"
#include "../../Solver.h"

BerkminHeuristicWithCache::~BerkminHeuristicWithCache()
{
}

void
BerkminHeuristicWithCache::updateMaxCounter(
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
BerkminHeuristicWithCache::updateMaxOccurrences(
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
BerkminHeuristicWithCache::pickLiteralUsingActivity()
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
        if( chosenVariable->getCachedTruthValue() != UNDEFINED )
            chosenPolarity = chosenVariable->getCachedTruthValue() == TRUE;
        else
            choosePolarityHigherGlobalCounter();
        return;
    }
    
    if( chosenVariableByOccurrences != NULL )
    {
        assert( Literal( chosenVariableByOccurrences ).isUndefined() );
        chosenVariable = chosenVariableByOccurrences;
        if( chosenVariable->getCachedTruthValue() != UNDEFINED )
            chosenPolarity = chosenVariable->getCachedTruthValue() == TRUE;
        else
            chosenPolarity = chosenVariable->numberOfPositiveWatchedClauses() > chosenVariable->numberOfNegativeWatchedClauses();
        return;
    }

    trace( heuristic, 1, "Choosing last undefined literal.\n" );
    chosenVariable = firstUndefined;
    if( chosenVariable->getCachedTruthValue() != UNDEFINED )
        chosenPolarity = chosenVariable->getCachedTruthValue() == TRUE;
    else
        chosenPolarity = false;
    assert( "Variable has not been set." && chosenVariable != NULL );
    assert( "The literal must be undefined." && chosenVariable->isUndefined() );
}

void
BerkminHeuristicWithCache::pickLiteralFromTopMostUnsatisfiedLearnedClause()
{
    trace( heuristic, 1, "Starting TopMostUnsatisfiedLearnedClause.\n" );
    trace( heuristic, 1, "Considering %d learned clauses.\n", numberOfLearnedClausesToConsider );

    unsigned int count = 0;
    for( Solver::ClauseReverseIterator it = solver.learnedClauses_rbegin(); it != solver.learnedClauses_rend(); ++it )
    {        
        if( ++count > numberOfLearnedClausesToConsider )
            break;

        resetCounters();
        assert( *it != NULL );
        Clause& learnedClause = **it;
        trace( heuristic, 1, "Learned clause %d: %s.\n", count, toString( learnedClause ).c_str() );
        if( checkUnsatisfiedAndOptimize( learnedClause ) )
        {
            assert( chosenVariable != NULL );
            assert( Literal( chosenVariable ).isUndefined() );
            assert( learnedClause.isUnsatisfied() );
            if( chosenVariable->getCachedTruthValue() != UNDEFINED )
                chosenPolarity = chosenVariable->getCachedTruthValue() == TRUE;
            else
                chosenPolarity = variableCounters[ chosenVariable->getId() ].getPositiveCounter() > variableCounters[ chosenVariable->getId() ].getNegativeCounter();
            break;
        }
    }
}


bool
BerkminHeuristicWithCache::checkUnsatisfiedAndOptimize( 
    Clause& clause )
{
    assert( "Unary clauses must be removed." && clause.size() > 1 );
    
    if( clause.getAt( clause.size() - 1 ).isTrue() ) //literals.back().isTrue() )
        return false;
    Variable* variable = clause.getAt( clause.size() - 1 ).getVariable();
    if( variable->isUndefined() )
        updateMaxCounter( variable );
    
    if( clause.getAt( 0 ).isTrue() )
        return false;
    variable = clause.getAt( 0 ).getVariable();
    if( variable->isUndefined() )
        updateMaxCounter( variable );
    
    unsigned size = clause.size() - 1;
    for( unsigned int i = 1; i < size; ++i )
    {
        if( clause.getAt( i ).isTrue() )
        {
            if( i == 1 )
                clause.swapWatchedLiterals();
            else
                clause.swapUnwatchedLiterals( i, size );
            
            return false;
        }
        variable = clause.getAt( i ).getVariable();
        if( variable->isUndefined() )
            updateMaxCounter( variable );
    }
    
    return true;
}

Literal
BerkminHeuristicWithCache::makeAChoice()
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
BerkminHeuristicWithCache::onLearning()
{
    numberOfConflicts++;
    //TODO: Implement aging.
}

void
BerkminHeuristicWithCache::onRestart()
{
}

void
BerkminHeuristicWithCache::choosePolarityHigherGlobalCounter()
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
BerkminHeuristicWithCache::estimatePropagation(
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
BerkminHeuristicWithCache::onNewVariable(
    Variable& variable )
{
    variableCounters.push_back( VariableCounters() );
}

void 
BerkminHeuristicWithCache::onLiteralInvolvedInConflict( 
    Literal literal )
{
    ++( variableCounters[ literal.getVariable()->getId() ].globalCounter[ literal.getSign() ] );
    ++( variableCounters[ literal.getVariable()->getId() ].counter[ literal.getSign() ] );
}