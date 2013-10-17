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

Literal
BerkminHeuristic::makeAChoiceProtected(
    Solver& solver )
{
    trace( heuristic, 1, "Starting Berkmin Heuristic.\n" );
    pickLiteralUsingLearnedClauses( solver );
    
    if( !hasChosenLiteral() )
        pickLiteralUsingActivity( solver );
    
    assert( hasChosenLiteral() );
    return getChosenLiteral();
}

void
BerkminHeuristic::pickLiteralUsingLearnedClauses(
    Solver& solver )
{
    topMostUndefinedLearnedClause( solver );
}

void
BerkminHeuristic::topMostUndefinedLearnedClause(
   Solver& solver )
{
    trace( heuristic, 1, "Starting TopMostUndefinedLearnedClause.\n" );
    const List< LearnedClause* >& learnedClauses = solver.getLearnedClauses();

    trace( heuristic, 1, "Considering %d learned clauses.\n", maxBerkminNumber );
    unsigned int count = 0;
    for( List< LearnedClause* >::const_reverse_iterator it = learnedClauses.rbegin(); it != learnedClauses.rend(); ++it )
    {        
        if( count++ > maxBerkminNumber )
            break;

        resetCounters();
        assert( *it != NULL );
        LearnedClause& learnedClause = **it;
        trace( heuristic, 1, "Learned clause %d: %s.\n", count, learnedClause.clauseToCharStar() );
        visit( &learnedClause );

        if( hasChosenLiteral() )
        {
            choosePolarityTopMostUndefinedClause();
            break;
        }
    }
}

void
BerkminHeuristic::pickLiteralUsingActivity(
    Solver& solver )
{
    resetCounters();
    trace( heuristic, 1, "Starting pickLiteralUsingActivity.\n" );
    Variable* variable = NULL;
    Variable* mostOccurrencesMax = NULL;
    
    do
    {
        variable = solver.getNextUndefinedVariable();
        
        assert( "Variable has not been set." && variable != NULL );
        assert( "The literal must be undefined." && variable->isUndefined() );
        onNavigatingVariable( variable );

        if( onNavigatingVariableForMostOccurrences( variable ) )
        {            
            mostOccurrencesMax = variable;
        }

    } while( solver.hasNextUndefinedVariable() );
    
    if( hasChosenLiteral() )
    {
        choosePolarityHigherGlobalCounter( solver );
        return;
    }
    
    if( mostOccurrencesMax != NULL )
    {
        setChosenVariable( mostOccurrencesMax );
        choosePolarityMostOccurrences();
        return;
    }

    trace( heuristic, 1, "Choosing first undefined literal.\n" );
    assert( "Variable has not been set." && variable != NULL );
    assert( "The literal must be undefined." && variable->isUndefined() );
    setChosenVariable( variable );
    setChosenPolarity( false );
}

void
BerkminHeuristic::onLearning(
    Solver& )
{
    numberOfConflicts++;
    //TODO: Implement aging.
}

void
BerkminHeuristic::onRestarting(
    Solver& )
{
}

void
BerkminHeuristic::visit(
    Clause* clause )
{
    clause->visitForHeuristic( this );
}

void
BerkminHeuristic::onNavigatingVariable(
    Variable* variable )
{
    assert( "Variable has not been set." && variable != NULL );
    assert( "Variable must be undefined." && variable->isUndefined() );
    
    BERKMIN_HEURISTIC_COUNTER totalCounter = getTotalCounter( variable );
    if( totalCounter > maxCounter )
    {
        maxCounter = totalCounter;
        setChosenVariable( variable );
    }
}

bool
BerkminHeuristic::onNavigatingVariableForMostOccurrences(
    Variable* variable )
{
    assert( "Variable has not been set." && variable != NULL );
    assert( "Variable must be undefined." && variable->isUndefined() );    
    unsigned int numberOfOccurrences = variable->numberOfPositiveWatchedClauses() + variable->numberOfNegativeWatchedClauses();
    if( numberOfOccurrences > maxOccurrences )
    {
        maxOccurrences = numberOfOccurrences;
        return true;
    }
    
    return false;
}

void
BerkminHeuristic::choosePolarityTopMostUndefinedClause()
{
    assert( "Variable has not been set." && getChosenVariable() != NULL );
    setChosenPolarity( getLiteralCounter( getChosenVariable()->getPositiveHeuristicCounter() ) > getLiteralCounter( getChosenVariable()->getNegativeHeuristicCounter() ) );
}

void
BerkminHeuristic::choosePolarityMostOccurrences()
{
    setChosenPolarity( getChosenVariable()->numberOfPositiveWatchedClauses() > getChosenVariable()->numberOfNegativeWatchedClauses() );
}

void
BerkminHeuristic::choosePolarityHigherGlobalCounter(
    Solver& solver )
{
    trace( heuristic, 2, "Higher Global Counter: choose polarity.\n" );
    assert( "Variable has not been set." && getChosenVariable() != NULL );

    Literal positiveLiteral( getChosenVariable() );    
    unsigned int value1 = estimatePropagation( positiveLiteral, solver );
    trace( heuristic, 2, "Estimated propagation of positive literal %s, value: %d.\n", positiveLiteral.literalToCharStar(), value1 );
    if( value1 == UINT_MAX )
    {
        setChosenPolarity( true );
        return;
    }
    
    Literal negativeLiteral( getChosenVariable(), false );
    unsigned int value2 = estimatePropagation( negativeLiteral, solver );
    trace( heuristic, 2, "Estimated propagation of negative literal %s, value: %d.\n", negativeLiteral.literalToCharStar(), value2 );
    
    setChosenPolarity( value1 > value2 );
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
