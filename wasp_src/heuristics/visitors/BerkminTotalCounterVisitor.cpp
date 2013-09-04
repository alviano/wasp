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

#include "BerkminTotalCounterVisitor.h"
#include "../../Literal.h"
#include "../counters/BerkminCounters.h"
#include <cassert>

void
BerkminTotalCounterVisitor::onNavigatingLiteral(
    Literal* literal )
{
    assert( "Literal has not been set." && literal != NULL );
    assert( "Literal must be undefined." && literal->isUndefined() );
    
    Literal* oppositeLiteral = literal->getOppositeLiteral();    
    assert( "OppositeLiteral has not been set." && oppositeLiteral != NULL );
    
    BERKMIN_HEURISTIC_COUNTER totalCounter = getTotalCounter( literal, oppositeLiteral );
    if( totalCounter > maxCounter )
    {
        maxCounter = totalCounter;
        choosePolarity( literal, oppositeLiteral );
    }
}

BERKMIN_HEURISTIC_COUNTER
BerkminTotalCounterVisitor::getLiteralCounter( 
    const Literal* literal ) const
{
    const HeuristicCounterForLiteral* heuristicCounter = literal->getHeuristicCounter();
    
    assert( heuristicCounter != NULL );
    const BerkminCounters* berkminCounter = static_cast< const BerkminCounters* >( heuristicCounter );
    
    return berkminCounter->getCounter();
}

BERKMIN_HEURISTIC_COUNTER
BerkminTotalCounterVisitor::getTotalCounter( 
    const Literal* literal, 
    const Literal* oppositeLiteral ) const
{
    BERKMIN_HEURISTIC_COUNTER totalCounter = getLiteralCounter( literal ) + getLiteralCounter( oppositeLiteral );
    return totalCounter;
}