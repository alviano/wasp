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
 * File:   BerkminHeuristic.h
 * Author: Carmine Dodaro
 *
 * Created on 02 September 2013, 16.36
 */

#ifndef BERKMINHEURISTIC_H
#define	BERKMINHEURISTIC_H

#include "DecisionHeuristic.h"
#include "HeuristicVisitor.h"
#include "counters/BerkminCounters.h"

#include "../Trace.h"
#include "../Variable.h"

#include <cassert>
using namespace std;

class BerkminHeuristic : public DecisionHeuristic, public HeuristicVisitor
{
    public:
        inline BerkminHeuristic( unsigned int maxBerkminNumber );
        virtual ~BerkminHeuristic();

    protected:
        virtual Literal makeAChoiceProtected( Solver& solver );
        virtual void onLearning( Solver& solver );
        virtual void onRestarting( Solver& solver );
        virtual void visit( Clause* );
        void onNavigatingVariable( Variable* );
        bool onNavigatingVariableForMostOccurrences( Variable* );
        
        virtual void pickLiteralUsingLearnedClauses( Solver& solver );
        virtual void pickLiteralUsingActivity( Solver& solver );
        
        virtual void choosePolarityTopMostUndefinedClause();
        virtual void choosePolarityHigherGlobalCounter( Solver& solver );
        virtual void choosePolarityMostOccurrences();
        inline void resetCounters();

    private:
        void topMostUndefinedLearnedClause( Solver& solver );        
        unsigned int estimatePropagation( Literal literal, Solver& solver );
        unsigned int maxBerkminNumber;
        unsigned int numberOfConflicts;

        inline BERKMIN_HEURISTIC_COUNTER getLiteralCounter( const HeuristicCounterForLiteral* heuristicCounter ) const;
        inline BERKMIN_HEURISTIC_COUNTER getTotalCounter( const Variable* ) const;
        BERKMIN_HEURISTIC_COUNTER maxCounter;
        unsigned int maxOccurrences;
};

BerkminHeuristic::BerkminHeuristic(
    unsigned int max ) : maxBerkminNumber( max ), numberOfConflicts( 0 ), maxCounter( 0 ), maxOccurrences( 0 )
{
}

BERKMIN_HEURISTIC_COUNTER
BerkminHeuristic::getLiteralCounter( 
    const HeuristicCounterForLiteral* heuristicCounter ) const
{
    assert( heuristicCounter != NULL );
    const BerkminCounters* berkminCounter = static_cast< const BerkminCounters* >( heuristicCounter );    
    return berkminCounter->getCounter();
}

BERKMIN_HEURISTIC_COUNTER
BerkminHeuristic::getTotalCounter( 
    const Variable* variable ) const
{
    assert( variable != NULL );
    BERKMIN_HEURISTIC_COUNTER totalCounter = getLiteralCounter( variable->getPositiveHeuristicCounter() ) + getLiteralCounter( variable->getNegativeHeuristicCounter() );
    return totalCounter;
}

void
BerkminHeuristic::resetCounters()
{
    maxCounter = 0;
    maxOccurrences = 0;
}

#endif	/* BERKMINHEURISTIC_H */

