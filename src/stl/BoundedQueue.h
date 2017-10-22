/***********************************************************************************[BoundedQueue.h]
 Glucose -- Copyright (c) 2009, Gilles Audemard, Laurent Simon
                CRIL - Univ. Artois, France
                LRI  - Univ. Paris Sud, France
 
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

/**
 * DISCLAIMER: This code is part of the software GLUCOSE 3.0. It has been adapted in order to work
 * with WASP data structures.
 * 
 */

#ifndef WASP_BOUNDEDQUEUE_H
#define WASP_BOUNDEDQUEUE_H

#include "Vector.h"

template < class T >
class bqueue
{        
    public:
        inline bqueue() :  first( 0 ), last( 0 ), sumOfQueue( 0 ),
                           maxSize_( 0 ), queueSize( 0 ), expComputed( false )
        {
        } 

        inline void initSize( int size ) { growTo( size ); exp = 2.0 / ( size + 1 ); } // Init size of bounded size queue
        inline T peek() { assert( queueSize > 0 ); return elems[ last ]; }
        inline unsigned long long getSum() const { return sumOfQueue; }
        inline unsigned int getAvg() const { return ( unsigned int )( sumOfQueue / ( ( unsigned long long ) queueSize ) ); }
        inline int maxSize() const { return maxSize_; }
        inline int isValid() const { return ( queueSize == maxSize_ ); }
        int size() { return queueSize; }        
        
        inline void push( T x );
        inline void pop();
        inline double getAvgDouble() const;        
        inline void growTo( int size );
        inline double getAvgExp();
        inline void fastClear();
        inline void clear( bool dealloc = false );

    private:
        Vector< T > elems;
        int first;
        int last;
        unsigned long long sumOfQueue;
        int maxSize_;
        int queueSize; // Number of current elements (must be < maxsize !)
        bool expComputed;
        double exp, value;
};

template < class T >
void
bqueue< T >::push(
    T x )
{
    expComputed = false;
    if( queueSize == maxSize_ )
    {
        assert( last == first ); // The queue is full, next value to enter will replace oldest one
        sumOfQueue -= elems[ last ];
        if ( ( ++last ) == maxSize_ )
            last = 0;
    }
    else 
        queueSize++;

    sumOfQueue += x;
    elems[ first ] = x;
    if( ( ++first ) == maxSize_ )
    {
        first = 0;
        last = 0;
    }
}

template < class T >
void
bqueue< T >::pop()
{
    sumOfQueue -= elems[ last ];
    queueSize--;
    if( ( ++last ) == maxSize_)
        last = 0;
}

template < class T >
double
bqueue< T >::getAvgDouble() const
{
    double tmp = 0;
    for( int i = 0; i < elems.size(); i++ )
        tmp += elems[ i ];
    
    return tmp / elems.size();
}

template < class T >
void
bqueue< T >::growTo(
    int size )
{
    //		elems.growTo(size); 
    first = 0;
    maxSize_ = size; 
    queueSize = 0;
    last = 0;
    elems.reserve( size );
    for( int i = 0; i < size; i++ )
        elems.push_back( 0 );         
}

template < class T >
double
bqueue< T >::getAvgExp()
{
    if( expComputed )
        return value;
    double a = exp;
    value = elems[ first ];
    for(int i = first; i < maxSize_; i++ )
    {
        value += a * ( ( double ) elems[ i ] );
        a = a * exp;
    }

    for( int i = 0; i < last; i++ )
    {
        value += a*( ( double ) elems[ i ] );
        a = a * exp;
    }
    value = value * ( 1 - exp ) / ( 1 - a );
    expComputed = true;
    return value;
}

template < class T >
void
bqueue< T >::fastClear()
{
    first = 0;
    last = 0;
    queueSize = 0;
    sumOfQueue = 0;
} // to be called after restarts... Discard the queue

template < class T >
void
bqueue< T >::clear(
    bool dealloc )
{
    dealloc ? elems.clearAndDelete() : elems.clear();
    first = 0;
    maxSize_ = 0;
    queueSize = 0;
    sumOfQueue = 0;
}

#endif
