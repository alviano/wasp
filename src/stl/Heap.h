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

#ifndef HEAP_H
#define HEAP_H

#include <boost/heap/fibonacci_heap.hpp>
using namespace boost::heap;

template< class T >
class Heap
{
	public:
		void increase( T* v ){ heap.increase( v->getHandle(), v ); }

		void decrease( T* v ){ heap.decrease( v->getHandle(), v ); }

		void update( T* v ){ heap.update( v->getHandle(), v ); }

        void pushNoCheck( T* v )
        {
            assert( !v->isInHeap() );
            v->setHandle( heap.push( v ) );
            v->setInHeap( true );
        }
        
		void push( T* v )
        {
            if( v->isInHeap() )
                return;
            v->setHandle( heap.push( v ) );
            v->setInHeap( true );
        }

		bool empty(){ return heap.empty(); }

		void pop()
        {
            assert( heap.top()->isInHeap() );
            heap.top()->setInHeap( false );
            heap.pop();
        }
        
        unsigned int size()
        {
            return heap.size();
        }

		T* top(){ return heap.top(); }        

	private:	
		fibonacci_heap< T*, compare< Comparator > > heap;
};

#endif