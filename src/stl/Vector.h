#ifndef W_VECTOR_H
#define W_VECTOR_H

#include <cassert>
#include <cstdio>
#include <cstring>
using namespace std;

template< class T >
class Vector
{
	public:
		inline Vector();
		inline ~Vector();

		inline unsigned int capacity() const { return capacity_; }
		inline unsigned int size() const { return size_; }
		inline bool empty() const { return size_ == 0; }

		inline void shrink( unsigned int newSize ) { assert( newSize <= size_ ); size_ = newSize; }
		inline void push_back( T element );
		inline void pop_back() { --size_; }
		inline void clear() { size_ = 0; }
				
		const T& back() const { assert( !empty() ); return vector[ size_ - 1 ]; }
		T& back() { assert( !empty() ); return vector[ size_ - 1 ]; }

		inline const T& operator[] ( unsigned int index ) const { assert( index < size_ ); return vector[ index ]; }
		inline T& operator[] ( unsigned int index ) { assert( index < size_ ); return vector[ index ]; }

		inline void swap( Vector< T* >& other ) { T* tmp = other.vector; other.vector = vector; vector = tmp; }
        
        inline bool existElement( T );
        inline unsigned int findElement( T );

	private:
		T*   vector;
	    unsigned int size_;
        unsigned int capacity_;

	   	Vector< T >& operator=( Vector< T >& );
	    Vector( const Vector< T >& );
	
		inline void resetCapacity( unsigned int value );
};

template< class T >
Vector< T >::Vector() : vector( NULL ), size_( 0 ), capacity_( 0 )
{
}

template< class T >
Vector< T >::~Vector()
{
	if( vector )
		delete [] vector;
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

inline unsigned int max( unsigned int x, unsigned int y ) { return ( x > y ) ? x : y; }
template< class T >
void
Vector< T >::resetCapacity(
	unsigned int min_cap )
{
    if( capacity_ >= min_cap )
		return;
    unsigned int add = max( ( min_cap - capacity_ + 1 ) & ~1, ( ( capacity_ >> 1 ) + 2 ) & ~1 );   // NOTE: grow by approximately 3/2

	T* tmpVector = new T[ capacity_ + add ];
	memcpy( tmpVector, vector, sizeof( T ) * capacity_ );

	capacity_ += add;
	delete [] vector;
	vector = tmpVector;	
}

template< class T >
bool
Vector< T >::existElement( T elem )
{
    return findElement( elem ) != MAXUNSIGNEDINT;
}

template< class T >
unsigned int
Vector< T >::findElement( T elem )
{
    for( unsigned int i = 0; i < size_; ++i )    
        if( elem == vector[ i ] )
            return i;
    
    return MAXUNSIGNEDINT;
}

#endif

