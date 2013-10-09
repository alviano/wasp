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
#include <vector>
using namespace std;

#include "Constants.h"
#include "WatchedList.h"
#include "heuristics/counters/HeuristicCounterForLiteral.h"
#include "heuristics/DecisionHeuristic.h"
#include "heuristics/factories/HeuristicCounterFactoryForLiteral.h"

class Clause;
class LearningStrategy;
class NegativeLiteral;
class PositiveLiteral;
class Solver;
class HeuristicVisitor;
class WaspRule;

/**
 * This class represents a Literal.
 * 
 */
class Literal
{
    friend ostream &operator<<( ostream& out, const Literal& lit );
    
    public:
        inline Literal();
        inline virtual ~Literal()
        {
            if( heuristicCounter )
                delete heuristicCounter;
        }

        inline WatchedList< Clause* >::iterator addWatchedClause( Clause* clause );
        inline void eraseWatchedClause( WatchedList< Clause* >::iterator it );

        inline void addWaspRule( WaspRule* waspRule );
        
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

        virtual bool isImplicant( const Clause* clause ) const = 0;
        
        virtual void setImplicant( Clause* clause ) = 0;
        inline void setOppositeLiteral( Literal* lit );
        
        inline void setHeuristicCounter( HeuristicCounterFactoryForLiteral* );
        
        virtual void onLearning( LearningStrategy* strategy ) = 0;
        
        inline void onNavigatingLearnedClause();
        inline void onNavigatingImplicationGraph();
        inline void onAging( unsigned int );
        
        void visitForHeuristic( HeuristicVisitor* );
        
        void unitPropagation( Solver& solver );
        virtual void supportPropagation( Solver& solver );
        
        inline Literal* getOppositeLiteral();
        
        inline const HeuristicCounterForLiteral* getHeuristicCounter() const;
        
        inline unsigned int numberOfWatchedClauses() const;        
        
    protected:
        
        /**
         * This variables stores the heuristic value for this literal.
         */
        HeuristicCounterForLiteral* heuristicCounter;

        /**
         * The literal with the opposite polarity.
         */
        Literal* oppositeLiteral;

        /**
         * List of all clauses in which the literal is watched.
         */
        WatchedList< Clause* > watchedClauses;
        
        vector< WaspRule* > allWaspRules;

        virtual ostream& print( ostream& out ) const = 0;
        
    private:
        
        Literal( const Literal& )
        {
            assert( "The copy constructor has been disabled." && 0 );
        }
        
};

Literal::Literal() : heuristicCounter( NULL )
{
}

WatchedList< Clause* >::iterator
Literal::addWatchedClause(
    Clause* clause )
{
    return watchedClauses.add( clause );
}

void
Literal::eraseWatchedClause(
    WatchedList< Clause* >::iterator it )
{
    watchedClauses.erase( it );
}

void
Literal::setOppositeLiteral(
    Literal* lit )
{
    oppositeLiteral = lit;
}

void
Literal::onNavigatingLearnedClause()
{
    assert( "Heuristic has not been set." && heuristicCounter != NULL );
    heuristicCounter->onNavigatingLearnedClause();
}

void
Literal::onNavigatingImplicationGraph()
{
    assert( "Heuristic has not been set." && heuristicCounter != NULL );
    heuristicCounter->onNavigatingImplicationGraph();
}

void
Literal::onAging(
    unsigned int value )
{
    assert( "Heuristic has not been set." && heuristicCounter != NULL );
    heuristicCounter->onAging( value );
}

void
Literal::setHeuristicCounter( 
    HeuristicCounterFactoryForLiteral* heuristicCounterFactoryForLiteral )
{
    heuristicCounter = heuristicCounterFactoryForLiteral->createHeuristicCounter();
}

const HeuristicCounterForLiteral*
Literal::getHeuristicCounter() const
{
    return heuristicCounter;
}

Literal*
Literal::getOppositeLiteral()
{
    return oppositeLiteral;
}

unsigned int
Literal::numberOfWatchedClauses() const
{
    return watchedClauses.size();
}

void
Literal::addWaspRule(
    WaspRule* waspRule )
{
    allWaspRules.push_back( waspRule );
}

#endif	/* LITERAL_H */

