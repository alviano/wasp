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

#include "OutputBuilder.h"
#include "../util/WaspOptions.h"
extern int EXIT_CODE;

void
OutputBuilder::foundModelOptimization(
    const Vector< uint64_t >& costs )
{     
    cout << COST;
    for( int i = costs.size() - 1; i >= 0; --i )
        cout << " " << costs[ i ] << WEIGHT_LEVEL_WEAKCONSTRAINT_SEPARATOR << ( i + 1 );
    cout << endl;    
    EXIT_CODE = 10;
}

void
OutputBuilder::optimumFound()
{
    cout << OPTIMUM_STRING << endl;
    EXIT_CODE = 30;
}

void
OutputBuilder::foundLowerBound(
    uint64_t lb )
{
    if( wasp::Options::printBounds )
        cout << "COST LB: " << lb << endl;
}

void
OutputBuilder::foundUpperBound(
    uint64_t ub )
{
    if( wasp::Options::printBounds )
        cout << "COST UB: " << ub << endl;    
}

void
OutputBuilder::onFinish()
{
}

void
OutputBuilder::onKill()
{
}