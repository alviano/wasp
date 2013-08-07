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

#include "SequenceBasedRestartsStrategy.h"

#include <cassert>
#include <cmath>

void
SequenceBasedRestartsStrategy::computeNextRestartValue()
{
    nextRestartValue = computeRestartNumber( numberOfRestarts ) * threshold;
}

bool
SequenceBasedRestartsStrategy::onLearningClause()
{
    if( ++conflictsCount == nextRestartValue )
    {
        conflictsCount = 0;
        numberOfRestarts++;
        computeNextRestartValue();
        return true;
    }

    return false;
}

void
SequenceBasedRestartsStrategy::onLearningUnaryClause()
{
    conflictsCount = 0;
}

unsigned int
SequenceBasedRestartsStrategy::computeRestartNumber( 
    unsigned int i )
{
    assert( i != 0 );
    static unsigned int a[] = { 1,3,7,15,31,63,127,255,511,1023, 2047, 4095, 8191, 16383, 32767 };
    //A binary search to compute
    int first = 0;
    int last = 32;
    int mid = ( last + first ) / 2;

    while( first < last )
    {
        if( ( i >> mid ) == 0 )
        {
            last = mid;
        }
        else
        {
            first = mid + 1;
        }
        mid = ( last + first ) / 2;
    }

    if( a[ first - 1 ] == i )
    {
        int pos = pow(2, ( first - 1 ) );
        return pos;
    }
    else
    {
        int newValue = i - pow( 2, ( first - 1 ) ) + 1 ;
        return computeRestartNumber( newValue );
    }
}