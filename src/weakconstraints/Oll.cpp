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

unsigned int
Oll::run()
{    
    unsigned int lb = 0;
    trace_msg( weakconstraints, 1, "Starting algorithm OLL" );    
    computeAssumptionsAND();        
    
    initInUnsatCore();    
    unsigned int originalNumberOfVariables = solver.numberOfVariables();
    
    unordered_map< Var, OllData* > guardMap;

    solver.setComputeUnsatCores( true );
    solver.turnOffSimplifications();
    while( solver.solve( assumptionsAND, assumptionsOR ) == INCOHERENT )
    {        
        vector< unsigned int > auxVariablesInUnsatCore;
        ++numberOfCalls;
        assert( solver.getUnsatCore() != NULL );
        const Clause& unsatCore = *( solver.getUnsatCore() );

        //The incoherence does not depend on weak constraints
        if( unsatCore.size() == 0 )
            return INCOHERENT;

        solver.clearConflictStatus();
        if( solver.getCurrentDecisionLevel() != 0 && !solver.doRestart() )
            return INCOHERENT;
        
        for( unsigned int i = 0; i < unsatCore.size(); i++ )
        {
            Var v = unsatCore[ i ].getVariable();
            visit( v );
            if( v > originalNumberOfVariables )
                auxVariablesInUnsatCore.push_back( v );
        }
        
        vector< Literal > literals;
        vector< unsigned int > weights;
        
        unsigned int minWeight = computeMinWeight();
        if( !processCoreOll( literals, weights, minWeight ) )
            return INCOHERENT;
        lb += minWeight;
        solver.foundLowerBound( lb );
        trace_msg( weakconstraints, 2, "Adding aggregate from unsat core" );
        if( !addAggregateOll( guardMap, literals, weights, 2, minWeight ) )
            return INCOHERENT;        
        
        for( unsigned int i = 0; i < auxVariablesInUnsatCore.size(); i++ )
        {
            Var guardId = auxVariablesInUnsatCore[ i ];
            assert( guardMap.find( guardId ) != guardMap.end() );
            
            OllData* ollData = guardMap[ guardId ];
            if( ollData->bound_ < ollData->literals_.size() )
            {
                trace_msg( weakconstraints, 3, "Incrementing by one bound (" << ollData->bound_ <<  ") of aggregate " << guardId );
                assert( ollData->literals_.size() > 0 );
                if( !addAggregateOll( guardMap, ollData->literals_, ollData->weights_, ollData->bound_ + 1, ollData->weight_ ) )
                    return INCOHERENT;
            }
        }
        
        assumptionsAND.clear();
        computeAssumptionsAND();
    }
        
    unsigned int ub = solver.computeCostOfModel();
    assert_msg( lb == ub, lb << " != " << ub );
    solver.printAnswerSet();
    solver.printOptimizationValue( ub );
    return OPTIMUM_FOUND;
}

bool
Oll::processCoreOll(
    vector< Literal >& literals,
    vector< unsigned int >& weights,
    unsigned int minWeight )
{
    trace_msg( weakconstraints, 2, "Processing core for algorithm OLL" );
    
    bool trivial = false;
    Clause* clause = new Clause();
    unsigned int originalSize = solver.numberOfOptimizationLiterals();
    for( unsigned int i = 0; i < originalSize; i++ )
    {
        OptimizationLiteralData& optLitData = solver.getOptimizationLiteral( i );        
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
                solver.addOptimizationLiteral( lit, optLitData.weight - minWeight, UINT_MAX, true );            
        }
    }
    
    if( trivial )
    {
        delete clause;
        return true;
    }

    bool res;
    if( clause->size() == 2 )
    {
        res = solver.addClause( clause->getAt( 0 ), clause->getAt( 1 ) );
        delete clause;        
    }
    else
    {
        res = solver.addClause( clause );
        if( res )
            solver.attachClause( *clause );
    }
    
    return res;
}

bool
Oll::addAggregateOll(
    unordered_map< Var, OllData* >& guardMap,    
    vector< Literal >& literals,
    vector< unsigned int >& weights,
    unsigned int bound,
    unsigned int weightOfOptimizationLiteral )
{
    Var aggrId = addAuxVariable();
    Aggregate* aggregate = createAggregate( aggrId, literals, weights );     
    if( !processAndAddAggregate( aggregate, bound ) )
        return false;
    
    if( solver.isFalse( aggrId ) )
        return true;
    
    Var guardId = addBinaryClauseForAggregateOll( aggrId, weightOfOptimizationLiteral );
    OllData* ollData = new OllData( bound, guardId, weightOfOptimizationLiteral, literals, weights );
    guardMap[ guardId ] = ollData;
    return true;
}