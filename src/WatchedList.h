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

#ifndef WATCHEDLIST_H
#define	WATCHEDLIST_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Constants.h"
using namespace std;

template< class T >
class WatchedList : private vector< T >
{
	public:
	    inline WatchedList();

	    using vector< T >::size;
        using vector< T >::empty;
        
        inline void add( T element );
        inline void remove( T element );
        inline void findAndRemove( T element );
        
        inline bool hasNext();
        inline T next();
        inline void startIteration();
	    
	private:
	    WatchedList( const WatchedList& );
	    WatchedList& operator=( const WatchedList& );
        unsigned nextIndex;
};

template< class T >
WatchedList< T >::WatchedList()
{
}

template< class T >
WatchedList< T >::WatchedList(
    const WatchedList& )
{
    assert( 0 );
}
 
template< class T >
WatchedList< T >& 
WatchedList< T >::operator=(
    const WatchedList< T >& )
{
    assert( 0 );
    return *this;
}

template< class T >
void
WatchedList< T >::add( 
    T element )
{
    assert( find( vector< T >::begin(), vector< T >::end(), element ) == vector< T >::end() );
    vector< T >::push_back( element );
}

template< class T >
void
WatchedList< T >::remove(
    T element )
{
    assert( nextIndex > 0 && nextIndex <= size() && vector< T >::operator[]( nextIndex - 1 ) == element );
    vector< T >::operator[]( --nextIndex ) = vector< T >::back();
    vector< T >::pop_back();
}

template< class T >
void
WatchedList< T >::findAndRemove(
    T element )
{
    typename vector< T >::iterator it = find( vector< T >::begin(), vector< T >::end(), element );
    assert( it != vector< T >::end() );
    *it = vector< T >::back();
    vector< T >::pop_back();
}

template< class T >
bool
WatchedList< T >::hasNext()
{
    return nextIndex < size();
}

template< class T >
T
WatchedList< T >::next()
{    
    return vector< T >::operator[]( nextIndex++ );
}

template< class T >
void
WatchedList< T >::startIteration()
{
    nextIndex = 0;
    
}

#endif	/* WATCHEDLIST_H */

