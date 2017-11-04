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

#include "Aggregate.h"
#include "../Solver.h"
#include "../util/WaspOptions.h"
#include "../Learning.h"
#include "../Clause.h"

void
Aggregate::reset(
    const Solver& solver )
{    
    trace_msg( aggregates, 2, "Calling reset for aggregate " << *this );        
    if( trail.empty() || ( !solver.isUndefined( literals[ abs( trail.back() ) ] ) && solver.getDecisionLevel( literals[ abs( trail.back() ) ] ) != 0 ) )
        return;    

    umax = 1;
    
    assert( literalOfUnroll != Literal::null || active == 0 );
    if( literalOfUnroll != Literal::null && solver.isUndefined( literalOfUnroll ) )
    {
        active = 0;
        literalOfUnroll = Literal::null;
    }

    int last = trail.back();
    unsigned int pos = abs( last );
    
    do
    {
        trail.pop_back();
        if( !watched[ pos ] )
            ( last > 0 ) ? counterW2 += weights[ pos ] : counterW1 += weights[ pos ];
        watched[ pos ] = true;        
        
        if( trail.empty() )
        {
            trace_msg( aggregates, 3, "Restored counters. Values: " << counterW1 << " - " << counterW2 );
            return;
        }
        
        last = trail.back();
        pos = abs( last );
    } while( solver.isUndefined( literals[ pos ] ) || solver.getDecisionLevel( literals[ pos ] ) == 0 );
}

bool
Aggregate::onLiteralFalse(
    Solver& solver,
    Literal currentLiteral,
    PropagatorData p )
{
    int position = p.position();
    assert_msg( abs( position ) > 0 && abs( position ) < static_cast< int >( literals.size() ), abs( position ) << " >= " << literals.size() );
    assert_msg( currentLiteral == ( position < 0 ? literals[ -position ].getOppositeLiteral() : literals[ position ] ), currentLiteral << " != " << ( position < 0 ? literals[ -position ].getOppositeLiteral() : literals[ position ] ) );
    trace_msg( aggregates, 10, "Aggregate: " << *this << ". Literal: " << currentLiteral.getOppositeLiteral() << " is true. Position: " << position );
    int ac = ( position < 0 ? POS : NEG );
    Literal aggrLiteral = ( ac == POS ? literals[ 1 ].getOppositeLiteral() : literals[ 1 ] );
    
    if( solver.isTrue( aggrLiteral ) || active + ac == 0 )
    {        
        trace_msg( aggregates, 2, "Return. AggrLiteral: " << aggrLiteral << " - Active: " << active << " - Ac: " << ac );
        return false;
    }
    
    unsigned int index = ( position > 0 ? position : -position ); 
    int64_t& counter = ( position > 0 ? counterW2 : counterW1 );

    trace_msg( aggregates, 2, "Updating counter. Old value: " << counter << " - New value: " << counter - weights[ index ] );
    
    if( counter < ( int64_t ) weights[ index ] )
    {
        assert_msg( solver.getDecisionLevel( currentLiteral ) == 0, "Literal " << currentLiteral << " in " << *this << " has a decision level " << solver.getDecisionLevel( currentLiteral ) );
        trace_msg( aggregates, 3, "A conflict happened." );        
        solver.assignLiteral( currentLiteral, this );
        return false;
    }
    assert( counter >= ( int64_t ) weights[ index ] );
    counter -= weights[ index ];
    watched[ index ] = false;
    
    if( solver.getDecisionLevel( currentLiteral ) != 0 )
        trail.push_back( position );

    trace_msg( aggregates, 2, "Umax: " << umax << " - size: " << size() );
    while( umax < literals.size() && ( int64_t ) weights[ umax ] > counter )
    {
        if( watched[ umax ] )
        {
            if( literalOfUnroll == Literal::null )
                literalOfUnroll = currentLiteral;
            active = ac;
            Literal lit = ( ac == POS ? literals[ umax ].getOppositeLiteral() : literals[ umax ] );
            if( !solver.isTrue( lit ) )
            {                
                //Maybe we don't need to add the position of this literal
                trail.push_back( umax * ac );
            
                trace_msg( aggregates, 9, "Inferring " << lit << " as true" );
//                createClauseFromTrail( lit );
                solver.assignLiteral( lit, this );
                if( solver.conflictDetected() )
                    return true;
            }
            else
            {
                trace_msg( aggregates, 9, "Skipping true literal " << lit );
            }
        }
        
        ++umax;
        trace_msg( aggregates, 3, "Updated umax. New Value: " << umax );        
    }
    
    return true;
}

#ifndef NDEBUG
bool
Aggregate::checkDecisionLevelsOrder(
    const Solver& solver,
    const Clause& clause ) const
{
    if( clause.size() <= 1 )
        return true;
    
    unsigned int max1 = solver.isUndefined( clause[ 0 ] ) ? MAXUNSIGNEDINT : solver.getDecisionLevel( clause[ 0 ] );
    unsigned int max2 = solver.getDecisionLevel( clause[ 1 ] );
    for( unsigned int i = 2; i < clause.size(); i++ )
    {
        unsigned int dl = solver.getDecisionLevel( clause[ i ] );
        
        if( dl > max1 || dl > max2 )
            return false;
    }
    
    return max1 >= max2;
}
#endif

bool
Aggregate::updateBound(
    Solver& solver,
    uint64_t bound )
{
    trace_msg( aggregates, 1, "Updating bound. New value: " << bound );
    uint64_t sumOfWeights = 0;    
    for( unsigned int i = 2; i < weights.size(); i++ )
    {
        if( weights[ i ] > bound )
            weights[ i ] = bound;

        // if( !literals[ i ].isFalse() || literals[ i ].getDecisionLevel() != 0 ) 
        sumOfWeights += weights[ i ]; 
    }
    trace_msg( aggregates, 1, "Sum of weights: " << sumOfWeights );    

    int64_t w1 = ( sumOfWeights - bound + 1 );
    int64_t w = max( w1, ( int64_t ) bound );

    counterW1 = sumOfWeights - w1 + w;
    counterW2 = sumOfWeights - bound + w;

    trace_msg( aggregates, 1, "Counters: " << counterW1 << "," << counterW2 );
    weights[ 1 ] = w;
        
    for( unsigned int i = 2; i < weights.size(); i++ )
    {
        if( solver.getDecisionLevel( literals[ i ] ) != 0 )
            continue;                        
        if( solver.isTrue( literals[ i ] ) )
        {
            this->onLiteralFalse( solver, literals[ i ].getOppositeLiteral(), -i );
//            counterW1 -= weights[ i ];
        }
        else if( solver.isFalse( literals[ i ] ) )
        {
            this->onLiteralFalse( solver, literals[ i ], i );
//            counterW2 -= weights[ i ];
        }
    }
    trace_msg( aggregates, 1, "Counters final: " << counterW1 << "," << counterW2 );
    return ( sumOfWeights >= bound );
}

unsigned int
Aggregate::getLevelOfBackjump(
    const Solver& solver,
    uint64_t bound )
{    
    uint64_t sum = 0;
    unsigned int level = 1;
    for( int i = trail.size() - 1; i >= 0; i-- )
    {
        int position = trail[ i ];
        
        int ac = ( position < 0 ? POS : NEG );
        if( ac == active )
        {
            unsigned int index = ( position > 0 ? position : -position );
            Literal l = literals[ index ];
            
            if( active == NEG )
                l = l.getOppositeLiteral();
            
            if( !watched[ index ] )
            {
                assert_msg( solver.getDecisionLevel( l ) > 0, "Literal " << l << " has been inferred at the decision level 0" );
                assert( solver.isTrue( l ) );
                
                sum += weights[ index ];
                if( sum >= bound && level == 1 )
                    level = solver.getDecisionLevel( l );
            }            
        }
    }      
    return level - 1;
}

ostream&
operator<<( 
    ostream& out, 
    const Aggregate& aggregate )
{
    return aggregate.print( out );
}

ostream&
Aggregate::print( ostream& out ) const
{
    #ifdef TRACE_ON
    out << "Counters: (" << counterW1 << ";" << counterW2 << ") - ";
    #endif
    if( literals.size() > 1 )
    {
        out << "[";
        unsigned i = 1;
        for( ; i < literals.size() - 1; ++i )
            out << literals[ i ] << "=" << weights[ i ] << ",";
        
        out << literals[ i ] << "=" << weights[ i ] << "]";        
    }
    else
        out << "[]";    

    return out;
}

void
Aggregate::onLearning(
    const Solver&,
    Learning* strategy,
    Literal lit )
{
//    for( int i = trail.size() - 1; i >= 0; i-- )
    for( unsigned int i = 0; i < trail.size(); i++ )
    {
        int position = trail[ i ];
        
        int ac = ( position < 0 ? POS : NEG );
        
        unsigned int index = ( position > 0 ? position : -position );
        Literal l = literals[ index ];    
        if( ac == active )
        {
            if( active == NEG )
                l = l.getOppositeLiteral();

            if( !watched[ index ] )
            {
//                assert_msg( solver.getDecisionLevel( l ) > 0, "Literal " << l << " has been inferred at the decision level 0" );
//                clause->addLiteral( l.getOppositeLiteral() );   
                strategy->onNavigatingLiteral( l.getOppositeLiteral() );                
            }
            
            assert( l != lit );            
        }
        
        if( l.getVariable() == lit.getVariable() )
            break;        
    }
}

void
Aggregate::onNavigatingForUnsatCore(
    const Solver& solver,
    vector< unsigned int >& visited,
    unsigned int numberOfCalls,
    Literal lit )
{
    for( unsigned int i = 0; i < trail.size(); i++ )
    {
        int position = trail[ i ];
        
        int ac = ( position < 0 ? POS : NEG );
        
        unsigned int index = ( position > 0 ? position : -position );
        Literal l = literals[ index ];    
        if( ac == active )
        {
            if( active == NEG )
                l = l.getOppositeLiteral();
            
            if( !watched[ index ] )
            {
//                assert_msg( solver.getDecisionLevel( l ) > 0, "Literal " << l << " has been inferred at the decision level 0" );
//                clause->addLiteral( l.getOppositeLiteral() );                
//                toVisit.push_back( l ); //l.getOppositeLiteral() );
                Var variable = l.getVariable();
                if( solver.getDecisionLevel( variable ) > 0 )
                    visited[ variable ] = numberOfCalls;                
            }
            
            assert( l != lit );            
        }
        
        if( l.getVariable() == lit.getVariable() )
            break;        
    }
}

bool
Aggregate::onNavigatingLiteralForAllMarked(
    const Solver&,
    Learning* strategy,
    Literal lit )
{
    for( unsigned int i = 0; i < trail.size(); i++ )
    {
        int position = trail[ i ];        
        int ac = ( position < 0 ? POS : NEG );
        
        unsigned int index = ( position > 0 ? position : -position );
        Literal l = literals[ index ];            
        if( ac == active )
        {
            if( active == NEG )
                l = l.getOppositeLiteral();
        
            if( !watched[ index ] )
            {
//                assert_msg( l.getDecisionLevel() > 0, "Literal " << l << " has been inferred at the decision level 0" );
                if( !strategy->onNavigatingLiteralForAllMarked( l.getOppositeLiteral() ) )
                    return false;
            }
            
            assert( l != lit );            
        }
        
        if( l.getVariable() == lit.getVariable() )
            break;
    }
    
    return true;
}