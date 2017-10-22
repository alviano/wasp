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

#include "WeakInterface.h"

bool
WeakInterface::createFalseAggregate(
    const vector< Literal >& literals,
    const vector< uint64_t >& weights,
    uint64_t bound )
{
    assert( literals.size() == weights.size() );
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );

    solver.addVariableRuntime();    
    Var aggregateVar = solver.numberOfVariables();
    Literal aggregateLit( aggregateVar, POSITIVE );    
    Aggregate* aggregate = createAggregate( aggregateVar, literals, weights );
    trace_msg( weakconstraints, 4, "Created aggregate " << *aggregate << " - with bound " << bound );
    assert( aggregate->size() >= 1 );
    #ifndef NDEBUG
    bool res =
    #endif
    solver.addClauseRuntime( aggregateLit.getOppositeLiteral() );
    assert( res );
        
    //aggregate is already false: return
    if( !aggregate->updateBound( solver, bound ) )
        return true;
    
    solver.attachAggregate( *aggregate );
    solver.addAggregate( aggregate );
    
    assert( solver.isFalse( aggregateLit ) );
    aggregate->onLiteralFalse( solver, aggregateLit, -1 );
    
    if( solver.conflictDetected() )
        return false;
    
    return true;
}

Aggregate*
WeakInterface::createAndReturnFalseAggregate(
    const vector< Literal >& literals,
    const vector< uint64_t >& weights,
    uint64_t bound )
{
    assert( literals.size() == weights.size() );
    solver.unrollToZero();
    
    solver.addVariableRuntime();    
    Var aggregateVar = solver.numberOfVariables();
    Literal aggregateLit( aggregateVar, POSITIVE );    
    Aggregate* aggregate = createAggregate( aggregateVar, literals, weights );   
    
    assert( aggregate->size() >= 1 );
    #ifndef NDEBUG
    bool res =
    #endif
    solver.addClauseRuntime( aggregateLit.getOppositeLiteral() );
    assert( res );
    aggregate->updateBound( solver, bound );    
    solver.attachAggregate( *aggregate );
    solver.addAggregate( aggregate );
    
    assert( solver.isFalse( aggregateLit ) );
    aggregate->onLiteralFalse( solver, aggregateLit, -1 );
    
    if( solver.conflictDetected() )
        return NULL;
    
    return aggregate;
}

Aggregate*
WeakInterface::createAggregate(
    Var aggrId,
    const vector< Literal >& literals,
    const vector< uint64_t >& weights )
{
    assert( literals.size() == weights.size() );

    Aggregate* aggregate = new Aggregate();
    aggregate->addLiteral( Literal( aggrId, NEGATIVE ), 0 );

    for( unsigned int i = 0; i < literals.size(); i++ )
        aggregate->addLiteral( literals[ i ], weights[ i ] );

    return aggregate;
}

Aggregate*
WeakInterface::createAggregateCount(
    Var aggrId,
    const vector< Literal >& literals )
{
    Aggregate* aggregate = new Aggregate();
    aggregate->addLiteral( Literal( aggrId, NEGATIVE ), 0 );

    for( unsigned int i = 0; i < literals.size(); i++ )
        aggregate->addLiteral( literals[ i ], 1 );

    return aggregate;
}

bool
WeakInterface::processAndAddAggregate(
    Aggregate* aggregate,
    uint64_t bound )
{    
    trace_msg( weakconstraints, 4, "Trying to add the aggregate " << *aggregate << " - with bound " << bound );

    #ifndef NDEBUG
    bool res = true;
    #endif
    if( !aggregate->updateBound( solver, bound ) )
    {
        trace_msg( weakconstraints, 5, "Aggregate is already false" );
        #ifndef NDEBUG
        res =
        #endif
        solver.addClauseRuntime( aggregate->getLiteral( 1 ) );
        assert( solver.isFalse( aggregate->getLiteral( 1 ).getVariable() ) );
    }
    else
    {        
        if( aggregate->isTrue() )
        {
            trace_msg( weakconstraints, 5, "Aggregate is true" );
            #ifndef NDEBUG
            res =
            #endif                    
            solver.addClauseRuntime( aggregate->getLiteral( 1 ).getOppositeLiteral() );
        }
        solver.attachAggregate( *aggregate );
    }

    assert( res );
    solver.addAggregate( aggregate );
    
    trace_msg( weakconstraints, 5, "Adding aggregate " << *aggregate );
    assert( !solver.conflictDetected() );
    return true;
}

bool
WeakInterface::createAggregateFromOptimizationLiterals()
{
    assert( solver.getCurrentDecisionLevel() == 0 );
    assert( !solver.conflictDetected() );
    trace_msg( weakconstraints, 3, "Creating aggregate from optimization literals for level " << level() );
    solver.sortOptimizationLiterals( level() );
    vector< Literal > literals;
    vector< uint64_t > weights;
    assert( ub() >= solver.getPrecomputedCost( level() ) );
    uint64_t cost = ( ub() - solver.getPrecomputedCost( level() ) ) + 1;
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals( level() ); i++ )
    {
        OptimizationLiteralData& opt = solver.getOptimizationLiteral( level(), i );
        if( mixedApproach && opt.isRemoved() )
            continue;
        literals.push_back( opt.lit );
        weights.push_back( opt.weight );
    }

    return createFalseAggregate( literals, weights, cost );    
}

uint64_t
WeakInterface::computeMinWeight()
{
    uint64_t minWeight = UINT64_MAX;
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals( level() ); i++ )
    {
        OptimizationLiteralData& optLitData = solver.getOptimizationLiteral( level(), i );        
        if( optLitData.isRemoved() )
            continue;
        if( visited( optLitData.lit.getVariable() ) && minWeight > optLitData.weight )
            minWeight = optLitData.weight;
    }
    
    assert( minWeight != UINT64_MAX );
    trace_msg( weakconstraints, 2, "Min weight " << minWeight );
    return minWeight;
}

bool
WeakInterface::disjointCorePreprocessing()
{
    computeAssumptions();
    initInUnsatCore();
    
    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();

    unsigned int originalNumberOfOptLiterals = solver.numberOfOptimizationLiterals( level() );
    while( true )
    {
        unsigned int result = solver.solve( assumptions );
        if( result != INCOHERENT )
        {
            solver.clearConflictStatus();
            solver.unrollToZero();
            assumptions.clear();            
            break;
        }

        if( !foundUnsat() )
            return false;
        
        assumptions.clear();
        computeAssumptionsOnlyOriginal( originalNumberOfOptLiterals );
    }        
    
    return true;
}

unsigned int
WeakInterface::solve()
{   
    if( !wasp::Options::useLazyWeakConstraints )
        solver.attachAnswerSetListener( utils );
        
    if( wasp::Options::computeFirstModel )
    {
        solver.setMaxNumberOfSeconds( wasp::Options::budget );
        unsigned int result = solver.solve();
        solver.setMaxNumberOfSeconds( UINT_MAX );
        if( result == INCOHERENT )
            return result;        
        solver.unrollToZero();
        solver.clearConflictStatus();
    }
    
    unsigned int res = OPTIMUM_FOUND;
    for( int i = solver.numberOfLevels() - 1; i >= 0; i-- )
    {
        utils->setLevel( i );
        utils->setLowerBound( solver.simplifyOptimizationLiterals( level() ) );
        utils->setUpperBound( UINT64_MAX );
        trace_msg( weakconstraints, 1, "Solving level " << level() << ": lb=" << lb() << ", ub=" << ub() );                
        
        res = run();
        if( res == INCOHERENT )
            return res;
        else if( res == OPTIMUM_FOUND_STOP )
            return OPTIMUM_FOUND;
        
        solver.unrollToZero();
        solver.clearConflictStatus();
        clearAssumptions();
        if( !hardening() )
            return res;
    }
    
    solver.removeAnswerSetListener( utils );
    return res;
}

bool
WeakInterface::hardening()
{
    trace_msg( weakconstraints, 2, "Starting hardening: lb=" << lb() << ", ub=" << ub() );
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals( level() ); i++ )
    {
        OptimizationLiteralData& opt = solver.getOptimizationLiteral( level(), i );
        if( opt.removed )
            continue;

        trace_msg( weakconstraints, 3, "Considering literal " << opt.lit << " - Weight: " << opt.weight );
        if( lb() + opt.weight > ub() )
        {
            if( !solver.addClauseRuntime( opt.lit.getOppositeLiteral() ) )
                return false;
        }
        
        assert( !solver.conflictDetected() );
    }
    
    return true;
}

void
WeakInterface::resetSolver()
{
    solver.unrollToZero();
    solver.clearConflictStatus();
}
