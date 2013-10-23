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

/* 
 * File:   List.h
 * Author: Mario Alviano
 *
 * Created on 24 July 2013, 17.27
 */

#ifndef LIST_H
#define	LIST_H

#include <list>

using std::list;

template< class T >
class List : private list< T >
{
public:
    typedef typename list< T >::iterator iterator;
    typedef typename list< T >::const_iterator const_iterator;
    typedef typename list< T >::reverse_iterator reverse_iterator;
    typedef typename list< T >::const_reverse_iterator const_reverse_iterator;

    inline List() : list< T >(), listSize( 0 ) {}
    
    using list< T >::begin;
    using list< T >::rbegin;
    using list< T >::end;
    using list< T >::rend;
    
    using list< T >::empty;
    
    using list< T >::front;
    using list< T >::back;
    
    using list< T >::sort;
    
    inline void clear();
    
    inline void pop_front();
    inline void pop_back();

    inline void push_front( const T& );
    inline void push_back( const T& );
    
    inline iterator erase( iterator );
    
    inline unsigned size() const { return listSize; }
    
private:
    unsigned listSize;
};

template< class T >
void
List< T >::pop_front()
{
    --listSize;
    list< T >::pop_front();
}

template< class T >
void
List< T >::pop_back()
{
    --listSize;
    list< T >::pop_back();
}

template< class T >
typename List< T >::iterator
List< T >::erase(
    typename List< T >::iterator it )
{
    --listSize;
    return list< T >::erase( it );
}

template< class T >
void
List< T >::push_front(
    const T& element )
{
    ++listSize;
    list< T >::push_front( element );
}

template< class T >
void
List< T >::push_back(
    const T& element )
{
    ++listSize;
    list< T >::push_back( element );
}

template< class T >
void
List< T >::clear()
{
    listSize = 0;
    list< T >::clear();
}

#endif	/* LIST_H */

