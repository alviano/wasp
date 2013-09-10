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
#include "../solvers/Solver.h"

#include <cassert>

void
FirstUIPLearningStrategy::onConflict(
    Literal* conflictLiteral,
    Clause* conflictClause,
    Solver& solver )
{
    clearDataStructures();
    assert( "No conflict literal is set." && conflictLiteral != NULL );
    assert( "Learned clause has to be NULL in the beginning." && learnedClause == NULL );
    assert( "The literalsToNavigate structure must to be empty in the beginning." && literalsToNavigate.empty() );

    learnedClause = new LearnedClause();
    decisionLevel = solver.getCurrentDecisionLevel();

    //Compute implicants of the conflicting literal.
    conflictClause->onLearning( this );
    conflictLiteral->onLearning( this );

	//If there is only one element, this element is the first UIP.
	while( literalsToNavigate.size() > 1 )
	{
        //Get next literal.
		Literal* currentLiteral = getNextLiteralToNavigate();

        //Compute implicants of the literal.
        currentLiteral->onLearning( this );
	}

	assert( "At this point of the computation the first UIP is computed." && literalsToNavigate.size() == 1 );

	Literal* firstUIP = literalsToNavigate.back();
    literalsToNavigate.pop_back();
	learnedClause->addLiteral( firstUIP );   
    
    assert( learnedClause->size() > 0 );
    if( learnedClause->size() == 1 )
    {
        solver.onLearningUnaryClause( firstUIP, learnedClause );
        restartsStrategy->onLearningUnaryClause();
    }
    else
    {
        assert( maxPosition < ( learnedClause->size() - 1 ) );

        learnedClause->attachClause( maxPosition, learnedClause->size() - 1 );
        solver.addLearnedClause( learnedClause );

        bool restartRequired = restartsStrategy->onLearningClause();
        if( restartRequired )
        {
            solver.onRestarting();
        }
        else
        {
            assert( maxDecisionLevel != 0 );
            solver.onLearningClause( firstUIP, learnedClause, maxDecisionLevel );
        }
    }
}

Literal*
FirstUIPLearningStrategy::getNextLiteralToNavigate()
{
    assert( "There is no next literal: list is empty." && !literalsToNavigate.empty() );
    
    list< Literal* >::iterator it = literalsToNavigate.begin();
    list< Literal* >::iterator maxLiteralIterator = it;
    unsigned int max = ( *maxLiteralIterator )->getOrderInThePropagation();
    ++it;
    
    for( ; it != literalsToNavigate.end(); ++it )
    {
        Literal* current = *it;
        if( max < current->getOrderInThePropagation() )
        {
            max = current->getOrderInThePropagation();
            maxLiteralIterator = it;
        }
    }
    
    //The most recent literal.
    Literal* maxLiteral = *maxLiteralIterator;
    literalsToNavigate.erase( maxLiteralIterator );
    return maxLiteral;
}

void
FirstUIPLearningStrategy::onNavigatingLiteral( 
    Literal* literal )
{
    assert( literal != NULL );
    unsigned int literalDecisionLevel = literal->getDecisionLevel();
    
    if( literalDecisionLevel == decisionLevel )
    {
        literal->onNavigatingImplicationGraph();
        addLiteralToNavigate( literal );
    }
    else if( literalDecisionLevel > 0 )
    {
        literal->onNavigatingLearnedClause();
        addLiteralInLearnedClause( literal );
    }
}

void
FirstUIPLearningStrategy::addLiteralInLearnedClause( 
    Literal* literal )
{
    assert( "Learned clause is not initialized." && learnedClause != NULL );
    
    if( addedLiterals.insert( literal ).second )
    {
        if( literal->getDecisionLevel() > maxDecisionLevel )
        {
            maxDecisionLevel = literal->getDecisionLevel();
            maxPosition = learnedClause->size();
        }
        learnedClause->addLiteral( literal );
    }
}