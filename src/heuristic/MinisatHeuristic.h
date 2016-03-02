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

#include "../util/Options.h"
#include "../util/Trace.h"
#include "../Literal.h"
#include "../stl/Heap.h"
#include "HeuristicStrategy.h"

#include <cassert>
#include <iostream>
using namespace std;
class Solver;
class Clause;

struct ActivityComparator
{
    const Vector< Activity >&  act;
    const Vector< unsigned int >&  factors;
    bool operator()( Var x, Var y ) const { assert( x < act.size() && y < act.size() ); assert( act.size() == factors.size() ); return act[ x ] * factors[ x ] > act[ y ] * factors[ y ]; }
    ActivityComparator( const Vector< Activity >& a, const Vector< unsigned int >& f ) : act( a ), factors( f ) {}
};

class MinisatHeuristic : public HeuristicStrategy
{
    public:
        inline MinisatHeuristic( Solver& s );
        virtual ~MinisatHeuristic(){};

        inline void onNewVariable( Var v );
        inline void onNewVariableRuntime( Var v );
        inline void onLitInvolvedInConflict( Literal literal );
        inline void onVarUndefined( Var var );
        inline void onConflict() { variableDecayActivity(); }
        inline void onLitInImportantClause( Literal lit ) { bumpActivity( lit.getVariable() ); };
        inline void onFinishedSimplifications() { simplifyVariablesAtLevelZero(); }        
        
        //Unused methods
        inline void addedVarName( Var, const string& ) {}
        inline void onDeletion() {}
        inline void onFinishedParsing() {}        
        inline void onLearningClause( unsigned int, const Clause* ) {}
        inline void onLitAtLevelZero( Literal ) {}
        inline void onLitInLearntClause( Literal ) {}
        inline void onRestart() {}
        inline void onAnswerSet() {}
        inline void onStartingSolver( unsigned int, unsigned int ) {}
        inline void onVariableElimination( Var ) {}
        inline void onStartingParsing() {}
        inline void onStartingSimplifications() {}
        inline void onUnfoundedSet( const Vector< Var >& ) {}
        inline void onLoopFormula( const Clause* ) {}
        inline void onNewClause( const Clause* ) {}
        inline void onNewBinaryClause( Literal, Literal ) {}
        inline void onLitTrue( Literal ) {}        
        
        inline void init( Var v, unsigned int value );
        inline void setFactor( Var v, unsigned int factor );
        inline void setSign( int lit );

    protected:
        Literal makeAChoiceProtected();
        
    private:        
        inline void rescaleActivity();
        inline void variableBumpActivity( Var variable );
        void randomChoice();
        inline void variableDecayActivity(){ trace_msg( heuristic, 1, "Calling decay activity" ); variableIncrement *= variableDecay; }
        void simplifyVariablesAtLevelZero();
        inline bool bumpActivity( Var var ){ assert( var < act.size() ); return ( ( act[ var ] += variableIncrement ) > 1e100 ); }

        Activity variableIncrement;
        Activity variableDecay;
        
        Vector< Activity > act;
        Vector< unsigned int > factors;
        Vector< unsigned int > signs;
        vector< Var > vars;        

        Var chosenVariable;
        Heap< ActivityComparator > heap;        
};

MinisatHeuristic::MinisatHeuristic( Solver& s ) :
    HeuristicStrategy( s ), variableIncrement( 1.0 ), variableDecay( 1 / 0.95 ), chosenVariable( 0 ), heap( ActivityComparator( act, factors ) )
{
    act.push_back( 0.0 );
    factors.push_back( 1 );
    signs.push_back( UINT_MAX );
}

void
MinisatHeuristic::init(
    Var v,
    unsigned int value )
{
    assert( v < act.size() );
    act[ v ] = value;
    if( heap.inHeap( v ) )
        heap.decrease( v );
}

void
MinisatHeuristic::setFactor(
    Var v,
    unsigned int factor )
{
    assert( v < factors.size() );
    factors[ v ] = factor;
    if( heap.inHeap( v ) )
        heap.decrease( v );
}

void
MinisatHeuristic::setSign(
    int lit )
{
    Var v = lit > 0 ? lit : -lit;
    assert( v < signs.size() );
    signs[ v ] = lit > 0 ? POSITIVE : NEGATIVE;
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
    factors.push_back( 1 );
    signs.push_back( UINT_MAX );
}

void
MinisatHeuristic::onNewVariableRuntime(
    Var v )
{
    assert( act.size() - 1 == v );
    heap.pushNoCheck( v );
}

void 
MinisatHeuristic::onLitInvolvedInConflict( 
    Literal literal )
{
    variableBumpActivity( literal.getVariable() );
}

void
MinisatHeuristic::onVarUndefined(
    Var variable )
{
    heap.push( variable );
}

#endif
