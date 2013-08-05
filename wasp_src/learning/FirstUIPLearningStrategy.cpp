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
#include <iostream>
#include <list>
using namespace std;

Clause*
FirstUIPLearningStrategy::learnClause(
    Literal* conflictLiteral,
    Solver& solver )
{
    assert( "Conflict literal is NULL." && conflictLiteral != NULL );
    assert( "Learned clause has to be NULL in the beginning." && learnedClause == NULL );
    
    cout << "CONFLICT LITERAL " << *conflictLiteral << endl;
    
    learnedClause = new Clause();
    decisionLevel = solver.getCurrentDecisionLevel();
	
    conflictLiteral->onConflict( this );
    
	//If there is only one element, this element is the first UIP.
	while( literalsOfTheSameLevel.size() > 1 )
	{
		Literal* currentLiteral = getNextToConsider();
        currentLiteral->onLearning( this );        
	}

	assert( literalsOfTheSameLevel.size() == 1 );

	Literal* uip = literalsOfTheSameLevel.back();
    literalsOfTheSameLevel.pop_back();    
	learnedClause->addLiteral( uip );

    return learnedClause;
}

Literal*
FirstUIPLearningStrategy::getNextToConsider()
{
    assert( "There is no next literal: list is empty." && !literalsOfTheSameLevel.empty() );
    
    list< Literal* >::iterator it = literalsOfTheSameLevel.begin();
    list< Literal* >::iterator maxLiteralIterator = it;
    unsigned int max = ( *maxLiteralIterator )->getOrderInThePropagation();
    ++it;
    
    for( ; it != literalsOfTheSameLevel.end(); ++it )
    {
        Literal* current = *it;
        if( max < current->getOrderInThePropagation() )
        {
            max = current->getOrderInThePropagation();
            maxLiteralIterator = it;
        }
    }
    
    Literal* maxLiteral = *maxLiteralIterator;
    literalsOfTheSameLevel.erase( maxLiteralIterator );    
    return maxLiteral;
}

void
FirstUIPLearningStrategy::onNavigatingLiteral( 
    Literal* literal )
{
    if( literal->getDecisionLevel() == decisionLevel )
    {
        addLiteralToNavigate( literal );
    }
    else
    {
        addLiteralInLearnedClause( literal );
    }
}

void
FirstUIPLearningStrategy::addLiteralInLearnedClause( 
    Literal* literal )
{
    assert( "Learned clause is not initialized." && learnedClause != NULL );
    if( addedLiterals.insert( literal ).second )
        learnedClause->addLiteral( literal );
}