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

void
Learning::onConflict(
    Literal conflictLiteral,
    Clause* conflictClause )
{
    ++numberOfCalls;
    clearDataStructures();
    assert( "No conflict literal is set." && conflictLiteral != NULL );
    assert( "Learned clause has to be NULL in the beginning." && learnedClause == NULL );
    assert( "The counter must be equal to 0." && pendingVisitedVariables == 0 );
    assert( isVisitedVariablesEmpty() );
    
    learnedClause = new LearnedClause();
    decisionLevel = solver.getCurrentDecisionLevel();

    trace( solving, 2, "Starting First UIP Learning Strategy. Current Level: %d.\n", decisionLevel );
    
    //Compute implicants of the conflicting literal.
    conflictClause->onLearning( this );
    assert( conflictLiteral.getVariable()->getImplicant() != NULL ); // FIXME: I added this assert. Is it right?
    conflictLiteral.getVariable()->getImplicant()->onLearning( this );    

    trace( solving, 2, "Conflict literal: %s.\n", conflictLiteral.literalToCharStar() );
    assert( conflictLiteral.getVariable()->getId() < visitedVariables.size() && visitedVariables[ conflictLiteral.getVariable()->getId() ] == numberOfCalls );
    solver.startIterationOnAssignedVariable();
    
    //If there is only one element, this element is the first UIP.
    while( pendingVisitedVariables > 1 )
	{
        //Get next literal.
		Literal currentLiteral = getNextLiteralToNavigate();
        trace( solving, 3, "Navigating %s for calculating UIP.\n", currentLiteral.literalToCharStar() );
        //Compute implicants of the literal.
        if( conflictLiteral.getVariable()->getImplicant() != NULL )
            currentLiteral.getVariable()->getImplicant()->onLearning( this );
	}

    Literal firstUIP = getNextLiteralToNavigate();
    trace( solving, 2, "First UIP: %s.\n", firstUIP.literalToCharStar() );
    
	learnedClause->addLiteral( firstUIP );    
    
    assert( learnedClause->size() > 0 );
    
    trace( solving, 2, "Learned Clause: %s.\n", learnedClause->clauseToCharStar() );
    
    if( learnedClause->size() == 1 )
    {
        solver.onLearningUnaryClause( firstUIP, learnedClause );
        assert( "The strategy for restarts must be initialized." && restartsStrategy != NULL );
        restartsStrategy->onLearningUnaryClause();
    }
    else
    {
        assert( maxPosition < ( learnedClause->size() - 1 ) );

        //Be careful. UIP should be always in position 0.
        learnedClause->attachClause( learnedClause->size() - 1, maxPosition );
        solver.addLearnedClause( learnedClause );

        assert( "The strategy for restarts must be initialized." && restartsStrategy != NULL );
        bool restartRequired = restartsStrategy->onLearningClause();
        if( restartRequired )
        {
            solver.onRestart();
        }
        else
        {
            assert( maxDecisionLevel != 0 );
            solver.onLearningClause( firstUIP, learnedClause, maxDecisionLevel );
        }
    }
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
    assert( literal != NULL );
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

