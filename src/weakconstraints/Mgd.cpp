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

#include "Mgd.h"

unsigned int
Mgd::run()
{
    unsigned int numberOfModels = 0;    

    solver.sortOptimizationLiterals( level() );
    while( solver.solve( assumptions ) == COHERENT )
    {
        numberOfModels++;
        
        assert( ub() >= solver.getPrecomputedCost( level() ) );
        uint64_t modelCost = ub() - solver.getPrecomputedCost( level() );
        if( modelCost == 0 || solver.getCurrentDecisionLevel() == 0 )
            break;

        vector< Literal > literals;
        vector< uint64_t > weights;
        for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals( level() ); i++ )
        {
            Literal l = solver.getOptimizationLiteral( level(), i ).lit;
            uint64_t weight = solver.getOptimizationLiteral( level(), i ).weight;
            assert( !solver.isUndefined( l ) );
            if( solver.getDecisionLevel( l ) == 0 )
            {
                if( solver.isTrue( l ) )
                    modelCost -= weight;

                continue;
            }

            if( solver.getOptimizationLiteral( level(), i ).isRemoved() || solver.isTrue( l ) )
            {
                literals.push_back( l );
                weights.push_back( weight );
                solver.getOptimizationLiteral( level(), i ).remove();
            }
        }
        
        resetSolver();
        Var aggrId = addAuxVariable();
        Aggregate* aggregate = createAggregate( aggrId, literals, weights );
        if( !processAndAddAggregate( aggregate, modelCost ) )
            break;
        assumptions.push_back( Literal( aggrId, NEGATIVE ) );
//        if( !createFalseAggregate( literals, weights, modelCost ) )
//            break;
    }

    resetSolver();
    if( numberOfModels > 0 )
    {
        #ifndef NDEBUG
        bool result =
        #endif
        createAggregateFromOptimizationLiterals();
        assert( result );
        return OPTIMUM_FOUND;
    }
    
    return INCOHERENT;
}
