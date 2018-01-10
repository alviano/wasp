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

#ifndef WASP_WEIGHTCONSTRAINT_H
#define WASP_WEIGHTCONSTRAINT_H

#include <iostream>
#include <vector>
#include "../stl/Vector.h"
using namespace std;

class WeightConstraint
{
    friend ostream& operator<<( ostream& out, const WeightConstraint& rule );
    public:            
        inline WeightConstraint( unsigned int id_, uint64_t bound_ ) : id( id_ ), bound( bound_ ), currentValue( 0 ), maxPossibleValue( 0 ), umax( 0 ) {}

        inline bool sameSizeOfInternalVectors() const { return literals.size() == weights.size(); }
        inline void setBound( uint64_t bound_ ) { bound = bound_; }
        inline uint64_t getBound() const { return bound; }
        inline unsigned int getId() const { return id; }
        inline uint64_t getMaxPossibleValue() const { return maxPossibleValue; }
        inline uint64_t getCurrentValue() const { return currentValue; }
        inline unsigned int size() const { return literals.size(); }
        
        inline void decrementMaxPossibleValue( uint64_t value ) { maxPossibleValue -= value; }
        inline void incrementMaxPossibleValue( uint64_t value ) { maxPossibleValue += value; }

        inline void decrementBound( uint64_t value ) { assert( bound >= value ); bound -= value; }
        inline void incrementCurrentValue( uint64_t value ) { currentValue += value; }        

        inline int getLiteral( unsigned int pos ) const { assert( pos < literals.size() ); return literals[ pos ]; }
        inline uint64_t getWeight( unsigned int pos ) const { assert( pos < weights.size() ); return weights[ pos ]; }
        inline void setWeight( unsigned int pos, uint64_t value ) { assert( pos < weights.size() ); weights[ pos ] = value; }

        inline bool canBeAClause() const { return ( weights.front() == weights.back() ) && ( weights.front() != 0 ) && ( bound / weights.front() == 1 ); }

        inline void addNegativeLiteral( unsigned int idLit ) { literals.push_back( -idLit ); }
        inline void addPositiveLiteral( unsigned int idLit ) { literals.push_back( idLit ); }
        
        inline void resize( unsigned int newSize );
        inline void overwrite( unsigned int toChange, unsigned int newPosition );        
        
        inline void addNegativeLiteralCount( unsigned int idLit );
        inline void addPositiveLiteralCount( unsigned int idLit );

        inline void addNegativeLiteralWeight( uint64_t weightLit );
        inline void addPositiveLiteralWeight( uint64_t weightLit );

        inline bool isTrue() { return currentValue >= bound; }
        inline bool isFalse() { return maxPossibleValue < bound; }
        inline void sort() { mergesort( 0, literals.size() - 1 ); }        

        inline void remove(){ umax = MAXUNSIGNEDINT; }            
        inline bool isRemoved() const { return umax == MAXUNSIGNEDINT; }

    private:
        friend class Solver;
        vector< int > literals;
        vector< uint64_t > weights;

        unsigned int id;
        uint64_t bound;
        uint64_t currentValue;
        uint64_t maxPossibleValue;
        unsigned int umax;
        
        inline void mergesort( int left, int right );
        inline void merge( int left, int center, int right );
};

void
WeightConstraint::mergesort(
    int left,
    int right )
{
    if( left < right )
    {
        int center = ( left + right ) / 2;
        mergesort( left, center );
        mergesort( center + 1, right );
        merge( left, center, right );
    }
}

void
WeightConstraint::merge(
    int left,
    int center,
    int right )
{
    int* auxLiterals = new int[ right + 1 ];
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

void
WeightConstraint::resize(
    unsigned int newSize )
{
    literals.resize( newSize );
    weights.resize( newSize );
}

void
WeightConstraint::overwrite(
    unsigned int toChange,
    unsigned int newPosition )
{
    assert( toChange < literals.size() );
    assert( literals.size() == weights.size() );
    literals[ toChange ] = literals[ newPosition ];
    weights[ toChange ] = weights[ newPosition ];
}

void
WeightConstraint::addNegativeLiteralCount(
    unsigned int idLit )
{
    addNegativeLiteral( idLit );
    addNegativeLiteralWeight( 1 );
}

void WeightConstraint::addPositiveLiteralCount(
    unsigned int idLit )
{
    addPositiveLiteral( idLit );
    addPositiveLiteralWeight( 1 );
}

void WeightConstraint::addNegativeLiteralWeight(
    uint64_t weightLit )
{
    if( weightLit > bound )
        weightLit = bound;
    weights.push_back( weightLit );
}

void WeightConstraint::addPositiveLiteralWeight(
    uint64_t weightLit )
{
    if( weightLit > bound )
        weightLit = bound;
    weights.push_back( weightLit );                
}

#endif