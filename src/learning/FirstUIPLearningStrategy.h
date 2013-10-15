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
 * File:   FirstUIPLearningStrategy.h
 * Author: Carmine Dodaro
 *
 * Created on 27 July 2013, 10.17
 */

#ifndef FIRSTUIPLEARNINGSTRATEGY_H
#define	FIRSTUIPLEARNINGSTRATEGY_H

#include "LearningStrategy.h"
#include <unordered_set>
#include <unordered_map>
using namespace std;

#include "../Literal.h"

class Variable;

class FirstUIPLearningStrategy : public LearningStrategy
{
    public:
        inline FirstUIPLearningStrategy( RestartsStrategy* restartsStrategy );
        
        virtual void onNavigatingLiteral( Literal );
        virtual void onConflict( Literal conflictLiteral, Clause* conflictClause, Solver& solver );        
        
    private:
        
        /**
         * This method computes the next literal to navigate in the implication graph.
         * The most recent (in the order of derivation) literal should be processed before.          
         * 
         * @return the next literal to consider.
         */
        Literal getNextLiteralToNavigate( Solver& solver );
        
        /**
         * This method cleans data structures.
         * It should be called in the end of each iteration.
         */
        inline void clearDataStructures();           
        
        /**
         * Add a literal in the new learned clause.
         * @param literal the literal to add.
         */
        void addLiteralInLearnedClause( Literal literal );
        
        /**
         * The literal added by this method is a literal which should be navigated.
         * @param literal the literal to navigate.
         */
        void addLiteralToNavigate( Literal literal );     
        
        /**
         * The variables already added in the learned clause.
         */
        unordered_set< const Variable* > addedVariablesInClause;
        
        /**
         * The variables of the current conflict level which have been visited.
         */
        unordered_set< const Variable* > visitedVariables;        
        
        unsigned int maxDecisionLevel;
        
        unsigned int maxPosition;
        
        unsigned int counter;
};

FirstUIPLearningStrategy::FirstUIPLearningStrategy(
    RestartsStrategy* restartsStrategy ) : LearningStrategy( restartsStrategy ), maxDecisionLevel( 0 ), counter( 0 )
{
}

void
FirstUIPLearningStrategy::clearDataStructures()
{
    learnedClause = NULL;    
    maxDecisionLevel = 0;
    visitedVariables.clear();
    addedVariablesInClause.clear();
    counter = 0;
}

#endif	/* FIRSTUIPLEARNINGSTRATEGY_H */

