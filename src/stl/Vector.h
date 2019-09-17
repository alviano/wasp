#ifndef WASP_VECTOR_H
#define WASP_VECTOR_H

#include <cassert>
#include <cstdio>
#include <cstring>
#include "../util/WaspConstants.h"
using namespace std;

template< class T >
class Vector
{
    public:
        inline Vector();
        inline ~Vector() { delete [] vector; }

        inline void reserve( unsigned int capacity ) { resetCapacity( capacity ); }
        
        inline unsigned int capacity() const { return capacity_; }
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

        inline void swap( Vector< T >& other );
        
        inline bool existElement( T ) const;
        inline unsigned int findElement( T ) const;
        
        void initFrom( const Vector< T >& v );
        void sort( int (*comparator)( T a, T b ) );
        
    private:
        T* vector;
        unsigned int size_;
        unsigned int capacity_;

        Vector< T >& operator=( Vector< T >& );
        Vector( const Vector< T >& );
    
        inline void resetCapacity( unsigned int value );
        inline static unsigned int max( unsigned int x, unsigned int y ) { return ( x > y ) ? x : y; }
};

template< class T >
Vector< T >::Vector() : vector( NULL ), size_( 0 ), capacity_( 0 )
{
}

template< class T >
void
Vector< T >::push_back(
    T element )
{
    if( size_ == capacity_ )
        resetCapacity( size_ + 1 );
    
    assert( size_ < capacity_ );
    vector[ size_++ ] = element;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
template< class T >
void
Vector< T >::resetCapacity(
    unsigned int min_cap )
{
    if( capacity_ >= min_cap )
        return;
    unsigned int add = max( ( min_cap - capacity_ + 1 ) & ~1, ( ( capacity_ >> 1 ) + 2 ) & ~1 );   // NOTE: grow by approximately 3/2

    T* tmpVector = new T[ capacity_ + add ];
    if( vector != NULL )
        memcpy( tmpVector, vector, sizeof( T ) * capacity_ );

    capacity_ += add;
    delete [] vector;
    vector = tmpVector;    
}
#pragma GCC diagnostic pop



template< class T >
bool
Vector< T >::existElement(
    T elem ) const
{
    return findElement( elem ) != MAXUNSIGNEDINT;
}

template< class T >
unsigned int
Vector< T >::findElement(
    T elem ) const
{
    for( unsigned int i = 0; i < size_; ++i )    
        if( elem == vector[ i ] )
            return i;
    
    return MAXUNSIGNEDINT;
}

template< class T >
void
Vector< T >::swap(
    Vector< T >& other )
{
    T* tmp = other.vector; other.vector = vector; vector = tmp;    
    unsigned int tmpSize = other.size_; other.size_ = size_; size_ = tmpSize;        
    unsigned int tmpCapacity = other.capacity_; other.capacity_ = capacity_; capacity_ = tmpCapacity;
}

template< class T >
void
Vector< T >::findAndRemove(
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

template< class T >
void
Vector< T >::initFrom(
    const Vector< T >& v )
{
    assert( empty() );
    assert( vector == NULL );
    assert( capacity_ == 0 );
    resetCapacity( v.size() );
    size_ = v.size();
    for( unsigned int i = 0; i < size_; ++i )
        vector[ i ] = v.vector[ i ];
}

#include <algorithm>

template< class T >
void
Vector< T >::sort(
    int (*comparator)( T a, T b ) )
{
    std::sort(vector, vector + size_, comparator );
}

#endif

