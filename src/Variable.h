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

using namespace std;

class Clause;
class HeuristicCounterForLiteral;
class HeuristicCounterFactoryForLiteral;
class Learning;
class Literal;

/**
 * This class stores all information about a Variable.
 */
class Variable
{
    friend ostream& operator<<( ostream&, const Variable& );

    public:
        
        inline Variable( unsigned id );
        inline Variable( unsigned id, const string& name );
        ~Variable();

        inline unsigned getId() const { return id; }

        inline bool isHidden() const;
        inline void setName( string& name );
        
        inline bool isTrue() const;
        
//        inline bool setFalse();
//        inline bool setTrue();
        
        inline bool setTruthValue( TruthValue truth );

        inline bool isUndefined() const;
        inline void setUndefined();

        inline bool isImplicant( const Clause* clause ) const;
        inline bool hasImplicant() const;
        inline Clause* getImplicant() const { return implicant; }
        inline void setImplicant( Clause* clause );

        inline unsigned int getDecisionLevel() const;
        inline void setDecisionLevel( unsigned int decisionLevel );

//        inline unsigned int getOrderInThePropagation() const;
//        inline void setOrderInThePropagation( unsigned int order );

        inline TruthValue getTruthValue() const;

        void onLearning( Learning* strategy );
        
        inline void addWatchedClause( Clause* clause, unsigned int sign );        
        inline void eraseWatchedClause( Clause* clause, unsigned int sign );
        inline void findAndEraseWatchedClause( Clause* clause, unsigned int sign );
        
        void setHeuristicCounterForLiterals( HeuristicCounterFactoryForLiteral* heuristicCounterFactoryForLiteral );
        inline const HeuristicCounterForLiteral* getPositiveHeuristicCounter() const;
        inline const HeuristicCounterForLiteral* getNegativeHeuristicCounter() const;
        
        inline void onAging( unsigned int value, unsigned int sign );
        
        inline unsigned int numberOfNegativeWatchedClauses() const;
        inline unsigned int numberOfPositiveWatchedClauses() const;
        inline unsigned int numberOfWatchedClauses( unsigned int sign ) const;
        
        inline void unitPropagationStart();
        inline bool unitPropagationHasNext();
        inline Clause* unitPropagationNext();        
        
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
        string name;

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
};

Variable::Variable(
    unsigned id_ ) :
    id( id_ ),
    decisionLevel( 0 ),
    name( "" ),
    truthValue( UNDEFINED ),
    implicant( NULL )    
{
}

Variable::Variable(
    unsigned id_, 
    const string& n ) :
    id( id_ ),
    decisionLevel( 0 ),
    name( n ),
    truthValue( UNDEFINED ),
    implicant( NULL )
{
}

Variable::Variable(
    const Variable& )
{
    assert( 0 );
}

bool
Variable::isHidden() const
{
    return name == "";
}

void
Variable::setName(
    string& n )
{
    name = n;
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
    return truthValue == TRUE;
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
    assert( "Truth Value parameter is neither true nor false." && ( truth == TRUE || truth == FALSE ) );
    if( truthValue == UNDEFINED )
    {
        truthValue = truth;
        return true;
    }
    
    /*  
     * If truthValue is not undefined we assume that there is a conflict.
     * The solver should check if the variable is already assigned.
     * In case you don't want to perform this check in the solver, you can
     * simply return truthValue == truth;
     */
    //assert( "A variable is assigned twice with the same truthValue." && truthValue != truth );
    return truthValue == truth;
}

bool
Variable::isUndefined() const
{
    return truthValue == UNDEFINED;
}

void
Variable::setUndefined()
{
    assert( "This assert is not strictly necessary. By the way, this assignment is useless." && truthValue != UNDEFINED );
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
    return truthValue;
}

void
Variable::addWatchedClause(
    Clause* clause,
    unsigned int sign )
{
    assert( "A literal must be true or false." && sign <= 1 );    
    watchedLists[ sign ].add( clause );
}

void
Variable::eraseWatchedClause( 
    Clause* clause,
    unsigned int sign )
{
    assert( "A literal must be true or false." && sign <= 1 );
    watchedLists[ sign ].remove( clause );
}

void
Variable::findAndEraseWatchedClause( 
    Clause* clause,
    unsigned int sign )
{
    assert( "A literal must be positive or negative." && sign <= 1 );
    watchedLists[ sign ].findAndRemove( clause );
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
    assert( "A literal must be true or false." && sign <= 1 );
    return watchedLists[ sign ].size();
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
    assert( "Variable to propagate is Undefined." && !this->isUndefined() );
    assert( FALSE == 1 && TRUE == 2 );

    #ifndef NDEBUG
    unsigned int sign = ( truthValue >> 1 );
    assert( sign <= 1 );
    assert( truthValue == TRUE ? sign == NEGATIVE : sign == POSITIVE );
    #endif
    
    watchedLists[ ( truthValue >> 1 ) ].startIteration();
}

bool
Variable::unitPropagationHasNext()
{
    assert( "Variable to propagate is Undefined." && !this->isUndefined() );
    assert( FALSE == 1 && TRUE == 2 );

    #ifndef NDEBUG
    unsigned int sign = ( truthValue >> 1 );
    assert( sign <= 1 );
    assert( truthValue == TRUE ? sign == NEGATIVE : sign == POSITIVE );
    #endif
    
    return watchedLists[ ( truthValue >> 1 ) ].hasNext();
}

Clause*
Variable::unitPropagationNext()
{
    assert( "Variable to propagate is Undefined." && !this->isUndefined() );
    assert( FALSE == 1 && TRUE == 2 );

    #ifndef NDEBUG
    unsigned int sign = ( truthValue >> 1 );
    assert( sign <= 1 );
    assert( truthValue == TRUE ? sign == NEGATIVE : sign == POSITIVE );
    #endif
    
    return watchedLists[ ( truthValue >> 1 ) ].next();
}

#endif /* VARIABLE_H */
