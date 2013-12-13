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
#include "Literal.h"
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
Clause::isUnsatisfied() const
{
    for( unsigned i = 0; i < literals.size(); ++i )
        if( literals[ i ].isTrue() )
            return false;
    return true;
}

