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

#include <iostream>
#include <list>
#include <string>
using namespace std;

#include "Constants.h"
#include "WatchedList.h"

class Clause;
class LearningStrategy;
class NegativeLiteral;
class PositiveLiteral;
class Solver;

/**
 * This class represents a Literal.
 * 
 */
class Literal
{
    friend ostream &operator<<( ostream& out, const Literal& lit );
    
    public:
        inline Literal();

        inline WatchedList< Clause* >::iterator addWatchedClause( Clause* clause );
        inline void eraseWatchedClause( WatchedList< Clause* >::iterator it );

        virtual bool isTrue() const = 0;
        virtual bool isFalse() const = 0;
        virtual bool isUndefined() const = 0;

        virtual bool setFalse() = 0;
        virtual bool setTrue() = 0;
        
        virtual unsigned int getDecisionLevel() const = 0;
        virtual void setDecisionLevel( unsigned int ) = 0;
        
        virtual unsigned int getOrderInThePropagation() const = 0;
        virtual void setOrderInThePropagation( unsigned int order ) = 0;
        
        virtual NegativeLiteral* getNegativeLiteral() = 0;
        virtual PositiveLiteral* getPositiveLiteral() = 0;
        
        inline void setImplicant( Clause* clause );        
        inline void setOppositeLiteral( Literal* lit );
        
        inline void onConflict( LearningStrategy* strategy );        
        void onLearning( LearningStrategy* strategy );        
        void unitPropagation( Solver& solver );
        
    protected:
        
        /**
         * This variable stores the clause which derived the literal.
         */
        Clause* implicant;        
                
        /**
         * The literal with the opposite polarity.
         */
        Literal* oppositeLiteral;

        /**
         * List of all clauses in which the literal is watched.
         */
        WatchedList< Clause* > watchedClauses;       
        
        virtual ostream& print( ostream& out ) const = 0;
        
};

Literal::Literal() : implicant( NULL )
{
}

WatchedList< Clause* >::iterator
Literal::addWatchedClause(
    Clause* clause )
{
    watchedClauses.add( clause );
}

void
Literal::eraseWatchedClause(
    WatchedList< Clause* >::iterator it )
{
    watchedClauses.erase( it );
}

void
Literal::setImplicant(
    Clause* clause )
{
    implicant = clause;
}

void
Literal::setOppositeLiteral(
    Literal* lit )
{
    oppositeLiteral = lit;
}

void
Literal::onConflict( 
    LearningStrategy* strategy )
{
    assert( "OppositeLiteral is not set properly." && oppositeLiteral != NULL );    
    onLearning( strategy );
    oppositeLiteral->onLearning( strategy );
}

#endif	/* LITERAL_H */

