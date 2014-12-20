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
    unsigned int minCost = UINT_MAX;
    unsigned int numberOfModels = 0;
    
    solver.sortOptimizationLiterals();
    while( solver.solve() == COHERENT )
    {
        numberOfModels++;
        unsigned int modelCost = solver.computeCostOfModel();
        if( modelCost < minCost )
        {
            minCost = modelCost;
            solver.printAnswerSet();        
            solver.printOptimizationValue( modelCost );
        }

        if( modelCost == 0 || solver.getCurrentDecisionLevel() == 0 )
            break;
        
        vector< Literal > literals;
        vector< unsigned int > weights;
        
        for( unsigned int i = 0; i < solver.numberOfOptimizationLiterals(); i++ )
        {
            Literal l = solver.getOptimizationLiteral( i ).lit;
            unsigned weight = solver.getOptimizationLiteral( i ).weight;
            assert( !solver.isUndefined( l ) );
            if( solver.getDecisionLevel( l ) == 0 )
            {
                if( solver.isTrue( l ) )
                    modelCost -= weight;

                continue;
            }

            if( !solver.getOptimizationLiteral( i ).isRemoved() )
            {
                if( solver.isTrue( l ) )
                {
                    literals.push_back( l );
                    weights.push_back( weight );
                    solver.getOptimizationLiteral( i ).remove();
                }
            }
            else
            {
                literals.push_back( l );
                weights.push_back( weight );                    
            }
        }
        
        if( !createFalseAggregate( literals, weights, modelCost ) )
            break;
    }

    if( numberOfModels > 0 )
        return OPTIMUM_FOUND;
    
    return INCOHERENT;
}
