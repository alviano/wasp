/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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

#include "GlucoseDeletionStrategy.h"
#include "../Clause.h"
#include "../Solver.h"
#include "../util/Assert.h"

void
GlucoseDeletionStrategy::onNewVariable(
    Variable& )
{
    levelsVector.push_back( 0 );
}

void
GlucoseDeletionStrategy::onLearning( 
    LearnedClause* )
{    
    if( hasToDelete() )
    {
        ++countOfDeletion;
        deleteClauses();
    }
}

void
GlucoseDeletionStrategy::onUnitPropagation(
    Clause* clause )
{
    assert_msg( clause != NULL, "Error while computing LBD. Clause is NULL.");
    if( !clause->isLearned() || clause->isGlue() )
    {
        return;
    }
    assert_msg( clause->isLearned(), "Error while computing LBD. Clause " << *clause << " is not learned." );
    LearnedClause& learnedClause = *( static_cast< LearnedClause* >( clause ) );
    ++numberOfUnitPropagations;
    
    unsigned int lbd = 0;
    LearnedClause::ClauseIterator it = learnedClause.clause_begin();
    ++it;
    for( ; it != learnedClause.clause_end(); ++it )
    {
        const Variable* variable = it->getVariable();
        unsigned int decisionLevel = variable->getDecisionLevel();
        
        assert_msg( decisionLevel < levelsVector.size(), "Error while computing LBD. Decision level is " << decisionLevel << " and it must be less than the size of levelsVector which is " << levelsVector.size() );
        if( levelsVector[ decisionLevel ] != numberOfUnitPropagations )
        {
            ++lbd;
            levelsVector[ decisionLevel ] = numberOfUnitPropagations;
        }
    }    
    
    assert_msg( lbd > 0, "Error while computing LBD. Found value is equal to 0." );
    learnedClause.setLbdValue( lbd );
}

bool
GlucoseDeletionStrategy::hasToDelete() const
{
    return ( solver.numberOfLearnedClauses() > ( 20000 + 500 * countOfDeletion ) );
}

void
GlucoseDeletionStrategy::deleteClauses()
{
    toDelete = solver.numberOfLearnedClauses() / 2;
    Solver::LearnedClausesIterator it = solver.learnedClauses_begin();
    assert( it != solver.learnedClauses_end() );
    do
    {
        LearnedClause* clause = *it;
        
        if( !clause->isLocked() && !clause->isGlue() )
        {
            toDelete--;
            solver.deleteLearnedClause( it++ );          
        }        
        else
            ++it;
        
        if( toDelete == 0 )
            break;
    } while( it != solver.learnedClauses_end() );    
}