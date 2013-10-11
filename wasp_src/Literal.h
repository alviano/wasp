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
 * File:   Literal.h
 * Author: Carmine Dodaro
 *
 * Created on 21 July 2013, 16.50
 */


#ifndef LITERAL_H
#define	LITERAL_H

#include <cstdint>
#include <cassert>
using namespace std;

#include "Constants.h"
#include "Variable.h"

class Clause;
class Solver;

class Literal
{
    friend ostream &operator<<( ostream &, const Literal & );

	public:

		// Instantiate a positive literal
		inline Literal( Variable* = NULL );

		// Instantiate a negative literal. The boolean value is ignored.
		inline Literal( Variable*, bool );

		inline Literal( const Literal& );
		inline ~Literal();

		inline bool isTrue() const;
		inline bool isFalse() const;
		inline bool isUndefined() const;

        inline bool setTrue();

		inline bool operator==( const Literal& ) const;
        inline bool operator!=( const Literal& ) const;

		inline void addWatchedClause( Clause* clause );
        inline void eraseWatchedClause( Clause* clause );

//        inline void addWaspRule( WaspRule* waspRule );

        inline unsigned int getDecisionLevel() const;
//        inline void setDecisionLevel( unsigned int );

        inline unsigned int getOrderInThePropagation() const;
        inline void setOrderInThePropagation( unsigned int order );

        inline bool isImplicant( const Clause* clause ) const;
//        inline void setImplicant( Clause* clause );
        
        inline void onLearning( LearningStrategy* strategy );

        inline void onNavigatingLearnedClause();
        inline void onNavigatingImplicationGraph();
        inline void onAging( unsigned int );

        inline void visitForHeuristic( HeuristicVisitor* );

        inline void unitPropagation( Solver& solver );
//        void supportPropagation( Solver& solver );
        
        inline unsigned int numberOfWatchedClauses() const;
        inline Variable* getVariable();
        inline const Variable* getVariable() const;
        
	private:
        
        inline Literal( unsigned int sign, Variable* variable );
        
        /**
         * This function returns 0 if the literal is positive, 1 otherwise.
         */
        inline unsigned int getSign() const;
        
        /**
         * This function returns 1 if the literal is positive, 0 otherwise.
         */
        inline unsigned int getOppositeSign() const;                
        
        inline bool isPositive() const;
        
        inline Literal getOppositeLiteral();

        uintptr_t signedVariable;
};

Literal::Literal(
    const Literal& l )
{
    this->signedVariable = l.signedVariable; 
}

Literal::Literal(
    Variable* v ) : signedVariable( reinterpret_cast< uintptr_t >( v ) )
{
    assert( isPositive() );
    assert( getVariable() == v );
}

Literal::Literal(
    Variable* v,
    bool ) : signedVariable( reinterpret_cast< uintptr_t >( v ) | 1 )
{
    assert( !isPositive() );
    assert( getVariable() == v );
}

Literal::Literal(
    unsigned int sign,
    Variable* v ) : signedVariable( reinterpret_cast< uintptr_t >( v ) | sign )
{
    assert( sign == 0 || sign == 1 );
    assert( ( sign == 0 && isPositive() ) || ( sign == 1 && !isPositive() ) );
    assert( getVariable() == v );
}

Literal::~Literal()
{
}

bool
Literal::isPositive() const
{
    return !( signedVariable & 1 );
}

unsigned int
Literal::getSign() const
{
    assert( "Variable has not been set." && signedVariable != 0 );
	return signedVariable & 1;
}

unsigned int
Literal::getOppositeSign() const
{
    assert( "Variable has not been set." && signedVariable != 0 );
    return ( ~signedVariable ) & 1;
}

const Variable*
Literal::getVariable() const
{
    return reinterpret_cast< Variable* >( signedVariable & ( ~1 ) );
}

Variable*
Literal::getVariable()
{
    return reinterpret_cast< Variable* >( signedVariable & ( ~1 ) );
}

bool
Literal::operator==(
    const Literal& literal ) const
{
    return signedVariable == literal.signedVariable;
}

bool
Literal::operator!=(
    const Literal& literal ) const
{
    return !( *this == literal );
}

bool
Literal::isTrue() const
{
    assert( "Variable has not been set." && getVariable() != NULL );
    return ( getVariable()->getTruthValue() << getSign() ) & 2;
}

bool
Literal::isFalse() const
{
    assert( "Variable has not been set." && getVariable() != NULL );
    return ( getVariable()->getTruthValue() >> getSign() ) & 1;
}

bool
Literal::isUndefined() const
{
    assert( "Variable has not been set." && getVariable() != NULL );
    return getVariable()->isUndefined();
}

unsigned int
Literal::getDecisionLevel() const
{
    assert( "Variable has not been set." && getVariable() != NULL );
    return getVariable()->getDecisionLevel();
}

//void
//Literal::setDecisionLevel( 
//    unsigned int dl )
//{
//    assert( "Variable has not been set." && getVariable() != NULL );
//    getVariable()->setDecisionLevel( dl );
//}

unsigned int
Literal::getOrderInThePropagation() const
{
    assert( "Variable has not been set." && getVariable() != NULL );
    return getVariable()->getOrderInThePropagation();
}

void
Literal::setOrderInThePropagation( 
    unsigned int order )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->setOrderInThePropagation( order );
}

bool
Literal::isImplicant(
    const Clause* clause ) const
{
    assert( "Variable has not been set." && getVariable() != NULL );
    return getVariable()->isImplicant( clause );
}

//void
//Literal::setImplicant(
//    Clause* clause )
//{
//    assert( "Variable has not been set." && getVariable() != NULL );
//    return getVariable()->setImplicant( clause );
//}

bool
Literal::setTrue()
{
    assert( "Variable has not been set." && getVariable() != NULL );
    TruthValue truth = TRUE >> getSign();    
    return getVariable()->setTruthValue( truth );    
}

void
Literal::onLearning( 
    LearningStrategy* strategy )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->onLearning( strategy );
}

void
Literal::onNavigatingLearnedClause()
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->onNavigatingLearnedClause( getSign() );
}

void
Literal::onNavigatingImplicationGraph()
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->onNavigatingImplicationGraph( getSign() );
}

void
Literal::onAging( 
    unsigned int value )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->onAging( value, getSign() );
}

Literal
Literal::getOppositeLiteral()
{
    Literal lit( getOppositeSign(), getVariable() );
    return lit;    
}

unsigned int
Literal::numberOfWatchedClauses() const
{
    assert( "Variable has not been set." && getVariable() != NULL );
    return getVariable()->numberOfWatchedClauses( getSign() );
}

void
Literal::addWatchedClause(
    Clause* clause )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->addWatchedClause( clause, getSign() );
}

void
Literal::eraseWatchedClause(
    Clause* clause )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->eraseWatchedClause( clause, getSign() );
}

void
Literal::visitForHeuristic( 
    HeuristicVisitor* heuristicVisitor )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->visitForHeuristic( heuristicVisitor );
}

void
Literal::unitPropagation(
    Solver& solver )
{
    assert( "Literal to propagate is Undefined." && !this->isUndefined() );
    assert( this->isTrue() );
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->unitPropagation( solver, getOppositeLiteral(), getOppositeSign() );
}

#endif	/* LITERAL_H */
