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
    const vector< unsigned int >& weights,
    unsigned int bound )
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
    if( !aggregate->updateBound( solver, bound ) )
        return false;    

    solver.attachAggregate( *aggregate );
    solver.addAggregate( aggregate );
    
    assert( solver.isFalse( aggregateLit ) );
    aggregate->onLiteralFalse( solver, aggregateLit, -1 );
    
    if( solver.conflictDetected() )
        return false;
    
    return aggregate;
}

Aggregate*
WeakInterface::createAndReturnFalseAggregate(
    const vector< Literal >& literals,
    const vector< unsigned int >& weights,
    unsigned int bound )
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
    const vector< unsigned int >& weights )
{
    assert( literals.size() == weights.size() );

    Aggregate* aggregate = new Aggregate();
    aggregate->addLiteral( Literal( aggrId, NEGATIVE ), 0 );

    for( unsigned int i = 0; i < literals.size(); i++ )
        aggregate->addLiteral( literals[ i ], weights[ i ] );

    return aggregate;
}

bool
WeakInterface::processAndAddAggregate(
    Aggregate* aggregate,
    unsigned int bound )
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

unsigned int
WeakInterface::computeMinWeight()
{
    unsigned int minWeight = UINT_MAX;
    for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
    {
        OptimizationLiteralData& optLitData = solver.getOptimizationLiteral( i );        
        if( optLitData.isRemoved() )
            continue;
        if( visited( optLitData.lit.getVariable() ) && minWeight > optLitData.weight )
            minWeight = optLitData.weight;
    }
    
    assert( minWeight != UINT_MAX );
    trace_msg( weakconstraints, 2, "Min weight " << minWeight );
    return minWeight;
}

bool
WeakInterface::disjointCorePreprocessing()
{
    computeAssumptionsAND();
    initInUnsatCore();
    
    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();

    unsigned int originalNumberOfOptLiterals = solver.numberOfOptimizationLiterals();
    while( true )
    {
        if( solver.solve( assumptions ) != INCOHERENT )
        {
            solver.clearConflictStatus();
            solver.unrollToZero();
            assumptions.clear();
            break;
        }

        if( !foundUnsat() )
            return false;
        
        assumptions.clear();
        computeAssumptionsANDOnlyOriginal( originalNumberOfOptLiterals );
    }        
    
    return true;
}