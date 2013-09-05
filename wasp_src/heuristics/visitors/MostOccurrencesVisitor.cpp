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

#include "MostOccurrencesVisitor.h"
#include <cassert>

#include "../../Literal.h"

void
MostOccurrencesVisitor::onNavigatingLiteral(    
    Literal* literal )
{
    assert( "Literal has not been set." && literal != NULL );
    assert( "Literal must be undefined." && literal->isUndefined() );
    Literal* oppositeLiteral = literal->getOppositeLiteral();
    unsigned int numberOfOccurrences = literal->numberOfWatchedClauses() + oppositeLiteral->numberOfWatchedClauses();;
    if( numberOfOccurrences > maxOccurrences )
    {
        maxOccurrences = numberOfOccurrences;        
        ( literal->numberOfWatchedClauses() > oppositeLiteral->numberOfWatchedClauses() ) ? chosenLiteral = literal : chosenLiteral = oppositeLiteral;
    }
}