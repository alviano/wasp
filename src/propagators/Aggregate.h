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

#ifndef WASP_AGGREGATE_H
#define WASP_AGGREGATE_H

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

#define POS 1
#define NEG -1

class Aggregate : public Propagator, public Reason
{
    friend ostream& operator<<( ostream& out, const Aggregate& aggregate );
    public:
        inline Aggregate();
        inline ~Aggregate() {}
        
        virtual bool onLiteralFalse( Solver& solver, Literal lit, PropagatorData p );
        virtual void simplifyAtLevelZero( Solver& ) {}

        
        inline unsigned int size() const { return literals.size() - 1; }

        inline void addLiteral( Literal lit, uint64_t weight ) { literals.push_back( lit ); weights.push_back( weight ); watched.push_back( true ); }        
        
        inline Literal getLiteral( unsigned int i ) const { assert( i < literals.size() ); return literals[ i ]; }
        inline uint64_t getWeight( unsigned int i ) const { assert( i < weights.size() ); return weights[ i ]; }
        
        unsigned int getLevelOfBackjump( const Solver& solver, uint64_t bound );
        bool updateBound( Solver& solver, uint64_t bound );
        inline bool isTrue() const;        
        
        inline Literal operator[]( unsigned int idx ) const { assert_msg( ( idx > 0 && idx < literals.size() ), "Index is " << idx << " - literals: " << literals.size() ); return literals[ idx ]; }
        inline Literal& operator[]( unsigned int idx ) { assert_msg( ( idx > 0 && idx < literals.size() ), "Index is " << idx << " - literals: " << literals.size() ); return literals[ idx ]; }

//        inline void setCounterW1( unsigned int value ){ counterW1 = value; }
//        inline void setCounterW2( unsigned int value ){ counterW2 = value; }
        virtual void reset( const Solver& solver );
        
        virtual void onLearning( const Solver& solver, Learning* strategy, Literal lit );
        virtual bool onNavigatingLiteralForAllMarked( const Solver& solver, Learning* strategy, Literal lit );
        virtual void onNavigatingForUnsatCore( const Solver& solver, vector< unsigned int >& visited, unsigned int numberOfCalls, Literal lit );
        
        inline void sort() { mergesort( 2, literals.size() - 1 ); }                
        
    protected:
        virtual ostream& print( ostream& out ) const;

    private:
        inline Aggregate( const Aggregate& orig );
        
        vector< Literal > literals;
        vector< uint64_t > weights;
        vector< bool > watched;
        
        int active;
        
        int64_t counterW1;
        int64_t counterW2;
        
        unsigned int umax;
        Vector< int > trail;
        
        Literal literalOfUnroll;

//        void createClauseFromTrail( Literal lit );
        #ifndef NDEBUG
        bool checkDecisionLevelsOrder( const Solver& solver, const Clause& clause ) const;        
        #endif

        void mergesort( int left, int right )
        {
            if( left < right )
            {
                int center = ( left + right ) / 2;
                mergesort( left, center );
                mergesort( center + 1, right );
                merge( left, center, right );
            }
        }

        void merge( int left, int center, int right )
        {
            Literal* auxLiterals = new Literal[ right + 1 ];
            uint64_t* auxWeights = new uint64_t[ right + 1 ];

            int i, j;
            for( i = center + 1; i > left; i-- )
            {                    
                auxLiterals[ i - 1 ] = literals[ i - 1 ];
                auxWeights[ i - 1 ] = weights[ i - 1 ];
            }
            for( j = center; j < right; j++ )
            {
                auxLiterals[ right + center - j ] = literals[ j + 1 ];
                auxWeights[ right + center - j ] = weights[ j + 1 ];
            }
            for( int k = left; k <= right; k++ )
            {
                if( auxWeights[ j ] > auxWeights[ i ] )
                {
                    literals[ k ] = auxLiterals[ j ];
                    weights[ k ] = auxWeights[ j-- ];
                }
                else
                {
                    literals[ k ] = auxLiterals[ i ];
                    weights[ k ] = auxWeights[ i++ ];
                }
            }

            delete [] auxWeights;
            delete [] auxLiterals;
        }
};

Aggregate::Aggregate() : Propagator(), active( 0 ), counterW1( 0 ), counterW2( 0 ), umax( 1 ), literalOfUnroll( Literal::null )
{
    literals.push_back( Literal::null );
    weights.push_back( 0 );    
    watched.push_back( false );
}

bool
Aggregate::isTrue() const
{
    int position = -1;
    unsigned int index = 1; 
    int64_t counter = ( position > 0 ? counterW2 : counterW1 );
    return ( counter < ( int64_t ) weights[ index ] );
}

#endif
