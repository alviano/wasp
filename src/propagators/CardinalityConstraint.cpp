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

#include "CardinalityConstraint.h"
#include "../Solver.h"
#include "../util/WaspOptions.h"
#include "../Learning.h"
#include "../Clause.h"

void
CardinalityConstraint::reset(
    const Solver& solver )
{    
    trace_msg( aggregates, 2, "Calling reset for cardinality constraint " << *this );         
    while( true )
    {
        if( trail.empty() )
            break;
        
        unsigned int pos = trail.back();
        if( !solver.isUndefined( at( pos ) ) )
            break;

        inTrail[ pos ] = false;        
        trail.pop_back();
        maxFalse++;
    }
}

bool
CardinalityConstraint::onLiteralFalse(
    Solver& solver,
    Literal,
    PropagatorData p )
{
    int pos = p.position();
    assert( pos > 0 );
    trace_msg( aggregates, 10, "CardinalityConstraint: " << *this << ". Literal " << literals[ pos ] << "is false" ); 
    assert_msg( maxFalse > 0, *this );
    trail.push_back( pos );
    assert( !inTrail[ pos ] );
    inTrail[ pos ] = true;
    
    if( --maxFalse > 0 )
        return true;
    
    for( unsigned int i = 0; i < literals.size(); i++ )
    {
        Literal lit = literals[ i ];
        if( solver.isTrue( lit ) || inTrail[ i ] )
            continue;
        
        solver.assignLiteral( lit, this );
        
        if( solver.conflictDetected() )
            return true;
        
    }    
    return true;
}

ostream&
operator<<( 
    ostream& out, 
    const CardinalityConstraint& cc )
{
    return cc.print( out );
}

ostream&
CardinalityConstraint::print( ostream& out ) const
{
    if( literals.size() > 0 )
    {
        out << "#count{";
        unsigned i = 0;
        for( ; i < literals.size() - 1; ++i )
            out << literals[ i ] << ";";

        out << literals[ i ] << "} >= " << bound;
    }
    else
        out << "#count{} >= " << bound;

    return out;
}

void
CardinalityConstraint::onLearning(
    const Solver& solver,
    Learning* strategy,
    Literal )
{
    for( unsigned int i = 0; i < trail.size(); i++ )
        if( solver.getDecisionLevel( at( trail[ i ] ) ) > 0 )
            strategy->onNavigatingLiteral( at( trail[ i ] ) );
}

void
CardinalityConstraint::onNavigatingForUnsatCore(
    const Solver& solver,
    vector< unsigned int >& visited,
    unsigned int numberOfCalls,
    Literal )
{
    for( unsigned int i = 0; i < trail.size(); i++ )
        if( solver.getDecisionLevel( at( trail[ i ] ) ) > 0 )
            visited[ at( trail[ i ] ).getVariable() ] = numberOfCalls;    
}

bool
CardinalityConstraint::onNavigatingLiteralForAllMarked(
    const Solver&,
    Learning* strategy,
    Literal )
{
    for( unsigned int i = 0; i < trail.size(); i++ )
        if( !strategy->onNavigatingLiteralForAllMarked( at( trail[ i ] ) ) )
            return false;

    return true;            
}

bool
CardinalityConstraint::checkConsistent(
    Solver& solver )
{
    assert( solver.getCurrentDecisionLevel() == 0 );
    for( unsigned int i = 0; i < literals.size(); i++ )
        if( solver.isFalse( literals[ i ] ) )
        {            
            if( maxFalse == 0 )
                return false;
            onLiteralFalse( solver, literals[ i ], i );
            if( solver.conflictDetected() )
                return false;
        }
    return true;    
}