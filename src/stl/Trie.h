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
 * File:   Trie.h
 * Author: Carmine Dodaro
 *
 * Created on 9 September 2013, 17.36
 */

#ifndef _TRIE_H
#define	_TRIE_H

#include <cassert>
#include <climits>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

class Node {

	public:
		Node( int content_for_node ) : content( content_for_node ), label( UINT_MAX ){}
		~Node() 
		{ 
			for( unordered_map< int, Node* >::iterator it = childrenMap.begin(); it != childrenMap.end(); ++it )
			{
				assert( it->second );
				delete it->second;
			}
		}

		int getContent() const { return content; }		
		bool hasLabel() const { return label != UINT_MAX; }
        unsigned int getLabel() const { return label; }
		void setLabel( unsigned int l ) { label = l; }

		Node* findChild( int c )
		{ 
			if( childrenMap.find( c ) != childrenMap.end() )
			{ 
				return childrenMap[ c ]; 
			}
			return NULL;
		}

		void addChild( Node* child )
		{
			assert( childrenMap.find( child->getContent() ) == childrenMap.end() );
			childrenMap[ child->getContent() ] = child;
		}

	private:
		int content;
		unsigned int label;
		unordered_map< int, Node* > childrenMap;
};

class Trie {
	public:
		Trie() : current( NULL ), numberOfLabels( 0 ){ root = new Node( UINT_MAX ); }

		~Trie()
        { 
			assert( root != NULL );
			delete root;
		}
        
		void startInsertion()
        {
            assert( current == NULL );
            current = root;
        }
		inline void addElement( int );		
		inline unsigned int endInsertion();

		inline unsigned int addElements( const vector< int >& );
	private:
		Node* root;
		Node* current;
        unsigned int numberOfLabels;
};

void
Trie::addElement(
	int element )
{
	Node* node = current->findChild( element );
	if( node == NULL )
	{
		node = new Node( element );
		current->addChild( node );
	}
	assert( node != NULL );
	current = node;	
}

unsigned int
Trie::endInsertion()
{
	assert( current != NULL );
    
    if( !current->hasLabel() )
	{
		current->setLabel( numberOfLabels++ );        
	}
    
    unsigned int value = current->getLabel();
    
    current = NULL;
    return value;
}

unsigned int
Trie::addElements(
	const vector< int >& elements )
{
	startInsertion();
	for( unsigned int i = 0; i < elements.size(); i++ )
	{
		addElement( elements[ i ] );
	}
	return endInsertion();
}

#endif	/* _TRIE_H */