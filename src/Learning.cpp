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

#include "Learning.h"
#include "Clause.h"
#include "Literal.h"
#include "Solver.h"

#include <cassert>

bool
Learning::isVisitedVariablesEmpty() const
{
    for( unsigned i = 1; i < visitedVariables.size(); ++i )
        if( visitedVariables[ i ] == numberOfCalls )
            return false;
    return true;
}

Clause*
Learning::onConflict(
    Literal conflictLiteral,
    Clause* conflictClause )
{
    ++numberOfCalls;
    clearDataStructures();
    assert( "No conflict literal is set." && conflictLiteral != Literal::null );
    assert( "Learned clause has to be NULL in the beginning." && learnedClause == NULL );
    assert( "The counter must be equal to 0." && pendingVisitedVariables == 0 );
    assert( isVisitedVariablesEmpty() );
    
    learnedClause = new Clause();
    solver.initClauseData( learnedClause );
    decisionLevel = solver.getCurrentDecisionLevel();

    trace_msg( learning, 2, "Starting First UIP Learning Strategy. Current Level: " << decisionLevel );
    
    //Compute implicants of the conflicting literal.
    conflictClause->onLearning( this );
    assert( conflictLiteral.getVariable()->getImplicant() != NULL ); // FIXME: I added this assert. Is it right?
    conflictLiteral.getVariable()->getImplicant()->onLearning( this );    

    trace( learning, 2, "Conflict literal: %s.\n", toString( conflictLiteral ).c_str() );
    assert( conflictLiteral.getVariable()->getId() < visitedVariables.size() && visitedVariables[ conflictLiteral.getVariable()->getId() ] == numberOfCalls );
    solver.startIterationOnAssignedVariable();
    
    //If there is only one element, this element is the first UIP.
    while( pendingVisitedVariables > 1 )
	{
        //Get next literal.
        Literal currentLiteral = getNextLiteralToNavigate();
        trace( learning, 3, "Navigating %s for calculating UIP.\n", toString( currentLiteral ).c_str() );
        //Compute implicants of the literal.
        if( currentLiteral.getVariable()->getImplicant() != NULL )
            currentLiteral.getVariable()->getImplicant()->onLearning( this );
	}

    Literal firstUIP = getNextLiteralToNavigate();
    trace( learning, 2, "First UIP: %s.\n", toString( firstUIP ).c_str() );
    
	learnedClause->addLiteral( firstUIP );    
    
    assert( learnedClause->size() > 0 );
    
    if( learnedClause->size() >= 2 )
    {
        assert( learnedClause->getAt( learnedClause->size() - 1).getDecisionLevel() == solver.getCurrentDecisionLevel() );
        learnedClause->swapLiterals( 0, learnedClause->size() - 1 );
        learnedClause->swapLiterals( 1, maxPosition != 0 ? maxPosition : learnedClause->size() - 1 );
        assert( learnedClause->getAt( 0 ).getDecisionLevel() == solver.getCurrentDecisionLevel() );
    }
    
    trace( learning, 1, "Learned Clause: %s.\n", toString( *learnedClause ).c_str() );
    
    return learnedClause;
}

Literal
Learning::getNextLiteralToNavigate()
{    
    while( true )
    {
        assert( solver.hasNextAssignedVariable() );
        Literal next = solver.getOppositeLiteralFromLastAssignedVariable();

        assert( !next.isUndefined() );
        solver.unrollLastVariable();
        assert( next.isUndefined() );

        if( visitedVariables[ next.getVariable()->getId() ] == numberOfCalls )
        {
            --pendingVisitedVariables;
            return next;
        }        
    }    
}

void
Learning::addLiteralInLearnedClause( 
    Literal literal )
{
    assert( "Learned clause is not initialized." && learnedClause != NULL );

    if( visitedVariables[ literal.getVariable()->getId() ] != numberOfCalls )
    {
        visitedVariables[ literal.getVariable()->getId() ] = numberOfCalls;
        assert( !literal.isUndefined() );
        if( literal.getDecisionLevel() > maxDecisionLevel )
        {
            maxDecisionLevel = literal.getDecisionLevel();
            maxPosition = learnedClause->size();
        }
        learnedClause->addLiteral( literal );
    }
}

void
Learning::addLiteralToNavigate( 
    Literal literal )
{
    if( visitedVariables[ literal.getVariable()->getId() ] != numberOfCalls )
    {
        visitedVariables[ literal.getVariable()->getId() ] = numberOfCalls;
        ++pendingVisitedVariables;    
    }
}

void
Learning::onNavigatingLiteral( 
    Literal literal )
{
    assert( literal != Literal::null );
    unsigned int literalDecisionLevel = literal.getDecisionLevel();
    
    if( literalDecisionLevel == decisionLevel )
    {
        // FIXME: Do we need to distinguish between the two cases? onNavigatingImplicationGraph();
        solver.onLiteralInvolvedInConflict( literal );
        addLiteralToNavigate( literal );
    }
    else if( literalDecisionLevel > 0 )
    {
        // FIXME: Do we need to distinguish between the two cases? literal.onNavigatingLearnedClause();
        solver.onLiteralInvolvedInConflict( literal );
        addLiteralInLearnedClause( literal );
    }
}

