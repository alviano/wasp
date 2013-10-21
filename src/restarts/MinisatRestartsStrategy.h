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

/* 
 * File:   MinisatRestartsStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 17 October 2013, 14.08
 */

#ifndef MINISATRESTARTSSTRATEGY_H
#define	MINISATRESTARTSSTRATEGY_H

#include <cassert>
#include <cmath>

#include "SequenceBasedRestartsStrategy.h"

class MinisatRestartsStrategy : public SequenceBasedRestartsStrategy
{
    public:
        inline MinisatRestartsStrategy( unsigned int threshold = 100, bool lubyRestars = true );        
        
    protected:
        virtual void computeNextRestartValue();

    private:
        inline double luby( double, int );
        inline void computeRestartBase();

        double restartBase;
        double restartIncrement;
        bool lubyRestarts;
};

MinisatRestartsStrategy::MinisatRestartsStrategy( 
    unsigned int threshold, bool luby ) : SequenceBasedRestartsStrategy( threshold ), restartBase( 0.0 ), restartIncrement( 2.0 ), lubyRestarts( luby )
{
    assert( threshold >= 100 );
    numberOfRestarts = 0;
    computeNextRestartValue();
}

void
MinisatRestartsStrategy::computeRestartBase()
{
    restartBase = lubyRestarts ? luby( restartIncrement, numberOfRestarts ) : pow( restartIncrement, numberOfRestarts );
}

double
MinisatRestartsStrategy::luby(
    double y,
    int x )
{
    // Find the finite subsequence that contains index 'x', and the
    // size of that subsequence:
    int size, seq;
    for( size = 1, seq = 0; size < x + 1; seq++, size = 2 * size + 1 );

    while( size - 1 != x )
    {
        size = ( size - 1 ) >> 1;
        seq--;
        x = x % size;
    }
    return pow( y, seq );
}

#endif	/* MINISATRESTARTSSTRATEGY_H */

