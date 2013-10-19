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
 * File:   Learning.h
 * Author: Carmine Dodaro
 *
 * Created on 27 July 2013, 9.51
 */

#ifndef LEARNING_H
#define	LEARNING_H

#include "restarts/RestartsStrategy.h"

#include <cassert>
#include <iostream>
#include <unordered_set>

using namespace std;

class Clause;
class LearnedClause;
class Literal;
class Solver;
class Variable;

class Learning
{
    public:
        inline Learning();
        inline ~Learning();
        
        void onNavigatingLiteral( Literal );
        void onConflict( Literal conflictLiteral, Clause* conflictClause, Solver& solver );
        
        inline void setRestartsStrategy( RestartsStrategy* value );
        
    private:
        /**
         * This method cleans data structures.
         * It should be called in the end of each iteration.
         */
        inline void clearDataStructures();           
        
        /**
         * Add a literal in the new learned clause.
         * @param literal the literal to add.
         */
        inline void addLiteralInLearnedClause( Literal literal );
        
        /**
         * The literal added by this method is a literal which should be navigated.
         * @param literal the literal to navigate.
         */
        inline void addLiteralToNavigate( Literal literal );     
        
        /**
         * The decision level of the conflict.
         */
        unsigned int decisionLevel;
        
        /**
         * The new learned clause.
         */
        LearnedClause* learnedClause;
        
        /**
         * The strategy used for restarts.
         */
        RestartsStrategy* restartsStrategy;             

        /**
         * This method computes the next literal to navigate in the implication graph.
         * The most recent (in the order of derivation) literal should be processed before.          
         * 
         * @return the next lStrategyiteral to consider.
         */
        Literal getNextLiteralToNavigate( Solver& solver );
        
        /**
         * The variables of the current conflict level which have been visited.
         */
        unordered_set< const Variable* > visitedVariables;        
        
        unsigned int maxDecisionLevel;
        
        unsigned int maxPosition;
        
        unsigned int visitedVariablesCounter;
};

Learning::Learning() : decisionLevel( 0 ), learnedClause( NULL ), restartsStrategy( NULL ), maxDecisionLevel( 0 ), visitedVariablesCounter( 0 )
{
}

Learning::~Learning()
{
    if( restartsStrategy != NULL )
        delete restartsStrategy;
}

void
Learning::clearDataStructures()
{
    learnedClause = NULL;    
    maxDecisionLevel = 0;
    visitedVariables.clear();
    visitedVariablesCounter = 0;
}

void
Learning::setRestartsStrategy( 
    RestartsStrategy* value )
{
    assert( value != NULL );
    restartsStrategy = value;    
}

#endif	/* LEARNING_H */

