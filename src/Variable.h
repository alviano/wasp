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

#ifndef VARIABLE_H
#define VARIABLE_H

//#include <cassert>
//#include <iostream>
//#include "util/Constants.h"
//#include "WatchedList.h"
//#include "util/Assert.h"
//#include "Reason.h"
//#include "LiteralInt.h"

//#include <boost/heap/fibonacci_heap.hpp>
//using namespace std;
//using namespace boost::heap;

//class Clause;
//class HeuristicCounterForLiteral;
//class HeuristicCounterFactoryForLiteral;
//class Learning;
//class Literal;
//class Component;
//class PostPropagator;
//class Propagator;
//
//class Variable;
//class Solver;
//
//class Comparator
//{
//public:
//    inline bool operator()( const Variable* n1, const Variable* n2 ) const;
//};
//
//class EliminationComparator
//{
//public:
//    inline bool operator()( const Variable* n1, const Variable* n2 ) const;
//};
//
////typedef fibonacci_heap< Variable*, compare< Comparator > >::handle_type heap_handle;
//typedef unsigned int heap_handle;

/**
 * This class stores all information about a Variable.
 */
//class Variable : public Reason
//{
//    friend ostream& operator<<( ostream&, const Variable& );

//    public:
        
//        inline Variable( unsigned id );
//        virtual ~Variable();
//
//        inline unsigned getId() const { return id; }
//
////        void onLearning( Learning* strategy );
//        
//        inline void addWatchedClause( Clause* clause, unsigned int sign );        
//        inline void findAndEraseWatchedClause( Clause* clause, unsigned int sign );
//        
//        inline void addClause( Clause* clause, unsigned int sign );
//        inline void findAndEraseClause( Clause* clause, unsigned int sign );        
//        
//        inline uint64_t getSignature() const { return static_cast< uint64_t >( 1 ) << ( ( id - 1 ) & 63 ); }        
        
//    private:
//};

//bool Comparator::operator()( const Variable* v1, const Variable* v2 ) const{ return v1->activity() > v2->activity(); }
//bool EliminationComparator::operator()( const Variable* v1, const Variable* v2 ) const{ return v1->cost() < v2->cost(); }

//Variable::Variable(
//    unsigned id_ ) :
//    id( id_ ),
//    decisionLevel( 0 ),
//    truthValue( UNDEFINED ),
//    implicant( NULL ),
//    act( 0.0 ),
//    handle( 0 ),
//    inHeap( false ),
//    visitedInLearning( 0 ),
//    definition( NULL ),
//    signOfEliminatedVariable( MAXUNSIGNEDINT ),
//    component( NULL ),
//    frozen( false )
//{
////    signature = ( ( uint64_t ) 1 ) << ( ( id - 1 ) & 63 );
//}



//void
//Variable::addWatchedClause(
//    Clause* clause,
//    unsigned int sign )
//{
//    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
//    watchedLists[ sign ].add( clause );
//}
//
//void
//Variable::findAndEraseWatchedClause( 
//    Clause* clause,
//    unsigned int sign )
//{
//    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
//    watchedLists[ sign ].findAndRemove( clause );
//}
//
//
//void
//Variable::addClause(
//    Clause* clause,
//    unsigned int sign )
//{
//    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
//    allOccurrences[ sign ].push_back( clause );
//}
//
//void
//Variable::findAndEraseClause( 
//    Clause* clause,
//    unsigned int sign )
//{
//    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
//    allOccurrences[ sign ].findAndRemove( clause );
//}




#endif /* VARIABLE_H */
