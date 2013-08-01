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
 * Created on 31 July 2013, 20.17
 */

#ifndef VARIABLE_H
#define	VARIABLE_H

#include <cassert>
#include <iostream>
using namespace std;

#include "Constants.h"
#include "Literal.h"
class Solver;

class Variable
{
    friend ostream &operator<<( ostream& out, const Variable& var );
    public:        
        inline Variable();
        inline Variable( const string& name );        
        inline ~Variable();
        
        inline bool isFalse() const;
        inline bool isTrue() const;
        inline bool isUndefined() const;
        
        inline bool setFalse();
        inline bool setTrue();
        inline void setUndefined();
        
        inline unsigned int getDecisionLevel() const;
        inline void setDecisionLevel( unsigned int decisionLevel );        
        
        inline Literal* getNegativeLiteral();
        inline void setNegativeLiteral( Literal* );
        inline Literal* getPositiveLiteral();
        inline void setPositiveLiteral( Literal* );
        
        inline void unitPropagation( Solver& solver );
        
    private:
        
        unsigned int decisionLevel;
        
        string name;
        
        TruthValue truthValue;        
        
        Literal* negativeLiteral;
        Literal* positiveLiteral;
};

#endif	/* VARIABLE_H */

Variable::Variable() :
    decisionLevel( 0 ),
    name(""),
    negativeLiteral( NULL ),
    positiveLiteral( NULL ),
    truthValue( UNDEFINED )
{
}

Variable::Variable(
    const string& n ) :
    decisionLevel( 0 ),
    name( n ),
    negativeLiteral( NULL ),
    positiveLiteral( NULL ),
    truthValue( UNDEFINED )
{
}

Variable::~Variable()
{
    if( negativeLiteral )
        delete negativeLiteral;
    
    if( positiveLiteral )
        delete positiveLiteral;
}

bool
Variable::isFalse() const
{
    return truthValue == FALSE;
}

bool
Variable::isTrue() const
{
    return truthValue == TRUE;
}

bool
Variable::isUndefined() const
{
    return truthValue == UNDEFINED;
}

bool
Variable::setFalse()
{
    if( truthValue == TRUE )
        return false;
    truthValue = FALSE;
    return true;
}

bool
Variable::setTrue()
{
    if( truthValue == FALSE )
        return false;
    truthValue = TRUE;
    return true;
}

void
Variable::setUndefined()
{
    assert( "This assert is not strictly necessary. By the way, this assignment is useless." && truthValue != UNDEFINED );
    truthValue = UNDEFINED;
}

void
Variable::unitPropagation(
    Solver& solver )
{
    if( isTrue() )
    {
        assert( "Negative literal is not initialized." && negativeLiteral != NULL );
        negativeLiteral->unitPropagation( solver );
    }
    else
    {
        assert( "Positive literal is not initialized." && positiveLiteral != NULL );
        positiveLiteral->unitPropagation( solver );
    }
}

Literal*
Variable::getNegativeLiteral()
{
    assert( negativeLiteral != NULL );
    return negativeLiteral;
}

Literal*
Variable::getPositiveLiteral()
{
    assert( positiveLiteral != NULL );
    return positiveLiteral;
}

void
Variable::setNegativeLiteral(
    Literal* negLit )
{
    negativeLiteral = negLit;
}

void
Variable::setPositiveLiteral(
    Literal* posLit )
{
    positiveLiteral = posLit;
}

unsigned int
Variable::getDecisionLevel() const
{
    assert( "Undefined variables have no decision level." && !isUndefined() );
    return decisionLevel;
}

void
Variable::setDecisionLevel(
    unsigned int dl )
{
    decisionLevel = dl;
}