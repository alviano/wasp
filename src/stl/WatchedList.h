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

#ifndef WASP_WATCHEDLIST_H
#define WASP_WATCHEDLIST_H

#include <cassert>
#include "Vector.h"
using namespace std;

template< class T >
class WatchedList : private Vector< T >
{
    public:
        inline WatchedList();

        using Vector< T >::size;
        using Vector< T >::empty;
        using Vector< T >::operator[];
        using Vector< T >::shrink;
        using Vector< T >::clearAndDelete;
        using Vector< T >::findAndRemove;
        
        inline void add( T element );
        inline void remove( unsigned index );
        
    private:
        WatchedList( const WatchedList& );
        WatchedList& operator=( const WatchedList& );
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
    assert( !Vector< T >::existElement( element ) );
    Vector< T >::push_back( element );
}

template< class T >
void
WatchedList< T >::remove(
    unsigned index )
{
    assert( index < size() );
    Vector< T >::operator[]( index ) = Vector< T >::back();
    Vector< T >::pop_back();
}

#endif    /* WATCHEDLIST_H */
