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
#define	VARIABLE_H

#include <cassert>
#include <iostream>
#include "util/Constants.h"
#include "WatchedList.h"
#include "util/Assert.h"
//#include <boost/heap/fibonacci_heap.hpp>
using namespace std;
//using namespace boost::heap;

class Clause;
class HeuristicCounterForLiteral;
class HeuristicCounterFactoryForLiteral;
class Learning;
class Literal;

class Variable;

class Comparator
{
public:
    inline bool operator()( const Variable* n1, const Variable* n2 ) const;
};

class EliminationComparator
{
public:
    inline bool operator()( const Variable* n1, const Variable* n2 ) const;
};

//typedef fibonacci_heap< Variable*, compare< Comparator > >::handle_type heap_handle;
typedef unsigned int heap_handle;

/**
 * This class stores all information about a Variable.
 */
class Variable
{
    friend ostream& operator<<( ostream&, const Variable& );

    public:
        
        inline Variable( unsigned id );
//        inline Variable( unsigned id, const string& name );
        ~Variable();

        inline unsigned getId() const { return id; }

//        inline bool isHidden() const;
//        inline void setName( string& name );
        
        inline bool isTrue() const;
        
//        inline bool setFalse();
//        inline bool setTrue();
        
        inline bool setTruthValue( TruthValue truth );

        inline bool isUndefined() const;
        inline void setUndefined();
        inline void setUndefinedBrutal();

        inline bool isImplicant( const Clause* clause ) const;
        inline bool hasImplicant() const;
        inline Clause* getImplicant() const { return implicant; }
        inline void setImplicant( Clause* clause );

        inline unsigned int getDecisionLevel() const;
        inline void setDecisionLevel( unsigned int decisionLevel );

//        inline unsigned int getOrderInThePropagation() const;
//        inline void setOrderInThePropagation( unsigned int order );

        inline TruthValue getTruthValue() const;
        inline TruthValue getCachedTruthValue() const;

        void onLearning( Learning* strategy );
        
        inline void addWatchedClause( Clause* clause, unsigned int sign );        
        inline void eraseWatchedClause( Clause* clause, unsigned int sign );
        inline void findAndEraseWatchedClause( Clause* clause, unsigned int sign );
        
        inline void addClause( Clause* clause, unsigned int sign );
        inline void eraseClause( Clause* clause, unsigned int sign );
        inline void findAndEraseClause( Clause* clause, unsigned int sign );
        
        void setHeuristicCounterForLiterals( HeuristicCounterFactoryForLiteral* heuristicCounterFactoryForLiteral );
        inline const HeuristicCounterForLiteral* getPositiveHeuristicCounter() const;
        inline const HeuristicCounterForLiteral* getNegativeHeuristicCounter() const;
        
        inline void onAging( unsigned int value, unsigned int sign );
        
        inline unsigned int numberOfNegativeWatchedClauses() const;
        inline unsigned int numberOfPositiveWatchedClauses() const;
        inline unsigned int numberOfWatchedClauses( unsigned int sign ) const;
        inline unsigned int numberOfOccurrences( unsigned int sign ) const;
        inline unsigned int numberOfOccurrences() const;
        
        inline void unitPropagationStart();
        inline bool unitPropagationHasNext();
        inline Clause* unitPropagationNext();        
        
        inline void startIterationOverOccurrences( unsigned int sign );
        inline bool hasNextOccurrence( unsigned int sign );
        inline Clause* nextOccurence( unsigned int sign );
        inline Clause* getOccurrence( unsigned int idx, unsigned int sign ) { return allOccurrences[ sign ][ idx ]; }

        inline uint64_t getSignature() const { return signature; }
        inline Activity& activity() { return act; }
        inline const Activity& activity() const { return act; }
        
        inline void setHandle( heap_handle h ){ handle = h; }
        inline heap_handle getHandle(){ return handle; }
        
        inline void setInHeap( bool v ){ inHeap = v; }
        inline bool isInHeap(){ return inHeap; }
        
        inline unsigned int& visited(){ return visitedInLearning; }
        inline const unsigned int& visited() const{ return visitedInLearning; }
        
        inline const Clause* getDefinition() const { return definition; }     
        inline void setEliminated( unsigned int sign, Clause* definition );
        inline unsigned int getSignOfEliminatedVariable() const { return signOfEliminatedVariable; }
        inline bool hasBeenEliminated() const { return signOfEliminatedVariable != MAXUNSIGNEDINT; }
        inline unsigned int cost() const { return numberOfOccurrences( POSITIVE ) * numberOfOccurrences( NEGATIVE ); }
        
    private:

        inline Variable( const Variable& );

        unsigned id;

        /**
         * The level in the backtracking tree in which this literal has been derived.  
         */        
        unsigned int decisionLevel;

        /**
         * The name associated to the literal.
         */
//        string name;

        /**
         * This variable represents the order in which the literal has been propagated.
         */
        unsigned int orderInThePropagation;
        
        /**
         * The truth value of the literal. It can assume three values: UNDEFINED, TRUE or FALSE.
         */
        TruthValue truthValue;        
        
        /**
         * This variable stores the clause which derived the literal.
         */
        Clause* implicant;
        
        /**
         * Position POSITIVE of this vector contains the watchedList of the positive literal associated with this variable.
         * Position NEGATIVE of this vector contains the watchedList of the negative literal associated with this variable.
         */
        WatchedList< Clause* > watchedLists[ 2 ];
        
        /**
         * Position POSITIVE of this vector contains the occurrences of the positive literal associated with this variable.
         * Position NEGATIVE of this vector contains the occurrences of the negative literal associated with this variable.
         */        
        WatchedList< Clause* > allOccurrences[ 2 ];
        
        uint64_t signature;
        
        Activity act;
        
        heap_handle handle;
        bool inHeap;
        
        unsigned int visitedInLearning;
        
        Clause* definition;
        unsigned int signOfEliminatedVariable;
};

bool Comparator::operator()( const Variable* v1, const Variable* v2 ) const{ return v1->activity() > v2->activity(); }
bool EliminationComparator::operator()( const Variable* v1, const Variable* v2 ) const{ return v1->cost() < v2->cost(); }

Variable::Variable(
    unsigned id_ ) :
    id( id_ ),
    decisionLevel( 0 ),
    truthValue( UNDEFINED ),
    implicant( NULL ),
    act( 0.0 ),
    inHeap( false ),
    visitedInLearning( 0 ),
    definition( NULL ),
    signOfEliminatedVariable( MAXUNSIGNEDINT )
{
    signature = ( ( uint64_t ) 1 ) << ( ( id - 1 ) & 63 );
}


Variable::Variable(
    const Variable& )
{
    assert( 0 );
}

//bool
//Variable::setFalse()
//{
//    if( truthValue == TRUE )
//        return false;
//    truthValue = FALSE;
//    return true;
//}

bool
Variable::isTrue() const
{
    return getTruthValue() == TRUE;
}

//bool
//Variable::setTrue()
//{
//    if( truthValue == FALSE )
//        return false;
//    truthValue = TRUE;
//    return true;
//}

bool
Variable::setTruthValue(
    TruthValue truth )
{
//    assert_msg( ( truth == TRUE || truth == FALSE ), "Truth Value parameter is neither true nor false. It is " << truth << "." );
    if( getTruthValue() == UNDEFINED )
    {
        truthValue = truth;
        return true;
    }
    
    /*  
     * If truthValue is not undefined we assume that there is a conflict.
     * The solver should check if the variable has been already assigned.
     */
    assert_msg( truthValue != truth, "A variable is assigned twice with the same truthValue."  );
    return false;
}

bool
Variable::isUndefined() const
{
    return getTruthValue() == UNDEFINED;
}

void
Variable::setUndefined()
{
    assert_msg( truthValue != UNDEFINED, "This assert is not strictly necessary. By the way, this assignment is useless." );
    assert( ( ( truthValue & ~UNROLL_MASK ) & UNROLL_MASK ) == UNDEFINED );
    assert( getTruthValue() == TRUE ? ( truthValue & ~UNROLL_MASK ) == CACHE_TRUE : ( truthValue & ~UNROLL_MASK ) == CACHE_FALSE );
    truthValue &= ~UNROLL_MASK;
}

void
Variable::setUndefinedBrutal()
{
    truthValue = UNDEFINED;
}

bool
Variable::isImplicant( 
    const Clause* clause ) const
{
    return !this->isUndefined() && implicant == clause;
}

bool
Variable::hasImplicant() const
{
    return implicant != NULL;
}

void
Variable::setImplicant(
    Clause* clause )
{
    implicant = clause;
}

unsigned int
Variable::getDecisionLevel() const
{
//    assert( "Undefined literals have no decision level." && !isUndefined() );
    return decisionLevel;
}

void
Variable::setDecisionLevel(
    unsigned int dl )
{
    decisionLevel = dl;
}

//unsigned int
//Variable::getOrderInThePropagation() const
//{
//    assert( "Undefined literals have not been propagated." && !isUndefined() );
//    return orderInThePropagation;
//}
//
//void
//Variable::setOrderInThePropagation(
//    unsigned int order )
//{
//    orderInThePropagation = order;
//}

TruthValue
Variable::getTruthValue() const
{
    return truthValue & UNROLL_MASK;
}

TruthValue
Variable::getCachedTruthValue() const
{
    return truthValue >> UNROLL_MASK;
}

void
Variable::addWatchedClause(
    Clause* clause,
    unsigned int sign )
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    watchedLists[ sign ].add( clause );
}

void
Variable::eraseWatchedClause( 
    Clause* clause,
    unsigned int sign )
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    watchedLists[ sign ].remove( clause );
}

void
Variable::findAndEraseWatchedClause( 
    Clause* clause,
    unsigned int sign )
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    watchedLists[ sign ].findAndRemove( clause );
}


void
Variable::addClause(
    Clause* clause,
    unsigned int sign )
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    allOccurrences[ sign ].add( clause );
}

void
Variable::eraseClause( 
    Clause* clause,
    unsigned int sign )
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    allOccurrences[ sign ].remove( clause );
}

void
Variable::findAndEraseClause( 
    Clause* clause,
    unsigned int sign )
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    allOccurrences[ sign ].findAndRemove( clause );
}

unsigned int
Variable::numberOfNegativeWatchedClauses() const
{
    return watchedLists[ 1 ].size();
}

unsigned int
Variable::numberOfPositiveWatchedClauses() const
{
    return watchedLists[ 0 ].size();
}

unsigned int
Variable::numberOfWatchedClauses(
    unsigned int sign ) const
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    return watchedLists[ sign ].size();
}

unsigned int
Variable::numberOfOccurrences(
    unsigned int sign ) const
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    return allOccurrences[ sign ].size();
}

unsigned int
Variable::numberOfOccurrences() const
{
    return allOccurrences[ NEGATIVE ].size() + allOccurrences[ POSITIVE ].size();
}

void
Variable::onAging(
    unsigned int value,
    unsigned int sign )
{
    //getHeuristicCounterInternal( sign )->onAging( value );
}

void
Variable::unitPropagationStart()
{
    assert_msg( !this->isUndefined(), "Propagating variable " << *this << ", the truth value is Undefined." );
    assert( FALSE == 1 && TRUE == 2 );

    #ifndef NDEBUG
    unsigned int sign = ( getTruthValue() >> 1 );
    assert( sign <= 1 );
    assert( getTruthValue() == TRUE ? sign == NEGATIVE : sign == POSITIVE );
    #endif
    
    watchedLists[ ( getTruthValue() >> 1 ) ].startIteration();
}

bool
Variable::unitPropagationHasNext()
{
    assert_msg( !this->isUndefined(), "Propagating variable " << *this << ", the truth value is Undefined." );
    assert( FALSE == 1 && TRUE == 2 );

    #ifndef NDEBUG
    unsigned int sign = ( getTruthValue() >> 1 );
    assert( sign <= 1 );
    assert( getTruthValue() == TRUE ? sign == NEGATIVE : sign == POSITIVE );
    #endif
    
    return watchedLists[ ( getTruthValue() >> 1 ) ].hasNext();
}

Clause*
Variable::unitPropagationNext()
{
    assert_msg( !this->isUndefined(), "Propagating variable " << *this << ", the truth value is Undefined." );
    assert( FALSE == 1 && TRUE == 2 );

    #ifndef NDEBUG
    unsigned int sign = ( getTruthValue() >> 1 );
    assert( sign <= 1 );
    assert( getTruthValue() == TRUE ? sign == NEGATIVE : sign == POSITIVE );
    #endif
    
    return watchedLists[ ( getTruthValue() >> 1 ) ].next();
}

void
Variable::startIterationOverOccurrences(
    unsigned int sign )
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    allOccurrences[ sign ].startIteration();
}

bool
Variable::hasNextOccurrence(
    unsigned int sign )
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    return allOccurrences[ sign ].hasNext();
}

Clause*
Variable::nextOccurence(
    unsigned int sign )
{
    assert_msg( sign <= 1, "The sign must be 0 or 1. Found value " << sign );
    return allOccurrences[ sign ].next();
}

void
Variable::setEliminated(
    unsigned int sign,
    Clause* def )
{
    assert_msg( sign <= 2, "The sign must be 0 or 1. Found value " << sign );
    assert( def != NULL || sign == ELIMINATED_BY_DISTRIBUTION );
    signOfEliminatedVariable = sign;
    definition = def;
    
    #ifndef NDEBUG
    bool result = 
    #endif
    setTruthValue( TRUE );    
    assert( result );
}

#endif /* VARIABLE_H */
