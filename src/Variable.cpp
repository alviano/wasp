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

#include "Variable.h"
#include "Clause.h"
#include "Literal.h"
#include "Solver.h"

ostream& operator<<( 
    ostream& out,
    const Variable& var )
{
    out << var.name;
    return out;
}

Variable::~Variable()
{
//    if( heuristicCounters[ 0 ] )
//        delete heuristicCounters[ 0 ];
//    
//    if( heuristicCounters[ 1 ] )
//        delete heuristicCounters[ 1 ];
}

//void
//Variable::setHeuristicCounterForLiterals( 
//    HeuristicCounterFactoryForLiteral* heuristicCounterFactoryForLiteral )
//{
//    heuristicCounters[ 0 ] = heuristicCounterFactoryForLiteral->createHeuristicCounter();
//    heuristicCounters[ 1 ] = heuristicCounterFactoryForLiteral->createHeuristicCounter();
//}

void
Variable::onLearning( 
    Learning* strategy )
{    
    //The implicant can be NULL if the literal is a choice.
    if( implicant != NULL )
    {
        implicant->onLearning( strategy );
    }
}

void
Variable::unitPropagation(
    Solver& solver )
{
    assert( "Variable to propagate is Undefined." && !this->isUndefined() );
    assert( FALSE == 1 && TRUE == 2 );

    unsigned int sign = ( truthValue >> 1 );
    assert( sign <= 1 );
    assert( truthValue == TRUE ? sign == NEGATIVE : sign == POSITIVE );
    
    watchedLists[ sign ].startIteration();
    while( watchedLists[ sign ].hasNext() && !solver.conflictDetected() )
    {
        Clause* clause = watchedLists[ sign ].next();
        trace( solving, 3, "Considering clause %s.\n", clause->clauseToCharStar() );
        if( clause->onLiteralFalse( Literal::createOppositeFromAssignedVariable( this ) ) )
            solver.assignLiteral( clause );
    }
}
