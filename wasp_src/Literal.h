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
class Variable;
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
        inline Literal( Variable* var );
//        inline Literal( const string& name );

        inline WatchedList< Clause* >::iterator addWatchedClause( Clause* clause );
        inline void eraseWatchedClause( WatchedList< Clause* >::iterator it );

        virtual bool isTrue() const = 0;
        virtual bool isFalse() const = 0;
        bool isUndefined() const;

        virtual bool setFalse() = 0;
        virtual bool setTrue() = 0;
        void setUndefined();
        
//        inline unsigned int getDecisionLevel() const;
//        inline void setDecisionLevel( unsigned int );
        
        inline Variable* getVariable();
        inline void setVariable( Variable* );
        
        inline const Clause* getImplicant() const;
        inline void setImplicant( Clause* clause );
        
//        inline void setOppositeLiteral( Literal* oppositeLiteral );        
        
        void unitPropagation( Solver& solver );
        
    protected:
        
        /**
         * The decision level of the literal.
         */
//        unsigned int decisionLevel;
        
        /**
         * This variable stores the clause which derived the literal.
         */
        Clause* implicant;
        
        Variable* variable;
        
        /**
         * The name of the literal.
         */
//        string name;
        
//        /**
//         * The literal with the opposite polarity.
//         */
//        Literal* oppositeLiteral;        
//        
//        /**
//         * A truth value assigned to the literal.
//         */
//        TruthValue truthValue;

        /**
         * List of all clauses in which the literal is watched.
         */
        WatchedList< Clause* > watchedClauses;
        
        virtual ostream& print( ostream& out ) const = 0;
        
//        inline bool setFalseInternal();
//        inline bool setTrueInternal();
//        inline void setUndefinedInternal();
//        inline void setDecisionLevelInternal( unsigned int dl );
};

Literal::Literal() : implicant( NULL )/*, name( "" ), decisionLevel( 0 ), oppositeLiteral( NULL ), truthValue( UNDEFINED )*/
{
}

Literal::Literal(
    Variable* var ) : implicant( NULL ), variable( var )/*, name( "" ), decisionLevel( 0 ), oppositeLiteral( NULL ), truthValue( UNDEFINED )*/
{
}

//Literal::Literal( 
//    const string& n ) : decisionLevel( 0 ), implicant( NULL ), name( n )/*, oppositeLiteral( NULL ), truthValue( UNDEFINED )*/
//{
//}

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

//bool
//Literal::isTrue() const
//{
//    return truthValue == TRUE;
//}
//
//bool
//Literal::isFalse() const
//{
//    return truthValue == FALSE;
//}
//
//bool
//Literal::isUndefined() const
//{
//    return truthValue == UNDEFINED;
//}

//bool
//Literal::setFalse()
//{
//    if( !setFalseInternal() )
//        return false;
//    assert( "Opposite literal is NULL." && oppositeLiteral != NULL );
//    bool result = oppositeLiteral->setTrueInternal();
//    assert( "Opposite literal can be conflictual iff current literal is also conflictual." && result );
//    return true;
//}
//
//bool
//Literal::setTrue()
//{
//    if( !setTrueInternal() )
//        return false;
//    
//    assert( "Opposite literal is NULL." && oppositeLiteral != NULL );
//    bool result = oppositeLiteral->setFalseInternal();
//    assert( "Opposite literal can be conflictual iff current literal is also conflictual." && result );
//    return true;
//}
//
//void
//Literal::setUndefined()
//{
//    setUndefinedInternal();
//    assert( "Opposite literal is NULL." && oppositeLiteral != NULL );
//    oppositeLiteral->setUndefinedInternal();
//}
//
//bool
//Literal::setFalseInternal()
//{
//    bool result = true;
//    if( truthValue == TRUE )
//        result = false;
//    truthValue = FALSE;
//    return result;
//}
//
//bool
//Literal::setTrueInternal()
//{
//    bool result = true;
//    if( truthValue == FALSE )
//        result = false;
//    
//    truthValue = TRUE;    
//    return result;
//}
//
//void
//Literal::setUndefinedInternal()
//{
//    truthValue = UNDEFINED;    
//}
//
//unsigned int
//Literal::getDecisionLevel() const
//{
//    assert( "Undefined literals have no decision level." && !isUndefined() );
//    return decisionLevel;
//}
//
//void
//Literal::setDecisionLevel(
//    unsigned int dl )
//{    
//    setDecisionLevelInternal( dl );
//    oppositeLiteral->setDecisionLevelInternal( dl );
//}
//
//void
//Literal::setDecisionLevelInternal(
//    unsigned int dl )
//{
//    decisionLevel = dl;
//}

const Clause*
Literal::getImplicant() const
{
    assert( "Undefined literals have no implicant." && !isUndefined() );
    return implicant;
}

void
Literal::setImplicant(
    Clause* clause )
{
    implicant = clause;
}

Variable*
Literal::getVariable()
{
    return variable;
}

void
Literal::setVariable(
    Variable* var )
{
    variable = var;
}

//void
//Literal::setOppositeLiteral(
//    Literal* opp )
//{
//    assert( "Opposite literal has been set." && oppositeLiteral == NULL );
//    oppositeLiteral = opp;
//}

#endif	/* LITERAL_H */

