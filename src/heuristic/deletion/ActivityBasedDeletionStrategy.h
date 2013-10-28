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
 
#ifndef ACTIVITYBASEDDELETIONSTRATEGY_H
#define ACTIVITYBASEDDELETIONSTRATEGY_H

#include "../../util/Constants.h"
#include "DeletionStrategy.h"
#include "../ActivityBasedHeuristic.h"
#include "../UseClauseData.h"

class Solver;

class ActivityBasedDeletionStrategy : public DeletionStrategy, public UseClauseData< ActivityBasedHeuristic >
{
    public:
        inline ActivityBasedDeletionStrategy( Solver& solver );
        inline virtual ~ActivityBasedDeletionStrategy() {}
        
        virtual void onLearning( Clause* clause ) = 0;
        virtual void onRestart() = 0;
        
    protected:
        inline void decrementActivity();
        
        void updateActivity( Clause* learnedClause );
        void deleteClauses();

        Solver& solver;
    
    private:
        void startIteration();

        Activity increment;
        Activity decrement;
        
        Activity activitySum;
        unsigned int activityCount;
        
        unsigned threshold;
        int toDelete;
};

ActivityBasedDeletionStrategy::ActivityBasedDeletionStrategy(
    Solver& s ) : solver( s ), increment( 1 ), decrement( 1/0.999 )
{
}

void
ActivityBasedDeletionStrategy::decrementActivity()
{
    increment *= decrement;
}

#endif
