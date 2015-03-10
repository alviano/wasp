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

#include "ReasonForBinaryClauses.h"
#include "Solver.h"

ReasonForBinaryClauses::~ReasonForBinaryClauses()
{
}

void
ReasonForBinaryClauses::onLearning(
    const Solver& solver,
    Learning* strategy,
    Literal )
{
    Literal lit = solver.createFromAssignedVariable( varId );
    strategy->onNavigatingLiteral( lit );
}

bool
ReasonForBinaryClauses::onNavigatingLiteralForAllMarked(
    const Solver& solver,
    Learning* strategy,
    Literal )
{
    Literal lit = solver.createFromAssignedVariable( varId );
    return strategy->onNavigatingLiteralForAllMarked( lit );
}

ostream&
ReasonForBinaryClauses::print(
    ostream& o ) const
{
    return o << VariableNames::getName( varId );
}

void
ReasonForBinaryClauses::onNavigatingForUnsatCore(
    const Solver& 
    #ifndef NDEBUG 
        solver 
    #endif
    ,
    vector< unsigned int >& visited,
    unsigned int numberOfCalls,
    Literal )
{
    assert( solver.getDecisionLevel( varId ) > 0 );
    visited[ varId ] = numberOfCalls;
//    Literal lit = solver.createFromAssignedVariable( varId );
////    toVisit.push_back( lit.getOppositeLiteral() );
//    toVisit.push_back( lit );
}