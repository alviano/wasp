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

#include "DisjunctionPropagator.h"
#include "../Solver.h"

DisjunctionPropagator::~DisjunctionPropagator()
{
    delete firstReason;
    delete secondReason;
}

bool
DisjunctionPropagator::onLiteralFalse(
    Solver& solver,
    Literal literal,
    PropagatorData p )
{
    int pos = p.position();
    trace_msg( disjunction, 1, "Literal " << literal.getOppositeLiteral() << " in position " << pos << " is true" );    
    if( pos >= 0 )
        origIsTrue( solver, literal.getOppositeLiteral(), pos );
    else
        auxIsTrue( solver, literal.getOppositeLiteral(), -pos );
    return false;
}


void
DisjunctionPropagator::origIsTrue(
    Solver& solver,
    Literal literal,
    unsigned int pos )
{
    assert_msg( firstReason, "First reason is null" );
    assert_msg( secondReason, "Second reason is null" );

    trace_msg( disjunction, 2, literal << " is a body literal or an head atom" );
    trace_msg( disjunction, 3, "First reason: " << *firstReason );
    trace_msg( disjunction, 3, "Second reason: " << *secondReason );
    assert_msg( !solver.conflictDetected(), "Solver is a conflictual state" );
    
    if( !solver.isTrue( firstReason->getAt( 1 ).getOppositeLiteral() ) || firstReason->getAt( 1 ).getOppositeLiteral() == literal )
    {
        trace_msg( disjunction, 4, "First true literal of the propagator" );
        Clause* reason = createReason( firstReason, literal.getOppositeLiteral() );
        trace_msg( disjunction, 5, "Reason is: " << *reason );
        assert( pos < auxLiterals.size() );
        for( unsigned int i = 1; i < pos; i++ )
        {
            inferLiteral( solver, auxLiterals[ i ].getOppositeLiteral(), reason );
            if( solver.conflictDetected() )
                return;            
        }
        for( unsigned int i = pos + 1; i < auxLiterals.size(); i++ )
        {
            inferLiteral( solver, auxLiterals[ i ].getOppositeLiteral(), reason );
            if( solver.conflictDetected() )
                return;
        }
        firstPos = pos;        
    }
    else if( !solver.isTrue( secondReason->getAt( 1 ).getOppositeLiteral() ) || secondReason->getAt( 1 ).getOppositeLiteral() == literal || firstReason->getAt( 1 ) == secondReason->getAt( 1 )  )
    {
        trace_msg( disjunction, 4, "Second true literal of the propagator" );        
        assert_msg( firstPos < auxLiterals.size(), firstPos << ">=" << auxLiterals.size() );        
        if( firstPos != 0 )
            inferLiteral( solver, auxLiterals[ firstPos ].getOppositeLiteral(), createReason( secondReason, literal.getOppositeLiteral() ) );
    }
}

void
DisjunctionPropagator::auxIsTrue(
    Solver& solver,
    Literal literal,
    unsigned int pos )
{
    trace_msg( disjunction, 2, literal << " is aux" );
    assert( pos >= startPos && pos < origLiterals.size() );
    
    assert_msg( !solver.conflictDetected(), "Solver is a conflictual state" );
    
    //If an aux is true then its corresponding original atom will be set as true and it will be the first reason
    Clause* reason = createReason( secondReason, literal.getOppositeLiteral() );
    trace_msg( disjunction, 3, "Created reason " << *reason );    
    for( unsigned int i = startPos; i < pos; i++ )
    {
        inferLiteral( solver, origLiterals[ i ].getOppositeLiteral(), reason );
        if( solver.conflictDetected() )
            return;        
    }
    for( unsigned int i = pos + 1; i < origLiterals.size(); i++ )
    {
        inferLiteral( solver, origLiterals[ i ].getOppositeLiteral(), reason );
        if( solver.conflictDetected() )
            return;        
    }
}

void
DisjunctionPropagator::inferLiteral(
    Solver& solver,
    Literal lit,
    Clause* reason )
{
    trace_msg( disjunction, 6, "Trying to infer " << lit << " which is " << ( solver.isTrue( lit ) ? "true" : "false/undefined" ) );
    if( !solver.isTrue( lit ) )
    {
        assert( solver.getCurrentDecisionLevel() == solver.getDecisionLevel( reason->getAt( 1 ) ) );
        solver.assignLiteral( lit, reason );
    }
}

void
DisjunctionPropagator::finalize(
    Solver& solver )
{
    trace_msg( disjunction, 3, "Finalizing the propagator" );
    assert_msg( origLiterals.size() > 2, origLiterals.size() << "<= 2" );
    assert_msg( origLiterals.size() == auxLiterals.size(), origLiterals.size() << "!=" << auxLiterals.size() );    
    if( origLiterals[ 0 ] != Literal::null )
        solver.addPropagator( origLiterals[ 0 ].getOppositeLiteral(), this, PropagatorData( 0 ) );

    for( unsigned int i = 1; i < origLiterals.size(); i++ )
    {
        solver.addPropagator( origLiterals[ i ].getOppositeLiteral(), this, PropagatorData( i ) );
        solver.addPropagator( auxLiterals[ i ].getOppositeLiteral(), this, PropagatorData( -i ) );
    }
    
    assert_msg( firstReason, "First reason is null" );
    assert_msg( secondReason, "Second reason is null" );
    firstReason->setAt( 1, origLiterals[ 1 ].getOppositeLiteral() );
    secondReason->setAt( 1, origLiterals[ 2 ].getOppositeLiteral() );
    assert( firstReason->getAt( 1 ) != secondReason->getAt( 1 ) );
    firstPos = 1;
    
    trace_msg( disjunction, 4, "First reason: " << *firstReason );
    trace_msg( disjunction, 4, "Second reason: " << *secondReason );
}

Clause*
DisjunctionPropagator::createReason(
    Clause* clause,
    Literal reason )
{
    assert( clause->size() == 2 );
    clause->setAt( 1, reason );
    assert_msg( firstReason, "First reason is null" );
    assert_msg( secondReason, "Second reason is null" );
//    assert( firstReason->getAt( 1 ) != secondReason->getAt( 1 ) );
    return clause;
}