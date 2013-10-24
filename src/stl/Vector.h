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

#ifndef VECTOR_H
#define	VECTOR_H

#include <cassert>
#include <vector>
using std::vector;

template< class T >
class Vector : private vector< T >
{
    public:    
        inline Vector() : vector< T >() {}

        using vector< T >::back;
        using vector< T >::clear;
        using vector< T >::empty;
        using vector< T >::operator[];
        using vector< T >::pop_back;
        using vector< T >::push_back;
        using vector< T >::reserve;
        using vector< T >::size;

        inline void erase( unsigned int position );        
};

template< class T >
void
Vector< T >::erase(
    unsigned int position )
{
    assert( position < size() );
    vector< T >::operator[]( position ) = vector< T >::back();
    vector< T >::pop_back();
}

#endif	/* VECTOR_H */

