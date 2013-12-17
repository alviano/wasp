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

#include "util/Options.h"
#include "util/Trace.h"
#include "Literal.h"
#include "Variable.h"
#include "stl/Heap.h"

#include <cassert>
using namespace std;

class MinisatHeuristic
{
    public:
        inline MinisatHeuristic();
        inline ~MinisatHeuristic(){};

        Literal makeAChoice();
        inline void onNewVariable( Variable* variable );
        inline void onLiteralInvolvedInConflict( Literal literal );
        inline void simplifyVariablesAtLevelZero();
        inline void onUnrollingVariable( Variable* variable );

    private:
        inline bool bumpActivity( Variable* variable ){ return ( ( variable->activity() += variableIncrement ) > 1e100 ); }
        inline void rescaleActivity();
        inline void variableDecayActivity(){ trace( heuristic, 1, "Calling decay activity.\n" ); variableIncrement *= variableDecay; }
        inline void variableBumpActivity( Variable* variable );
        void randomChoice();

        Activity variableIncrement;
        Activity variableDecay;

        vector< Variable* > variables;

        Variable* chosenVariable;
        Heap< Variable > heap;
};

MinisatHeuristic::MinisatHeuristic() :
    variableIncrement( 1.0 ), variableDecay( 1 / 0.95 ), chosenVariable( NULL )
{
}

void
MinisatHeuristic::variableBumpActivity(
    Variable* variable )
{
    trace( heuristic, 1, "Bumping activity for variable %s.\n", toString( *variable ).c_str() );
	if( bumpActivity( variable ) )
		rescaleActivity();
    
    if( variable->isInHeap() )
        heap.decrease( variable );
}

void
MinisatHeuristic::rescaleActivity()
{
    trace( heuristic, 1, "Rescaling activity.\n" );
    unsigned int size = variables.size();
	for( unsigned int i = 0; i < size; ++i )
        variables[ i ]->activity() *= 1e-100;		
	variableIncrement *= 1e-100;
}

void
MinisatHeuristic::onNewVariable(
    Variable* variable )
{
    variables.push_back( variable );
}

void 
MinisatHeuristic::onLiteralInvolvedInConflict( 
    Literal literal )
{
    variableBumpActivity( literal.getVariable() );
}

void
MinisatHeuristic::simplifyVariablesAtLevelZero()
{
    for( unsigned int i = 0; i < variables.size(); )
    {
        if( !variables[ i ]->isUndefined() )
        {
            assert_msg( variables[ i ]->getDecisionLevel() == 0, "Variable " << *variables[ i ] << " has not been inferred at level 0.");            
            variables[ i ] = variables.back();
            variables.pop_back();
        }
        else
        {       
            heap.pushNoCheck( variables[ i ] );
            ++i;
        }
    }
}

void
MinisatHeuristic::onUnrollingVariable(
    Variable* variable )
{
    heap.push( variable );
}

#endif	/* MINISATHEURISTIC_H */

