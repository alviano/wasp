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

#ifndef WASP_CARDINALITYCONSTRAINT_H
#define WASP_CARDINALITYCONSTRAINT_H

#include <cassert>
#include <iostream>

#include "Propagator.h"
#include "../Clause.h"
#include "../Learning.h"
#include "../Literal.h"
#include "../stl/Vector.h"
#include "../util/WaspOptions.h"
#include "../util/WaspTrace.h"

class Solver;
using namespace std;

class CardinalityConstraint : public Propagator, public Reason
{
    friend ostream& operator<<( ostream& out, const CardinalityConstraint& cc );
    public:
        inline CardinalityConstraint();
        inline CardinalityConstraint( const vector< Literal >& lits );
        
        inline ~CardinalityConstraint() {}
        
        virtual bool onLiteralFalse( Solver& solver, Literal lit, PropagatorData pos );
        virtual void simplifyAtLevelZero( Solver& ) {}
        
        inline unsigned int size() const { return literals.size(); }

        inline void addLiteral( Literal lit ) { literals.push_back( lit ); inTrail.push_back( false ); }        
        inline bool setBound( uint64_t b ){ bound = b; maxFalse = literals.size() - b; return literals.size() >= b; }
        bool checkConsistent( Solver& solver );
        
        inline Literal operator[]( unsigned int idx ) const { assert_msg( idx < literals.size(), idx << ">=" << literals.size() ); return literals[ idx ]; }
        inline Literal& operator[]( unsigned int idx ) { assert_msg( idx < literals.size(), idx << ">=" << literals.size() ); return literals[ idx ]; }
        
        virtual void reset( const Solver& solver );        
        virtual void onLearning( const Solver& solver, Learning* strategy, Literal lit );
        virtual bool onNavigatingLiteralForAllMarked( const Solver& solver, Learning* strategy, Literal lit );
        virtual void onNavigatingForUnsatCore( const Solver& solver, vector< unsigned int >& visited, unsigned int numberOfCalls, Literal lit );                
        
    protected:
        virtual ostream& print( ostream& out ) const;

    private:
        inline CardinalityConstraint( const CardinalityConstraint& orig );
        inline Literal at( unsigned int pos ) { assert( pos < literals.size() ); return literals[ pos ]; }
        
        vector< Literal > literals;
        uint64_t bound;
        uint64_t maxFalse;
        vector< unsigned int > trail;
        vector< bool > inTrail;
};

CardinalityConstraint::CardinalityConstraint() : Propagator(), bound( 0 )
{    
}

CardinalityConstraint::CardinalityConstraint(
    const vector< Literal >& lits ) : Propagator(), bound( 0 )
{
    for( unsigned int i = 0; i < lits.size(); i++ )
    {
        literals.push_back( lits[ i ] );
        inTrail.push_back( false );
    }
}

#endif
