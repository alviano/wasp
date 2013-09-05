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

        /**
         * Constructor of Clause which takes in input the number of literals in the clause.
         * Note that the watches are random generated.
         * 
         * @param size the numbers of literals.
         */
        inline Clause( unsigned int size );
        inline Clause( unsigned int size, unsigned int firstWatch, unsigned int secondWatch );

        inline void addLiteral( Literal* literal );

        inline void attachClause();
        inline void attachClause( unsigned int firstWatch, unsigned int secondWatch );
        inline void detachClause();

        inline void onLearning( LearningStrategy* strategy );
        void onLiteralFalse( Literal* literal, Solver& solver );

        inline void setFirstWatch( unsigned int firstWatch );
        inline void setSecondWatch( unsigned int secondWatch );
        inline void setWatchesInRandomPositions();
        
        inline unsigned int size() const;
        
        inline void visitForHeuristic( HeuristicVisitor* );
        
    protected:
        inline bool isImplicantOfALiteral() const;

    private:
        Clause( const Clause& )
        {
            assert( "The copy constructor has been disabled." && 0 );
        }

        vector< Literal* > literals;

        unsigned int firstWatch;
        unsigned int secondWatch;

        WatchedList< Clause* >::iterator iterator_firstWatch;
        WatchedList< Clause* >::iterator iterator_secondWatch;
        
        inline void attachWatch( const unsigned int& watchToAttach, WatchedList< Clause* >::iterator& iterator );
        inline void detachWatch( const unsigned int& watchToDetach, WatchedList< Clause* >::iterator& iterator );
        
//        void moveWatchToFirstPosition( unsigned int& watch, unsigned int& otherWatch );
        
        /**
         * 
         * This function updates the position of one watch.
         * It is required that both watches are pointing to an undefined literal.
         * 
         * @param watchToUpdate The watch to update
         * @param otherWatch The other watch
         * @param solver The current solver
         * @return true if the watch has been moved, false otherwise
         */        
        void updateWatch( unsigned int& watchToUpdate, unsigned int& otherWatch, WatchedList< Clause* >::iterator iteratorWatchToUpdate, Solver& solver );
};

Clause::Clause() : firstWatch( 0 ), secondWatch( 0 )
{
}

Clause::Clause(
    unsigned int size )
{
    literals.reserve( size );
    setWatchesInRandomPositions();
}

Clause::Clause(
    unsigned int size,
    unsigned int first,
    unsigned int second ) : firstWatch( first ), secondWatch( second )
{
    assert( "First watch is out of range." && first < size );
    assert( "Second watch is out of range." && second < size );

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
Clause::attachWatch(
    const unsigned int& watchToAttach,
    WatchedList< Clause* >::iterator& iterator )
{
    assert( "The watchToAttach is not in range." && watchToAttach < literals.size() );
    assert( "The watchToAttach points to a NULL literal." && literals[ watchToAttach ] != NULL );
    iterator = literals[ watchToAttach ]->addWatchedClause( this );
}

void
Clause::attachClause()
{
    if( firstWatch == secondWatch )
        setWatchesInRandomPositions();
    assert( "First watch and second watch point to the same literal." && firstWatch != secondWatch );
    attachWatch( firstWatch, iterator_firstWatch );
    attachWatch( secondWatch, iterator_secondWatch );
}

void
Clause::attachClause( 
    unsigned int first,
    unsigned int second )
{
    setFirstWatch( first );
    setSecondWatch( second );
    assert( "First watch and second watch point to the same literal." && firstWatch != secondWatch );
    attachWatch( firstWatch, iterator_firstWatch );
    attachWatch( secondWatch, iterator_secondWatch );
}

void
Clause::detachWatch(
    const unsigned int& watchToDetach,
    WatchedList< Clause* >::iterator& iterator )
{
    assert( "The watchToDetach is not in range." && watchToDetach < literals.size() );
    assert( "The watchToDetach points to a NULL literal." && literals[ watchToDetach ] != NULL );
    literals[ watchToDetach ]->eraseWatchedClause( iterator );
}

void
Clause::detachClause()
{
    assert( "First watch and second watch point to the same literal." && firstWatch != secondWatch );
    detachWatch( firstWatch, iterator_firstWatch );
    detachWatch( secondWatch, iterator_secondWatch );    
}

bool
Clause::isImplicantOfALiteral() const
{    
    assert( "First watch is out of range." && firstWatch < size() );
    assert( "Second watch is out of range." && secondWatch < size() );
    
//    return ( literals[ firstWatch ]->isImplicant( this ) || literals[ secondWatch ]->isImplicant( this ) );
    
//    assert( "This clause is the implicant of a literal which is not in the first position." 
//         && ( !literals[ 0 ]->isImplicant( this ) || 
//          ( !literals[ firstWatch ]->isImplicant( this ) 
//         && !literals[ secondWatch ]->isImplicant( this ) ) ) );
    
    //We assume that the literal inferred is always in the first position.
    return ( literals[ firstWatch ]->isImplicant( this ) || literals[ secondWatch ]->isImplicant( this ) );
}

void
Clause::setFirstWatch(
    unsigned int first )
{
    assert( "Inserted value is not valid." && first < literals.size() );
    firstWatch = first;
}

void
Clause::setSecondWatch(
    unsigned int second )
{
    assert( "Inserted value is not valid." && second < literals.size() );
    secondWatch = second;
}

void
Clause::setWatchesInRandomPositions()
{
    assert( "Unary clause must be removed." && literals.size() > 1 );
    firstWatch = rand() % literals.size();
    secondWatch = rand() % ( literals.size() - 1 );

    if( secondWatch >= firstWatch )
        secondWatch++;

    assert( "First watch is not in range." && firstWatch < literals.size() );
    assert( "Second watch is not in range." && secondWatch < literals.size() );
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
    if( literals.size() == 1 || literals[ firstWatch ]->isTrue() || literals[ secondWatch ]->isTrue() )
        return;
    
    for( unsigned int i = 0; i < literals.size(); i++ )
    {
        literals[ i ]->visitForHeuristic( heuristicVisitor );
    }
}

#endif	/* CLAUSE_H */

