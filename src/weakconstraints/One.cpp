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

#include "One.h"

One::~One()
{
}

unsigned int
One::run()
{
    if( disjCoresPreprocessing && !disjointCorePreprocessing() )
        return INCOHERENT;
    
    if( ub() == lb() )
        return OPTIMUM_FOUND;
    
    if( wasp::Options::stratification && solver.isWeighted( level() ) )
        return runWeighted();
    return runUnweighted();
}

unsigned int
One::runUnweighted()
{    
    trace_msg( weakconstraints, 1, "Starting algorithm ONE" );
    computeAssumptions();        
    
    initInUnsatCore();    

    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();
    while( solver.solve( assumptions ) == INCOHERENT )
    {        
        if( !foundUnsat() )
            return INCOHERENT;
        assumptions.clear();
        computeAssumptions();        
    }
        
    assert_msg( lb() == ub(), lb() << " != " << ub() );
    return OPTIMUM_FOUND;
}

unsigned int
One::runWeighted()
{    
    trace_msg( weakconstraints, 1, "Starting algorithm ONE" );
    
    preprocessingWeights();
    changeWeight();
    computeAssumptionsStratified();        
    
    initInUnsatCore();

    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();

    while( true )
    {
        if( solver.solve( assumptions ) != INCOHERENT )
        {
            solver.unrollToZero();
            solver.clearConflictStatus();
            if( !changeWeight() )
                break;
        }
        else if( !foundUnsat() )
            return INCOHERENT;
        
        assumptions.clear();
        computeAssumptionsStratified();
        if( lb() == ub() )
            break;
    }

    assert_msg( lb() == ub(), lb() << " != " << ub() );    
    
    return OPTIMUM_FOUND;
}

bool
One::processCoreOne(
    vector< Literal >& literals,
    vector< uint64_t >& weights,
    uint64_t minWeight,
    unsigned int& n )
{
    trace_msg( weakconstraints, 2, "Processing core for algorithm One" );
    
    bool trivial = false;
    Clause* clause = new Clause();
    unsigned int originalSize = solver.numberOfOptimizationLiterals( level() );
    for( unsigned int i = 0; i < originalSize; i++ )
    {
        OptimizationLiteralData& optLitData = solver.getOptimizationLiteral( level(), i );        
        if( optLitData.isRemoved() )
            continue;

        Literal lit = optLitData.lit;
        trace_msg( weakconstraints, 3, "Considering literal " << lit << " - weight " << optLitData.weight );
        Var v = lit.getVariable();
        if( visited( v ) )
        {
            trace_msg( weakconstraints, 4, "is in unsat core" );
            literals.push_back( lit );
//            weights.push_back( optLitData.weight );
            weights.push_back( 1 );
            optLitData.remove();
            
            if( solver.isTrue( lit ) )
                trivial = true;
            
            if( !solver.isFalse( lit ) )
                clause->addLiteral( lit );
            
            if( optLitData.weight > minWeight )
                solver.addOptimizationLiteral( lit, optLitData.weight - minWeight, level(), true );            
        }
    }
    
    n = literals.size();
    vector< Var > newVars;
    for( unsigned int i = 0; i < n - 1; i++ )
    {
        Var auxVar = addAuxVariable();
        literals.push_back( Literal( auxVar, POSITIVE ) );
        weights.push_back( 1 );
        solver.addOptimizationLiteral( Literal( auxVar, NEGATIVE ), minWeight, level(), true );
        newVars.push_back( auxVar );
    }
    
    for( int i = newVars.size() - 1; i >= 1; i-- )
    {
        Clause* c = new Clause();
        c->addLiteral( Literal( newVars[ i ], NEGATIVE ) );
        c->addLiteral( Literal( newVars[ i - 1 ], POSITIVE ) );
        solver.addClauseRuntime( c );        
    }
    
    if( trivial )
    {
        delete clause;
        return true;
    }

    return solver.addClauseRuntime( clause );
}

bool
One::addAggregateOne(
    vector< Literal >& literals,
    vector< uint64_t >& weights,
    uint64_t bound )
{
    if( literals.size() == 1 )
    {
        trace_msg( weakconstraints, 2, "Literal " << literals[ 0 ] << " is removed from assumptions. Nothing more to do" );
        return true;
    }
    
    trace_msg( weakconstraints, 2, "Adding aggregate from unsat core" );
    Var aggrId = addAuxVariable();    
    Aggregate* aggregate = createAggregate( aggrId, literals, weights );     
    if( !processAndAddAggregate( aggregate, bound ) )
        return false;
    
    assert( !solver.isTrue( aggrId ) );
    solver.addClauseRuntime( Literal( aggrId, NEGATIVE ) );
    assert( solver.isFalse( aggrId ) );
    return true;
}

bool
One::foundUnsat()
{
    ++numberOfCalls;
    
    const Clause* core = solver.getUnsatCore();
    assert( core != NULL );
    const Clause& unsatCore = *( core );
    
    //The incoherence does not depend on weak constraints
    if( unsatCore.size() == 0 )
        return false;    
        
    solver.clearConflictStatus();
    solver.unrollToZero();

    for( unsigned int i = 0; i < unsatCore.size(); i++ )
        visit( unsatCore[ i ].getVariable() );
    
    vector< Literal > literals;
    vector< uint64_t > weights;

    unsigned int n = 0;
    uint64_t minWeight = computeMinWeight();
    if( !processCoreOne( literals, weights, minWeight, n ) || !addAggregateOne( literals, weights, n + 1 ) )
        return false;
    incrementLb( minWeight );
    solver.foundLowerBound( lb() ); 
    
    return true;
}
