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

#ifndef WASP_ONEBB_H
#define WASP_ONEBB_H

#include "WeakInterface.h"
#include "One.h"
#include "Opt.h"
using namespace std;

class OneBB : public One
{
    public:
        inline OneBB( Solver& s, bool useRestarts = false ) : One( s ), numberOfModels( 0 ), useRestarts_( useRestarts ), increment( 0 ), counter( 0 )
        {
            strategyModelGuided = new Opt( s, true );
            strategyModelGuided->setMixedApproach();
        }
        unsigned int run();
        
    private:
        unsigned int one();
        unsigned int bb();
        
        unsigned int numberOfModels;
        bool useRestarts_;
        
        unsigned int increment;
        unsigned int counter;
        
        inline void initHeuristicValues();
        inline void setAndUpdateHeuristicValues();
        
        inline void addOptimizationLiteralInAssumptions();
        
        Opt* strategyModelGuided;
};

void
OneBB::initHeuristicValues()
{
    if( useRestarts_ )
    {
        increment = 10;
        counter = 50;
    }
    else
    {
        increment = 10000;
        counter = 50000;
    }
}

void
OneBB::setAndUpdateHeuristicValues()
{
    useRestarts_ ? solver.setMaxNumberOfRestarts( counter ) : solver.setMaxNumberOfChoices( counter );
    counter += increment;
    increment *= 1.05;    
}

void
OneBB::addOptimizationLiteralInAssumptions()
{
    assert( assumptions.empty() );
    Literal lit = strategyModelGuided->getAssumptionToAdd();
    if( lit != Literal::null )
        assumptions.push_back( lit );
}

#endif