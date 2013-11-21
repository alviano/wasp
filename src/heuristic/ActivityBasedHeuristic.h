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

#ifndef ACTIVITYBASEDHEURISTIC_H
#define ACTIVITYBASEDHEURISTIC_H

#include "../Heuristic.h"
#include "decision/DecisionStrategy.h"
#include "deletion/DeletionStrategy.h"
#include "restart/RestartStrategy.h"
#include "../Clause.h"

class ActivityBasedHeuristic : public Heuristic
{
public:
    struct ClauseData : public Heuristic::ClauseData
    {
        inline ClauseData() { activity = 0.0; }
        inline ClauseData( const ClauseData& init ) : activity( init.activity ) {}
        double activity;
        
    private:
        ClauseData& operator=( const ClauseData& right ) { assert( 0 ); activity = right.activity; return *this; }
    };

    virtual void onUnitPropagation( Clause* ) {}
    
    inline ActivityBasedHeuristic();
    virtual inline ~ActivityBasedHeuristic();

    virtual void initClauseData( Clause* clause ) { clause->setHeuristicData( new ClauseData() ); }

    inline void setDecisionStrategy( DecisionStrategy* value );
    inline void setRestartStrategy( RestartStrategy* value );
    inline void setDeletionStrategy( DeletionStrategy* value );

    virtual Literal makeAChoice() { return decisionStrategy->makeAChoice(); }
    
    virtual void onNewVariable( Variable& variable ) { decisionStrategy->onNewVariable( variable ); }
    virtual void onRestart() { deletionStrategy->onRestart(); restartStrategy->onRestart(); decisionStrategy->onRestart(); }
    virtual void onConflict() { return decisionStrategy->onLearning(); }
    virtual void onLearning( Clause* clause ) { deletionStrategy->onLearning( clause ); }
    virtual void onLiteralInvolvedInConflict( Literal literal ) { decisionStrategy->onLiteralInvolvedInConflict( literal ); }
    virtual void deleteClauses() {  }
    virtual bool hasToRestart() { return restartStrategy->hasToRestart(); }
    virtual void onClauseInvolvedInConflict( Clause* clause ){ deletionStrategy->onClauseInvolvedInConflict( clause ); }
    
private:
    DecisionStrategy* decisionStrategy;
    RestartStrategy* restartStrategy;
    DeletionStrategy* deletionStrategy;
};

ActivityBasedHeuristic::ActivityBasedHeuristic()
: decisionStrategy( NULL ), restartStrategy( NULL ), deletionStrategy( NULL )
{
}

ActivityBasedHeuristic::~ActivityBasedHeuristic()
{
    if( decisionStrategy != NULL )
        delete decisionStrategy;
    if( restartStrategy != NULL )
        delete restartStrategy;
    if( deletionStrategy != NULL )
        delete deletionStrategy;
}

void
ActivityBasedHeuristic::setDecisionStrategy(
    DecisionStrategy* value )
{
    assert( value != NULL );
    if( decisionStrategy != NULL )
        delete decisionStrategy;
    decisionStrategy = value;
}

void
ActivityBasedHeuristic::setRestartStrategy(
    RestartStrategy* value )
{
    assert( value != NULL );
    if( restartStrategy != NULL )
        delete restartStrategy;
    restartStrategy = value;
}

void
ActivityBasedHeuristic::setDeletionStrategy(
    DeletionStrategy* value )
{
    assert( value != NULL );
    if( deletionStrategy != NULL )
        delete deletionStrategy;
    deletionStrategy = value;
}

#endif
