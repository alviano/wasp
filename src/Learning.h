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

#ifndef LEARNING_H
#define	LEARNING_H

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

class Clause;
class LearnedClause;
class Literal;
class Solver;
class Variable;

class Learning
{
    public:
        inline Learning( Solver& solver );
        inline ~Learning();
        
        void onNavigatingLiteral( Literal );
        LearnedClause* onConflict( Literal conflictLiteral, Clause* conflictClause );
        
        inline void onNewVariable();
        
    private:
    
        Solver& solver;
        
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
         * This method computes the next literal to navigate in the implication graph.
         * The most recent (in the order of derivation) literal should be processed before.          
         * 
         * @return the next lStrategyiteral to consider.
         */
        Literal getNextLiteralToNavigate();
        
        /**
         * The variables of the current conflict level which have been visited.
         */
        bool isVisitedVariablesEmpty() const;
        vector< unsigned > visitedVariables;
        unsigned int pendingVisitedVariables;
        unsigned numberOfCalls;
        
        unsigned int maxDecisionLevel;
        
        unsigned int maxPosition;
};

Learning::Learning( Solver& s ) : solver( s ), decisionLevel( 0 ), learnedClause( NULL ), pendingVisitedVariables( 0 ), numberOfCalls( 0 ), maxDecisionLevel( 0 )
{
    // variable 0 is not used
    visitedVariables.push_back( 0 );
}

Learning::~Learning()
{
}

void
Learning::clearDataStructures()
{
    learnedClause = NULL;    
    maxDecisionLevel = 0;
    pendingVisitedVariables = 0;
    if( numberOfCalls == 0 )
    {
        for( unsigned i = 1; i < visitedVariables.size(); ++i )
            visitedVariables[ i ] = 0;
    }
}

void
Learning::onNewVariable()
{
    visitedVariables.push_back( 0 );
}

#endif	/* LEARNING_H */
