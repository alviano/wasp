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

#include "K.h"

K::~K()
{
}

unsigned int
K::run()
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
K::runUnweighted()
{    
    trace_msg( weakconstraints, 1, "Starting algorithm K" );
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
K::runWeighted()
{    
    trace_msg( weakconstraints, 1, "Starting algorithm K" );
    
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


/*
 * Adapted from aspino: https://github.com/alviano/aspino/
 */
bool
K::processCoreK(
    uint64_t minWeight )
{
    trace_msg( weakconstraints, 2, "Processing core for algorithm K" );
    
    vector< Literal > literals;
    bool trivial = false;
    Clause* clause = new Clause();
    unsigned int originalSize = solver.numberOfOptimizationLiterals( level() );
    for( unsigned int i = 0; i < originalSize; i++ )
    {
        OptimizationLiteralData& optLitData = solver.getOptimizationLiteral( level(), i );        
        if( optLitData.isRemoved() )
            continue;

        Literal lit = optLitData.lit;
        Var v = lit.getVariable();
        if( visited( v ) )
        {
            trace_msg( weakconstraints, 3, lit << " is in unsat core" );
            literals.push_back( lit.getOppositeLiteral() );
            optLitData.remove();
            
            if( solver.isTrue( lit ) )
                trivial = true;
            
            if( !solver.isFalse( lit ) )
                clause->addLiteral( lit );
            
            if( optLitData.weight > minWeight )
                solver.addOptimizationLiteral( lit, optLitData.weight - minWeight, level(), true );            
        }
    }
    
    
    assert( solver.getCurrentDecisionLevel() == 0 );
       
    int b, m, N;
    initCounters( b, m, N, literals.size() );
    
    Var prec = 0;
    while( true )
    {
        assert( literals.size() > 0 );
        vector< Literal > ls;
        int i = N;
        if( prec != 0 )
        {
            ls.push_back( Literal( prec, POSITIVE ) );
            i--;
        }
        
        for( ; i > 0; i-- )
        {
            if( literals.size() == 0 )
                break;            
            ls.push_back( literals.back() );
            literals.pop_back();
        }
        assert( ls.size() > 0 );
        int bound_ = ls.size() - 1;
        
        if( literals.size() > 0 )
            bound_++;

        for( i = 0; i < bound_; i++ )
        {
            Var v = addAuxVariable();
            ls.push_back( Literal( v, NEGATIVE ) );
            
            //Simmetry breaker
            if( i != 0 )
                addImplication( Literal( solver.numberOfVariables() - 1, POSITIVE ), Literal( v, POSITIVE ) );                
            
            if( i == 0 && literals.size() > 0 )
                prec = v;
            else
                solver.addOptimizationLiteral( Literal( v, NEGATIVE ), minWeight, level(), true );            
        }
        
        if( !addCardinalityConstraintK( ls, bound_ ) )
            return false;
        
        if( literals.size() == 0 )
            break;
    }
    
    assert( literals.size() == 0 );    
    if( trivial )
    {
        delete clause;
        return true;
    }

    return solver.addClauseRuntime( clause );
}

bool
K::addCardinalityConstraintK(
    vector< Literal >& literals,
    uint64_t bound )
{
    if( literals.size() == 1 )
    {
        trace_msg( weakconstraints, 2, "Literal " << literals[ 0 ] << " is removed from assumptions. Nothing more to do" );
        return true;
    }
    
    assert( solver.getCurrentDecisionLevel() == 0 );
    CardinalityConstraint* cc = new CardinalityConstraint( literals );
    if( !cc->setBound( bound ) )
        return false;
    trace_msg( weakconstraints, 2, "Creating cardinality constraint " << *cc );
    if( !cc->checkConsistent( solver ) )
        return false;
    solver.attachCardinalityConstraint( *cc );
    solver.addCardinalityConstraint( cc );        
    return true;
}

bool
K::addAggregateK(
    vector< Literal >& literals,
    uint64_t bound )
{
    if( literals.size() == 1 )
    {
        trace_msg( weakconstraints, 2, "Literal " << literals[ 0 ] << " is removed from assumptions. Nothing more to do" );
        return true;
    }
    
    Var aggrId = addAuxVariable();
    Aggregate* aggregate = createAggregateCount( aggrId, literals );     
    trace_msg( weakconstraints, 2, "Adding aggregate from unsat core " << *aggregate );
    if( !processAndAddAggregate( aggregate, bound ) )
        return false;
    
    assert( !solver.isFalse( aggrId ) );
    solver.addClauseRuntime( Literal( aggrId, POSITIVE ) );
    assert( solver.isTrue( aggrId ) );
    return true;
}

bool
K::foundUnsat()
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
    
    uint64_t minWeight = computeMinWeight();
    
    if( !processCoreK( minWeight ) )
        return false;
    incrementLb( minWeight );
    solver.foundLowerBound( lb() ); 
    
    return true;
}

void
K::addImplication(
    Literal l1,
    Literal l2 )
{
    trace_msg( weakconstraints, 3, "Adding " << l1 << "->" << l2 );
    Clause* c = new Clause();
    c->addLiteral( l1.getOppositeLiteral() );
    c->addLiteral( l2 );
    solver.addClauseRuntime( c );
}

void
K::initCounters(
    int& b,
    int& m,
    int& N,
    unsigned int size )
{
    switch( kthreshold )
    {        
        case 0:
            //Algorithm kdyn
            b = size <= 2 ? 8 : ceil( log10( size ) * 16 );                        
        break;
        
        case 1:
            //Algorithm one
            b = INT_MAX;
            break;
    
        default:
            b = kthreshold * 2;
            break;
    }
    m = ceil( 2.0 * size / ( b - 2.0 ) );
    N = ceil( ( size + size - 1 + 2 * ( m - 1 ) ) / ( m * 2.0 ) );
}