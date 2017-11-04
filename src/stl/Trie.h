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

#ifndef WASP_TRIE_H
#define WASP_TRIE_H

#include <cassert>
#include <climits>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

class Node {

    public:
        inline Node( int content_for_node ) : content( content_for_node ), label( UINT_MAX ){}
        ~Node() 
        { 
            for( unordered_map< int, Node* >::iterator it = childrenMap.begin(); it != childrenMap.end(); ++it )
            {
                assert( it->second );
                delete it->second;
            }
        }

        inline int getContent() const { return content; }        
        inline bool hasLabel() const { return label != UINT_MAX; }
        inline unsigned int getLabel() const { return label; }
        inline void setLabel( unsigned int l ) { label = l; }

        Node* findChild( int c )
        { 
            if( childrenMap.find( c ) != childrenMap.end() )
            { 
                return childrenMap[ c ]; 
            }
            return NULL;
        }

        inline void addChild( Node* child )
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
        Trie() : current( NULL ), numberOfLabels( 0 ), disabled( false ) { root = new Node( UINT_MAX ); }

        ~Trie()
        { 
            if( root )                
                clear();
        }
        
        void clear()
        {            
            delete root;
            root = NULL;
        }
        
        void disable()
        {
            clear();
            disabled = true;
        }
        
        void startInsertion()
        {
            if( disabled )
                return;
            assert( current == NULL );
            current = root;
        }
        inline void addElement( int );        
        inline bool endInsertion();
        inline unsigned int endInsertionGetId();        

//        inline unsigned int addElements( const vector< int >& );
    private:
        Node* root;
        Node* current;
        unsigned int numberOfLabels;
        bool disabled;
};

void
Trie::addElement(
    int element )
{
    if( disabled )
        return;
    Node* node = current->findChild( element );
    if( node == NULL )
    {
        node = new Node( element );
        current->addChild( node );
    }
    assert( node != NULL );
    current = node;    
}

bool
Trie::endInsertion()
{
    if( disabled )
        return false;

    assert( current != NULL );
    
    bool tmp = true;
    if( !current->hasLabel() )
    {
        current->setLabel( ++numberOfLabels );
        tmp = false;
    }
    current = NULL;
    return tmp;
    
//    if( !current->hasLabel() )
//    {
//        current->setLabel( ++numberOfLabels );        
//    }
//    
//    unsigned int value = current->getLabel();
//    
//    current = NULL;
//    return value;
}

unsigned int
Trie::endInsertionGetId()
{
    if( disabled )
        return UINT_MAX;

    assert( current != NULL );
    
    unsigned int value = UINT_MAX;
    if( !current->hasLabel() )
        current->setLabel( ++numberOfLabels );
    else
        value = current->getLabel();
    current = NULL;
    return value;
}

//unsigned int
//Trie::addElements(
//    const vector< int >& elements )
//{
//    startInsertion();
//    for( unsigned int i = 0; i < elements.size(); i++ )
//    {
//        addElement( elements[ i ] );
//    }
//    return endInsertion();
//}

#endif
