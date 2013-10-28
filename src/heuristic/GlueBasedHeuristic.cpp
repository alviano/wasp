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

#include "GlueBasedHeuristic.h"

#include "../util/Assert.h"

GlueBasedHeuristic::~GlueBasedHeuristic()
{
    if( decisionStrategy != NULL )
        delete decisionStrategy;
    if( restartStrategy != NULL )
        delete restartStrategy;
    if( deletionStrategy != NULL )
        delete deletionStrategy;
}

void
GlueBasedHeuristic::onUnitPropagation(
    Clause* clause )
{
    assert_msg( clause != NULL, "Error while computing LBD. Clause is NULL.");
    if( clause->getHeuristicData() == NULL )
        return;

    assert( clause->getAt( 0 ).getVariable()->getImplicant() == clause );
    
    ++numberOfUnitPropagations;
    
    unsigned int& lbd = getHeuristicData( *clause )->lbd;
    lbd = 0;
    for( unsigned i = 1; i < clause->size(); ++i )
    {
        const Variable* variable = clause->getAt( i ).getVariable();
        unsigned int decisionLevel = variable->getDecisionLevel();
        
        assert_msg( decisionLevel < levelsVector.size(), "Error while computing LBD. Decision level is " << decisionLevel << " and it must be less than the size of levelsVector which is " << levelsVector.size() );
        if( levelsVector[ decisionLevel ] != numberOfUnitPropagations )
        {
            ++lbd;
            levelsVector[ decisionLevel ] = numberOfUnitPropagations;
        }
    }    
    
    assert_msg( lbd > 0, "Error while computing LBD. Found value is equal to 0." );
}
