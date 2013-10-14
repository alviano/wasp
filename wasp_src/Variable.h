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
 * File:   Variable.h
 * Author: Carmine Dodaro
 *
 * Created on 10 October 2013, 16.10
 */

#ifndef VARIABLE_H
#define	VARIABLE_H

#include <cassert>
#include <iostream>
#include "Constants.h"
#include "WatchedList.h"
#include "heuristics/counters/HeuristicCounterForLiteral.h"

using namespace std;

class Clause;
class HeuristicCounterForLiteral;
class HeuristicCounterFactoryForLiteral;
class HeuristicVisitor;
class LearningStrategy;
class Literal;
class Solver;

/**
 * This class stores all information about a Variable.
 */
class Variable
{
    friend ostream& operator<<( ostream&, const Variable& );

    public:
        
        inline Variable();
        inline Variable( const string& name );
        ~Variable();

        inline bool isHidden() const;
        inline void setName( string& name );
        
        inline bool isTrue() const;
        
//        inline bool setFalse();
//        inline bool setTrue();
        
        inline bool setTruthValue( TruthValue truth );

        inline bool isUndefined() const;
        inline void setUndefined();

        inline bool isImplicant( const Clause* clause ) const;
        inline void setImplicant( Clause* clause );

        inline unsigned int getDecisionLevel() const;
        inline void setDecisionLevel( unsigned int decisionLevel );

//        inline unsigned int getOrderInThePropagation() const;
//        inline void setOrderInThePropagation( unsigned int order );

        inline TruthValue getTruthValue() const;

        void onLearning( LearningStrategy* strategy );
        
        inline void addWatchedClause( Clause* clause, unsigned int sign );        
        inline void eraseWatchedClause( Clause* clause, unsigned int sign );
        inline void findAndEraseWatchedClause( Clause* clause, unsigned int sign );
        
        void setHeuristicCounterForLiterals( HeuristicCounterFactoryForLiteral* heuristicCounterFactoryForLiteral );
        inline const HeuristicCounterForLiteral* getPositiveHeuristicCounter() const;
        inline const HeuristicCounterForLiteral* getNegativeHeuristicCounter() const;
        
        inline void onNavigatingLearnedClause( unsigned int sign );
        inline void onNavigatingImplicationGraph( unsigned int sign );
        inline void onAging( unsigned int value, unsigned int sign );
        
        inline unsigned int numberOfNegativeWatchedClauses() const;
        inline unsigned int numberOfPositiveWatchedClauses() const;
        inline unsigned int numberOfWatchedClauses( unsigned int sign ) const;
        
        void visitForHeuristic( HeuristicVisitor* heuristicVisitor );
        
        void unitPropagation( Solver& solver, Literal literal, unsigned int sign );
        
//    protected:
//        
//        inline const Clause* getImplicant() const;
    
    private:

        inline HeuristicCounterForLiteral* getHeuristicCounterInternal( unsigned int sign );        

        inline Variable( const Variable& );

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
         * Position 0 of this vector contains the watchedList of the positive literal associated with this variable.
         * Position 1 of this vector contains the watchedList of the negative literal associated with this variable.
         */
        WatchedList< Clause* > watchedLists[ 2 ];
        
        /**
         * Position 0 of this vector contains the heuristic counter of the negative literal associated with this variable.
         * Position 1 of this vector contains the heuristic counter of the positive literal associated with this variable.
         */
        HeuristicCounterForLiteral* heuristicCounters[ 2 ];
};

Variable::Variable() :
    decisionLevel( 0 ),
    name( "" ),
    truthValue( UNDEFINED ),
    implicant( NULL )    
{
}

Variable::Variable(
    const string& n ) :
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
    assert( "A variable is assigned twice with the same truthValue." && truthValue != truth );
    return false;
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

void
Variable::setImplicant(
    Clause* clause )
{
    implicant = clause;
}

unsigned int
Variable::getDecisionLevel() const
{
    assert( "Undefined literals have no decision level." && !isUndefined() );
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

const HeuristicCounterForLiteral*
Variable::getPositiveHeuristicCounter() const
{
    assert( "Heuristic has not been set." && heuristicCounters[ 1 ] != NULL );
    return heuristicCounters[ 0 ];
}

const HeuristicCounterForLiteral*
Variable::getNegativeHeuristicCounter() const
{
    assert( "Heuristic has not been set." && heuristicCounters[ 0 ] != NULL );
    return heuristicCounters[ 1 ];
}

HeuristicCounterForLiteral*
Variable::getHeuristicCounterInternal(
    unsigned int sign )
{
    assert( "A literal must be true or false." && sign <= 1 );
    assert( "Heuristic has not been set." && heuristicCounters[ sign ] != NULL );
    return heuristicCounters[ sign ];
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
Variable::onNavigatingLearnedClause(
    unsigned int sign )
{
    getHeuristicCounterInternal( sign )->onNavigatingLearnedClause();
}

void
Variable::onNavigatingImplicationGraph(
    unsigned int sign )
{
    getHeuristicCounterInternal( sign )->onNavigatingImplicationGraph();
}

void
Variable::onAging(
    unsigned int value,
    unsigned int sign )
{
    getHeuristicCounterInternal( sign )->onAging( value );
}

#endif /* VARIABLE_H */
