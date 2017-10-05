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
#include "OptimizationProblemUtils.h"
#include <climits>
#include "../Solver.h"

void
OptimizationProblemUtils::foundAnswerSet()
{
    uint64_t cost = solver.computeCostOfModel( level_ );
    trace_msg( weakconstraints, 2, "Found answer set with cost " << cost  << " - upper bound " << ub_ );
    if( cost >= ub_ )
        return;

    ub_ = cost;
    solver.printAnswerSet();
    solver.foundUpperBound( ub_ );
    Vector< uint64_t > costs;
    solver.computeCostOfModel( costs );
    solver.printOptimizationValue( costs );
    return;
}