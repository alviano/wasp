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

#ifndef THREESTRATEGIESHEURISTIC_H
#define THREESTRATEGIESHEURISTIC_H

#include "../Heuristic.h"

#include "decision/DecisionHeuristic.h"
#include "deletion/DeletionStrategy.h"
#include "restart/RestartStrategy.h"

class ThreeStrategiesHeuristic : public Heuristic
{
public:
    inline ThreeStrategiesHeuristic();
    virtual ~ThreeStrategiesHeuristic();

    inline void setDecisionStrategy( DecisionHeuristic* value );
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
    
private:
    DecisionHeuristic* decisionStrategy;
    RestartStrategy* restartStrategy;
    DeletionStrategy* deletionStrategy;
};

ThreeStrategiesHeuristic::ThreeStrategiesHeuristic()
: decisionStrategy( NULL ), restartStrategy( NULL ), deletionStrategy( NULL )
{
}

void
ThreeStrategiesHeuristic::setDecisionStrategy(
    DecisionHeuristic* value )
{
    assert( value != NULL );
    if( decisionStrategy != NULL )
        delete decisionStrategy;
    decisionStrategy = value;
}

void
ThreeStrategiesHeuristic::setRestartStrategy(
    RestartStrategy* value )
{
    assert( value != NULL );
    if( restartStrategy != NULL )
        delete restartStrategy;
    restartStrategy = value;
}

void
ThreeStrategiesHeuristic::setDeletionStrategy(
    DeletionStrategy* value )
{
    assert( value != NULL );
    if( deletionStrategy != NULL )
        delete deletionStrategy;
    deletionStrategy = value;
}

#endif
