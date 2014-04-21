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

#ifndef LITERAL_H
#define LITERAL_H

#include <cstdint>
#include <cassert>
#include <sstream>
#include <string>
using namespace std;

#include "util/Constants.h"
#include "Variable.h"

class Clause;

class Literal
{
    friend ostream &operator<<( ostream &, const Literal & );

    public:
        static const Literal null;
        static const Literal conflict;

        static inline Literal createFromAssignedVariable( Variable* );
        static inline Literal createOppositeFromAssignedVariable( Variable* );

        inline explicit Literal( Variable* variable = NULL, unsigned int sign = POSITIVE );
        
        inline Literal( const Literal& );
        inline ~Literal();

        inline int getId() const { return isPositive() ? getVariable()->getId() : -getVariable()->getId(); }

        inline bool isTrue() const;
        inline bool isFalse() const;
        inline bool isUndefined() const;

        inline bool setTrue();

        inline bool operator==( const Literal& ) const;
        inline bool operator!=( const Literal& ) const;

        inline void addWatchedClause( Clause* clause );
        inline void findAndEraseWatchedClause( Clause* clauses );

        inline void addClause( Clause* clause );
        inline void findAndEraseClause( Clause* clauses );   
        
        inline void addPropagator( Propagator* p, int position );
        inline void addPostPropagator( PostPropagator* p, int position );
        
        inline unsigned int getDecisionLevel() const;

        inline bool isImplicant( const Clause* clause ) const;
        
//        inline void onLearning( Learning* strategy );

        inline void onAging( unsigned int );

        inline unsigned int numberOfWatchedClauses() const;
        inline unsigned int numberOfOccurrences() const;
        
        inline Variable* getVariable();
        inline const Variable* getVariable() const;        
        
        /**
         * This function returns 0 if the literal is positive, 1 otherwise.
         */
        inline unsigned int getSign() const;
        
        inline Literal getOppositeLiteral();
        
        inline Clause* getOccurrence( unsigned int idx ) { return getVariable()->getOccurrence( idx, getSign() ); }
        
        inline bool isPositive() const;
        inline bool isNegative() const { return !isPositive(); }
        
        inline void checkSubsumptionForClause( Solver& solver, Clause* clause ) { getVariable()->checkSubsumptionForClause( solver, clause, getSign() ); }

    private:
        
        /**
         * This function returns 1 if the literal is positive, 0 otherwise.
         */
        inline unsigned int getOppositeSign() const;                
        
        uintptr_t signedVariable;
};

Literal
Literal::createFromAssignedVariable(
    Variable* v )
{
    assert( TRUE == 2 && FALSE == 1 );
    return Literal( v, v->getTruthValue() & 1 );
}

Literal
Literal::createOppositeFromAssignedVariable(
    Variable* v )
{
    assert( TRUE == 2 && FALSE == 1 );
    return Literal( v, ~( v->getTruthValue() ) & 1 );
}

Literal::Literal(
    const Literal& l ) : signedVariable( l.signedVariable )
{
}

Literal::Literal(
    Variable* v,
    unsigned int sign ) : signedVariable( reinterpret_cast< uintptr_t >( v ) | sign )
{
    assert( POSITIVE == 0 && NEGATIVE == 1 );
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
    assert_msg( getVariable() != NULL, "Variable has not been set." );
    return ( getVariable()->getTruthValue() << getSign() ) & 2;
}

bool
Literal::isFalse() const
{
    assert_msg( getVariable() != NULL, "Variable has not been set." );
    return ( getVariable()->getTruthValue() >> getSign() ) & 1;
}

bool
Literal::isUndefined() const
{    
    assert_msg( getVariable() != NULL, "Variable has not been set." );
    return getVariable()->isUndefined();
}

unsigned int
Literal::getDecisionLevel() const
{
    assert_msg( getVariable() != NULL, "Variable has not been set." );
    return getVariable()->getDecisionLevel();
}

bool
Literal::isImplicant(
    const Clause* clause ) const
{
    assert_msg( getVariable() != NULL, "Variable has not been set." );
    return getVariable()->isImplicant( clause );
}

bool
Literal::setTrue()
{
    assert( "Variable has not been set." && getVariable() != NULL );
    TruthValue truth = ( TRUE | CACHE_TRUE ) >> getSign();    
    return getVariable()->setTruthValue( truth );    
}

//void
//Literal::onLearning( 
//    Learning* strategy )
//{
//    assert( "Variable has not been set." && getVariable() != NULL );
//    getVariable()->onLearning( strategy );
//}

Literal
Literal::getOppositeLiteral()
{
    return Literal( getVariable(), getOppositeSign() );
}

unsigned int
Literal::numberOfWatchedClauses() const
{
    assert( "Variable has not been set." && getVariable() != NULL );
    return getVariable()->numberOfWatchedClauses( getSign() );
}

unsigned int
Literal::numberOfOccurrences() const
{
    assert( "Variable has not been set." && getVariable() != NULL );
    return getVariable()->numberOfOccurrences( getSign() );
}

void
Literal::addWatchedClause(
    Clause* clause )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->addWatchedClause( clause, getSign() );
}

void
Literal::findAndEraseWatchedClause(
    Clause* clause )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->findAndEraseWatchedClause( clause, getSign() );
}

void
Literal::addClause(
    Clause* clause )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->addClause( clause, getSign() );
}

void
Literal::addPropagator(
    Propagator* p,
    int position )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->addPropagator( p, getSign(), position );
}


void
Literal::addPostPropagator(
    PostPropagator* p,
    int position )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->addPostPropagator( p, getSign(), position );
}

void
Literal::findAndEraseClause(
    Clause* clause )
{
    assert( "Variable has not been set." && getVariable() != NULL );
    getVariable()->findAndEraseClause( clause, getSign() );
}

#endif
