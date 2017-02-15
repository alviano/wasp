/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
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

#ifndef WASP_RESTART_H
#define WASP_RESTART_H

#include <cassert>
#include <cmath>

class Restart
{
    public:
        inline Restart( unsigned int threshold = 100, bool lubyRestars = true );
        inline bool hasToRestart();
        inline void onRestart(){ conflictsCount = 0; }
        inline Restart* clone() const { return new Restart( *this ); }

    private:                
        inline double luby( double, int );
        inline void computeRestartBase();
        inline void computeNextRestartValue();

        unsigned int nextRestartValue;
        unsigned int conflictsCount;        
        unsigned int threshold;
        unsigned int numberOfRestarts;
        
        double restartBase;
        double restartIncrement;
        bool lubyRestarts;       
};

Restart::Restart( 
    unsigned int t,
    bool luby ) : conflictsCount( 0 ), threshold( t ), numberOfRestarts( 0 ), restartBase( 0.0 ), restartIncrement( 2.0 ), lubyRestarts( luby )
{
    assert( threshold >= 32 );
    computeNextRestartValue();
}

void
Restart::computeRestartBase()
{
    restartBase = lubyRestarts ? luby( restartIncrement, numberOfRestarts ) : pow( restartIncrement, numberOfRestarts );
}

double
Restart::luby(
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

void
Restart::computeNextRestartValue()
{
    computeRestartBase();
    nextRestartValue =  restartBase * threshold;
}

bool
Restart::hasToRestart()
{
    if( ++conflictsCount == nextRestartValue )
    {
        numberOfRestarts++;
        computeNextRestartValue();
        return true;
    }

    return false;
}

#endif
