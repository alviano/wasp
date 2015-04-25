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
#include "../Aggregate.h"

unsigned int
Opt::run()
{
    trace_msg( weakconstraints, 1, "Starting algorithm " << ( disableprefchoices_ ? "BASIC" : "OPT" ) );
    solver.sortOptimizationLiterals();
    createOptimizationAggregate();
    
    if( !disableprefchoices_ )
        solver.addPreferredChoicesFromOptimizationLiterals();
    unsigned int numberOfModels = 0;
    while( solver.solve() == COHERENT )
    {
        numberOfModels++;
        solver.printAnswerSet();
        uint64_t modelCost = solver.computeCostOfModel(); 

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
        if( !updateOptimizationAggregate( modelCost ) )
        {
            trace_msg( weakconstraints, 3, "Failed updating of optimization aggregate: return" );
            break;        
        }                
        
        trace_msg( weakconstraints, 2, "Calling solver..." );
    }
    
    if( numberOfModels > 0 )
        return OPTIMUM_FOUND;
    return INCOHERENT;
}

void
Opt::createOptimizationAggregate()
{
    assert_msg( aggregate == NULL, "Aggregate has been created" );
    assert_msg( solver.getCurrentDecisionLevel() == 0, "The decision level is " << solver.getCurrentDecisionLevel() );
    vector< Literal > literals;
    vector< uint64_t > weights;

    uint64_t bound = 1;
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
    {
        OptimizationLiteralData& optData = solver.getOptimizationLiteral( i );
        Literal lit = optData.lit;
        uint64_t weight = optData.weight;
         
        if( !solver.isUndefined( lit ) )
            continue;

        bound += weight;
        literals.push_back( lit );
        weights.push_back( weight );        
    }
   
    aggregate = createAndReturnFalseAggregate( literals, weights, bound );    
    assert( aggregate != NULL );    
}

bool
Opt::updateOptimizationAggregate(
    uint64_t modelCost )
{
    assert( aggregate != NULL );    
    trace_msg( weakconstraints, 2, "Precomputed cost is " << solver.getPrecomputedCost() );
    if( solver.getPrecomputedCost() >= modelCost )
        return false;
        
    unsigned int backjumpingLevel = 0; //optimizationAggregate->getLevelOfBackjump( *this, modelCost - precomputedCost );
    trace_msg( weakconstraints, 2, "Backjumping level is " << backjumpingLevel );
    if( solver.getCurrentDecisionLevel() != 0 )
        solver.unroll( backjumpingLevel );
    solver.clearConflictStatus();
        
    if( !aggregate->updateBound( solver, modelCost - solver.getPrecomputedCost() ) )
        return false;
    
    assert( solver.isFalse( aggregate->getLiteral( 1 ).getOppositeLiteral() ) );
    aggregate->onLiteralFalse( solver, aggregate->getLiteral( 1 ).getOppositeLiteral(), -1 );

    if( solver.conflictDetected() )
        return false;    
    
    return true;
}
