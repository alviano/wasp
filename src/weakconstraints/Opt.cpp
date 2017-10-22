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
    assumptions.clear();
    varId = addAuxVariable();
    assumptions.push_back( Literal( varId, NEGATIVE ) );
    trace_msg( weakconstraints, 1, "Starting algorithm " << ( disableprefchoices_ ? "BASIC" : "OPT" ) );
    solver.sortOptimizationLiterals( level() );
//    createOptimizationAggregate();

    if( !disableprefchoices_ )
        solver.addPreferredChoicesFromOptimizationLiterals( level() );
    numberOfModels = 0;
    ( wasp::Options::weakConstraintsAlg != BBBT ) ? basic() : basicBT();
    
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
        if( optData.isAux() )
            continue;
        literals.push_back( optData.lit );
        weights.push_back( optData.weight );        
    }
    
    Var aggrId = addAuxVariable();
    Clause* c = new Clause( 2 );
    assert( varId != 0 );
    c->addLiteral( Literal( varId, POSITIVE ) );
    c->addLiteral( Literal( aggrId, NEGATIVE ) );
    solver.addClauseRuntime( c );
    
    aggregate = createAggregate( aggrId, literals, weights );
    processAndAddAggregate( aggregate, modelCost );
    trace_msg( weakconstraints, 5, "...done! Aggregate: " << *aggregate );    
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
        trace_msg( weakconstraints, 3, "Optimization aggregate is null: need to be created" );
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

void
Opt::flipLatestChoice(
    vector< bool >& checked )
{
    unsigned int size;
    while( true )
    {
        size = assumptions.size();
        if( size == 1 )
            return;
        if( checked[ size ] )
            assumptions.pop_back();
        else
            break;
    }
    
    assumptions[ size - 1 ] = assumptions[ size - 1 ].getOppositeLiteral();    
    checked[ size ] = true;
    for( unsigned int i = size + 1; i < checked.size(); i++ )
        checked[ i ] = false;
}

bool
Opt::foundModel()
{
    numberOfModels++;
    uint64_t modelCost = solver.computeCostOfModel( level() );
    
    trace_msg( weakconstraints, 2, "Decision level of solver: " << solver.getCurrentDecisionLevel() );
    if( modelCost == 0 )
        return false;
    
    solver.getChoicesWithoutAssumptions( assumptions );
    if( !updateOptimizationAggregate( modelCost ) )
    {
        trace_msg( weakconstraints, 3, "Failed updating of optimization aggregate: return" );
        return false;
    }
    return true;
}

void
Opt::backjump()
{
    const Clause* core = solver.getUnsatCore();
    assert( core != NULL );
    if( core->size() == 0 )
        assumptions.resize( 1 );

    unsigned int bl = solver.getMaxLevelOfClause( core );
    if( bl == 0 )
    {
        unsigned int k = 1;
        for( unsigned int i = 1; i < assumptions.size(); i++ )
        {
            assumptions[ k ] = assumptions[ i ];
            if( solver.getDecisionLevel( assumptions[ i ] ) != 0 )
                k++;
        }
        assumptions.resize( k );
    }
    else
    {
        while( assumptions.size() > 1 && solver.getDecisionLevel( assumptions.back() ) > bl )
            assumptions.pop_back();
    }
}

void
Opt::basic()
{
    while( solver.solve( assumptions ) == COHERENT )
    {
        numberOfModels++;
        uint64_t modelCost = solver.computeCostOfModel( level() );
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
}

void
Opt::basicBT()
{
    vector< bool > checked;
    while( checked.size() <= solver.numberOfVariables() )
        checked.push_back( false );
    unsigned int result = solver.solve( assumptions );
    if( result == INCOHERENT )
        return;

    if( !foundModel() )
        return;

    flipLatestChoice( checked );
    if( assumptions.size() == 1 )
        return;

    solver.setComputeUnsatCores( true );
    begin:;
    solver.unrollToZero();
    solver.clearConflictStatus();
    result = solver.solve( assumptions );
    if( result == INCOHERENT )
        backjump();
    else if( !foundModel() )
        return;    

    flipLatestChoice( checked );
    if( assumptions.size() == 1 )
        return;
    goto begin;
}