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

#include "NegativeLiteral.h"
#include "PositiveLiteral.h"

ostream&
NegativeLiteral::print( 
    ostream& out ) const
{
    assert( oppositeLiteral != NULL );
    out << "not " << *oppositeLiteral;
    return out;
}

bool
NegativeLiteral::isFalse() const
{
    assert( oppositeLiteral != NULL );
    return oppositeLiteral->isTrue();
}

bool
NegativeLiteral::isTrue() const
{
    assert( oppositeLiteral != NULL );
    return oppositeLiteral->isFalse();
}

bool
NegativeLiteral::isUndefined() const
{
    assert( oppositeLiteral != NULL );
    return oppositeLiteral->isUndefined();
}

bool
NegativeLiteral::setFalse()
{
    assert( oppositeLiteral != NULL );
    return oppositeLiteral->setTrue();
}

bool
NegativeLiteral::setTrue()
{
    assert( oppositeLiteral != NULL );
    return oppositeLiteral->setFalse();
}

unsigned int
NegativeLiteral::getDecisionLevel() const
{
    assert( "Undefined literals have no decision level." && !isUndefined() );
    return oppositeLiteral->getDecisionLevel();
}

void
NegativeLiteral::setDecisionLevel(
    unsigned int dl )
{
    oppositeLiteral->setDecisionLevel( dl );
}

unsigned int
NegativeLiteral::getOrderInThePropagation() const
{
    assert( "Undefined literals have not been propagated." && !isUndefined() );
    return oppositeLiteral->getOrderInThePropagation();
}

void
NegativeLiteral::setOrderInThePropagation(
    unsigned int order )
{
    oppositeLiteral->setOrderInThePropagation( order );
}

PositiveLiteral*
NegativeLiteral::getPositiveLiteral()
{
    return static_cast< PositiveLiteral* >( oppositeLiteral );
}

NegativeLiteral*
NegativeLiteral::getNegativeLiteral()
{
    return this;
}