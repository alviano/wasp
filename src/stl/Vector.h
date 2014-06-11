#ifndef W_VECTOR_H
#define W_VECTOR_H

#include <cassert>
#include <cstdio>
#include <cstring>
#include "../util/Constants.h"
using namespace std;

template< class T, typename SizeCapacityType >
class TVector
{
    public:
        inline TVector();
        virtual ~TVector()
        {
            if( vector )
                delete [] vector;
        }

        inline SizeCapacityType capacity() const { return capacity_; }
        inline SizeCapacityType size() const { return size_; }
        inline bool empty() const { return size_ == 0; }

        inline void shrink( SizeCapacityType newSize ) { assert( newSize <= size_ ); size_ = newSize; }
        inline void push_back( T element );
        inline void pop_back() { assert( !empty() ); --size_; }
        inline void clear() { size_ = 0; }
        inline void clearAndDelete() { delete[] vector; size_ = capacity_ = 0; vector = NULL; }
        inline void findAndRemove( T element );
                
        const T& back() const { assert( !empty() ); return vector[ size_ - 1 ]; }
        T& back() { assert( !empty() ); return vector[ size_ - 1 ]; }

        inline const T& operator[] ( SizeCapacityType index ) const { assert( index < size_ ); return vector[ index ]; }
        inline T& operator[] ( SizeCapacityType index ) { assert( index < size_ ); return vector[ index ]; }

        inline void swap( TVector< T, SizeCapacityType >& other );
        
        inline bool existElement( T );
        inline SizeCapacityType findElement( T );

    private:
        T* vector;
        SizeCapacityType size_;
        SizeCapacityType capacity_;

        TVector< T, SizeCapacityType >& operator=( TVector< T, SizeCapacityType >& );
        TVector( const TVector< T, SizeCapacityType >& );
    
        inline void resetCapacity( SizeCapacityType value );
        inline static SizeCapacityType max( SizeCapacityType x, SizeCapacityType y ) { return ( x > y ) ? x : y; }
};

template< class T, typename SizeCapacityType >
TVector< T, SizeCapacityType >::TVector() : vector( NULL ), size_( 0 ), capacity_( 0 )
{
}

template< class T, typename SizeCapacityType >
void
TVector< T, SizeCapacityType >::push_back(
    T element )
{
    if( size_ == capacity_ )
        resetCapacity( size_ + 1 );
    
    assert( size_ < capacity_ );
    vector[ size_++ ] = element;
}

template< class T, typename SizeCapacityType >
void
TVector< T, SizeCapacityType >::resetCapacity(
    SizeCapacityType min_cap )
{
    if( capacity_ >= min_cap )
        return;
    SizeCapacityType add = max( ( min_cap - capacity_ + 1 ) & ~1, ( ( capacity_ >> 1 ) + 2 ) & ~1 );   // NOTE: grow by approximately 3/2

    T* tmpVector = new T[ capacity_ + add ];
    memcpy( tmpVector, vector, sizeof( T ) * capacity_ );

    capacity_ += add;
    delete [] vector;
    vector = tmpVector;    
}

template< class T, typename SizeCapacityType >
bool
TVector< T, SizeCapacityType >::existElement(
    T elem )
{
    return findElement( elem ) != MAXUNSIGNEDINT;
}

template< class T, typename SizeCapacityType >
SizeCapacityType
TVector< T, SizeCapacityType >::findElement(
    T elem )
{
    for( SizeCapacityType i = 0; i < size_; ++i )    
        if( elem == vector[ i ] )
            return i;
    
    return MAXUNSIGNEDINT;
}

template< class T, typename SizeCapacityType >
void
TVector< T, SizeCapacityType >::swap(
    TVector< T, SizeCapacityType >& other )
{
    T* tmp = other.vector; other.vector = vector; vector = tmp;    
    SizeCapacityType tmpSize = other.size_; other.size_ = size_; size_ = tmpSize;        
    SizeCapacityType tmpCapacity = other.capacity_; other.capacity_ = capacity_; capacity_ = tmpCapacity;
}

template< class T, typename SizeCapacityType >
void
TVector< T, SizeCapacityType >::findAndRemove(
    T element )
{
//    typename vector< T >::iterator it = find( vector< T >::begin(), vector< T >::end(), element );
//    assert( it != vector< T >::end() );
//    *it = vector< T >::back();
//    vector< T >::pop_back();
    assert( existElement( element ) );
    SizeCapacityType position = findElement( element );
    assert( position < size() );
    operator[]( position ) = back();
    pop_back();
}

template< class T >
class Vector : public TVector< T, unsigned int >
{
};

template< class T >
class ShortVector : public TVector< T, unsigned short int >
{
};

#endif

