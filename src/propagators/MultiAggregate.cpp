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

#include "MultiAggregate.h"

#include "../Solver.h"
#include "../Literal.h"

ostream&
operator<<( 
    ostream& out, 
    const MultiAggregate& aggregate )
{
    return aggregate.print( out );
}

bool
MultiAggregate::onLiteralFalse( Solver& solver, Literal lit, PropagatorData propData )
{
    int pos = propData.position();
    assert( pos != 0 );
    lit = lit.getOppositeLiteral();
    trace_msg( multiaggregates, 1, "Literal " << lit << " in position " << pos << " is true " << ( propData.isId() ? "(id)" : "(not id)" ) );
    return propData.isId() ? notificationIdLit( solver, lit, pos ) : notificationAggregateLit( solver, lit, pos );
}

bool MultiAggregate::notificationIdLit( Solver& solver, Literal lit, int p )
{
    assert( p != 0 );
    unsigned int pos = p > 0 ? p : -p;
    if( ( p > 0 && pos <= w1 ) || ( p < 0 && pos >= w2 ) ) return false;    
    if( solver.getImplicant( lit.getVariable() ) != this )        
        addInTrail( solver, lit, true, p );
    if( p > 0 )
    {
        assert( pos > w1 );
        trace_msg( multiaggregates, 2, "Updating w1 to " << pos << " (was " << w1 << ")" );
        w1 = pos;
        assert( solver.isTrue( ids[ w1 ] ) );
        checkTrueInference( solver, lit );        
    }
    else
    {
        assert( pos < w2 );
        trace_msg( multiaggregates, 2, "Updating w2 to " << pos << " (was " << w2 << ")" );
        w2 = pos;
        assert( solver.isFalse( ids[ w2 ] ) );        
        checkFalseInference( solver, lit );        
    }
    return true;
}

bool MultiAggregate::notificationAggregateLit( Solver& solver, Literal lit, int p )
{
    assert( p != 0 );
    if( solver.getImplicant( lit.getVariable() ) != this )
        addInTrail( solver, lit, false, p );    
    
    if( solver.getDecisionLevel( lit ) > 0 )
    {
        assert( positionInTrail.find( lit.getId() ) != positionInTrail.end() );
        assert( positionInTrail[ lit.getId() ] < trail.size() );
        trail[ positionInTrail[ lit.getId() ] ].setPropagated();
    }
    
    if( p > 0 )
    {
        unsigned int pos = p;        
        trace_msg( multiaggregates, 2, "Updating current sum to " << ( currentSum + getWeight( pos ) ) << " (was " << currentSum << ")");           
        currentSum += getWeight( pos );
        
        if( currentSum >= getBound( w1 + 1 ) )
        {
            assert( solver.isTrue( getId( w1 ) ) );            
            unsigned int tmp = w1;
            while( currentSum >= getBound( tmp + 1 ) )
                ++tmp;
            if( !inferLiteral( solver, getId( tmp ), true, tmp ) )
                return true;
        }
        checkFalseInference( solver, lit );
    }
    else
    {
        unsigned int pos = -p;
        trace_msg( multiaggregates, 2, "Updating max possible sum to " << ( maxPossibleSum - getWeight( pos ) ) << " (was " << maxPossibleSum << ")");
        maxPossibleSum -= getWeight( pos );        

        if( maxPossibleSum < getBound( w2 - 1 ) )
        {
            assert( solver.isFalse( getId( w2 ) ) );
            unsigned int tmp = w2;
            while( maxPossibleSum < getBound( tmp - 1 ) )
                --tmp;
            if( !inferLiteral( solver, getId( tmp ).getOppositeLiteral(), true, -tmp ) )
                return true;            
        }
        checkTrueInference( solver, lit );
    }
    return true;
}

void
MultiAggregate::addInTrail( Solver& solver, Literal lit, bool isId, int pos )
{
    assert( !solver.isUndefined( lit ) );
    if( solver.getDecisionLevel( lit ) > 0 )
    {
        //assert( !isInTrail( pos, isId ) );
        trace_msg( multiaggregates, 4, "Adding literal " << lit << " in trail (size=" << ( trail.size() + 1 ) << ") " << pos << " " << ( isId ? "(id)" : "(not id)" ) << " ");
        positionInTrail[ lit.getId() ] = trail.size();
        trail.push_back( TrailElement( pos, isId ) );
    }
}

bool
MultiAggregate::inferLiteral( Solver& solver, Literal lit, bool isId, int pos )
{
    if( solver.isTrue( lit ) )
    {
        trace_msg( multiaggregates, 3, "Trying to infer literal " << lit << " in position " << pos << " " << ( isId ? "(id)" : "(not id)" ) << " which is already true: skipped");        
        return true;
    }
    trace_msg( multiaggregates, 3, "Inferring literal " << lit << " in position " << pos << " " << ( isId ? "(id)" : "(not id)" ) << " ");    
    solver.assignLiteral( lit, this );
    addInTrail( solver, lit, isId, pos );
    return !solver.conflictDetected();
}

void
MultiAggregate::checkFalseInference( Solver& solver, Literal lit )
{
    trace_msg( multiaggregates, 3, "Checking false inference: current sum = " << currentSum );
    unsigned int posInTrail = solver.getPositionInTrail( lit.getVariable() );
    for( unsigned int i = 1; i < literals.size(); i++ )
    {
        Literal current = getLiteral( i ).getOppositeLiteral();
        trace_msg( multiaggregates, 4, "Considering literal " << current );        
        if( solver.isTrue( current ) ) continue;
        if( solver.isUndefined( current ) || solver.getPositionInTrail( current.getVariable() ) > posInTrail )
        {
            if( currentSum + getWeight( i ) < getBound( w2 ) ) break;
            if( !inferLiteral( solver, current, false, -i ) )
                return;
        }
    }        
}

void
MultiAggregate::checkTrueInference( Solver& solver, Literal lit )
{
    trace_msg( multiaggregates, 3, "Checking true inference: max possible sum = " << maxPossibleSum );
    unsigned int posInTrail = solver.getPositionInTrail( lit.getVariable() );
    for( unsigned int i = 1; i < literals.size(); i++ )
    {
        Literal current = getLiteral( i );
        trace_msg( multiaggregates, 4, "Considering literal " << current );
        if( solver.isTrue( current ) ) continue;
        if( solver.isUndefined( current ) || solver.getPositionInTrail( current.getVariable() ) > posInTrail )
        {
            if( maxPossibleSum - getWeight( i ) >= getBound( w1 ) ) break;
            if( !inferLiteral( solver, current, false, i ) )
                return;
        }
    }
}

void
MultiAggregate::addBinaryClauses( Solver& solver )
{
    assert( solver.getCurrentDecisionLevel() == 0 );
    //ids[ i + 1 ] -> ids[ i ]
    for( unsigned int i = 1; i < ids.size() - 2; i++ )
    {
        trace_msg( multiaggregates, 1, "Adding " << ids[ i + 1 ] << "->" << ids[ i ] );
        Clause* clause = new Clause();
        clause->addLiteral( ids[ i + 1 ].getOppositeLiteral() );
        clause->addLiteral( ids[ i ] );
        solver.cleanAndAddClause( clause );
    }
}

void
MultiAggregate::reset( const Solver& solver )
{
    trace_msg( multiaggregates, 1, "Reset to level " << solver.getCurrentDecisionLevel() );
    while( !trail.empty() )
    {
        TrailElement te = trail.back();
        Literal lit = getLiteralFromTrailElement( te );
        trace_msg( multiaggregates, 2, "Lit " << lit << " is in trail" );
        if( !solver.isUndefined( lit ) ) break;
        updateCounters( te );
        trail.pop_back();        
    }
    while( solver.isUndefined( ids[ w1 ] ) ) --w1;
    while( solver.isUndefined( ids[ w2 ] ) ) w2++;
    trace_msg( multiaggregates, 3, "Restored w1=" << w1 << " and w2=" << w2 );
    trace_msg( multiaggregates, 3, "Restored current sum=" << currentSum << " and max possible sum=" << maxPossibleSum );
    assert_msg( solver.isTrue( ids[ w1 ] ), ids[ w1 ] << " (w1=" << w1 << "), is not true" );
    assert_msg( solver.isFalse( ids[ w2 ] ), ids[ w2 ] << " (w2=" << w2 << "), is not false" );
}

void MultiAggregate::onLearning( const Solver& solver, Learning* strategy, Literal lit )
{
    vector< Literal > inferences;
    computeInferences( inferences, lit.getOppositeLiteral() );
    for( unsigned int i = 0; i < inferences.size(); i++ )
        if( solver.getDecisionLevel( inferences[ i ] ) > 0 )
            strategy->onNavigatingLiteral( inferences[ i ].getOppositeLiteral() );  
}

void MultiAggregate::onNavigatingForUnsatCore( const Solver& solver, vector<unsigned int>& visited, unsigned int numberOfCalls, Literal lit )
{
    vector< Literal > inferences;
    computeInferences( inferences, lit.getOppositeLiteral() );
    for( unsigned int i = 0; i < inferences.size(); i++ )
        if( solver.getDecisionLevel( inferences[ i ] ) > 0 )
            visited[ inferences[ i ].getVariable() ] = numberOfCalls;        
}

bool MultiAggregate::onNavigatingLiteralForAllMarked( const Solver&, Learning* strategy, Literal lit )
{
    vector< Literal > inferences;
    computeInferences( inferences, lit.getOppositeLiteral() );    
    for( unsigned int i = 0; i < inferences.size(); i++ )
        if( !strategy->onNavigatingLiteralForAllMarked( inferences[ i ].getOppositeLiteral() ) )
            return false;
    return true;
}

void MultiAggregate::computeInferences( vector< Literal >& inferences, Literal lit )
{
    assert_msg( positionInTrail.find( lit.getId() ) != positionInTrail.end(), "Cannot find " << lit << " in trail" );
    assert( inferences.empty() );
    unsigned int position = positionInTrail[ lit.getId() ];    
    assert_msg( position < trail.size(), position << " >= " << trail.size() );
    trace_msg( multiaggregates, 3, "Computing inferences for literal " << lit );
    computeInferences( inferences, position, trail[ position ].isId(), trail[ position ].position() );
    trace_action( multiaggregates, 5, { trace_tag( cerr, multiaggregates, 5 ); cerr << "Inferences are:"; for( unsigned int i = 0; i < inferences.size(); i++ ) cerr << " " << inferences[ i ]; cerr << endl; } );
}

void MultiAggregate::computeInferences( vector< Literal >& inferences, unsigned int position, bool isId, int type )
{
    assert_msg( position < trail.size(), position << " >= " << trail.size() );
    trace_msg( multiaggregates, 4, "Computing inferences: position=" << position << ", is id=" << ( isId ? "yes" : "no" ) << ", type=" << type );
    if( isId )
    {
        for( unsigned int i = 0; i < position; i++ )
        {
            assert_msg( i < trail.size(), i << " >= " << trail.size() );
            int pos = trail[ i ].position();
            bool sameSign = ( pos > 0 && type > 0 ) || ( pos < 0 && type < 0 );
            if( sameSign && !trail[ i ].isId() )
                inferences.push_back( pos > 0 ? literals[ pos ] : literals[ -pos ].getOppositeLiteral() );
        }
    }
    else
    {
        unsigned int lastId = UINT_MAX;        
        for( unsigned int i = 0; i < position; i++ )
        {
            assert_msg( i < trail.size(), i << " >= " << trail.size() );
            int pos = trail[ i ].position();
            bool sameSign = ( pos > 0 && type > 0 ) || ( pos < 0 && type < 0 );
            if( sameSign || trail[ i ].isId() )
            {
                if( sameSign && trail[ i ].isId() ) lastId = i;
                continue;
            }
            if( abs( pos ) == abs( type ) ) continue;
            inferences.push_back( pos > 0 ? literals[ pos ] : literals[ -pos ].getOppositeLiteral() );
        }
        if( lastId != UINT_MAX )
        {
            assert_msg( lastId < trail.size(), lastId << " >= " << trail.size() );
            int pos = trail[ lastId ].position();
            inferences.push_back( pos > 0 ? ids[ pos ] : ids[ -pos ].getOppositeLiteral() );        
        }
    }
}

ostream&
MultiAggregate::print( ostream& out ) const
{
    assert( !literals.empty() );
    assert( literals[ 0 ] == Literal::null );    
    
    out << "[undef=undef";
    for( unsigned i = 1; i < literals.size(); ++i )
        out << ", " << literals[ i ] << "=" << weights[ i ];
    out << "]";
    
    assert( bounds.size() >= 2 );
    out << " bounds: ";
    out << "[";
    for( unsigned int i = 0; i < bounds.size() - 1; i++ )
        out << bounds[ i ] << ", ";
    out << bounds.back();
    out << "]";
    
    assert( ids.size() >= 2 );
    out << " ids: ";
    out << "[";
    for( unsigned int i = 0; i < ids.size() - 1; i++ )
        out << ids[ i ] << ", ";
    out << ids.back();
    out << "]";
        
    return out;
}

void
MultiAggregate::clean( Solver& solver )
{
    //Do not need to clean for count
    if( weights.size() == 0 || getWeight( 1 ) == 1 )
        return;
    
    unordered_set< uint64_t > sumsSet;
    vector< uint64_t > possibleSums;
    possibleSums.push_back( 0 );
    for( unsigned int i = 1;  i < weights.size(); i++ )
    {
        unsigned size = possibleSums.size();
        for(unsigned int j = 0; j < size; j++)
        {
            uint64_t sum = possibleSums[ j ] + getWeight( i );
            if( sumsSet.insert( sum ).second )
                possibleSums.push_back( sum );
        }
    }
    
    if( bounds.size() > 3 )
    {
        unsigned int j = 1;
        for( unsigned int i = 1; i < bounds.size() - 2; i++ )
        {
            bounds[ j ] = bounds[ i ];
            ids[ j ] = ids[ i ];

            //Adding ids[ i ] -> ids[ i + 1 ] ( ids[ i + 1 ] -> ids[ i ] has been already added )
            if( sumsSet.find( bounds[ i ] ) == sumsSet.end() )
            {
                Clause* clause = new Clause( 2 );
                clause->addLiteral( ids[ i ].getOppositeLiteral() );
                clause->addLiteral( ids[ i + 1 ] );
                solver.cleanAndAddClause( clause );
            }
            else
                j++;
        }
        bounds[ j ] = bounds[ bounds.size() - 2 ];
        ids[ j ] = ids[ ids.size() - 2 ];    
        j++;
        bounds[ j ] = bounds[ bounds.size() - 1 ];
        ids[ j ] = ids[ ids.size() - 1 ];
        j++;
        bounds.resize( j );
        ids.resize( j );
    }
}

void
MultiAggregate::attach( Solver& solver )
{
    clean( solver );
    computeWatches( solver );
    trace_msg( multiaggregates, 1, "Attaching aggregate " << *this );
    for( unsigned int i = 1; i < literals.size(); i++ )
    {
        solver.setFrozen( literals[ i ].getVariable() );
        solver.addPropagator( literals[ i ], this, PropagatorData( -i, false ) );
        solver.addPropagator( literals[ i ].getOppositeLiteral(), this, PropagatorData( i, false ) );
    }
    
    for( unsigned int i = 1; i < ids.size() - 1; i++ )
    {
        solver.setFrozen( ids[ i ].getVariable() );
        solver.addPropagator( ids[ i ], this, PropagatorData( -i, true ) );
        solver.addPropagator( ids[ i ].getOppositeLiteral(), this, PropagatorData( i, true ) );
    }    
    trace_msg( multiaggregates, 1, "Current sum " << currentSum << " - max possible sum " << maxPossibleSum );    
}

void
MultiAggregate::computeWatches( Solver& solver )
{
    w2 = ids.size() - 1;
    unsigned int tmpw1 = w1;
    unsigned int tmpw2 = w2;
    for( unsigned int i = 1; i < ids.size() - 1; i++ )
    {
        if( solver.isTrue( ids[ i ] ) )
            tmpw1 = i;
        else if( solver.isFalse( ids[ i ] ) )
        {
            tmpw2 = i;
            break;
        }
    }
        
    trace_msg( multiaggregates, 1, "w1=" << tmpw1 << " and w2=" << tmpw2 );
    if(tmpw1 > w1) {
        onLiteralFalse(solver, ids[tmpw1].getOppositeLiteral(), PropagatorData(tmpw1,true) );        
    }
    if(tmpw2 < w2) {
        onLiteralFalse(solver, ids[tmpw2], PropagatorData(-((int) tmpw2),true) );
    }
    solver.propagateAtLevelZero();
    
    trace_msg( multiaggregates, 1, "w1=" << w1 << " and w2=" << w2 );
//    assert_msg( maxUnd < weights.size() && currentSum + getWeight( maxUnd ) < getBound( w2 ), "Missing inferences" );    
//    assert_msg( maxUnd < weights.size() && maxPossibleSum - getWeight( maxUnd ) >= getBound( w1 ), "Missing inferences" );
}

#ifndef NDEBUG
bool MultiAggregate::checkUndefined( Solver& solver )
{
    for( unsigned int i = 1; i < literals.size(); i++ )
        if( !solver.isUndefined( literals[ i ] ) || solver.hasBeenEliminated( literals[ i ].getVariable() ) )
            return false;        
    return true;
}

bool MultiAggregate::isInTrail( int position, bool isId )
{
    for( unsigned int i = 0; i < trail.size(); i++ )
        if( abs( trail[ i ].position() ) == abs( position ) && trail[ i ].isId() == isId )
            return true;
    return false;
}
#endif