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

#include "ActivityBasedDeletionStrategy.h"
#include "../../Solver.h"

void
ActivityBasedDeletionStrategy::updateActivity( 
    Clause* learnedClause )
{
    learnedClause->incrementActivity( increment );
    decrementActivity();
    if( learnedClause->getActivity() > 1e20 )
    {
        solver.decreaseLearnedClausesActivity();
        increment *= 1e-20;
    }
}

void
ActivityBasedDeletionStrategy::deleteClauses()
{
    startIteration();
    Solver::ClauseIterator it = solver.learnedClauses_begin();
    assert( it != solver.learnedClauses_end() );
    do
    {
        Clause* clause = *it;
        
        if( !clause->isImplicantOfALiteral() )
        {
            Activity activity = clause->getActivity();
            activitySum += activity;
            ++activityCount;
            if ( activity < threshold )
            {
                toDelete--;
                solver.deleteLearnedClause( it++ );            
            }
            else
                ++it;
        }
        else
            ++it;
    }while( it != solver.learnedClauses_end() );
    
    
    if( activityCount > 0 && toDelete > 0 )
    {
        activitySum = activitySum / activityCount;
        it = solver.learnedClauses_begin();
        assert( it != solver.learnedClauses_end() );
        do
        {
            Clause* clause = *it;

    
            if( !clause->isImplicantOfALiteral() && clause->getActivity() < activitySum )
            {
                toDelete--;
                solver.deleteLearnedClause( it++ );            
            }
            else
                ++it;
        }while( it != solver.learnedClauses_end() && toDelete > 0 );
    }
}

void
ActivityBasedDeletionStrategy::startIteration()
{
    activitySum = 0;
    activityCount = 0;
    threshold = increment / solver.numberOfLearnedClauses();        
    toDelete = solver.numberOfLearnedClauses() / 2;
}