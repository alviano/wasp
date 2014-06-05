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
class Literal
{
    friend ostream &operator<<( ostream &, const Literal & );

    public:
        static const Literal null;
        static const Literal conflict;

        inline explicit Literal( Var v = 0, unsigned int sign = POSITIVE );
        
        inline Literal( const Literal& );
        inline ~Literal();

        inline int getId() const;
        inline unsigned int getIndex() const { return ( getVariable() << 1 ) | getSign(); }

        inline bool operator==( const Literal& ) const;
        inline bool operator!=( const Literal& ) const;

        inline Var getVariable() const;
        
        /**
         * This function returns 0 if the literal is positive, 1 otherwise.
         */
        inline unsigned int getSign() const;
        
        inline Literal getOppositeLiteral();

        inline bool isPositive() const;
        inline bool isNegative() const;        
        
    private:
        
        /**
         * This function returns 1 if the literal is positive, 0 otherwise.
         */
        inline unsigned int getOppositeSign() const;                
        
        unsigned int signedVariable;
};

Literal::Literal(
    const Literal& l ) : signedVariable( l.signedVariable )
{
}

Literal::Literal(
    Var v,
    unsigned int sign ) : signedVariable( ( v << 1 ) | sign )
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

bool
Literal::isNegative() const
{
    return !isPositive();
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

Var
Literal::getVariable() const
{
    return ( signedVariable >> 1 );
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

Literal
Literal::getOppositeLiteral()
{
    return Literal( getVariable(), getOppositeSign() );
}

int
Literal::getId() const
{
    return isPositive() ? getVariable() : -getVariable();
}

#endif