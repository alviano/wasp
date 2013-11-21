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
    ActivityBasedHeuristic::ClauseData& data = *getHeuristicData( *learnedClause );
    if( ( data.activity += increment ) > 1e20 )
    {
        for( Solver::ClauseIterator it = solver.learnedClauses_begin(); it != solver.learnedClauses_end(); ++it )
        {
            getHeuristicData( **it )->activity *= 1e-20;
        }

        increment *= 1e-20;
    }
}

void
ActivityBasedDeletionStrategy::deleteClauses()
{
    startIteration();
    Solver::ClauseIterator i = solver.learnedClauses_begin();
    Solver::ClauseIterator j = solver.learnedClauses_begin();
    
    unsigned int size = solver.numberOfLearnedClauses();
    unsigned int halfSize = size / 2;    
    unsigned int numberOfDeletions = 0;

    assert( i != solver.learnedClauses_end() );
    
    do
    {
//        Clause& clause = **it;
        Clause& clause = **i;
        
        if( !clause.isLocked() )
        {
            ActivityBasedHeuristic::ClauseData& data = *getHeuristicData( clause );
            activitySum += data.activity;
            ++activityCount;
            if ( data.activity < threshold )
            {
//                toDelete--;
                solver.deleteLearnedClause( i );
                numberOfDeletions++;
            }
            else
            {
                *j = *i;
                j++;
            }
        }
        else
        {
            *j = *i;
            j++;
        }

        ++i;
    } while( i != solver.learnedClauses_end() );
    
    solver.finalizeDeletion( size - numberOfDeletions );
    
    numberOfDeletions = 0;
    size = solver.numberOfLearnedClauses();

    if( activityCount > 0 && numberOfDeletions < halfSize )
    {
        activitySum = activitySum / activityCount;
        i = solver.learnedClauses_begin();
        j = solver.learnedClauses_begin();
        assert( i != solver.learnedClauses_end() );
        do
        {
            if( numberOfDeletions < halfSize )
            {
                Clause& clause = **i;

                if( !clause.isLocked() && getHeuristicData( clause )->activity < activitySum )
                {
    //                toDelete--;
                    solver.deleteLearnedClause( i );                
                    numberOfDeletions++;
                }
                else
                {
                    *j = *i;
                    j++;
                }
            }
            else
            {
                *j = *i;
                j++;
            }
            ++i;
        } while( i != solver.learnedClauses_end() );//&& toDelete > 0 );
    }
    
    solver.finalizeDeletion( size - numberOfDeletions );
}

void
ActivityBasedDeletionStrategy::startIteration()
{
    activitySum = 0;
    activityCount = 0;
    threshold = increment / solver.numberOfLearnedClauses();        
    //toDelete = solver.numberOfLearnedClauses() / 2;
}