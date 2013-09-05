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

#include "ProgramBuilder.h"
#include "Clause.h"
#include "Literal.h"
#include "Solver.h"

#include <iostream>
using namespace std;

ProgramBuilder::ProgramBuilder(
    Solver* s ) : solver( s )
{
}

void
ProgramBuilder::newVar()
{
    solver->addVariable();
}

void
ProgramBuilder::newVar( 
    const string& name )
{
    solver->addVariable( name );
}

Clause* 
ProgramBuilder::startClause()
{
    return new Clause();
}

Clause* 
ProgramBuilder::startClause( 
    unsigned int size )
{
    return new Clause( size );
}

void 
ProgramBuilder::addLiteralInClause( 
    int lit, 
    Clause* clause )
{
    Literal* literal = solver->getLiteral( lit );
    clause->addLiteral( literal );
}

void 
ProgramBuilder::addTrueLiteral( 
    int lit )
{
    solver->addTrueLiteral( lit );
}

void 
ProgramBuilder::endClause(
    Clause* clause )
{
    assert( "Clauses must contain at least two literals." && clause->size() > 1 );
    solver->addClause( clause );
    clause->attachClause();    
}

void
ProgramBuilder::startBuilding()
{
}

void
ProgramBuilder::endBuilding()
{
}