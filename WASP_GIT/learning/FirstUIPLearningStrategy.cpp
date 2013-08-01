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

#include <cassert>
#include <iostream>
#include <list>
using namespace std;

Clause*
FirstUIPLearningStrategy::learnClause(
    Literal* conflictLiteral )
{
    assert( "Conflict literal is NULL." && conflictLiteral != NULL );
    cout << "CONFLICT LITERAL " << *conflictLiteral << endl;
    
    assert( "Implement Me!!!" && 0 );
    /*
    Clause* clause = new Clause();

	list< Literal* > literalsOfSameLevel;    

    const Clause* implicant = conflictLiteral->getImplicant();
    
    computeNeighbours( conflictLiteral, literalsOfSameLevel, literalsOfPreviousLevel );

	//Se c'è un solo letterale dello stesso livello, questo deve essere il primo uip. Penso che valga solo se si fa una visita in ampiezza.
	while( literalsOfSameLevel.size() > 1 )
	{
		Literal lit = literalsOfSameLevel.pop_front(); //Visita del grafo in ampiezza
		computeNeighbours( lit, literalsOfSameLevel, literalsOfPreviousLevel );
	}

	assert( literalsOfSameLevel.size() == 1 );

	Literal uip = literalsOfSameLevel.pop();
	literalsOfPreviousLevel.push( uip );

	insertConstraint( literalsOfPreviousLevel );
    ***/
    return NULL;
}