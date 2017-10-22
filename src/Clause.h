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

#ifndef WASP_CLAUSE_H
#define WASP_CLAUSE_H

#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Clause.h"
#include "Literal.h"
#include "Learning.h"
#include "Reason.h"
#include "util/WaspAssert.h"

using namespace std;

class Solver;
enum SubsumptionData { NO_SUBSUMPTION = 0, SUBSUMPTION = 1, SELFSUBSUMPTION = 2 };

/**
 *  This class represents a clause.
 *  A clause is a disjunction of literals.
 *  For example (a1 v a2 v not a3) is a clause and a1, a2 are positive literals and not a3 is a negative literal.
 * 
 */
class Clause : public Reason
{
    friend ostream &operator<<( ostream & out, const Clause & clause );
    friend Clause* Learning::onConflict( Literal conflictLiteral, Reason* conflictClause );
    friend Clause* Learning::learnClausesFromUnfoundedSet( Vector< Var >& unfoundedSet );

    public:                
        inline ~Clause(){}
        inline Clause( unsigned reserve = 8 );

        inline Literal getAt( unsigned idx ) const { assert( idx < literals.size() ); return literals[ idx ]; }
        inline void setAt( unsigned idx, Literal lit ) { assert( idx < literals.size() ); literals[ idx ] = lit; }
        inline void flipLiteralAt( unsigned idx ) { assert( idx < literals.size() ); literals[ idx ] = literals[ idx ].getOppositeLiteral(); }
        inline void markAsDeleted() { literals.push_back( literals[ 0 ] ); literals[ 0 ] = Literal::null; }
        inline bool hasBeenDeleted() const { assert( !literals.empty() ); return literals[ 0 ] == Literal::null; }
        inline void addLiteral( Literal literal );
        inline void addLiteralInLearnedClause( Literal literal );        

        inline bool contains( Literal literal );
        inline bool containsAnyComplementOf( Clause* clause );

        inline Literal operator[]( unsigned int idx ) const { assert_msg( idx < literals.size(), "Index is " << idx << " - literals: " << literals.size() ); return literals[ idx ]; }
        inline Literal& operator[]( unsigned int idx ) { assert_msg( idx < literals.size(), "Index is " << idx << " - literals: " << literals.size() ); return literals[ idx ]; }

        inline void removeLiteral( Literal literal );
        inline void removeLastLiteralNoWatches(){ literals.pop_back(); }
        
        void onLearning( const Solver& solver, Learning* strategy, Literal lit );
        bool onNavigatingLiteralForAllMarked( const Solver& solver, Learning* strategy, Literal lit );
        void onNavigatingForUnsatCore( const Solver& solver, vector< unsigned int >& visited, unsigned int numberOfCalls, Literal lit );
//        inline bool onLiteralFalse( Solver& solver, Literal literal );

        inline unsigned int size() const;
        
        unsigned getMaxDecisionLevel( Solver& solver, unsigned from, unsigned to ) const;      

        inline void swapLiteralsNoWatches( unsigned int pos1, unsigned int pos2 ){ swapLiterals( pos1, pos2 ); }
        inline void swapUnwatchedLiterals( unsigned int pos1, unsigned int pos2 );
        inline void swapWatchedLiterals();        
          
        inline uint64_t getSignature() const { return signature(); }
//        inline Literal getLiteralWithMinOccurrences() const;
//        inline Variable* getVariableWithMinOccurrences();
        
        inline void setPositionInSolver( unsigned int pos ) { clauseData.positionInSolver = pos; }
        inline unsigned int getPositionInSolver(){ return clauseData.positionInSolver; }

        /**
         * This method returns true if the clause is a subset of another clause.
         * Note that the check is expensive.
         */
        inline bool isSubsetOf( const Clause* clause ) const;
        
        inline Activity& activity(){ return act(); }
        inline const Activity& activity() const { return act(); }
        inline void setLearned() { clauseData.learned = 1; }
        inline void setOriginal() { clauseData.learned = 0; }
        inline bool isLearned() const { return clauseData.learned == 1; }
        
//        inline bool removeSatisfiedLiterals();
        inline void removeDuplicates();
        inline bool removeDuplicatesAndCheckIfTautological();
        bool removeDuplicatesAndFalseAndCheckIfTautological( Solver& solver );
        
        inline void free();
        inline SubsumptionData subsumes( Clause& other );        
        inline bool checkEquality( Clause& other );        
        
        inline void resetInQueue(){ clauseData.inQueue = 0; }
        inline void setInQueue(){ clauseData.inQueue = 1; }
        inline bool isInQueue(){ return clauseData.inQueue == 1; }
        
        inline void copyLiterals( const Clause& c );
        
        inline void swapLiterals( unsigned int pos1, unsigned int pos2 );
        
        inline void recomputeSignature();                
        #ifndef NDEBUG
        inline bool isTautology() const
        {
            for( unsigned i = 0; i < size(); ++i )
                for( unsigned j = i+1; j < size(); ++j )
                    if( getAt( i ) == getAt( j ).getOppositeLiteral() )
                        return true;
            return false;
        }
        #endif
        
        inline void setLbd( unsigned int lbd ) { lbd_ = lbd; }
        inline unsigned int lbd() const { return lbd_; }
        
        inline void setCanBeDeleted( bool b ) { canBeDeleted_ = b; }
        inline bool canBeDeleted() const { return canBeDeleted_; }                
        
        inline void shrink( unsigned int value ) { literals.shrink( value ); } 
        
        /* USE ONLY FOR DEBUG */
        inline void printOrderedById()
        {
            vector< int > tmp;
            for( unsigned int i = 0; i < literals.size(); i++ )
                tmp.push_back( literals[ i ].getId() );
            
            sort( tmp.begin(), tmp.end() );
            
            cout << "[";
            for( unsigned int i = 0; i < tmp.size(); i++ )
                cout << " " << tmp[ i ];
            cout << "]";
        }
        
    private:
        Vector< Literal > literals;
        ostream& print( ostream& out ) const;        
        Clause( const Clause& );
        
        union SignatureAct
        {
            uint64_t signature;
            Activity act;
            
            SignatureAct() { memset( this, 0, sizeof( SignatureAct ) ); }
        } signature_act;
        inline uint64_t& signature() { assert( !isLearned() ); return signature_act.signature; }
        inline const uint64_t& signature() const { assert( !isLearned() ); return signature_act.signature; }
        inline Activity& act() { assert( isLearned() ); return signature_act.act; }
        inline const Activity& act() const { assert( isLearned() ); return signature_act.act; }
        
        inline uint64_t getSignature( Var v ) const { return ( ( uint64_t ) 1 ) << ( ( v - 1 ) & 63 ); }
        
        struct
        {        
            unsigned inQueue                : 1;
            unsigned learned                : 1;            
            unsigned positionInSolver       : 30;
        } clauseData;
        
        unsigned int lbd_ : 31;
        unsigned int canBeDeleted_ : 1;
};

Clause::Clause(
    unsigned reserve ) : lbd_( 0 ), canBeDeleted_( 1 )
{
    literals.reserve( reserve );
    clauseData.inQueue = 0;
    clauseData.learned = 0;    
}

void
Clause::addLiteral(
    Literal literal )
{
    assert( !isLearned() );
    literals.push_back( literal );
    signature() |= getSignature( literal.getVariable() );
}

void
Clause::addLiteralInLearnedClause(
    Literal literal )
{
    assert( isLearned() );
    literals.push_back( literal );
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

    assert( literals.back() == literal || literals.back() == literals[ i ] );
    literals.pop_back();
    
    recomputeSignature();
}

unsigned int
Clause::size() const
{
    return literals.size();
}

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

//bool
//Clause::updateWatch()
//{
//    assert( "Unary clauses must be removed." && literals.size() > 1 );
//    
//    for( unsigned i = 2; i < literals.size(); ++i )
//    {
//        if( !literals[ i ].isFalse() )
//        {
//            //Swap the two literals
//            swapLiterals( 1, i );
//
//            //Attach the watch in the new position
//            attachSecondWatch();            
//            return true;
//        }
//    }   
//    assert( "The other watched literal cannot be true." && !literals[ 0 ].isTrue() );    
//    //Propagate literals[ 0 ];
//    return false;
//}
//
//bool
//Clause::onLiteralFalse(
//    Solver& solver,
//    Literal literal )
//{
//    assert( "The literal must be false." && literal.isFalse() );    
//    assert( "Unary clauses must be removed." && literals.size() > 1 );
//
//    if( literal == literals[ 0 ] )
//    {
//        //The watch to update should be always in position 1.
//        literals[ 0 ] = literals[ 1 ];
//        literals[ 1 ] = literal;
//    }
//
//    assert( "Literal is not watched." && literal == literals[ 1 ] );
//    //if the clause is already satisfied do nothing.
//    if( literals[ 0 ].isTrue() )
//        return false;
//        
//    //update watch
//    return !updateWatch();
//}

bool
Clause::isSubsetOf(
    const Clause* clause ) const
{
    assert_msg( clause != NULL, "Clause cannot be null" );
    for( unsigned int i = 0; i < literals.size(); i++ )
    {
        if( !clause->literals.existElement( literals[ i ] ) )
            return false;        
    }
    
    return true;
}

bool
Clause::contains(
    Literal literal )
{
    for( unsigned k = 0; k < literals.size(); ++k )
        if( literals[ k ] ==  literal )
            return true;
    return false;
}

bool
Clause::containsAnyComplementOf(
    Clause* clause )
{
    for( unsigned i = 0; i < clause->size(); ++i )
        if( contains( clause->getAt( i ).getOppositeLiteral() ) )
            return true;
    return false;
}

inline int literalComparator( Literal l1, Literal l2 ){ return l1.getVariable() < l2.getVariable(); }

void
Clause::removeDuplicates()
{
    literals.sort( literalComparator );
    
    Literal previousLiteral = literals[ 0 ];
    
    unsigned int i = 1;
    unsigned int j = 1;
    while( i < literals.size() )
    {
        if( previousLiteral != literals[ i ] )
            previousLiteral = literals[ j++ ] = literals[ i++ ];
        else
            ++i;
    }

    if( i != j )
    {
        literals.shrink( j );
        recomputeSignature();
    }
}

bool
Clause::removeDuplicatesAndCheckIfTautological()
{
    literals.sort( literalComparator );
    
    Literal previousLiteral = literals[ 0 ];
    
    unsigned int i = 1;
    unsigned int j = 1;
    while( i < literals.size() )
    {
        if( previousLiteral != literals[ i ] )
        {
            //The same variable with two different polarities: clause is tautological
            if( previousLiteral.getVariable() == literals[ i ].getVariable() )
            {
                //TAUTOLOGICAL
                return true;
            }
            else
            {
               previousLiteral = literals[ j++ ] = literals[ i ];
            }
        }
        
        ++i;
    }

    if( i != j )
    {
        literals.shrink( j );
        recomputeSignature();
    }

    return false;
}

void
Clause::recomputeSignature()
{
    signature() = 0;
    for( unsigned int i = 0; i < literals.size(); i++ )    
         signature() |= getSignature( literals[ i ].getVariable() );
}

void
Clause::free()
{
//    lastSwapIndex = 1;
    clauseData.inQueue = 0;
    clauseData.learned = 0;
    if( isLearned() )
        act() = 0;
    else
        signature() = 0;
//    act = 0.0;
    literals.clear();
    lbd_ = 0;
    canBeDeleted_ = 1;
}

SubsumptionData
Clause::subsumes(
    Clause& other )
{
    unsigned int size = this->size();
    unsigned int otherSize = other.size();
    if( size < otherSize && ( signature() & ~other.signature() ) != 0 )
        return NO_SUBSUMPTION;
    
    SubsumptionData ret = SUBSUMPTION;
    unsigned int position = MAXUNSIGNEDINT;    
    
    for( unsigned int i = 0; i < size; i++ )
    {
        for( unsigned int j = 0; j < otherSize; j++ )
        {
            if( getAt( i ) == other.getAt( j ) )
                goto ok;
            else if( ret == SUBSUMPTION && getAt( i ) == other.getAt( j ).getOppositeLiteral() )
            {
                ret = SELFSUBSUMPTION;
                position = j;
                goto ok;
            }
        }
        
        return NO_SUBSUMPTION;
        ok:;
    }
    
    if( ret == SELFSUBSUMPTION )
    {
        assert_msg( position < otherSize, "Position is " << position << " while the size of the clause is " << otherSize );
        other.swapLiterals( position, otherSize - 1 );
    }

    return ret;
}

bool
Clause::checkEquality(
    Clause& other )
{
    unsigned int size = this->size();
    unsigned int otherSize = other.size();
    if( size != otherSize || signature() != other.signature() )
        return false;    
    
    for( unsigned int i = 0; i < size; i++ )
    {
        for( unsigned int j = 0; j < otherSize; j++ )
        {
            if( getAt( i ) == other.getAt( j ) )
                goto ok;            
        }
        
        return false;
        ok:;
    }    

    return true;
}

void
Clause::copyLiterals(
    const Clause& c )
{
    for( unsigned int i = 0; i < c.literals.size(); i++ )    
        this->addLiteral( c.literals[ i ] );    
}

#endif
