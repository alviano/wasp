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
#include "Literal.h"
#include "Variable.h"
#include "util/Options.h"
#include "Learning.h"
#include "Clause.h"

Clause*
Aggregate::getClauseToPropagate(
    Learning& )
{
    while( !clausesToPropagate.empty() )
    {
        Clause* tmp = clausesToPropagate.back();
        clausesToPropagate.pop_back();
        if( tmp->size() == 0 || !tmp->getAt( 0 ).isTrue() )
            return tmp;
        else
            delete tmp;
    }

    return NULL;
}

void
Aggregate::reset()
{
    trace_msg( aggregates, 2, "Calling reset" );
    if( trail.empty() || ( !literals[ abs( trail.back() ) ].isUndefined() && literals[ abs( trail.back() ) ].getDecisionLevel() != 0 ) )
        return;

    while( !clausesToPropagate.empty() )
    {
        delete clausesToPropagate.back();
        clausesToPropagate.pop_back();
    }
    umax = 1;
    active = 0;
    int last = trail.back();
    unsigned int pos = abs( last );
    
    do
    {        
        trail.pop_back();
        ( last > 0 ) ? counterW2 += weights[ pos ] : counterW1 += weights[ pos ];
        watched[ pos ] = true;        
        
        if( trail.empty() )
        {
            trace_msg( aggregates, 3, "Restored counters. Values: " << counterW1 << " - " << counterW2 );
            return;
        }
        
        last = trail.back();
        pos = abs( last );
    } while( literals[ pos ].isUndefined() || literals[ pos ].getDecisionLevel() == 0 );
}

bool
Aggregate::onLiteralFalse(
    Literal currentLiteral,
    int position )
{
    bool toAddInSolver = false;
    assert( abs( position ) > 0 && abs( position ) < static_cast< int >( literals.size() ) );
    assert( currentLiteral == ( position < 0 ? literals[ -position ].getOppositeLiteral() : literals[ position ] ) );
    trace_msg( aggregates, 1, "Aggregate: " << *this << ". Literal: " << currentLiteral.getOppositeLiteral() << " is true. Position: " << position );
    int ac = ( position < 0 ? POS : NEG );
    Literal aggrLiteral = ( ac == POS ? literals[ 1 ].getOppositeLiteral() : literals[ 1 ] );
    
    if( aggrLiteral.isTrue() || active + ac == 0 )
    {
        trace_msg( aggregates, 2, "Return. AggrLiteral: " << aggrLiteral << " - Active: " << active << " - Ac: " << ac );
        return toAddInSolver;
    }
    
    unsigned int index = ( position > 0 ? position : -position ); 
    unsigned int& counter = ( position > 0 ? counterW2 : counterW1 );
    
    trace_msg( aggregates, 2, "Updating counter. Old value: " << counter << " - New value: " << counter - weights[ index ] );
    
    if( counter < weights[ index ] )
    {
        assert( checkLiteralHasBeenInferred( currentLiteral ) );
        trace_msg( aggregates, 3, "A conflict happened." );
        return toAddInSolver;
    }
    assert( counter >= weights[ index ] );
    counter -= weights[ index ];
    watched[ index ] = false;
    
    if( currentLiteral.getDecisionLevel() != 0 )
        trail.push_back( position );

    trace_msg( aggregates, 2, "Umax: " << umax << " - size: " << size() );
    while( umax < literals.size() && weights[ umax ] > counter )
    {
        if( watched[ umax ] )
        {
            active = ac;
            //Maybe we don't need to add the position of this literal
//            trail.push_back( umax * ac );
            Literal lit = ( ac == POS ? literals[ umax ].getOppositeLiteral() : literals[ umax ] );
            
            if( !lit.isTrue() )
            {                
                trace_msg( aggregates, 1, "Inferring " << lit << " as true" );
                createClauseFromTrail( lit );
                toAddInSolver = true;
            }
            else
            {
                trace_msg( aggregates, 1, "Skipping true literal " << lit );
            }
        }
        
        ++umax;
        trace_msg( aggregates, 3, "Updated umax. New Value: " << umax );        
    }
    
    return toAddInSolver;
}

#ifndef NDEBUG
bool
Aggregate::checkDecisionLevelsOrder(
    Clause* clause ) const
{
    if( clause->size() <= 1 )
        return true;
    
    unsigned int max1 = clause->getAt( 0 ).isUndefined() ? MAXUNSIGNEDINT : clause->getAt( 0 ).getDecisionLevel();
    unsigned int max2 = clause->getAt( 1 ).getDecisionLevel();
    for( unsigned int i = 2; i < clause->size(); i++ )
    {
        unsigned int dl = clause->getAt( i ).getDecisionLevel();
        
        if( dl > max1 || dl > max2 )
            return false;
    }
    
    return max1 >= max2;
}

bool
Aggregate::checkLiteralHasBeenInferred(
    Literal lit ) const
{
    for( list< Clause* >::const_iterator it = clausesToPropagate.begin(); it != clausesToPropagate.end(); ++it )
    {
        const Clause* c = *it;
        if( c->size() > 0 && c->getAt( 0 ) == lit )
            return true;
    }
    
    return false;
}
#endif

void
Aggregate::createClauseFromTrail(
    Literal lit )
{
    trace_msg( aggregates, 2, "Creating clause from trail for inferring " << lit );
    Clause* clause = new Clause();    
    clause->addLiteral( lit );
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
                assert_msg( l.getDecisionLevel() > 0, "Literal " << l << " has been inferred at the decision level 0" );
                clause->addLiteral( l.getOppositeLiteral() );
            }
            
            assert( l != lit );
//            else if( l == lit )
//                break;            
        }
    }
    
    trace_msg( aggregates, 2, "Created clause: " << *clause );
    assert( checkDecisionLevelsOrder( clause ) );
    clausesToPropagate.push_back( clause );
}

ostream&
operator<<( 
    ostream& out, 
    const Aggregate& aggregate )
{
    #ifdef TRACE_ON
    out << "Counters: (" << aggregate.counterW1 << ";" << aggregate.counterW2 << ") - ";
    #endif
    if( aggregate.literals.size() > 1 )
    {
        out << "[";
        unsigned i = 1;
        for( ; i < aggregate.literals.size() - 1; ++i )
            out << aggregate.literals[ i ] << "=" << aggregate.weights[ i ] << ",";
        
        out << aggregate.literals[ i ] << "=" << aggregate.weights[ i ] << "]";        
    }
    else
        out << "[]";    

    return out;
}