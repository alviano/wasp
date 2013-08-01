/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, Wolfgang Faber, Nicola Leone, Francesco Ricca, and Marco Sirianni.
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

/* 
 * File:   WatchedList.h
 * Author: Carmine Dodaro
 *
 * Created on 24 July 2013, 17.25
 */

#ifndef WATCHEDLIST_H
#define	WATCHEDLIST_H

#include <cassert>
#include <iostream>

#include "stl/List.h"
#include "Constants.h"
using namespace std;

template< class T >
class WatchedList : private List< T >
{
	public:
        typedef typename List< T >::iterator iterator;
        typedef typename List< T >::const_iterator const_iterator;
	    inline WatchedList();

	    using List< T >::size;
        using List< T >::begin;
        using List< T >::end;
        using List< T >::erase;
        
        inline iterator add( T element );
        
        inline bool hasNext();
        inline T next();
        inline void startIteration();
	    
	private:
	    WatchedList( const WatchedList& );
	    WatchedList& operator=( const WatchedList& );
        iterator it;
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
typename WatchedList< T >::iterator
WatchedList< T >::add( 
    T element )
{
    List< T >::push_front( element );
    return List< T >::begin();
}

template< class T >
bool
WatchedList< T >::hasNext()
{
    return it != end();
}

template< class T >
T
WatchedList< T >::next()
{
    
    return *( it++ );
}

template< class T >
void
WatchedList< T >::startIteration()
{
    it = begin();
}

#endif	/* WATCHEDLIST_H */

