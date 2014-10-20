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

#include "Opt.h"

unsigned int
Opt::run()
{
    trace_msg( weakconstraints, 1, "Starting algorithm OPT" );
    if( !disableprefchoices_ )
        solver.addPreferredChoicesFromOptimizationLiterals();    
    unsigned int numberOfModels = 0;
    while( solver.solve() == COHERENT )
    {
        numberOfModels++;
        solver.printAnswerSet();
        unsigned int modelCost = solver.computeCostOfModel(); 

        solver.printOptimizationValue( modelCost );
        trace_msg( weakconstraints, 2, "Decision level of solver: " << solver.getCurrentDecisionLevel() );
        if( modelCost == 0 || solver.getCurrentDecisionLevel() == 0 )
            break;
        
//        /*Implement a better policy of unroll*/
//        solver.doRestart();
//        solver.simplifyOnRestart();
//        solver.clearConflictStatus();
        
//        solver.removePrefChoices();
        
        trace_msg( weakconstraints, 2, "Updating bound of optimization aggregate. Model cost: " << modelCost );
        if( !solver.updateOptimizationAggregate( modelCost ) )
        {
            trace_msg( weakconstraints, 3, "Failed updating of optimization aggregate: return" );
            break;        
        }
        
        static int loop = 0;
        if( ++loop > 15 )
            exit( 0 );
        
        trace_msg( weakconstraints, 2, "Calling solver..." );
    }
    
    if( numberOfModels > 0 )
        return OPTIMUM_FOUND;
    return INCOHERENT;
}