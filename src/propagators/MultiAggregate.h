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

#ifndef WASP_MULTIAGGREGATE_H
#define	WASP_MULTIAGGREGATE_H

class Solver;
#include "../Literal.h"

#include "Propagator.h"
#include "../Reason.h"
#include "../util/WaspAssert.h"
#include "../util/WaspOptions.h"
#include "../util/WaspTrace.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <numeric>
using namespace std;

class TrailElement
{
    public:
        TrailElement( int position, bool isId ) { position_ = position > 0 ? position : -position; isPositive_ = position > 0 ? 1 : 0; isId_ = isId; propagated_ = false; }        
        bool isId() const { return isId_; }
        int position() const { return isPositive_ ? position_ : -position_; }
        void setPropagated() { propagated_ = 1; }
        bool propagated() const { return propagated_; }

    private:
        unsigned int position_ : 29;
        unsigned int isPositive_ : 1;
        unsigned int isId_ : 1;
        unsigned int propagated_ : 1;
};

class MultiAggregate : public Propagator, public Reason
{
    friend ostream& operator<<( ostream& out, const MultiAggregate& aggregate );
    public:        
        inline MultiAggregate() : currentSum( 0 ), maxPossibleSum( 0 ), w1( 0 ), w2( 0 ) { addLiteral( Literal::null, 0 ); addBound( Literal( 1, NEGATIVE ), 0 ); }
        virtual ~MultiAggregate() {}
        bool onLiteralFalse( Solver& solver, Literal literal, PropagatorData propData );        
        inline void addLiteral( Literal literal, uint64_t weight ) { literals.push_back( literal ); weights.push_back( weight ); maxPossibleSum += weight; }
        inline unsigned int hasBound( uint64_t bound )
        {
            for( unsigned int i = 0; i < bounds.size(); i++ )
                if( bounds[ i ] == bound )
                    return i;
            return UINT_MAX;
        }
        inline void addBound( Literal boundId, uint64_t bound ) { assert( hasBound( bound ) == UINT_MAX ); ids.push_back( boundId ); bounds.push_back( bound ); }
                
        void reset( const Solver& solver );
        void simplifyAtLevelZero( Solver& ) {}

        void onLearning( const Solver& solver, Learning* strategy, Literal lit );
        bool onNavigatingLiteralForAllMarked( const Solver& solver, Learning* strategy, Literal lit );
        void onNavigatingForUnsatCore( const Solver& solver, vector< unsigned int >& visited, unsigned int numberOfCalls, Literal lit );        
        
        inline void sort() { bubblesort( literals, weights, true ); bubblesort( ids, bounds, false ); }
        void attach( Solver& solver );
        
        inline Literal getId( unsigned int pos ) const { assert_msg( pos < ids.size(), pos << ">=" << ids.size() ); return ids[ pos ]; }        
        
        inline void finalize( Solver& solver ) { sort(); addBound( Literal( 1, POSITIVE ), UINT64_MAX ); addBinaryClauses( solver ); assert( checkUndefined( solver ) ); }
        inline void finalizeNoBinaryClauses( Solver& 
        #ifndef NDEBUG
        solver
        #endif
        )
        { sort(); addBound( Literal( 1, POSITIVE ), UINT64_MAX ); assert( checkUndefined( solver ) ); }                        
        
        inline unsigned int numberOfBounds() const { return bounds.size() - 2; }
        
    protected:
        virtual ostream& print( ostream& out ) const;
    
    private:
        vector< Literal > literals;
        vector< uint64_t > weights;
        
        vector< Literal > ids;
        vector< uint64_t > bounds;
        
        uint64_t currentSum;
        uint64_t maxPossibleSum;
        
        vector< TrailElement > trail;
        
        unordered_map<int, unsigned int> positionInTrail; 
        
        unsigned int w1;
        unsigned int w2;        
        
        bool notificationIdLit( Solver&, Literal, int );
        bool notificationAggregateLit( Solver&, Literal, int );
        
        void addInTrail( Solver& solver, Literal lit, bool isId, int pos );                
        bool inferLiteral( Solver& solver, Literal lit, bool isId, int pos );
        void checkFalseInference( Solver& solver, Literal lit );
        void checkTrueInference( Solver& solver, Literal lit );
        
        inline uint64_t getWeight( unsigned int pos ) const { assert_msg( pos < weights.size(), pos << ">=" << weights.size() ); return weights[ pos ]; }
        inline uint64_t getBound( unsigned int pos ) const { assert_msg( pos < bounds.size(), pos << ">=" << bounds.size() ); return bounds[ pos ]; }
        inline Literal getLiteral( unsigned int pos ) const { assert_msg( pos < literals.size(), pos << ">=" << literals.size() ); return literals[ pos ]; }
        
        void addBinaryClauses( Solver& solver );
        
        void computeInferences( vector< Literal >& inferences, Literal lit );
        void computeInferences( vector< Literal >& inferences, unsigned int position, bool isId, int type );                
              
        void clean( Solver& solver );                
        
        inline Literal getLiteralFromTrailElement( TrailElement te )
        {
            int pos = te.position();
            if( te.isId() )
                return pos > 0 ? getId( pos ) : getId( -pos ).getOppositeLiteral();
            else
                return pos > 0 ? getLiteral( pos ) : getLiteral( -pos ).getOppositeLiteral();
        }
        
        inline void updateCounters( TrailElement te )
        {
            trace_msg( multiaggregates, 2, "Updating counters. Considering element in position " << te.position() << " which is " << ( te.isId() ? "" : "not" ) << " id" );
            if( te.isId() || !te.propagated() )
                return;
            int pos = te.position();
            unsigned int p;
            if( pos > 0 )
            {
                p = pos;
                assert_msg( currentSum >= getWeight( p ), "Error while restoring current sum: " << currentSum << " < " << getWeight( p ) );
                currentSum -= getWeight( p );                
            }
            else
            {   
                p = -pos;                
                assert_msg( maxPossibleSum + getWeight( p ) <= ( unsigned int ) accumulate( weights.begin(), weights.end(), 0 ), "Error while restoring max possible sum: " << maxPossibleSum << " + " << getWeight( p ) << " > " << accumulate( weights.begin(), weights.end(), 0 ) );
                maxPossibleSum += getWeight( p );
            }        
        }
        
        template<class T>
        void swap( vector< T >& elems, unsigned int j )
        {
            T tmp = elems[ j ]; 
            elems[ j ] = elems[ j + 1 ]; 
            elems[ j + 1 ] = tmp;
        }
        
        void bubblesort( vector< Literal >& lits, vector< uint64_t >& ws, bool descOrd )
        {
            assert( lits.size() == ws.size() );
            for( int i = lits.size() - 1; i > 1; i-- ) {
                bool swapped = false;
                if( descOrd )
                {
                    for( int j = 1; j < i; j++ )
                        if( ws[ j ] < ws[ j + 1 ] ) { swap( lits, j ); swap( ws, j ); swapped = true; }                    
                }
                else
                {
                    for( int j = 1; j < i; j++ )
                        if( ws[ j ] > ws[ j + 1 ] ) { swap( lits, j ); swap( ws, j ); swapped = true; }                    
                }
                if( !swapped )
                    break;
            }
        }        
        
        void computeWatches( Solver& solver );
        
        #ifndef NDEBUG
        bool checkUndefined( Solver& solver );
        bool isInTrail( int position, bool isId );
        #endif
};

#endif

