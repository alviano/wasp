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
 * File:   PositiveLiteral.h
 * Author: Carmine Dodaro
 *
 * Created on 21 July 2013, 16.55
 */

#ifndef POSITIVELITERAL_H
#define	POSITIVELITERAL_H

#include "Literal.h"
#include "NegativeLiteral.h"

/**
 * This class represents a positive literal.
 *  
 */
class PositiveLiteral : public Literal
{
    public:
        inline PositiveLiteral();
        inline PositiveLiteral( const string& name );
        inline ~PositiveLiteral();

        virtual bool isFalse() const;
        virtual bool isTrue() const;
        virtual bool isUndefined() const;

        virtual bool setFalse();
        virtual bool setTrue();
        inline void setUndefined();

        virtual NegativeLiteral* getNegativeLiteral();
        virtual PositiveLiteral* getPositiveLiteral();
        
        virtual unsigned int getDecisionLevel() const;
        virtual void setDecisionLevel( unsigned int decisionLevel );
        
        virtual unsigned int getOrderInThePropagation() const;
        virtual void setOrderInThePropagation( unsigned int order );

    private:        
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
        
        virtual ostream& print( ostream& out ) const;
};

PositiveLiteral::PositiveLiteral() :
    decisionLevel( 0 ),
    name(""),
    truthValue( UNDEFINED )
{
}

PositiveLiteral::PositiveLiteral(
    const string& n ) :
    decisionLevel( 0 ),
    name( n ),
    truthValue( UNDEFINED )
{
}

PositiveLiteral::~PositiveLiteral()
{
    if( oppositeLiteral )
        delete oppositeLiteral;
}

void
PositiveLiteral::setUndefined()
{
    assert( "This assert is not strictly necessary. By the way, this assignment is useless." && truthValue != UNDEFINED );
    truthValue = UNDEFINED;
}

#endif	/* POSITIVELITERAL_H */

