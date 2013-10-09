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
 * File:   Clause.h
 * Author: Carmine Dodaro
 *
 * Created on 21 July 2013, 16.47
 */

#ifndef CLAUSE_H
#define	CLAUSE_H

#include <cassert>
#include <iostream>
#include <vector>

#include "Literal.h"
#include "learning/LearningStrategy.h"
using namespace std;

class Solver;

/**
 *  This class represents a clause.
 *  A clause is a disjunction of literals.
 *  For example (a1 v a2 v not a3) is a clause and a1, a2 are positive literals and not a3 is a negative literal.
 * 
 */
class Clause
{
    friend ostream &operator<<( ostream & out, const Clause & clause );
    
    public:
        inline Clause();
        virtual ~Clause() {}

        /**
         * Constructor of Clause which takes in input the number of literals in the clause.
         * Note that the watches are random generated.
         * 
         * @param size the numbers of literals.
         */
        inline Clause( unsigned int size );        

        inline void addLiteral( Literal* literal );

        inline void attachClause();
        inline void attachClause( unsigned int firstWatch, unsigned int secondWatch );
        inline void detachClause();

        inline void onLearning( LearningStrategy* strategy );
        void onLiteralFalse( Literal* literal, Solver& solver );

        inline unsigned int size() const;
        
        inline void visitForHeuristic( HeuristicVisitor* );
        
    protected:
        inline bool isImplicantOfALiteral() const;
        vector< Literal* > literals;
        
        virtual ostream& print( ostream& out ) const;
        
    private:
        Clause( const Clause& )
        {
            assert( "The copy constructor has been disabled." && 0 );
        }

        WatchedList< Clause* >::iterator iterator_firstWatch;
        WatchedList< Clause* >::iterator iterator_secondWatch;
        
        inline void setWatchesInRandomPositions();
        
        inline void attachFirstWatch();
        inline void attachSecondWatch();
        inline void detachFirstWatch();
        inline void detachSecondWatch();
        
        void updateWatch( Solver& solver );
        void updateSecondWatch( Solver& solver );
        
        inline void swapLiterals( unsigned int pos1, unsigned int pos2 );        
};

Clause::Clause()
{
}

Clause::Clause(
    unsigned int size )
{
    literals.reserve( size );
}

void
Clause::addLiteral(
    Literal* literal )
{
    assert( literal != NULL );
    literals.push_back( literal );
}

void
Clause::attachFirstWatch()
{
    assert( "Unary clause must be removed." && literals.size() > 1 );
    iterator_firstWatch = literals[ 0 ]->addWatchedClause( this );
    assert( "The iterator must point to this clause." && this == *iterator_firstWatch );
}

void
Clause::attachSecondWatch()
{
    assert( "Unary clause must be removed." && literals.size() > 1 );
    iterator_secondWatch = literals[ 1 ]->addWatchedClause( this );
    assert( "The iterator must point to this clause." && this == *iterator_secondWatch );
}
        
void
Clause::detachFirstWatch()
{
    assert( "The watchToDetach points to a NULL literal." && literals[ 0 ] != NULL );
    assert( "The iterator must point to this clause." && this == *iterator_firstWatch );
    literals[ 0 ]->eraseWatchedClause( iterator_firstWatch );
}
        
void
Clause::detachSecondWatch()
{
    assert( "The watchToDetach points to a NULL literal." && literals[ 1 ] != NULL );
    assert( "The iterator must point to this clause." && this == *iterator_secondWatch );
    literals[ 1 ]->eraseWatchedClause( iterator_secondWatch );
}

void
Clause::attachClause()
{
    assert( "Unary clauses must be removed." && literals.size() > 1 );
    attachFirstWatch();
    attachSecondWatch();
}

void
Clause::attachClause( 
    unsigned int first,
    unsigned int second )
{
    assert( "First watch is out of range." && first < literals.size() );
    assert( "Second watch is out of range." && second < literals.size() );
    assert( "First watch and second watch point to the same literal." && first != second );   
    
    #ifndef NDEBUG
    Literal* tmp1 = literals[ first ];
    Literal* tmp2 = literals[ second ];
    #endif

    swapLiterals( 0, first );
    if( second == 0 )
        swapLiterals( 1, first );
    else
        swapLiterals( 1, second );
    
//    if( first != 0 && second != 0 )
//    {
//        swapLiterals( 0, first );
//        if( second != 1 )
//            swapLiterals( 1, second );
//    }
//    else if( first != 0 && second == 0 )
//    {        
//        if( first == 1 )
//        {
//            //In this case second is equal to 0 and first is equal to 1. You need to do just one swap.
//            swapLiterals( 0, 1 );
//        }
//        else
//        {        
//            swapLiterals( 1, second );
//            swapLiterals( 0, first );   
//        }
//    }
//    else
//    {
//        assert( first == 0 );        
//        //Useless to do: swapLiterals( 0, first );
//        assert( second != 0 );
//        if( second != 1 )
//        {
//            swapLiterals( 1, second );
//        }
//    }

    assert( literals[ 0 ] == tmp1 );
    assert( literals[ 1 ] == tmp2 );
    
    attachFirstWatch();
    attachSecondWatch();
}

void
Clause::detachClause()
{
    detachFirstWatch();
    detachSecondWatch();
}

bool
Clause::isImplicantOfALiteral() const
{
    assert( "Unary clauses must be removed." && literals.size() > 1 );

    //We assume that the literal inferred is always in the first position.
    return ( literals[ 0 ]->isImplicant( this ) );// || literals[ 1 ]->isImplicant( this ) );
}

void
Clause::setWatchesInRandomPositions()
{
    assert( "Unary clauses must be removed." && literals.size() > 1 );
    unsigned int first = rand() % literals.size();
    unsigned int second = rand() % ( literals.size() - 1 );

    if( second >= first )
        second++;    

    assert( "First watch is not in range." && first < literals.size() );
    assert( "Second watch is not in range." && second < literals.size() );
    swapLiterals( 0, first );
    swapLiterals( 1, second );    
}

unsigned int
Clause::size() const
{
    return literals.size();
}

void
Clause::onLearning(
    LearningStrategy* strategy )
{
    assert( "LearningStrategy is not initialized." && strategy != NULL );

    //Navigating all literals in the clause.    
    for( unsigned int i = 0; i < literals.size(); i++ )
    {
        Literal* literal = literals[ i ];
        strategy->onNavigatingLiteral( literal );
    }
}

void
Clause::visitForHeuristic( 
    HeuristicVisitor* heuristicVisitor )
{
    assert( "Unary clauses must be removed." && literals.size() > 1 );
    if( literals[ 0 ]->isTrue() || literals[ 1 ]->isTrue() )
        return;
    
    for( unsigned int i = 0; i < literals.size(); i++ )
    {
        literals[ i ]->visitForHeuristic( heuristicVisitor );
    }
}

void
Clause::swapLiterals( 
    unsigned int pos1, 
    unsigned int pos2 )
{
    assert( "First position is out of range." && pos1 < literals.size() );
    assert( "Second position is out of range." && pos2 < literals.size() ); 
    std::swap( literals[ pos1 ], literals[ pos2 ] );
}

#endif	/* CLAUSE_H */

