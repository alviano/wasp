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

#include <cassert>
#include <iostream>
#include "util/Constants.h"
#include "WatchedList.h"
#include "util/Assert.h"
#include "Reason.h"
#include "LiteralInt.h"

//#include <boost/heap/fibonacci_heap.hpp>
using namespace std;
//using namespace boost::heap;

class Clause;
class HeuristicCounterForLiteral;
class HeuristicCounterFactoryForLiteral;
class Learning;
class Literal;
class Component;
class PostPropagator;
class Propagator;

class Variable;
class Solver;

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
class Variable : public Reason
{
    friend ostream& operator<<( ostream&, const Variable& );

    public:
        
        inline Variable( unsigned id );
        ~Variable();

        inline unsigned getId() const { return id; }
        
        inline bool isTrue() const;
        inline bool isFalse() const;        
        
        inline bool setTruthValue( TruthValue truth );

        inline bool isUndefined() const;
        inline void setUndefined();
        inline void setUndefinedBrutal();

        inline bool isImplicant( const Reason* reason ) const;
        inline bool hasImplicant() const;
        inline Reason* getImplicant() const { return implicant; }
        inline void setImplicant( Reason* reason );

        inline unsigned int getDecisionLevel() const;
        inline void setDecisionLevel( unsigned int decisionLevel );

        inline TruthValue getTruthValue() const;
        inline TruthValue getCachedTruthValue() const;

//        void onLearning( Learning* strategy );
        
        inline void addWatchedClause( Clause* clause, unsigned int sign );        
        inline void findAndEraseWatchedClause( Clause* clause, unsigned int sign );
        
        inline void addClause( Clause* clause, unsigned int sign );
        inline void findAndEraseClause( Clause* clause, unsigned int sign );
        
        void setHeuristicCounterForLiterals( HeuristicCounterFactoryForLiteral* heuristicCounterFactoryForLiteral );
        inline const HeuristicCounterForLiteral* getPositiveHeuristicCounter() const;
        inline const HeuristicCounterForLiteral* getNegativeHeuristicCounter() const;
        
        inline unsigned int numberOfNegativeWatchedClauses() const;
        inline unsigned int numberOfPositiveWatchedClauses() const;
        inline unsigned int numberOfWatchedClauses( unsigned int sign ) const;
        inline unsigned int numberOfOccurrences( unsigned int sign ) const;
        inline unsigned int numberOfOccurrences() const;
        
        inline Clause* getOccurrence( unsigned index, unsigned sign ) { return allOccurrences[ sign ][ index ]; }
        
        inline uint64_t getSignature() const { return static_cast< uint64_t >( 1 ) << ( ( id - 1 ) & 63 ); }
        inline Activity& activity() { return act; }
        inline const Activity& activity() const { return act; }
        
        inline void setHandle( heap_handle h ){ handle = h; }
        inline heap_handle getHandle(){ return handle; }
        
        inline void setInHeap( bool v ){ inHeap = v; }
        inline bool isInHeap(){ return inHeap; }
        
        inline unsigned int& visited(){ return visitedInLearning; }
        inline unsigned int visited() const{ return visitedInLearning; }
        
        inline const Clause* getDefinition() const { return definition; }     
        inline void setEliminated( unsigned int sign, Clause* definition );
        inline unsigned int getSignOfEliminatedVariable() const { return signOfEliminatedVariable; }
        inline bool hasBeenEliminated() const { return signOfEliminatedVariable != MAXUNSIGNEDINT; }
        inline unsigned int cost() const { return numberOfOccurrences( POSITIVE ) * numberOfOccurrences( NEGATIVE ); }
        
        inline bool inTheSameComponent( const Variable* var ) const { return component != NULL && component == var->component; } 
        inline bool isInCyclicComponent() const { return component != NULL; }
        inline void setComponent( Component* c ){ assert( component == NULL ); component = c; }
        inline Component* getComponent() { return component; }
        
        void onLearningForUnfounded( Learning& learning );
        inline void addPropagator( Propagator* p, unsigned int sign, int position ) { propagators[ sign ].push_back( pair< Propagator*, int >( p, position ) ); }
        inline void addPostPropagator( PostPropagator* p, unsigned int sign, int position ) { postPropagators[ sign ].push_back( pair< PostPropagator*, int >( p, position ) ); }
        inline void addLiteralInShortClause( Literal lit, unsigned int sign ) { binaryClauses[ sign ].push_back( lit ); }
                
        bool isFrozen() const { return frozen; }
        void setFrozen() { frozen = true; }
        
        void unitPropagation( Solver& solver );
        void shortPropagation( Solver& solver );
        void propagation( Solver& solver );
        void postPropagation( Solver& solver );
        
        void propagateAtLevelZero( Solver& solver );
        void propagateAtLevelZeroSatelite( Solver& solver );
        
        void removeAllClauses( Solver& solver );
        void markAllClauses( Solver& solver );
        
        void checkSubsumptionForClause( Solver& solver, Clause* clause, unsigned sign );
        inline void clearOccurrences();
        
        virtual void onLearning( Learning* strategy, Literal lit );
        virtual bool onNavigatingLiteralForAllMarked( Learning* strategy, Literal lit );                
        
    private:

        virtual ostream& print( ostream& out ) const;
        inline Variable( const Variable& );

        unsigned id;

        /**
         * The level in the backtracking tree in which this literal has been derived.  
         */        
        unsigned int decisionLevel;
        
        /**
         * The truth value of the literal. It can assume three values: UNDEFINED, TRUE or FALSE.
         */
        TruthValue truthValue;        
        
        /**
         * This variable stores the clause which derived the literal.
         */
        Reason* implicant;
        
        /**
         * Position POSITIVE of this vector contains the watchedList of the positive literal associated with this variable.
         * Position NEGATIVE of this vector contains the watchedList of the negative literal associated with this variable.
         */
        WatchedList< Clause* > watchedLists[ 2 ];
        
        /**
         * Position POSITIVE of this vector contains the occurrences of the positive literal associated with this variable.
         * Position NEGATIVE of this vector contains the occurrences of the negative literal associated with this variable.
         */        
        Vector< Clause* > allOccurrences[ 2 ];
        
        /**
         * Position POSITIVE of this vector contains the occurrences of the positive literal associated with this variable.
         * Position NEGATIVE of this vector contains the occurrences of the negative literal associated with this variable.
         */        
        Vector< pair< PostPropagator*, int > > postPropagators[ 2 ];
        
        Vector< pair< Propagator*, int > > propagators[ 2 ];
        
        Vector< Literal > binaryClauses[ 2 ];
//        uint64_t signature;
        
        Activity act;
        
        heap_handle handle;
        bool inHeap;
        
        unsigned int visitedInLearning;
        
        Clause* definition;
        unsigned int signOfEliminatedVariable;
        
        Component* component;
        bool frozen;
};

#include "LiteralImpl.h"
bool Comparator::operator()( const Variable* v1, const Variable* v2 ) const{ return v1->activity() > v2->activity(); }
bool EliminationComparator::operator()( const Variable* v1, const Variable* v2 ) const{ return v1->cost() < v2->cost(); }

Variable::Variable(
    unsigned id_ ) :
    id( id_ ),
    decisionLevel( 0 ),
    truthValue( UNDEFINED ),
    implicant( NULL ),
    act( 0.0 ),
    handle( 0 ),
    inHeap( false ),
    visitedInLearning( 0 ),
    definition( NULL ),
    signOfEliminatedVariable( MAXUNSIGNEDINT ),
    component( NULL ),
    frozen( false )
{
//    signature = ( ( uint64_t ) 1 ) << ( ( id - 1 ) & 63 );
}


Variable::Variable(
    const Variable& )
{
    assert( 0 );
}

bool
Variable::isTrue() const
{
    return getTruthValue() == TRUE;
}

bool
Variable::isFalse() const
{
    return getTruthValue() == FALSE;
}

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
    const Reason* clause ) const
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
    Reason* reason )
{
    implicant = reason;
}

unsigned int
Variable::getDecisionLevel() const
{
    return decisionLevel;
}

void
Variable::setDecisionLevel(
    unsigned int dl )
{
    decisionLevel = dl;
}

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
    allOccurrences[ sign ].push_back( clause );
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

void
Variable::clearOccurrences()
{
    if( signOfEliminatedVariable == ELIMINATED_BY_DISTRIBUTION )
        return;

    allOccurrences[ 0 ].clearAndDelete();
    allOccurrences[ 1 ].clearAndDelete();
}

#endif /* VARIABLE_H */
