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

#include "GlueBasedDeletionStrategy.h"

void
GlueBasedDeletionStrategy::deleteClauses()
{
    unsigned int halfSize = solver.numberOfLearnedClauses() / 2;
    Solver::ClauseIterator i = solver.learnedClauses_begin();
    Solver::ClauseIterator j = solver.learnedClauses_begin();
    
    unsigned int size = solver.numberOfLearnedClauses();
    unsigned int numberOfDeletions = 0;
    assert( i != solver.learnedClauses_end() );
    do
    {
        if( numberOfDeletions < halfSize )
        {
            Clause* clause = *i;

            if( !clause->isLocked() && getHeuristicData( *clause )->lbd > thresholdGlueClauses )
            {
                numberOfDeletions++;
                solver.deleteLearnedClause( i );          
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
}

void
GlueBasedDeletionStrategy::onLearning( 
    Clause* )
{
    if( solver.numberOfLearnedClauses() > ( 20000 + 500 * countOfDeletion ) )
    {
        ++countOfDeletion;
        deleteClauses();
    }
}
