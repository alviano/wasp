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
#define MINISATHEURISTIC_H

#include "util/Options.h"
#include "util/Trace.h"
#include "Literal.h"
#include "stl/Heap.h"

#include <cassert>
#include <iostream>
using namespace std;
class Solver;

struct ActivityComparator
{
    const Vector< Activity >&  act;
    bool operator()( Var x, Var y ) const { return act[ x ] > act[ y ]; }
    ActivityComparator( const Vector< Activity >& a ) : act( a ) {}
};

class MinisatHeuristic
{
    public:
        inline MinisatHeuristic( Solver& s );
        inline ~MinisatHeuristic(){};

        Literal makeAChoice();
        inline void onNewVariable( Var v );
        inline void onNewVariableRuntime( Var v );
        inline void onLiteralInvolvedInConflict( Literal literal );
        inline void onUnrollingVariable( Var var );
        inline void variableDecayActivity(){ trace_msg( heuristic, 1, "Calling decay activity" ); variableIncrement *= variableDecay; }
        inline void addPreferredChoice( Literal lit ){ assert( lit != Literal::null ); preferredChoices.push_back( lit ); }
        void removePrefChoices();
        void simplifyVariablesAtLevelZero();
        inline bool bumpActivity( Var var ){ return ( ( act[ var ] += variableIncrement ) > 1e100 ); }        
        
    private:        
        inline void rescaleActivity();        
        inline void variableBumpActivity( Var variable );
        void randomChoice();

        Solver& solver;
        Activity variableIncrement;
        Activity variableDecay;
        
        Vector< Activity > act;
        vector< Literal > preferredChoices;
        
        vector< Var > vars;

        Var chosenVariable;
        Heap< ActivityComparator > heap;        
};

MinisatHeuristic::MinisatHeuristic( Solver& s ) :
    solver( s ), variableIncrement( 1.0 ), variableDecay( 1 / 0.95 ), chosenVariable( 0 ), heap( ActivityComparator( act ) )
{
    act.push_back( 0.0 );
}

void
MinisatHeuristic::variableBumpActivity(
    Var variable )
{
    trace_msg( heuristic, 1, "Bumping activity for variable " << variable );
    if( bumpActivity( variable ) )
        rescaleActivity();
    
    if( heap.inHeap( variable ) )
        heap.decrease( variable );    
}

void
MinisatHeuristic::rescaleActivity()
{
    trace_msg( heuristic, 1, "Rescaling activity" );
    unsigned int size = act.size();
    for( unsigned int i = 0; i < size; ++i )
        act[ i ] *= 1e-100;
    variableIncrement *= 1e-100;
}

void
MinisatHeuristic::onNewVariable(
    Var v )
{    
    act.push_back( 0.0 );
    vars.push_back( v );
}

void
MinisatHeuristic::onNewVariableRuntime(
    Var v )
{
    heap.pushNoCheck( v );
}

void 
MinisatHeuristic::onLiteralInvolvedInConflict( 
    Literal literal )
{
    variableBumpActivity( literal.getVariable() );
}

void
MinisatHeuristic::onUnrollingVariable(
    Var variable )
{
    heap.push( variable );
}

#endif

