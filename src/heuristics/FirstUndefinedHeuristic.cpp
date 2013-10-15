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

#include "FirstUndefinedHeuristic.h"

#include "../Literal.h"
#include "../Solver.h"

Literal
FirstUndefinedHeuristic::makeAChoice(
    Solver& solver )
{
    Variable* variable = solver.getNextUndefinedVariable(); // solver.getUndefinedVariables().at( 0 );
    assert( "The literal must be undefined." && variable->isUndefined() );
    return Literal( variable, false );
}

void
FirstUndefinedHeuristic::onLearning(
    Solver& )
{
}
        
void
FirstUndefinedHeuristic::onRestarting(
    Solver& )
{
}