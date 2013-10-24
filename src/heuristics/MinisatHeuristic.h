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

#ifndef MINISATHEURISTIC_H
#define	MINISATHEURISTIC_H

#include "DecisionHeuristic.h"
#include "UndefinedCollector.h"

#include "../Options.h"
#include "../Trace.h"
#include "../Variable.h"

#include <cassert>
using namespace std;

class MinisatHeuristic : public DecisionHeuristic
{
    public:
        inline MinisatHeuristic( Solver& solver );
        virtual ~MinisatHeuristic();

        virtual Literal makeAChoice();
        virtual void onLearning();
        virtual void onRestart();

        virtual void onNewVariable( Variable& variable );

        virtual void onLiteralInvolvedInConflict( Literal literal );

    private:
        struct VariableCounters
        {
            public:
                inline VariableCounters() : variableActivity( 0 ){}

                inline Activity getActivity() const { return variableActivity; }
                inline void rescaleActivity(){ variableActivity *= 1e-100; }
                inline bool bumpActivity( Activity increment ){ return ( ( variableActivity += increment ) > 1e100 ); }

            private:
                Activity variableActivity;
        };

        inline void rescaleActivity();
        inline void variableDecayActivity();
        inline void variableBumpActivity( const Variable* variable );
        inline Activity getVariableActivity( const Variable* variable );

        Solver& solver;
        
        vector< VariableCounters > variableCounters;
        Activity variableIncrement;
        Activity variableDecay;

        Variable* chosenVariable;
        Variable* nextUndefinedVariable;   
};

MinisatHeuristic::MinisatHeuristic( Solver& s ) : solver( s ), variableIncrement( 1.0 ), variableDecay( 1/0.95 ), chosenVariable( NULL ), nextUndefinedVariable( NULL )
{
    // variable 0 is not used
    variableCounters.push_back( VariableCounters() );
}

void
MinisatHeuristic::variableBumpActivity(
    const Variable* variable )
{
    trace( heuristic, 1, "Bumping activity for variable %s.\n", toString( *variable ).c_str() );
	if( variableCounters[ variable->getId() ].bumpActivity( variableIncrement ) )
		rescaleActivity();	
}

void
MinisatHeuristic::variableDecayActivity()
{
    trace( heuristic, 1, "Calling decay activity.\n" );
	variableIncrement *= variableDecay;
}

void
MinisatHeuristic::rescaleActivity()
{
    trace( heuristic, 1, "Rescaling activity.\n" );
    unsigned int size = variableCounters.size();
	for( unsigned int i = 1; i < size; i++ )
		variableCounters[ i ].rescaleActivity();
	variableIncrement *= 1e-100;
}

Activity
MinisatHeuristic::getVariableActivity(
    const Variable* variable )
{
    assert( "Variable has not been set." && variable != NULL );
    assert( "Variable id is out of the range." && variable->getId() < variableCounters.size() );
    return variableCounters[ variable->getId() ].getActivity();
}

#endif	/* MINISATHEURISTIC_H */
