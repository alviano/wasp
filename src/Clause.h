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

#ifndef CLAUSE_H
#define CLAUSE_H

#include <cassert>
#include <iostream>
#include <vector>

#include "Clause.h"
#include "Literal.h"
#include "Learning.h"

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
    friend Clause* Learning::onConflict( Literal conflictLiteral, Clause* conflictClause );

    public:
        inline Clause();
        virtual ~Clause();

        inline Literal getAt( unsigned idx ) const { assert( idx < literals.size() ); return literals[ idx ]; }
        inline void flipLiteralAt( unsigned idx ) { assert( idx < literals.size() ); literals[ idx ] = literals[ idx ].getOppositeLiteral(); }
        inline void markAsDeleted() { literals[ 0 ] = Literal::null; }
        inline bool hasBeenDeleted() const { return literals[ 0 ] == Literal::null; }
        inline void addLiteral( Literal literal );

        inline void attachClause();
        inline void attachClause( unsigned int firstWatch, unsigned int secondWatch );
        inline void attachClauseToAllLiterals();
        inline void detachClause();
        inline void detachClauseToAllLiterals( Literal literal );
        inline void removeLiteral( Literal literal );
        
        inline void onLearning( Learning* strategy );
        inline bool onLiteralFalse( Literal literal );

        inline unsigned int size() const;
//        inline bool checkUnsatisfiedAndOptimize( Heuristic* collector );
        bool isUnsatisfied() const;
        
        unsigned getMaxDecisionLevel( unsigned from, unsigned to) const;       

        inline bool isLocked() const;        

        inline void swapUnwatchedLiterals( unsigned int pos1, unsigned int pos2 );
        inline void swapWatchedLiterals();
          
        inline long getSignature() const { return signature; }
        inline Literal getLiteralWithMinOccurrences() const;
        
        inline void setPositionInSolver( unsigned int pos ) { positionInSolver = pos; }
        inline unsigned int getPositionInSolver(){ return positionInSolver; }

        /**
         * This method returns true if the clause is a subset of another clause.
         * Note that the check is expensive.
         */
        inline bool isSubsetOf( const Clause* clause ) const;
        
        inline Activity& activity(){ return act; }
        inline const Activity& activity() const { return act; }
        inline void setLearned(){ learned = true; }
        inline bool isLearned() const { return learned; }
        
    protected:
        vector< Literal > literals;
        unsigned lastSwapIndex;

        virtual ostream& print( ostream& out ) const;
  
    private:
        Clause( const Clause& )
        {
            assert( "The copy constructor has been disabled." && 0 );
        }
        
        inline void setWatchesInRandomPositions();
        
        inline void attachFirstWatch();
        inline void attachSecondWatch();        
        inline void detachSecondWatch();
        
        inline bool updateWatch();
        void notifyImplication( Solver& solver );

        inline void swapLiterals( unsigned int pos1, unsigned int pos2 );
        
        long signature;
        unsigned int positionInSolver;
        
        Activity act;
        bool learned;
};

Clause::Clause() : lastSwapIndex( 1 ), signature( 0 ), act( 0.0 ), learned( false )
{
}

//Clause::Clause(
//    unsigned int size ) : lastSwapIndex( 1 ), activity( 0 )
//{
//    literals.reserve( size );
//}

void
Clause::addLiteral(
    Literal literal )
{
    literals.push_back( literal );
    signature |= literal.getVariable()->getSignature();
}

void
Clause::attachFirstWatch()
{
    assert( "Unary clause must be removed." && literals.size() > 1 );
    literals[ 0 ].addWatchedClause( this );
}

void
Clause::attachSecondWatch()
{
    assert( "Unary clause must be removed." && literals.size() > 1 );
    literals[ 1 ].addWatchedClause( this );
}

void
Clause::detachSecondWatch()
{
    assert( "The watchToDetach points to a NULL literal." && literals[ 1 ].getVariable() != NULL );
    literals[ 1 ].eraseWatchedClause( this );
}

void
Clause::attachClause()
{
    assert( "Unary clauses must be removed." && literals.size() > 1 );
    attachFirstWatch();
    attachSecondWatch();
}

void
Clause::attachClauseToAllLiterals()
{
    unsigned int size = literals.size();
    for( unsigned int i = 0; i < size; i++ )
    {
        literals[ i ].addClause( this );
    }
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
    Literal tmp1 = literals[ first ];
    Literal tmp2 = literals[ second ];
    #endif

    swapLiterals( 0, first );
    second == 0 ? swapLiterals( 1, first ) : swapLiterals( 1, second );    

    assert( literals[ 0 ] == tmp1 );
    assert( literals[ 1 ] == tmp2 );
    
    attachFirstWatch();
    attachSecondWatch();
}

void
Clause::detachClause()
{
    literals[ 0 ].findAndEraseWatchedClause( this );
    literals[ 1 ].findAndEraseWatchedClause( this );
}

void
Clause::detachClauseToAllLiterals(
    Literal literal )
{
    for( unsigned int i = 0; i < literals.size(); ++i )
    {
        if( literals[ i ] == literal )
            literals[ i ].eraseClause( this );
        else
            literals[ i ].findAndEraseClause( this );
    }    
}

void
Clause::removeLiteral(
    Literal literal )
{
    unsigned int size = literals.size() - 1;
    unsigned int i = 0;
    for( ; i < size; ++i )
    {
        if( literals[ i ] == literal )
        {
            literals[ i ] = literals.back();
            break;
        }
    }

    literal.eraseClause( this );
    assert( literals.back() == literal || literals.back() == literals[ i ] );
    literals.pop_back();
}

bool
Clause::isLocked() const
{
    assert( "Unary clauses must be removed." && literals.size() > 1 );

    //We assume that the literal inferred is always in the first position.
    return ( literals[ 0 ].isImplicant( this ) );
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

//void
//Clause::onLearning(
//    Learning* strategy )
//{
//    assert( "LearningStrategy is not initialized." && strategy != NULL );
//
//    //Navigating all literals in the clause.    
//    for( unsigned int i = 0; i < literals.size(); i++ )
//    {
//        Literal literal = literals[ i ];
//        strategy->onNavigatingLiteral( literal );
//    }
//}

void
Clause::onLearning(
    Learning* strategy )
{
    assert( "LearningStrategy is not initialized." && strategy != NULL );

    //Navigating all literals in the clause.
    assert_msg( literals[ 0 ].getDecisionLevel() != 0, "Literal " << literals[ 0 ] << " is in position 0 and it has been inferred at level 0. Clause: " << *this );
    strategy->onNavigatingLiteral( literals[ 0 ] );

    assert_msg( literals[ 1 ].getDecisionLevel() != 0, "Literal " << literals[ 1 ] << " is in position 1 and it has been inferred at level 0. Clause: " << *this );
    strategy->onNavigatingLiteral( literals[ 1 ] );

    for( unsigned int i = 2; i < literals.size(); )
    {
        Literal literal = literals[ i ];
        if( literal.getDecisionLevel() != 0 )
        {
            strategy->onNavigatingLiteral( literal );
            i++;
        }
        else
        {
            assert_msg( literal.isFalse(), "Literal " << literal << " is not false." );
            swapUnwatchedLiterals( i, literals.size() - 1 );
            literals.pop_back();
        }
    }
}


//bool
//Clause::checkUnsatisfiedAndOptimize( 
//    Heuristic* collector )
//{
//    assert( "Unary clauses must be removed." && literals.size() > 1 );
//    
//    if( literals.back().isTrue() )
//        return false;
//    Variable* variable = literals.back().getVariable();
//    if( variable->isUndefined() )
//        collector->collectUndefined( variable );
//    
//    if( literals[ 0 ].isTrue() )
//        return false;
//    variable = literals[ 0 ].getVariable();
//    if( variable->isUndefined() )
//        collector->collectUndefined( variable );
//    
//    unsigned size = literals.size() - 1;
//    for( unsigned int i = 1; i < size; ++i )
//    {
//        if( literals[ i ].isTrue() )
//        {
//            if( i == 1 )
//                swapLiterals( 0, 1 );
//            else
//                swapLiterals( i, size );
//            
//            return false;
//        }
//        variable = literals[ i ].getVariable();
//        if( variable->isUndefined() )
//            collector->collectUndefined( variable );
//    }
//    
//    return true;
//}

void
Clause::swapLiterals( 
    unsigned int pos1, 
    unsigned int pos2 )
{
    assert_msg( pos1 < literals.size(), "First position is out of range: " << pos1 << " of " << literals.size() );
    assert_msg( pos2 < literals.size(), "Second position is out of range: " << pos2 << " of " << literals.size() );
    std::swap( literals[ pos1 ], literals[ pos2 ] );
}

void
Clause::swapWatchedLiterals()
{
    swapLiterals( 0, 1 );
}

void
Clause::swapUnwatchedLiterals( 
    unsigned int pos1, 
    unsigned int pos2 )
{
    assert( "First position is out of range." && pos1 < literals.size() );
    assert( "Second position is out of range." && pos2 < literals.size() );
    assert( pos1 >= 2 );
    assert( pos2 >= 2 );
    swapLiterals( pos1, pos2 );
}

bool
Clause::updateWatch()
{
    assert( "Unary clauses must be removed." && literals.size() > 1 );
    
    for( unsigned i = lastSwapIndex + 1; i < literals.size(); ++i )
    {
        if( !literals[ i ].isFalse() )
        {
            //Detach the old watch
            detachSecondWatch();

            lastSwapIndex = i;
            //Swap the two literals
            swapLiterals( 1, lastSwapIndex );

            //Attach the watch in the new position
            attachSecondWatch();            
            return true;
        }
    }
    
    for( unsigned i = 2; i <= lastSwapIndex; ++i )
    {
        if( !literals[ i ].isFalse() )
        {
            //Detach the old watch
            detachSecondWatch();

            lastSwapIndex = i;
            //Swap the two literals
            swapLiterals( 1, lastSwapIndex );

            //Attach the watch in the new position
            attachSecondWatch();            
            return true;
        }
    }

    assert( "The other watched literal cannot be true." && !literals[ 0 ].isTrue() );
    
    //Propagate literals[ 0 ];
    return false;
//    notifyImplication( solver );
}

bool
Clause::onLiteralFalse(
    Literal literal )
{
    assert( "The literal must be false." && literal.isFalse() );    
    assert( "Unary clauses must be removed." && literals.size() > 1 );

    if( literal == literals[ 0 ] )
    {
        //The watch to update should be always in position 1.
        literals[ 0 ] = literals[ 1 ];
        literals[ 1 ] = literal;
    }

    assert( "Literal is not watched." && literal == literals[ 1 ] );
    //if the clause is already satisfied do nothing.
    if( literals[ 0 ].isTrue() )
        return false;
        
    //update watch
    return !updateWatch();
}

Literal
Clause::getLiteralWithMinOccurrences() const
{
    assert( literals.size() > 1 );
    Literal minLiteral = literals[ 0 ];
    assert( minLiteral.numberOfOccurrences() > 0 );

    unsigned int i = 1;
    do
    {
        assert( literals[ i ].numberOfOccurrences() > 0 );
        if( literals[ i ].numberOfOccurrences() < minLiteral.numberOfOccurrences() )
        {
            minLiteral = literals[ i ];
        }
    } while( ++i < literals.size() );    
        
    return minLiteral;
}

bool
Clause::isSubsetOf(
    const Clause* clause ) const
{
    assert_msg( clause != NULL, "Clause cannot be null" );
    for( unsigned int i = 0; i < literals.size(); i++ )
    {
        if( find( clause->literals.begin(), clause->literals.end(), literals[ i ] ) == clause->literals.end() )
            return false;        
    }
    
    return true;
}

#endif

