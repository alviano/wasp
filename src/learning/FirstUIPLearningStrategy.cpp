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

#include "FirstUIPLearningStrategy.h"
#include "../Clause.h"
#include "../Literal.h"
#include "../Solver.h"

#include <cassert>

void
FirstUIPLearningStrategy::onConflict(
    Literal conflictLiteral,
    Clause* conflictClause,
    Solver& solver )
{
    clearDataStructures();
    assert( "No conflict literal is set." && conflictLiteral != NULL );
    assert( "Learned clause has to be NULL in the beginning." && learnedClause == NULL );
    assert( "The counter must be equal to 0." && visitedVariablesCounter == 0 );
    assert( "Data structures must be empty." && visitedVariables.empty() );
    
    learnedClause = new LearnedClause();
    decisionLevel = solver.getCurrentDecisionLevel();

    trace( solving, 2, "Starting First UIP Learning Strategy. Current Level: %d.\n", decisionLevel );
    
    //Compute implicants of the conflicting literal.
    conflictClause->onLearning( this );
    conflictLiteral.onLearning( this );    

    trace( solving, 2, "Conflict literal: %s.\n", conflictLiteral.literalToCharStar() );
    visitedVariables.insert( conflictLiteral.getVariable() );
    solver.startIterationOnAssignedVariable();
    
    //If there is only one element, this element is the first UIP.
    while( visitedVariables.size() - visitedVariablesCounter > 1 )
	{
        //Get next literal.
		Literal currentLiteral = getNextLiteralToNavigate( solver );
        trace( solving, 3, "Navigating %s for calculating UIP.\n", currentLiteral.literalToCharStar() );
        //Compute implicants of the literal.
        currentLiteral.onLearning( this );
	}

    Literal firstUIP = getNextLiteralToNavigate( solver );
    trace( solving, 2, "First UIP: %s.\n", firstUIP.literalToCharStar() );
    
    //literalsToNavigate.pop_back();
	learnedClause->addLiteral( firstUIP );    
    
    assert( learnedClause->size() > 0 );
    
    trace( solving, 2, "Learned Clause: %s.\n", learnedClause->clauseToCharStar() );
    
    if( learnedClause->size() == 1 )
    {
        solver.onLearningUnaryClause( firstUIP, learnedClause );
        restartsStrategy->onLearningUnaryClause();
    }
    else
    {
        assert( maxPosition < ( learnedClause->size() - 1 ) );

        //Be careful. UIP should be always in position 0.
        learnedClause->attachClause( learnedClause->size() - 1, maxPosition );
        solver.addLearnedClause( learnedClause );

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
FirstUIPLearningStrategy::getNextLiteralToNavigate(
    Solver& solver )
{    
    while( true )
    {
        assert( solver.hasNextAssignedVariable() );
        Literal next = solver.getOppositeLiteralFromLastAssignedVariable();

        assert( !next.isUndefined() );
        solver.unrollLastVariable();
        assert( next.isUndefined() );

        if( visitedVariables.find( next.getVariable() ) != visitedVariables.end() )
        {
            ++visitedVariablesCounter;            
            return next;
        }        
    }    
}

void
FirstUIPLearningStrategy::addLiteralInLearnedClause( 
    Literal literal )
{
    assert( "Learned clause is not initialized." && learnedClause != NULL );

    if( visitedVariables.insert( literal.getVariable() ).second )
    {
        assert( !literal.isUndefined() );
        if( literal.getDecisionLevel() > maxDecisionLevel )
        {
            maxDecisionLevel = literal.getDecisionLevel();
            maxPosition = learnedClause->size();
        }
        learnedClause->addLiteral( literal );
        ++visitedVariablesCounter;
    }
}

void
FirstUIPLearningStrategy::addLiteralToNavigate( 
    Literal literal )
{
    visitedVariables.insert( literal.getVariable() );
}

void
FirstUIPLearningStrategy::onNavigatingLiteral( 
    Literal literal )
{
    assert( literal != NULL );
    unsigned int literalDecisionLevel = literal.getDecisionLevel();
    
    if( literalDecisionLevel == decisionLevel )
    {
        literal.onNavigatingImplicationGraph();
        addLiteralToNavigate( literal );
    }
    else if( literalDecisionLevel > 0 )
    {
        literal.onNavigatingLearnedClause();
        addLiteralInLearnedClause( literal );
    }
}

