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
    solver.sortOptimizationLiterals( level() );
//    createOptimizationAggregate();
    
    if( !disableprefchoices_ )
        solver.addPreferredChoicesFromOptimizationLiterals( level() );
    unsigned int numberOfModels = 0;
    while( solver.solve( assumptions ) == COHERENT )
    {
        numberOfModels++;
        uint64_t modelCost = solver.computeCostOfModel( level() );
        foundAnswerSet( modelCost );        
//        solver.printOptimizationValue( modelCost );
        trace_msg( weakconstraints, 2, "Decision level of solver: " << solver.getCurrentDecisionLevel() );
        if( modelCost == 0 || solver.getCurrentDecisionLevel() == 0 )
            break;
        
//        /*Implement a better policy of unroll*/
//        solver.doRestart();
//        solver.simplifyOnRestart();
//        solver.clearConflictStatus();
        
//        solver.removePrefChoices();        
        if( !updateOptimizationAggregate( modelCost ) )
        {
            trace_msg( weakconstraints, 3, "Failed updating of optimization aggregate: return" );
            break;        
        }
        
        trace_msg( weakconstraints, 2, "Calling solver..." );
    }        
    
    if( numberOfModels > 0 )
        return completedLevel() ? OPTIMUM_FOUND : OPTIMUM_FOUND_STOP;
    return INCOHERENT;
}

void
Opt::createOptimizationAggregate(
    uint64_t modelCost )
{
    assert_msg( aggregate == NULL, "Aggregate has been created" );
    assert_msg( solver.getCurrentDecisionLevel() == 0, "The decision level is " << solver.getCurrentDecisionLevel() );
    trace_msg( weakconstraints, 4, "Creating optimization aggregate..." );
    vector< Literal > literals;
    vector< uint64_t > weights;
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals( level() ); i++ )
    {
        OptimizationLiteralData& optData = solver.getOptimizationLiteral( level(), i );
        literals.push_back( optData.lit );
        weights.push_back( optData.weight );        
    }

    varId = addAuxVariable();
    Var aggrId = addAuxVariable();
    Clause* c = new Clause( 2 );
    c->addLiteral( Literal( varId, POSITIVE ) );
    c->addLiteral( Literal( aggrId, NEGATIVE ) );
    solver.addClauseRuntime( c );
    
    aggregate = createAggregate( aggrId, literals, weights );
    processAndAddAggregate( aggregate, modelCost );
    trace_msg( weakconstraints, 5, "...done! Aggregate: " << *aggregate );
    
    assumptions.push_back( Literal( varId, NEGATIVE ) );
}

bool
Opt::updateOptimizationAggregate(
    uint64_t modelCost )
{
    trace_msg( weakconstraints, 2, "Updating bound of optimization aggregate. Model cost: " << modelCost << " - Precomputed cost: " << solver.getPrecomputedCost( level() ) );
    if( solver.getPrecomputedCost( level() ) >= modelCost )
        return false;
    modelCost = modelCost - solver.getPrecomputedCost( level() );    
    resetSolver();        
    if( aggregate == NULL )
    {
        trace_msg( weakconstraints, 3, "Optimization aggregate has not been created" );
        createOptimizationAggregate( modelCost );
        return true;
    }    
        
    if( !aggregate->updateBound( solver, modelCost ) )
        return false;    
    return true;
}

bool
Opt::completedLevel()
{
    resetSolver();
    solver.removePrefChoices();
    aggregate = NULL;
    varId = 0;

    if( !createAggregateFromOptimizationLiterals() )
        return false;
    return true;
}