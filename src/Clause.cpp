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

#include "Clause.h"
#include "Solver.h"

#include <vector>
using namespace std;

ostream&
operator<<(
    ostream & out, 
    const Clause & clause )
{
    return clause.print( out );
}

Clause::~Clause()
{
}

ostream&
Clause::print(
    ostream& out ) const
{
    if( literals.empty() || hasBeenDeleted() )
        return out;

    out << literals[ 0 ];
    for( unsigned int i = 1; i < literals.size(); i++ )
    {
        out << " | " << literals[ i ];
    }

    return out;
}

unsigned
Clause::getMaxDecisionLevel(
    unsigned from,
    unsigned to ) const
{
    assert( from < literals.size() );
    assert( to <= literals.size() );
    assert( from < to );
    unsigned max = literals[ from ].getDecisionLevel();
    for( unsigned i = from + 1; i < to; ++i )
        if( literals[ i ].getDecisionLevel() > max )
            max = literals[ i ].getDecisionLevel();
    return max;
}

//void
//Clause::updateFirstWatch(
//    Solver& solver )
//{
//    assert( "Unary clauses must be removed." && literals.size() > 1 );
//    
//    for( unsigned int i = 2; i < literals.size(); ++i )
//    {
//        if( !literals[ i ]->isFalse() )
//        {
//            //Detach the old watch
//            detachFirstWatch();
//            //Swap the two literals
//            swapLiterals( 0, i );
//            //Attach the watch in the new position
//            attachFirstWatch();
//            
//            return;
//        }
//    }
//    
//    assert( "The other watched literal cannot be true." && !literals[ 1 ]->isTrue() );
//    //Propagate literals[ 1 ];
//    solver.onLiteralAssigned( literals[ 1 ], TRUE, this );    
//}

bool
Clause::isSatisfied() const
{
    assert( size() > 0 );
    
    unsigned i = 0;
    if( hasBeenDeleted() )
        i = 1;
    
    for( ; i < literals.size(); ++i )
        if( literals[ i ].isTrue() )
            return true;
    return false;
}

void
Clause::printDimacs() const
{
    for( unsigned i = 0; i < literals.size(); i++ )
        cout << ( literals[ i ].isPositive() ? "" : "-" ) << literals[ i ].getVariable()->getId() << " ";
    cout << "0" << endl;
}

bool
Clause::allUndefined() const
{
    for( unsigned i = 0; i < size(); ++i )
        if( !getAt( i ).isUndefined() )
            return false;
    return true;
}

bool
Clause::isTautology() const
{
    for( unsigned i = 0; i < size(); ++i )
        for( unsigned j = i+1; j < size(); ++j )
            if( getAt( i ) == getAt( j ).getOppositeLiteral() )
                return true;
    return false;
}

void
Clause::onLearning(
    Learning* strategy,
    Literal )
{
    assert( "LearningStrategy is not initialized." && strategy != NULL );

    //Navigating all literals in the clause.
    assert_msg( literals[ 0 ].getDecisionLevel() != 0, "Literal " << literals[ 0 ] << " is in position 0 and it has been inferred " << ( literals[ 0 ].isTrue() ? "TRUE" : "FALSE" ) << " at level 0. Clause: " << *this );
    
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

//            if( lastSwapIndex >= literals.size() )
//                resetLastSwapIndex();
        }
    }
}

bool
Clause::onNavigatingLiteralForAllMarked(
    Learning* strategy,
    Literal )
{
    assert( "LearningStrategy is not initialized." && strategy != NULL );    
    for( unsigned int i = 1; i < literals.size(); i++ )
    {
        trace_msg( learning, 5, "Considering literal " << literals[ i ] << " in position " << i );

        if( !strategy->onNavigatingLiteralForAllMarked( literals[ i ] ) )
            return false;
    }
    
    return true;
//    for( unsigned int i = 2; i < literals.size(); i++ )
//    {
//        Literal literal = literals[ i ];
//        assert( literal.getDecisionLevel() != 0 );
//        if( !strategy->onNavigatingLiteralForAllMarked( literal ) )
//            return false;
//    }
//    
//    return true;
}