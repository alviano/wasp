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

#include "Dimacs.h"

#include "../Solver.h"
#include "../Clause.h"
#include "../util/Constants.h"
#include "../util/Trace.h"
#include "../util/Options.h"

#include <cassert>
#include <sstream>
#include <unordered_set>
using namespace std;

Dimacs::Dimacs(
    Solver& s ) : solver( s )
{
}

void
Dimacs::parse()
{
    Istream input( cin );
    this->parse( input );
}

void
Dimacs::parse(
    Istream& input )
{
    if( !input.readInfoDimacs( numberOfVariables, numberOfClauses ) )
        ErrorMessage::errorDuringParsing( "Unexpected symbol." ); 
    insertVariables( numberOfVariables );
    readAllClauses( input );
}

void
Dimacs::readAllClauses(
    Istream& input )
{
    trace_msg( parser, 1, "Starting iteration on " << numberOfClauses << " clauses.");
    for( unsigned int i = 0; i < numberOfClauses; i++ )
        if( !readClause( input ) )
            return;    
}

bool
Dimacs::readClause(
    Istream& input )
{
    //a variable containing the literal to read
    int next;

    //read the next literal
    readNextLiteral( input, next );

    if( next == 0 )
    {
        solver.foundEmptyClause();
        return false;
    }
//        ErrorMessage::errorDuringParsing( "Empty clause are not allowed." );

    unordered_set< int > tempSet;    
    bool trivial = false;
//    Clause* clause = new Clause();
    Clause* clause = solver.newClause();

    do
    {
        //insert the current literal in the set
        bool inserted = !tempSet.insert( next ).second;
        trace_msg( parser, 2, "Reading " << next );
        Literal literal = solver.getLiteral( next );

        //if a literal appears in a clause C with both polarities then C is a tautology
        //if a literal is true the clause is satisfied
        if( tempSet.find( -next ) != tempSet.end() || literal.isTrue() )
            trivial = true;
        else if( literal.isUndefined() && !inserted ) //skip false literals and already inserted literals
            clause->addLiteral( literal );

        //read the next literal
        readNextLiteral( input, next );
    } while( next != 0 );
    
    if( !trivial )
    {
        trace_msg( parser, 1, "Adding clause " << *clause << " to the solver" );
        return solver.addClause( clause );
    }
    else
    {
        trace_msg( parser, 1, "Deleting clause " << *clause << " because is trivial" );
//        delete clause;
        solver.releaseClause( clause );
    }
    
    return true;
}

void
Dimacs::insertVariables(
    unsigned int numberOfVariables )
{
    trace_msg( parser, 1, "Adding " << numberOfVariables << " variables." );
    for( unsigned int i = 1; i <= numberOfVariables; i++ )
    {
        solver.addVariable();
        #ifdef TRACE_ON
        stringstream s;
        s << i;
        VariableNames::setName( solver.getLiteral( i ).getVariable(), s.str() );
        #endif
    }
}
