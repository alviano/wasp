#ifndef SMALL_VECTOR_H
#define SMALL_VECTOR_H

#include <cassert>
#include <cstdio>
#include <cstring>
#include "../util/Constants.h"
#include "Vector.h"
using namespace std;

template< class T >
class SmallVector
{
    public:
        inline SmallVector();
        virtual ~SmallVector()
        {
            if( vector )
                delete [] vector;
        }

        inline unsigned int size() const { return size_; }
        inline bool empty() const { return size_ == 0; }

        inline void shrink( unsigned int newSize ) { assert( newSize <= size_ ); size_ = newSize; }
        inline void push_back( T element );
        inline void pop_back() { assert( !empty() ); --size_; }
        inline void clear() { size_ = 0; }
        inline void clearAndDelete() { delete[] vector; size_ = capacity_ = 0; vector = NULL; }
        inline void findAndRemove( T element );
                
        const T& back() const { assert( !empty() ); return vector[ size_ - 1 ]; }
        T& back() { assert( !empty() ); return vector[ size_ - 1 ]; }

        inline const T& operator[] ( unsigned int index ) const { assert( index < size_ ); return vector[ index ]; }
        inline T& operator[] ( unsigned int index ) { assert( index < size_ ); return vector[ index ]; }

        inline void swap( SmallVector< T >& other );
        
        inline bool existElement( T ) const;
        inline unsigned int findElement( T ) const;
        
        inline void reserve( unsigned capacity ) { resetCapacity( capacity ); }
        inline void sort( int (*comp)( const void*, const void* ) ) { qsort( vector, size_, sizeof( T ), comp ); }

    private:
        T* vector;
        unsigned int size_ : 16;
        unsigned int capacity_ : 16;

        SmallVector< T >& operator=( SmallVector< T >& );
        SmallVector( const SmallVector< T >& );
    
        inline void resetCapacity( unsigned int value );
};

template< class T >
SmallVector< T >::SmallVector() : vector( NULL ), size_( 0 ), capacity_( 0 )
{
}

template< class T >
void
SmallVector< T >::push_back(
    T element )
{
    if( size_ == capacity_ )
        resetCapacity( size_ + 1 );
    
    assert( size_ < capacity_ );
    vector[ size_++ ] = element;
}

template< class T >
void
SmallVector< T >::resetCapacity(
    unsigned int min_cap )
{
    if( capacity_ >= min_cap )
        return;
    unsigned int add = max( ( min_cap - capacity_ + 1 ) & ~1, ( ( capacity_ >> 1 ) + 2 ) & ~1 );   // NOTE: grow by approximately 3/2
    assert( capacity_ + add < 65536 );

    T* tmpVector = new T[ capacity_ + add ];
    memcpy( tmpVector, vector, sizeof( T ) * capacity_ );

    capacity_ += add;
    delete [] vector;
    vector = tmpVector;    
}

template< class T >
bool
SmallVector< T >::existElement(
    T elem ) const
{
    return findElement( elem ) != MAXUNSIGNEDINT;
}

template< class T >
unsigned int
SmallVector< T >::findElement(
    T elem ) const
{
    for( unsigned int i = 0; i < size_; ++i )    
        if( elem == vector[ i ] )
            return i;
    
    return MAXUNSIGNEDINT;
}

template< class T >
void
SmallVector< T >::swap(
    SmallVector< T >& other )
{
    T* tmp = other.vector; other.vector = vector; vector = tmp;    
    unsigned int tmpSize = other.size_; other.size_ = size_; size_ = tmpSize;        
    unsigned int tmpCapacity = other.capacity_; other.capacity_ = capacity_; capacity_ = tmpCapacity;
}

template< class T >
void
SmallVector< T >::findAndRemove(
    T element )
{
//    typename vector< T >::iterator it = find( vector< T >::begin(), vector< T >::end(), element );
//    assert( it != vector< T >::end() );
//    *it = vector< T >::back();
//    vector< T >::pop_back();
    assert( existElement( element ) );
    unsigned int position = findElement( element );
    assert( position < size() );
    operator[]( position ) = back();
    pop_back();
}

#endif
