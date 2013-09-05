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

#include "PositiveLiteral.h"
#include "heuristics/BerkminHeuristic.h"
#include "Clause.h"

ostream&
PositiveLiteral::print( 
    ostream& out ) const
{
    out << name;
    return out;
}

bool
PositiveLiteral::isFalse() const
{
    return truthValue == FALSE;
}

bool
PositiveLiteral::isTrue() const
{
    return truthValue == TRUE;
}

bool
PositiveLiteral::isUndefined() const
{
    return truthValue == UNDEFINED;
}

bool
PositiveLiteral::setFalse()
{
    if( truthValue == TRUE )
        return false;
    truthValue = FALSE;
    return true;
}

bool
PositiveLiteral::setTrue()
{
    if( truthValue == FALSE )
        return false;
    truthValue = TRUE;
    return true;
}

unsigned int
PositiveLiteral::getDecisionLevel() const
{
    assert( "Undefined literals have no decision level." && !isUndefined() );
    return decisionLevel;
}

void
PositiveLiteral::setDecisionLevel(
    unsigned int dl )
{
    decisionLevel = dl;
}

unsigned int
PositiveLiteral::getOrderInThePropagation() const
{
    assert( "Undefined literals have not been propagated." && !isUndefined() );
    return orderInThePropagation;
}

void
PositiveLiteral::setOrderInThePropagation(
    unsigned int order )
{
    orderInThePropagation = order;
}

PositiveLiteral*
PositiveLiteral::getPositiveLiteral()
{
    return this;
}

NegativeLiteral*
PositiveLiteral::getNegativeLiteral()
{
    return static_cast< NegativeLiteral* >( oppositeLiteral );
}

void
PositiveLiteral::setImplicant(
    Clause* clause )
{
    implicant = clause;
}

bool
PositiveLiteral::isImplicant( 
    const Clause* clause ) const
{
    return !this->isUndefined() && implicant == clause;
}

void
PositiveLiteral::onLearning( 
    LearningStrategy* strategy )
{    
    //The implicant can be NULL if the literal is a choice.
    if( implicant != NULL )
    {
        implicant->onLearning( strategy );
    }    
}