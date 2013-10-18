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

#include "BerkminHeuristic.h"

#include <cassert>
#include "../Variable.h"
#include "../LearnedClause.h"
#include "../Literal.h"
#include "../Solver.h"

BerkminHeuristic::~BerkminHeuristic()
{
}

void
BerkminHeuristic::updateMaxCounter(
    Variable* variable )
{
    assert( "Variable has not been set." && variable != NULL );
    assert( "Variable must be undefined." && variable->isUndefined() );

    BERKMIN_HEURISTIC_COUNTER totalCounter = getTotalCounter( variable );
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
BerkminHeuristic::collectUndefined(
    Variable* variable )
{
    updateMaxCounter( variable );
}

void
BerkminHeuristic::pickLiteralUsingActivity(
    Solver& solver )
{
    trace( heuristic, 1, "Starting pickLiteralUsingActivity.\n" );
    
    assert( "This variable should be reset at the beginning of the heuristic." && chosenVariable == NULL );
    
    resetCounters();

    Variable* variable = NULL;
    do
    {
        variable = solver.getNextUndefinedVariable();
        
        assert( "Variable has not been set." && variable != NULL );
        assert( "The literal must be undefined." && variable->isUndefined() );
        updateMaxCounter( variable );
        updateMaxOccurrences( variable );
    } while( solver.hasNextUndefinedVariable() );
    
    if( chosenVariable != NULL )
    {
        assert( Literal( chosenVariable ).isUndefined() );
        choosePolarityHigherGlobalCounter( solver );
        return;
    }
    
    if( chosenVariableByOccurrences != NULL )
    {
        assert( Literal( chosenVariableByOccurrences ).isUndefined() );
        chosenVariable = chosenVariableByOccurrences;
        chosenPolarity = chosenVariable->numberOfPositiveWatchedClauses() > chosenVariable->numberOfNegativeWatchedClauses();
    }
    
    trace( heuristic, 1, "Choosing last undefined literal.\n" );
    assert( "Variable has not been set." && variable != NULL );
    assert( "The literal must be undefined." && variable->isUndefined() );
    chosenVariable = variable;
    chosenPolarity = false;
}

void
BerkminHeuristic::pickLiteralFromTopMostUndefinedLearnedClause(
   Solver& solver )
{
    trace( heuristic, 1, "Starting TopMostUndefinedLearnedClause.\n" );
    trace( heuristic, 1, "Considering %d learned clauses.\n", numberOfLearnedClausesToConsider );

    unsigned int count = 0;
    for( Solver::LearnedClausesReverseIterator it = solver.learnedClauses_rbegin(); it != solver.learnedClauses_rend(); ++it )
    {        
        if( ++count > numberOfLearnedClausesToConsider )
            break;

        resetCounters();
        assert( *it != NULL );
        LearnedClause& learnedClause = **it;
        trace( heuristic, 1, "Learned clause %d: %s.\n", count, learnedClause.clauseToCharStar() );
        if( learnedClause.checkUnsatisfiedAndOptimize( this ) )
        {
            assert( chosenVariable != NULL );
            assert( Literal( chosenVariable ).isUndefined() );
            assert( chosenVariableByOccurrences != NULL );
            chosenPolarity = getLiteralCounter( chosenVariable->getPositiveHeuristicCounter() ) > getLiteralCounter( chosenVariable->getNegativeHeuristicCounter() );
            break;
        }
    }
}

Literal
BerkminHeuristic::makeAChoice(
    Solver& solver )
{
    trace( heuristic, 1, "Starting Berkmin Heuristic.\n" );

    chosenVariable = NULL;
    pickLiteralFromTopMostUndefinedLearnedClause( solver );
    
    if( chosenVariable == NULL )
        pickLiteralUsingActivity( solver );

    assert( chosenVariable != NULL );
    assert( Literal( chosenVariable ).isUndefined() );
    
    // FIXME: use a different constructor
    return chosenPolarity ? Literal( chosenVariable ) : Literal( chosenVariable, false );
}

void
BerkminHeuristic::onLearning(
    Solver& )
{
    numberOfConflicts++;
    //TODO: Implement aging.
}

void
BerkminHeuristic::onRestart(
    Solver& )
{
}

void
BerkminHeuristic::choosePolarityHigherGlobalCounter(
    Solver& solver )
{
    trace( heuristic, 2, "Higher Global Counter: choose polarity.\n" );
    assert( "Variable has not been set." && chosenVariable != NULL );

    Literal positiveLiteral( chosenVariable );    
    unsigned int value1 = estimatePropagation( positiveLiteral, solver );
    trace( heuristic, 2, "Estimated propagation of positive literal %s, value: %d.\n", positiveLiteral.literalToCharStar(), value1 );
    if( value1 == UINT_MAX )
    {
        trace( heuristic, 3, "Conflict detected by look-ahead!\n" );
        chosenPolarity = true;
        return;
    }
    
    Literal negativeLiteral( chosenVariable, false );
    unsigned int value2 = estimatePropagation( negativeLiteral, solver );
    trace( heuristic, 2, "Estimated propagation of negative literal %s, value: %d.\n", negativeLiteral.literalToCharStar(), value2 );
    
    chosenPolarity = value1 > value2;
}

unsigned int
BerkminHeuristic::estimatePropagation(
    Literal literal,
    Solver& solver )
{
    assert( literal.isUndefined() );
    solver.incrementCurrentDecisionLevel();
    solver.setAChoice( literal );
    
    while( solver.hasNextLiteralToPropagate() )
    {
        Literal literalToPropagate = solver.getNextLiteralToPropagate();
        solver.propagate( literalToPropagate );        
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
