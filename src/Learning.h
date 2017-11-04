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

#ifndef WASP_LEARNING_H
#define WASP_LEARNING_H

#include <cassert>
#include <iostream>
#include <vector>
#include "stl/Vector.h"
#include "Literal.h"
using namespace std;

class Clause;
class Literal;
class Solver;
class Reason;

class Learning
{
    public:
        inline Learning( Solver& solver );
        inline ~Learning();
        
        void onNavigatingLiteral( Literal );
        void onNavigatingLiteralForUnfoundedSetLearning( Literal );
        
        bool onNavigatingLiteralForAllMarked( Literal );
        Clause* onConflict( Literal conflictLiteral, Reason* conflictClause );
        
        inline void onNewVariable();
        
        Clause* learnClausesFromUnfoundedSet( Vector< Var >& unfoundedSet );
        Clause* learnClausesFromDisjunctiveUnfoundedSet( Vector< Var >& unfoundedSet );

        bool isVisited( Var v, unsigned int value ) const { assert( v < visited.size() ); return visited[ v ] == value; }
        void setVisited( Var v, unsigned int value ) { assert( v < visited.size() ); visited[ v ] = value; }        
        Clause* analyzeFinal( Literal lit );
        
        static void sortClause( Clause* clause, const Solver& solver );
        
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
        Clause* learnedClause;
        
        /**
         * This method computes the next literal to navigate in the implication graph.
         * The most recent (in the order of derivation) literal should be processed before.          
         * 
         * @return the next literal to consider.
         */
        Literal getNextLiteralToNavigate();
        
        void simplifyLearnedClause( Clause* lc );
        bool allMarked( Reason* clause, Literal literal );
        
        void computeMaxDecisionLevel( const Clause& lc );                
        
        #ifndef NDEBUG
        /**
        * The variables of the current conflict level which have been visited.
        */
        bool isVisitedVariablesEmpty() const;
        #endif

        void resetVariablesNumberOfCalls();
        unsigned int pendingVisitedVariables;
        unsigned numberOfCalls;
        
        unsigned int maxDecisionLevel;
        
        unsigned int maxPosition;
        
        vector< unsigned int > visited;
        
        inline bool sameDecisionLevelOfSolver( Literal lit ) const;
        
        Vector< Var > lastDecisionLevel;
};

Learning::Learning( Solver& s ) : solver( s ), decisionLevel( 0 ), learnedClause( NULL ), pendingVisitedVariables( 0 ), numberOfCalls( 0 ), maxDecisionLevel( 0 ), maxPosition( 0 )
{
    visited.push_back( 0 );
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
    if( numberOfCalls == 0 ) resetVariablesNumberOfCalls();
    lastDecisionLevel.clear();
}

void
Learning::onNewVariable()
{
    visited.push_back( 0 );
}

#endif

