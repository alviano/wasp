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

#include "Literal.h"
#include "Clause.h"
#include "WaspRule.h"
#include "heuristics/visitors/HeuristicVisitor.h"
#include "solvers/Solver.h"

#include <cassert>

ostream&
operator<<( 
    ostream& out, 
    const Literal& lit )
{
    return lit.print( out );
}

void
Literal::unitPropagation(
    Solver& solver )
{
    assert( "Literal to propagate is Undefined." && !this->isUndefined() );
    if( this->isFalse() )
    {
        watchedClauses.startIteration();
        while( watchedClauses.hasNext() && !solver.conflictDetected() )
        {
            Clause* clause = watchedClauses.next();
            clause->onLiteralFalse( this, solver );            
        }
    }
    else
    {
        oppositeLiteral->unitPropagation( solver );
    }
}

void
Literal::visitForHeuristic(
    HeuristicVisitor* heuristicVisitor )
{    
    assert( "Heuristic visitor has not been set." && heuristicVisitor != NULL );
    if( isUndefined() )
        heuristicVisitor->onNavigatingLiteral( this );
}

void
Literal::supportPropagation( 
    Solver& solver )
{
    if( isTrue() )
    {
        for( unsigned int i = 0; i < allWaspRules.size(); i++ )
        {
            WaspRule* waspRule = allWaspRules[ i ];
            waspRule->onLiteralTrue( this, solver );
        }
    }
}