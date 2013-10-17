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

/* 
 * File:   DeletionStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 8 August 2013, 15.25
 */

#ifndef DELETIONSTRATEGY_H
#define	DELETIONSTRATEGY_H

#include "../../Constants.h"

class Solver;
class LearnedClause;

class DeletionStrategy
{
    public:
        inline DeletionStrategy();
        inline virtual ~DeletionStrategy() {}
        inline bool continueIterationAvg();        
        bool hasToDeleteClauseThreshold( LearnedClause* clause );
        bool hasToDeleteClauseAvg( LearnedClause* clause );
        void onLearning( Solver& solver, LearnedClause* clause );
        virtual void onRestarting(){}
        
    protected:
        inline void decrementActivity();
        virtual bool onLearningProtected( Solver& solver ) = 0;        
        
    private:
        void startIteration( Solver& solver );
        Activity increment;
        Activity decrement;
        
        Activity activitySum;
        unsigned int activityCount;
        
        unsigned threshold;
        int toDelete;
};

DeletionStrategy::DeletionStrategy() : increment( 1 ), decrement( 1/0.999 )
{
}

bool
DeletionStrategy::continueIterationAvg()
{
    return toDelete > 0;
}

void
DeletionStrategy::decrementActivity()
{
    increment *= decrement;
}

#endif	/* DELETIONSTRATEGY_H */

