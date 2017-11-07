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

#ifndef WASP_MINISATHEURISTIC_H
#define WASP_MINISATHEURISTIC_H

#include "../util/WaspOptions.h"
#include "../util/WaspTrace.h"
#include "../Literal.h"
#include "../stl/Heap.h"
#include "../stl/HeapLiterals.h"

#include <cassert>
#include <iostream>
using namespace std;
class Solver;
class Clause;

class Preference
{
    public:
        Preference(bool neg, bool pref) : factor_(1), sign_(2), neg_(neg), isPref_(pref) {}
        
        inline unsigned int factor() const { return factor_; }
        inline void setFactor( unsigned int factor ) { factor_ = factor; }
        inline bool hasSign() const { return sign_ <= 1; }
        inline bool sign() const { return sign_; }        
        inline void setSign( bool sign ) { sign_ = sign; }
        inline Literal lit( unsigned int var ) const { return Literal( var, neg_ ? NEGATIVE : POSITIVE ); }
        inline bool isPreference() const { return isPref_; }
        inline void setPreference( Literal lit ) { neg_ = lit.isNegative(); isPref_ = true; }
        inline void removePreference() { isPref_ = false; }                
        
    private:
        unsigned int factor_ : 28;
        unsigned int sign_ : 2;
        unsigned int neg_ : 1;
        unsigned int isPref_ : 1;
};

struct ActivityComparator
{
    const Vector< Activity >&  act;
    const vector< Preference >&  vars;
    bool operator()( Var x, Var y ) const { assert( x < act.size() && y < act.size() ); assert( act.size() == vars.size() ); return act[ x ] * vars[ x ].factor() > act[ y ] * vars[ y ].factor(); }
    ActivityComparator( const Vector< Activity >& a, const vector< Preference >& v ) : act( a ), vars( v ) {}
};

struct LiteralActivityComparator
{
    const Vector< Activity >&  act;
    const vector< Preference >&  vars;
    bool operator()( Literal x, Literal y ) const
    {
        Var xVar = x.getVariable(); Var yVar = y.getVariable();
        assert( xVar < act.size() && yVar < act.size() );
        assert( act.size() == vars.size() );        
        return act[ xVar ] * vars[ xVar ].factor() > act[ yVar ] * vars[ yVar ].factor();
    }
    LiteralActivityComparator( const Vector< Activity >& a, const vector< Preference >& v ) : act( a ), vars( v ) {}
};

class MinisatHeuristic
{
    public:
        inline MinisatHeuristic( Solver& s );
        virtual ~MinisatHeuristic() {};

        inline void onNewVariable( Var v );
        inline void onNewVariableRuntime( Var v );
        inline void onFinishedSimplifications() { simplifyVariablesAtLevelZero(); onFinishedSimplificationsProtected(); }
        inline void onUnrollingVariable( Var v );
        
        inline void onLitInvolvedInConflict( Literal lit ) { variableBumpActivity( lit.getVariable() ); onLitInvolvedInConflictProtected( lit ); }
        inline void onConflict() { variableDecayActivity(); onConflictProtected(); }
        inline void onLitInImportantClause( Literal lit ) { bumpActivity( lit.getVariable() ); onLitInImportantClauseProtected( lit ); }
                
        virtual void onDeletion() {}
        virtual void onLearningClause( unsigned int, const Clause* ) {}
        virtual void onLitInLearntClause( Literal ) {}
        virtual void onLoopFormula( const Clause* ) {}
        void onNewBinaryClause( Literal l1, Literal l2 ) { chosenVariable = 1; onNewBinaryClauseProtected( l1, l2 ); }
        virtual void onNewClause( const Clause* ) {}
        virtual void onRestart() {}
        virtual void onUnfoundedSet( const Vector< Var >& ) {}
        
        inline void addPreferredChoice( Literal lit );
        inline void removePrefChoices();
        
        Literal makeAChoice();

    protected:
        Solver& solver;        
        virtual Literal makeAChoiceProtected();
        inline void init( Var v, double value );
        inline void setFactor( Var v, unsigned int factor );
        inline void setSign( int lit );        
        
        virtual void onLitInvolvedInConflictProtected( Literal ) {}
        virtual void onConflictProtected() {}
        virtual void onLitInImportantClauseProtected( Literal ) {}
        virtual void onFinishedSimplificationsProtected() {}
        virtual void onNewBinaryClauseProtected( Literal, Literal ) {}
        
    private:        
        inline void rescaleActivity();
        inline void variableBumpActivity( Var variable );
        void randomChoice();
        inline void variableDecayActivity(){ trace_msg( heuristic, 1, "Calling decay activity" ); variableIncrement *= variableDecay; }
        void simplifyVariablesAtLevelZero();
        inline bool bumpActivity( Var var ){ assert( var < act.size() ); return ( ( act[ var ] += variableIncrement ) > 1e100 ); }

        
        void computeScore( Var v, unsigned int& score );
        void initMinisatMoms( Var v, unsigned int& score );
        void initMinisatBinary( Var v, unsigned int& score );
        void initMinisatWatches( Var v, unsigned int& score );
        void initMinisatPropagators( Var v, unsigned int& score );
        void initMinisatVisibleAtoms( Var v, unsigned int& score );
        void initMinisatHiddenAtoms( Var v, unsigned int& score );
        void initMinisatCombination( Var v, unsigned int& score );
        
        Activity variableIncrement;
        Activity variableDecay;
        
        Vector< Activity > act;
        vector< Preference > vars;        

        Var chosenVariable;
        Heap< ActivityComparator > heap;
        HeapLiterals< LiteralActivityComparator > preferredChoices;                
};

MinisatHeuristic::MinisatHeuristic( Solver& s ) :
    solver( s ), variableIncrement( wasp::Options::initVariableIncrement ), variableDecay( wasp::Options::initVariableDecay ), chosenVariable( 0 ), heap( ActivityComparator( act, vars ) ), preferredChoices( LiteralActivityComparator( act, vars ) )
{
    act.push_back( 0.0 );
    vars.push_back( Preference( false, false ) );    
}

void
MinisatHeuristic::init(
    Var v,
    double value )
{
    assert( v < act.size() );
    act[ v ] += value;
    if( heap.inHeap( v ) )
        heap.decrease( v );
    
    if( vars[ v ].isPreference() && preferredChoices.inHeap( v ) )
        preferredChoices.decrease( v );
}

void
MinisatHeuristic::setFactor(
    Var v,
    unsigned int factor )
{
    assert( v < vars.size() );
    vars[ v ].setFactor( factor );
    if( heap.inHeap( v ) )
        heap.decrease( v );
    
    if( vars[ v ].isPreference() && preferredChoices.inHeap( v ) )
        preferredChoices.decrease( v );
}

void
MinisatHeuristic::setSign(
    int lit )
{
    Var v = lit > 0 ? lit : -lit;
    assert( v < vars.size() );
    vars[ v ].setSign( lit > 0 );
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
    
    assert( variable < vars.size() );
    if( vars[ variable ].isPreference() && preferredChoices.inHeap( variable ) )
        preferredChoices.decrease( variable );
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
    Var
    #ifndef NDEBUG
    v 
    #endif
    )
{    
    act.push_back( 0.0 );
    assert_msg( v == vars.size(), v << " != " << vars.size() );
    vars.push_back( Preference( false, false ) );
}

void
MinisatHeuristic::onNewVariableRuntime(
    Var v )
{
    assert( act.size() - 1 == v );
    assert( vars.size() - 1 == v );
    heap.pushNoCheck( v );
}

void
MinisatHeuristic::onUnrollingVariable(
    Var variable )
{
    heap.push( variable );         
    assert( variable < vars.size() );
    if( vars[ variable ].isPreference() )
        preferredChoices.push( vars[ variable ].lit( variable ) );
}

void
MinisatHeuristic::addPreferredChoice(
    Literal lit )
{
    assert( lit != Literal::null );
    assert( lit.getVariable() < vars.size() );    
    if( vars[ lit.getVariable() ].isPreference() )
        return;
    vars[ lit.getVariable() ].setPreference( lit );
    preferredChoices.pushNoCheck( lit );
}

void
MinisatHeuristic::removePrefChoices()
{
    preferredChoices.clear();
    for( unsigned int i = 0; i < vars.size(); i++ )
        vars[ i ].removePreference();
}

#endif
