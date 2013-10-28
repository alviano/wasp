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

#ifndef BERKMINHEURISTIC_H
#define	BERKMINHEURISTIC_H

#include "DecisionStrategy.h"

#include "../../util/Trace.h"
#include "../../Variable.h"

#include <cassert>
using namespace std;

class BerkminHeuristic : public DecisionStrategy
{
    public:
        inline BerkminHeuristic( Solver& solver, unsigned int numberOfLearnedClausesToConsider = static_cast< unsigned >( -1 ) );
        virtual ~BerkminHeuristic();
        
        virtual Literal makeAChoice();
        virtual void onLearning();
        virtual void onRestart();
        
        virtual void onNewVariable( Variable& variable );
        
        virtual void onLiteralInvolvedInConflict( Literal literal );

    private:
        struct VariableCounters
        {
            public:
                VariableCounters() { counter[ 0 ] = counter[ 1 ] = globalCounter[ 0 ] = globalCounter[ 1 ] = 0; }
                
                unsigned getPositiveCounter() const { return counter[ 0 ]; }
                unsigned getNegativeCounter() const { return counter[ 1 ]; }
                unsigned getTotalCounter() const { return counter[ 0 ] + counter[ 1 ]; }
                
                /**
                 * Position 0 of this vector contains the heuristic counter of the positive literal associated with this variable.
                 * Position 1 of this vector contains the heuristic counter of the negative literal associated with this variable.
                 */
                // FIXME: introduce macros to refer to positive and negative positions
                unsigned counter[ 2 ];
                unsigned globalCounter[ 2 ];
        };
    
        inline void pickLiteralUsingActivity();
        inline void pickLiteralFromTopMostUnsatisfiedLearnedClause();
        
        inline void choosePolarityTopMostUndefinedClause();
        inline void choosePolarityHigherGlobalCounter();
        inline void choosePolarityMostOccurrences();
        inline void resetCounters();

        Solver& solver;
        vector< VariableCounters > variableCounters;

        Variable* chosenVariableByOccurrences;
        unsigned int maxOccurrences;
        unsigned maxCounter;

        Variable* chosenVariable;
        bool chosenPolarity;
    
        unsigned int numberOfLearnedClausesToConsider;

        unsigned int estimatePropagation( Literal literal );
        unsigned int numberOfConflicts;

        inline void updateMaxCounter( Variable* );
        inline void updateMaxOccurrences( Variable* );
        inline bool checkUnsatisfiedAndOptimize( Clause& clause );
};

BerkminHeuristic::BerkminHeuristic(
    Solver& s, 
    unsigned int max ) : solver( s ), chosenVariableByOccurrences( NULL ), maxOccurrences( 0 ), maxCounter( 0 ), chosenVariable( NULL ), chosenPolarity( true ), numberOfLearnedClausesToConsider( max ), numberOfConflicts( 0 )
{
    // variable 0 is not used
    variableCounters.push_back( VariableCounters() );
}

void
BerkminHeuristic::resetCounters()
{
    maxCounter = maxOccurrences = 0;
    chosenVariableByOccurrences = NULL;
}

#endif	/* BERKMINHEURISTIC_H */

