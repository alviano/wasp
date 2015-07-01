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

#include "Oll.h"

Oll::~Oll()
{
    for( unsigned int i = 0; i < elements.size(); i++ )
        delete elements[ i ];
}

unsigned int
Oll::run()
{
    if( disjCoresPreprocessing && !disjointCorePreprocessing() )
        return INCOHERENT;
    
    if( wasp::Options::stratification && solver.isWeighted( level() ) )
        return runWeighted();
    return runUnweighted();
}

unsigned int
Oll::runUnweighted()
{    
    statistics( &solver, disable() );    
    trace_msg( weakconstraints, 1, "Starting algorithm OLL" );
    computeAssumptions();        
    
    initInUnsatCore();    
    originalNumberOfVariables = solver.numberOfVariables();        

    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();
    while( solver.solve( assumptions ) == INCOHERENT )
    {        
        if( !foundUnsat() )
            return INCOHERENT;
        assumptions.clear();
        computeAssumptions();        
    }
        
    statistics( &solver, enable() );
    statistics( &solver, endSolving() );
    uint64_t cost = solver.computeCostOfModel( level() );
    foundAnswerSet( cost );
    assert_msg( lb() == ub(), lb() << " != " << ub() );
    return OPTIMUM_FOUND;
}

unsigned int
Oll::runWeighted()
{    
    statistics( &solver, disable() );
    trace_msg( weakconstraints, 1, "Starting algorithm OLL" );
    
    preprocessingWeights();
    changeWeight();
    computeAssumptionsStratified();        
    
    initInUnsatCore();    
    originalNumberOfVariables = solver.numberOfVariables();        

    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();

    while( true )
    {
        if( solver.solve( assumptions ) != INCOHERENT )
        {
            uint64_t cost = solver.computeCostOfModel( level() );
            foundAnswerSet( cost );
            solver.unrollToZero();
            solver.clearConflictStatus();
            if( !changeWeight() )
                break;
            assumptions.clear();
            computeAssumptionsStratified();            
        }
        else
        {
            if( !foundUnsat() )
                return INCOHERENT;
            assumptions.clear();
            computeAssumptionsStratified();
        }
        
        if( lb() == ub() )
            break;
    }

    statistics( &solver, enable() );
    statistics( &solver, endSolving() );    
    assert_msg( lb() == ub(), lb() << " != " << ub() );    
    
    return OPTIMUM_FOUND;
}

bool
Oll::processCoreOll(
    vector< Literal >& literals,
    vector< uint64_t >& weights,
    uint64_t minWeight )
{
    trace_msg( weakconstraints, 2, "Processing core for algorithm OLL" );
    
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
    
    if( trivial )
    {
        delete clause;
        return true;
    }

    return solver.addClauseRuntime( clause );
//    bool res;
//    if( clause->size() == 1 )
//    {
//        res = solver.addClause( clause->getAt( 0 ) );
//        delete clause;
//    }    
//    else if( clause->size() == 2 )
//    {
//        res = solver.addClause( clause->getAt( 0 ), clause->getAt( 1 ) );
//        delete clause;        
//    }
//    else
//    {
//        res = solver.addClause( clause );
//        if( res )
//            solver.attachClause( *clause );
//    }
//    
//    return res;
}

bool
Oll::processCoreOll(
    vector< Literal >& literals,
    vector< uint64_t >& weights,
    uint64_t minWeight,
    unsigned int& n )
{
    trace_msg( weakconstraints, 2, "Processing core for algorithm OLL" );
    
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
Oll::addAggregateOll(
    vector< Literal >& literals,
    vector< uint64_t >& weights,
    uint64_t bound,
    uint64_t /*weightOfOptimizationLiteral*/ )
{
    if( literals.size() == 1 )
    {
        trace_msg( weakconstraints, 2, "Literal " << literals[ 0 ] << " is removed from assumptions. Nothing more to do" );
        return true;
    }
        
//    if( literals.size() == bound )
//    {
//        trace_msg( weakconstraints, 2, "The size of aggregate is equal to the bound (" << literals.size() << ")" );
//        Literal relaxLiteral( addAuxVariable(), POSITIVE );
//        solver.addOptimizationLiteral( relaxLiteral, weightOfOptimizationLiteral, UINT_MAX, true );
//
//        bool trivial = false;
//        Clause* clause = solver.newClause( literals.size() + 1 );
//        for( unsigned int i = 0; i < literals.size(); i++ )
//        {
//            Literal l = literals[ i ].getOppositeLiteral();
//            if( solver.isUndefined( l ) )
//                clause->addLiteral( l );
//            else if( solver.isTrue( l ) )
//            {
//                trivial = true;
//                break;
//            }                
//        }
//        
//        vector< Literal > tmp;
//        literals.swap( tmp );
//        if( trivial )
//        {
//            solver.releaseClause( clause );
//            return true;
//        }        
//        clause->addLiteral( relaxLiteral );
//        trace_msg( weakconstraints, 3, "Adding clause " << *clause );
//        if( clause->size() == 1 )
//            return solver.addClause( clause->getAt( 0 ) );
//        else if( clause->size() == 2 )
//            return solver.addClause( clause->getAt( 0 ), clause->getAt( 1 ) );        
//
//        solver.attachClause( *clause );
//        return solver.addClause( clause );
//    }
    
    trace_msg( weakconstraints, 2, "Adding aggregate from unsat core" );
    Var aggrId = addAuxVariable();    
//    elements.push_back( NULL );
    Aggregate* aggregate = createAggregate( aggrId, literals, weights );     
    if( !processAndAddAggregate( aggregate, bound ) )
        return false;
    
    assert( !solver.isTrue( aggrId ) );
    solver.addClauseRuntime( Literal( aggrId, NEGATIVE ) );
    assert( solver.isFalse( aggrId ) );
//    if( solver.isFalse( aggrId ) )
//        return true;
    
//    Var guardId = addBinaryClauseForAggregateOll( aggrId, weightOfOptimizationLiteral );
//    Literal lit( aggrId, POSITIVE );
//    solver.addOptimizationLiteral( lit, weightOfOptimizationLiteral, UINT_MAX, true );
//    OllData* ollData = new OllData( bound, aggrId, weightOfOptimizationLiteral, literals, weights );
//    setOllData( aggrId, ollData );
    return true;
}

bool
Oll::foundUnsat()
{
    vector< unsigned int > auxVariablesInUnsatCore;
    ++numberOfCalls;
    assert( solver.getUnsatCore() != NULL );
    const Clause& unsatCore = *( solver.getUnsatCore() );
    
    //The incoherence does not depend on weak constraints
    if( unsatCore.size() == 0 )
        return false;    
        
    solver.clearConflictStatus();
    solver.unrollToZero();

    for( unsigned int i = 0; i < unsatCore.size(); i++ )
    {
        Var v = unsatCore[ i ].getVariable();
        visit( v );
        if( v > originalNumberOfVariables )
            auxVariablesInUnsatCore.push_back( v );
    }

    vector< Literal > literals;
    vector< uint64_t > weights;

    unsigned int n = 0;
    uint64_t minWeight = computeMinWeight();
    if( !processCoreOll( literals, weights, minWeight, n ) )
        return false;
    incrementLb( minWeight );
    solver.foundLowerBound( lb() );    
    if( !addAggregateOll( literals, weights, n + 1, minWeight ) )
        return false;        

//    for( unsigned int i = 0; i < auxVariablesInUnsatCore.size(); i++ )
//    {
//        Var guardId = auxVariablesInUnsatCore[ i ];
////            if( guardMap.find( guardId ) == guardMap.end() )
////                continue;
////            assert( guardMap.find( guardId ) != guardMap.end() );
//        assert( hasOllData( guardId ) );
//
//        OllData* ollData = getOllData( guardId ); //guardMap[ guardId ];
//        if( ollData->bound_ < ollData->literals_.size() )
//        {                
//            trace_msg( weakconstraints, 3, "Incrementing by one bound (" << ollData->bound_ <<  ") of aggregate " << guardId );
//            assert( ollData->literals_.size() > 0 );
//            if( !addAggregateOll( ollData->literals_, ollData->weights_, ollData->bound_ + 1, ollData->weight_ ) )
//                return false;
//        }
//    }
    
    return true;
}
