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

#include "OllBB.h"

unsigned int
OllBB::run()
{   
    trace_msg( weakconstraints, 1, "Starting algorithm OLLBB" );        
    
    originalNumberOfVariables = solver.numberOfVariables();
    
    solver.turnOffSimplifications();    
    initInUnsatCore();
    initHeuristicValues();
    unsigned int i = 0;
    while( true )
    {
        setAndUpdateHeuristicValues();
        unsigned int res;
        trace_msg( weakconstraints, 1, i << " iteration - number of choices: " << numberOfChoices  );
        if( i++ % 2 == 0 )
            res = oll();
        else
            res = bb();
        
        if( lb == ub || res == OPTIMUM_FOUND )
            return OPTIMUM_FOUND;        
        
        if( res == INCOHERENT )
            return numberOfModels == 0 ? INCOHERENT : OPTIMUM_FOUND;
    }    
}

unsigned int
OllBB::bb()
{
    trace_msg( weakconstraints, 1, "Starting BB" );
    if( solver.getCurrentDecisionLevel() != 0 && !solver.doRestart() )
        return OPTIMUM_FOUND;

    solver.setComputeUnsatCores( false );
    unsigned int res = solver.solve();
    while( res == COHERENT )
    {
        numberOfModels++;
        solver.printAnswerSet();
        ub = solver.computeCostOfModel(); 

        solver.printOptimizationValue( ub );
        trace_msg( weakconstraints, 2, "Decision level of solver: " << solver.getCurrentDecisionLevel() );
        if( ub == lb || ub == 0 || solver.getCurrentDecisionLevel() == 0 )
            break;
        
        trace_msg( weakconstraints, 2, "Updating bound of optimization aggregate. Model cost: " << ub );
        if( !solver.updateOptimizationAggregate( ub ) )
        {
            trace_msg( weakconstraints, 3, "Failed updating of optimization aggregate: return" );
            break;        
        }                
        
        trace_msg( weakconstraints, 2, "Calling solver..." );
        res = solver.solve();
    }

    if( res == INTERRUPTED )
        return res;
    return OPTIMUM_FOUND;
}

unsigned int
OllBB::oll()
{
    if( solver.getCurrentDecisionLevel() != 0 && !solver.doRestart() )
        return OPTIMUM_FOUND;

    computeAssumptionsAND();
    solver.setComputeUnsatCores( true );
    
    unsigned int res = solver.solve( assumptionsAND, assumptionsOR );    
    while( res == INCOHERENT )
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
        
        if( lb == ub )
            return OPTIMUM_FOUND;
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
        
        res = solver.solve( assumptionsAND, assumptionsOR );        
    }
        
    if( res == INTERRUPTED )
        return res;

    ub = solver.computeCostOfModel();
    assert_msg( lb == ub, lb << " != " << ub );
    solver.printAnswerSet();
    solver.printOptimizationValue( ub );
    numberOfModels++;
    
    return OPTIMUM_FOUND;    
}