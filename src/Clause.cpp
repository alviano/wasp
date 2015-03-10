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

ostream&
Clause::print(
    ostream& out ) const
{
    if( literals.empty() )
        return out << "[]";
    if( hasBeenDeleted() )
        return out << "-";

    out << "[ " << literals[ 0 ];
    for( unsigned int i = 1; i < literals.size(); i++ )
    {
        out << " | " << literals[ i ];
    }

    return out << " ]";
}

unsigned
Clause::getMaxDecisionLevel(
    Solver& solver,
    unsigned from,
    unsigned to ) const
{
    assert( from < literals.size() );
    assert( to <= literals.size() );
    assert( from < to );
    unsigned max = solver.getDecisionLevel( literals[ from ] );
    for( unsigned i = from + 1; i < to; ++i )
        if( solver.getDecisionLevel( literals[ i ] ) > max )
            max = solver.getDecisionLevel( literals[ i ] );
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

void
Clause::onLearning(
    const Solver& solver,
    Learning* strategy,
    Literal )
{
    assert( "LearningStrategy is not initialized." && strategy != NULL );

    //Navigating all literals in the clause.
//    assert_msg( solver.getDecisionLevel( literals[ 0 ] ) != 0, "Literal " << literals[ 0 ] << " is in position 0 and it has been inferred " << ( solver.isTrue( literals[ 0 ] ) ? "TRUE" : "FALSE" ) << " at level 0. Clause: " << *this );    
//    strategy->onNavigatingLiteral( literals[ 0 ] );

    assert_msg( solver.getDecisionLevel( literals[ 1 ] ) != 0, "Literal " << literals[ 1 ] << " is in position 1 and it has been inferred at level 0. Clause: " << *this );
    strategy->onNavigatingLiteral( literals[ 1 ] );

    for( unsigned int i = 2; i < literals.size(); )
    {
        Literal literal = literals[ i ];
        if( solver.getDecisionLevel( literal ) != 0 )
        {
            strategy->onNavigatingLiteral( literal );
            i++;
        }
        else
        {
            assert_msg( solver.isFalse( literal ), "Literal " << literal << " is not false." );
            swapUnwatchedLiterals( i, literals.size() - 1 );
            literals.pop_back();
        }
    }
}

bool
Clause::onNavigatingLiteralForAllMarked(
    const Solver&,
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

void
Clause::onNavigatingForUnsatCore(
    const Solver& solver,
    vector< unsigned int >& visited,
    unsigned int numberOfCalls,
    Literal )
{
    //Navigating all literals in the clause.    
    //It was 0 but position 0 contains the assigned literal
    for( unsigned int i = 1; i < literals.size(); i++ )
    {
        Var v = literals[ i ].getVariable();
        if( solver.getDecisionLevel( v ) > 0 )
            visited[ v ] = numberOfCalls;
    }
}

bool
Clause::removeDuplicatesAndFalseAndCheckIfTautological(
    Solver& solver )
{
    literals.sort( literalComparator );

    Literal previousLiteral = Literal::null;
    
    unsigned int i = 0;
    unsigned int j = 0;
    while( i < literals.size() )
    {
        if( solver.isTrue( literals[ i ] ) )
            return true;
        if( !solver.isFalse( literals[ i ] ) && previousLiteral != literals[ i ] )
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