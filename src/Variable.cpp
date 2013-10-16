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
#include "heuristics/factories/HeuristicCounterFactoryForLiteral.h"
#include "heuristics/visitors/HeuristicVisitor.h"
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
    if( heuristicCounters[ 0 ] )
        delete heuristicCounters[ 0 ];
    
    if( heuristicCounters[ 1 ] )
        delete heuristicCounters[ 1 ];
}

void
Variable::setHeuristicCounterForLiterals( 
    HeuristicCounterFactoryForLiteral* heuristicCounterFactoryForLiteral )
{
    heuristicCounters[ 0 ] = heuristicCounterFactoryForLiteral->createHeuristicCounter();
    heuristicCounters[ 1 ] = heuristicCounterFactoryForLiteral->createHeuristicCounter();
}

void
Variable::onLearning( 
    LearningStrategy* strategy )
{    
    //The implicant can be NULL if the literal is a choice.
    if( implicant != NULL )
    {
        implicant->onLearning( strategy );
    }
}

void
Variable::visitForHeuristic(
    HeuristicVisitor* heuristicVisitor )
{    
    assert( "Heuristic visitor has not been set." && heuristicVisitor != NULL );
    if( isUndefined() )
        heuristicVisitor->onNavigatingVariable( this );
}

void
Variable::unitPropagation(
    Solver& solver,
    Literal literal,
    unsigned int sign )
{
    assert( "Variable to propagate is Undefined." && !this->isUndefined() );
    assert( "A literal must be true or false." && sign <= 1 );
    
    watchedLists[ sign ].startIteration();
    while( watchedLists[ sign ].hasNext() && !solver.conflictDetected() )
    {
        Clause* clause = watchedLists[ sign ].next();
        trace( solving, 3, "Considering clause %s.\n", clause->clauseToCharStar() );
        clause->onLiteralFalse( literal, solver );
    }
}
