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

#include "RestartsBasedDeletionStrategy.h"
#include "../../Solver.h"

#include <cmath>

bool
RestartsBasedDeletionStrategy::onLearningProtected(
    Solver& solver )
{
    unsigned int programSize = solver.numberOfClauses();
    unsigned int value1 = ( programSize / 3.0 ) * pow( 1.1, countRestarts );
    unsigned int value2 = programSize * 3;
    
    unsigned int min = value1 > value2 ? value1 : value2;    
    
    return( solver.numberOfLearnedClauses() > min );    
}

void
RestartsBasedDeletionStrategy::onRestarting()    
{
    countRestarts++;
}