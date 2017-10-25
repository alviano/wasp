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

#include "PMRes.h"

unsigned int
PMRes::run()
{
    if( disjCoresPreprocessing && !disjointCorePreprocessing() )
        return INCOHERENT;
    if( wasp::Options::stratification && solver.isWeighted( level() ) )
        return runWeighted();
    return runUnweighted();
}

unsigned int
PMRes::runUnweighted()
{
    trace_msg( weakconstraints, 1, "Starting algorithm PMRes" );
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
PMRes::runWeighted()
{
    trace_msg( weakconstraints, 1, "Starting algorithm PMRes" );    
    
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

    assert_msg( lb() == ub(), lb() << " != " << ub() );    
    
    return OPTIMUM_FOUND;
}


bool
PMRes::addAuxClauses(
    vector< Literal >& optLiterals )
{
    assert( !optLiterals.empty() );
    trace_msg( weakconstraints, 2, "Adding aux clauses for " << optLiterals.size() << " elements" );
    for( unsigned int i = 0; i < optLiterals.size() - 1; i++ )
    {
        Literal lit = optLiterals[ i ];
        assert_msg( solver.isUndefined( lit ), "Lit " << lit << " is " << ( solver.isTrue( lit ) ? "true" : "false" ) );
        trace_msg( weakconstraints, 3, "Considering literal " << lit );
                
        Var aux = addAuxVariable();
        Clause* clause = new Clause();
        clause->addLiteral( lit );
        clause->addLiteral( Literal( aux, NEGATIVE ) );
        Var relaxVar = relaxClause( clause );

        solver.addOptimizationLiteral( Literal( relaxVar, POSITIVE ), 1, level(), true );
        trace_msg( weakconstraints, 4, "Adding clause " << *clause );
        if( !addClauseToSolver( clause ) )
            return false;

        Clause* auxClause = new Clause();
        auxClause->addLiteral( Literal( aux, NEGATIVE ) );
        for( unsigned int j = i + 1; j < optLiterals.size(); j++ )
        {
            auxClause->addLiteral( optLiterals[ j ].getOppositeLiteral() );
            Clause* c1 = new Clause( 2 );
            c1->addLiteral( Literal( aux, POSITIVE ) );
            c1->addLiteral( optLiterals[ j ] );
            trace_msg( weakconstraints, 4, "Adding clause " << *c1 );
            if( !addClauseToSolver( c1 ) )
            {
                delete auxClause;
                return false;
            }
        }
        
        trace_msg( weakconstraints, 4, "Adding clause " << *auxClause );
        if( !addClauseToSolver( auxClause ) )
            return false;
    }        

    return true;
}

bool
PMRes::foundUnsat()
{
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
    }        
    uint64_t minWeight = computeMinWeight();
    incrementLb( minWeight );
    solver.foundLowerBound( lb() );
    vector< Literal > optLiterals;

    bool trivial = false;
    Clause* clause = new Clause();
    trace_msg( weakconstraints, 1, "Computing hard clause " );
    unsigned int originalSize = solver.numberOfOptimizationLiterals( level() );

    uint64_t atLevelZeroWeight = 0;
    for( unsigned int i = 0; i < originalSize; i++ )
    {
        OptimizationLiteralData& data = solver.getOptimizationLiteral( level(), i );
        Literal lit = data.lit;
        if( data.isRemoved() || !visited( lit.getVariable() ) )
            continue;

        trace_msg( weakconstraints, 2, "Considering literal " << lit );
        trace_msg( weakconstraints, 3, "which is " << ( visited( lit.getVariable() ) ? "in the core" : "not in the core: skipped" ) );
        trace_msg( weakconstraints, 3, "its decision level is " << solver.getDecisionLevel( lit ) );
        assert_msg( !solver.isFalse( lit ), "Lit " << lit << " is false" );
        trace_msg( weakconstraints, 3, "and it is " << ( solver.isUndefined( lit ) ? "undefined" : "true" ) );
        trace_msg( weakconstraints, 3, "its weight is " << data.weight );

        if( solver.isTrue( lit ) )
            trivial = true;

        data.remove();
        if( solver.getDecisionLevel( lit ) > 0 )
        {
            clause->addLiteral( lit );
            optLiterals.push_back( lit.getOppositeLiteral() );

            if( data.weight > minWeight )
                solver.addOptimizationLiteral( lit, data.weight - minWeight, level(), true );
        }
        else
        {
            atLevelZeroWeight += data.weight;
        }
    }        

    if( clause->size() != 0 )
    {
        if( !trivial )
        {
            trace_msg( weakconstraints, 1, "Adding clause " << *clause );            
            addClauseToSolver( clause );
        }
        #ifndef NDEBUG
        bool res =
        #endif
        addAuxClausesCompressed( optLiterals, minWeight );
        assert( res );
    }
    else
    {
        trace_msg( weakconstraints, 1, "Derived empty clause" );
        if( atLevelZeroWeight > minWeight )
            incrementLb( atLevelZeroWeight - minWeight );
    }       
    return true;
}

bool
PMRes::addAuxClausesCompressed(
    vector< Literal >& optLiterals,
    uint64_t minWeight )
{
    assert( !optLiterals.empty() );
    vector< Literal > auxLits;
    auxLits.reserve( optLiterals.size() );

    auxLits.push_back( Literal::null );
    for( unsigned int i = 1; i < optLiterals.size() - 1; i++ )
        auxLits.push_back( Literal( addAuxVariable(), POSITIVE ) );    
    auxLits.push_back( optLiterals[ optLiterals.size() - 1 ].getOppositeLiteral() );    
    
    assert( !optLiterals.empty() );
    trace_msg( weakconstraints, 2, "Adding aux clauses for " << optLiterals.size() << " elements" );
    for( unsigned int i = 0; i < optLiterals.size() - 1; i++ )
    {
        Literal lit = optLiterals[ i ];
        assert_msg( solver.isUndefined( lit ), "Lit " << lit << " is " << ( solver.isTrue( lit ) ? "true" : "false" ) );
        trace_msg( weakconstraints, 3, "Considering literal " << lit );
        
        Clause* clause = new Clause();
        clause->addLiteral( lit );
        clause->addLiteral( auxLits[ i + 1 ].getOppositeLiteral() );
        Var relaxVar = relaxClause( clause );

        solver.addOptimizationLiteral( Literal( relaxVar, POSITIVE ), minWeight, level(), true );
        trace_msg( weakconstraints, 4, "Adding clause1 " << *clause );
        if( !addClauseToSolver( clause ) )
            return false;

        if( i + 2 < optLiterals.size() )
        {
            Clause* auxClause = new Clause();
            auxClause->addLiteral( auxLits[ i + 1 ].getOppositeLiteral() );
            auxClause->addLiteral( optLiterals[ i + 1 ].getOppositeLiteral() );
            auxClause->addLiteral( auxLits[ i + 2 ] );
                
            Clause* c1 = new Clause( 2 );
            c1->addLiteral( auxLits[ i + 1 ] );
            c1->addLiteral( optLiterals[ i + 1 ] );
            trace_msg( weakconstraints, 4, "Adding clause2 " << *c1 );
            if( !addClauseToSolver( c1 ) )
            {
                delete auxClause;
                return false;
            }        
        
            Clause* c2 = new Clause( 2 );
            c2->addLiteral( auxLits[ i + 1 ] );
            c2->addLiteral( auxLits[ i + 2 ].getOppositeLiteral() );
            trace_msg( weakconstraints, 4, "Adding clause3 " << *c2 );
            if( !addClauseToSolver( c2 ) )
            {
                delete auxClause;
                return false;
            }
            trace_msg( weakconstraints, 4, "Adding clause4 " << *auxClause );
            if( !addClauseToSolver( auxClause ) )
                return false;
        }        
    }        

    return true;
}

bool
PMRes::addClauseToSolver(
    Clause* clause )
{
    return solver.addClauseRuntime( clause );
}

Var
PMRes::relaxClause(
    Clause* clausePointer )
{
    Clause& clause = *clausePointer;
    Var aux = addAuxVariable();
    
    clause.addLiteral( Literal( aux, POSITIVE ) );    
    return aux;
}